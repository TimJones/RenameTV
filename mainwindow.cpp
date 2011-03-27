#include "mainwindow.hpp"

#include "ui_mainwindow.h"

#include <QRegExp>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    return formattedName;
}

void MainWindow::on_buttonSearch_clicked()
{
    if( ui->editName->text().isEmpty() )
    {
        QMessageBox::warning( this, tr( "No Title" ), tr( "Please enter a series title to search for" ) );
        return;
    }

    qApp->setOverrideCursor( Qt::WaitCursor );
    QList< ShowDetail > results = m_show.Search( ui->editName->text() );
    qApp->restoreOverrideCursor();

    QMessageBox::information( this, tr( "Results" ), tr( "Found %1 shows" ).arg( results.count() ) );

    if( results.count() > 0 )
    {
        ui->listEpisodes->clear();
        qApp->setOverrideCursor( Qt::WaitCursor );
        QList< EpisodeDetail > episodes = m_show.Fetch( results[ 0 ] );
        qApp->restoreOverrideCursor();

        foreach( EpisodeDetail const& episode, episodes )
            ui->listEpisodes->addItem( FormatEpisode( episode ) );
    }
}

void MainWindow::on_editName_returnPressed()
{
    ui->buttonSearch->click();
}

void MainWindow::on_editDisplay_returnPressed()
{
    ui->buttonDisplay->click();
}

void MainWindow::on_buttonDisplay_clicked()
{

}
