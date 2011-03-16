#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDir>
#include <QTextStream>
#include <QProgressDialog>
#include <QMessageBox>

#include "showholder.hpp"

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

void MainWindow::on_buttonSearch_clicked()
{
    if( ui->editName->text().isEmpty() )
    {
        QMessageBox::warning( this, tr( "No Title" ), tr( "Please enter a series title to search for" ), QMessageBox::Ok, QMessageBox::Ok );
        return;
    }

    QString strIMDBfile = QDir::toNativeSeparators( QDir::homePath() + tr( "/movies.list" ) );
    QFile fileIMDB( strIMDBfile );

    if( !fileIMDB.exists() )
    {
        QMessageBox::warning( this, tr( "File Not Found" ), tr( "IMDB movie list not found\nPlease download movies.list.gz from IMDB, decompress it and place it in the location below\n" ) + strIMDBfile, QMessageBox::Ok, QMessageBox::Ok );
        return;
    }

    ui->listEpisodes->clear();

    fileIMDB.open( QFile::ReadOnly | QFile::Text );

    int season, episode;
    QTextStream fileIMDBstream( &fileIMDB );
    QString line, name, show = ui->editName->text(), title( tr( "\"" ) + ui->editName->text() + tr( "\"" ) );
    QProgressDialog progress( tr( "Searching file" ), tr( "Abort search" ), 0, fileIMDB.size(), this );
    ShowHolder shows;
    shows.show( show );

    while( !fileIMDBstream.atEnd() )
    {
        line = fileIMDBstream.readLine();
        if( line.startsWith( title, Qt::CaseInsensitive ) )
        {
            season = line.section( '#', 1 ).section( '.', 0, 0 ).toInt();
            episode = line.section( '#', 1 ).section( '.', 1, 1 ).section( ')', 0, 0 ).toInt();
            name = line.section( '{', 1, 1 ).section( '(', 0, 0 ).simplified();
            shows.addEpisode( season, episode, name );
        }

        progress.setValue( fileIMDB.pos() );
        if( progress.wasCanceled() )
            break;
    }

    ui->listEpisodes->addItems( shows.episodeList() );

    fileIMDB.close();
}

void MainWindow::on_editName_returnPressed()
{
    ui->buttonSearch->click();
}
