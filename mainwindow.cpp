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

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_lastDir( QDir::toNativeSeparators( QDir::homePath() ) )
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::on_editName_textChanged(QString const& text)
{
    ui->buttonSearch->setEnabled( !text.isEmpty() );
}

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
}

void MainWindow::on_buttonAddFiles_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames( this, tr( "Select one or more files to be renamed" ), m_lastDir, "Videos (*.avi *.m4v *.mov);;All files (*.*)" );
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

void MainWindow::on_buttonRemoveFiles_clicked()
{
    foreach( QListWidgetItem* item, ui->listFiles->selectedItems() )
        delete item;
    ui->buttonRemoveFiles->setEnabled( ui->listFiles->selectedItems().count() > 0 );
    ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );
}

void MainWindow::on_listFiles_itemSelectionChanged()
{
    ui->buttonRemoveFiles->setEnabled( ui->listFiles->selectedItems().count() > 0 );
}

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

void MainWindow::on_buttonRemoveEpisode_clicked()
{
    foreach( QListWidgetItem* item, ui->listEpisodes->selectedItems() )
        delete item;
    ui->buttonRemoveEpisode->setEnabled( ui->listEpisodes->selectedItems().count() > 0 );
    ui->buttonRename->setEnabled( ui->listFiles->count() && ui->listEpisodes->count() );
}

void MainWindow::on_listEpisodes_itemSelectionChanged()
{
    ui->buttonEditEpisode->setEnabled( ui->listEpisodes->selectedItems().size() > 0 );
    ui->buttonRemoveEpisode->setEnabled( ui->listEpisodes->selectedItems().size() > 0 );
}

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
