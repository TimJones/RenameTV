/*
Copyright (c) 2010 Tim Jones

This file is part of RenameTV.

RenameTV is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RenameTV is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RenameTV.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file mainwindow.hpp
    \brief Definition for the MainWindow class
    \file mainwindow.cpp
    \brief Implimentation for the MainWindow class
*/

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "episodeselector.hpp"
#include "searchresults.hpp"

#include <QRegExp>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDirIterator>

/*! \class MainWindow
    \headerfile mainwindow.hpp
    \brief The main GUI window class
*/

/*! \brief Constructor
    \param parent QWidget pointer to the parent QWidget of the MainWindow
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_lastDir( QDir::toNativeSeparators( QDir::homePath() ) )
{
    m_videoFileTypes << "*.avi" << "*.m4v" << "*.mov" << "*.mpg" << "*.wmv";
    ui->setupUi(this);
}

/*! \brief Destructor
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*! \brief Formats an EpisodeDetail object into a QString
    \sa EpisodeDetail
    \param episode A constant reference to an EpisodeDetail object
    \return A QString object with the episode details formatted as per user input
*/
QString MainWindow::FormatEpisode( EpisodeDetail const& episode ) const
{
    QString formattedName = ui->editDisplay->text();

    formattedName.replace( "%show%", episode.show );
    formattedName.replace( "%title%", episode.title );

    int index = 0;
    while( ( index = formattedName.indexOf( QRegExp( "%e+%" ) ) ) != -1 )
    {
        int width = 0;
        while( formattedName.at( index + width + 1 ) != '%' )
            width++;
        formattedName.replace( index, width + 2, QString( "%1" ).arg( episode.episode, width, 10, QChar( '0' ) ) );
    }

    index = 0;
    while( ( index = formattedName.indexOf( QRegExp( "%s+%" ) ) ) != -1 )
    {
        int width = 0;
        while( formattedName[ index + width + 1 ] != '%' )
            width++;
        formattedName.replace( index, width + 2, QString( "%1" ).arg( episode.season, width, 10, QChar( '0' ) ) );
    }

#ifdef Q_WS_WIN
    QList< QChar > badChars;
    badChars << '\\' << '/' << ':' << '*' << '?' << '"' << '<' << '>' << '|';

    foreach( QChar const& c, badChars )
        formattedName.remove( c );
#endif

    return formattedName;
}

/*! \brief Called when the text in editName changes
    \param text A constant reference to the new text in editName
*/
void MainWindow::on_editName_textChanged(QString const& text)
{
    ui->buttonSearch->setEnabled( !text.isEmpty() );
}

/*! \brief Called when buttonSearch is clicked
    \sa ShowHolder
    When buttonSearch is clicked, a ShowHolder object is created to search for the show name
    (held in editName) and, if more than one match is found, a selection window is show to the user.
    Once a show is selected, the XML data is fetched and parsed.
*/
void MainWindow::on_buttonSearch_clicked()
{
    ShowHolder show;
    qApp->setOverrideCursor( Qt::WaitCursor );
    QList< ShowDetail > results = show.Search( ui->editName->text() );
    qApp->restoreOverrideCursor();

    if( results.count() > 0)
    {
        int chosen = 0;
        if( results.count() == 1 )
            QMessageBox::information( this, tr( "Result" ), tr( "Found '%1'" ).arg( results.front().name ) );
        else
        {
            SearchResults resultsWindow( results, this );
            if( resultsWindow.exec() != QDialog::Accepted )
                return;
            chosen = resultsWindow.GetSelectedShowIndex();
        }
        qApp->setOverrideCursor( Qt::WaitCursor );
        m_episodes = show.Fetch( results[ chosen ] );
        qApp->restoreOverrideCursor();
        ui->buttonAddEpisodes->setEnabled( true );
    }
    else
    {
        QMessageBox::information( this, tr( "Result" ), tr( "No shows found to match '%1'" ).arg( ui->editName->text() ) );
    }
}

/*! \brief Called when buttonAddFiles is clicked
    Opens a QFileDialog and allows the user to select mutliple files to rename.
 */
void MainWindow::on_buttonAddFiles_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames( this, tr( "Select one or more files to be renamed" ), m_lastDir, tr( "Videos (%1);;All files (*.*)" ).arg( m_videoFileTypes.join( " " ) ) );
    if( files.count() > 0 )
    {
        m_lastDir = QDir::toNativeSeparators( QFileInfo( files.back() ).absolutePath() + "/" );

        foreach( QString const& file, files )
        {
            QListWidgetItem* item = new QListWidgetItem( QFileInfo( file ).fileName() , ui->listFiles );
            item->setToolTip( file );
            ui->listFiles->addItem( item );
        }
        ui->buttonRemoveFiles->setEnabled( ui->listFiles->selectedItems().count() > 0 );
        ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );
    }
}

/*! \brief Called when buttonAddDir is clicked
    Opens a QFileDialog and allows the user to select a directory, the contents of which are scanned for video files and added.
 */
void MainWindow::on_buttonAddDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory( this, tr( "Select directory to scan" ), m_lastDir );
    if( !dir.isEmpty() )
    {
        qApp->setOverrideCursor( Qt::WaitCursor );

        m_lastDir = dir;
        QDirIterator dirIt( dir, m_videoFileTypes, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories );
        while( dirIt.hasNext() )
        {
            dirIt.next();
            QListWidgetItem* item = new QListWidgetItem( dirIt.fileName() , ui->listFiles );
            item->setToolTip( dirIt.filePath() );
            ui->listFiles->addItem( item );
        }
        ui->buttonRemoveFiles->setEnabled( ui->listFiles->selectedItems().count() > 0 );
        ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );

        qApp->restoreOverrideCursor();
    }
}

/*! \brief Called when buttonRemoveFiles is clicked
    Removes all the currently highlighted items in listFiles
*/
void MainWindow::on_buttonRemoveFiles_clicked()
{
    foreach( QListWidgetItem* item, ui->listFiles->selectedItems() )
        delete item;
    ui->buttonRemoveFiles->setEnabled( ui->listFiles->selectedItems().count() > 0 );
    ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );
}

/*! \brief Called when the selection of listFiles changes
    Used to update whether buttonRemoveFiles should be enabled or not.
*/
void MainWindow::on_listFiles_itemSelectionChanged()
{
    ui->buttonRemoveFiles->setEnabled( ui->listFiles->selectedItems().count() > 0 );
}

/*! \brief Called when buttonAddEdpisodes is clicked
    \sa EpisodeSelector
    Opens the EpisodeSelector window allowing the user to select episode names
*/
void MainWindow::on_buttonAddEpisodes_clicked()
{
    qApp->setOverrideCursor( Qt::WaitCursor );
    EpisodeSelector selectorWindow( this );

    int currSeason = -1;
    foreach( EpisodeDetail episode, m_episodes )
    {
        if( currSeason != episode.season )
        {
            currSeason = episode.season;
            selectorWindow.AddSeason( currSeason );
        }
        selectorWindow.AddEpisode( FormatEpisode( episode ) );
    }

    qApp->restoreOverrideCursor();
    if( selectorWindow.exec() != QDialog::Accepted )
        return;

    if( selectorWindow.isSeasonSelected() )
    {
        currSeason = selectorWindow.GetSelectedSeason();
        foreach( EpisodeDetail episode, m_episodes )
        {
            if( currSeason == -1 || episode.season == currSeason )
                ui->listEpisodes->addItem( FormatEpisode( episode ) );
        }
    }
    else if( selectorWindow.isEpisodeSelected() )
        ui->listEpisodes->addItems( selectorWindow.GetSelectedEpisodes() );
    ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );
}

/*! \brief Called when buttonEditEpisode is clicked
    Allows the user to manually edit an episode name.
*/
void MainWindow::on_buttonEditEpisode_clicked()
{
    if( ui->listEpisodes->selectedItems().count() < 1 )
        return;

    bool ok;
    QListWidgetItem* item = ui->listEpisodes->selectedItems().front();
    QString episodeTitle = item->text();
    QString edit = QInputDialog::getText( this, tr( "Edit episode title" ), tr( "Original title:\n  %1  " ).arg( episodeTitle ), QLineEdit::Normal, episodeTitle, &ok );
    if( ok && !edit.isEmpty() )
        item->setText( edit );
    ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );
}

/*! \brief Called when buttonRemoveEpisode is clicked
*/
void MainWindow::on_buttonRemoveEpisode_clicked()
{
    foreach( QListWidgetItem* item, ui->listEpisodes->selectedItems() )
        delete item;
    ui->buttonRemoveEpisode->setEnabled( ui->listEpisodes->selectedItems().count() > 0 );
    ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );
}

/*! \brief Called when the selection of listEpisodes changes
*/
void MainWindow::on_listEpisodes_itemSelectionChanged()
{
    ui->buttonEditEpisode->setEnabled( ui->listEpisodes->selectedItems().size() > 0 );
    ui->buttonRemoveEpisode->setEnabled( ui->listEpisodes->selectedItems().size() > 0 );
}

/*! \brief Called when buttonRename is clicked
*/
void MainWindow::on_buttonRename_clicked()
{
    if( ui->listFiles->count() < 1 || ui->listEpisodes->count() < 1 )
        return;

    qApp->setOverrideCursor( Qt::WaitCursor );
    for( int i = 0; i < ui->listFiles->count() && i < ui->listEpisodes->count(); i++ )
    {
        QListWidgetItem* fileitem = ui->listFiles->item( i );
        QListWidgetItem* episodeitem = ui->listEpisodes->item( i );
        QFileInfo fileinfo( fileitem->toolTip() );

        QString newFilename = fileinfo.absolutePath();
        newFilename += "/";
        newFilename += episodeitem->text();
        newFilename += ".";
        newFilename += fileinfo.suffix();

        if( QFile::rename( QDir::toNativeSeparators( fileinfo.absoluteFilePath() ), QDir::toNativeSeparators( newFilename ) ) )
        {
            delete fileitem;
            delete episodeitem;
            i--;
        }
    }
    qApp->restoreOverrideCursor();
    ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );
}
