#include "showholder.hpp"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QtAlgorithms>
#include <QEventLoop>

QList< ShowDetail > ShowHolder::Search( QString const& showName )
{
    QEventLoop waitLoop;
    QNetworkAccessManager netManager;
    QNetworkReply* netReply = netManager.get( QNetworkRequest( QUrl( "http://services.tvrage.com/feeds/search.php?show=" + showName ) ) );
    QObject::connect( netReply, SIGNAL( finished() ), &waitLoop, SLOT( quit() ) );
    waitLoop.exec();

    QList< ShowDetail > results;
    QXmlStreamReader xmlData;
    xmlData.addData( netReply->readAll() );

    while( !xmlData.atEnd() )
    {
        if( xmlData.readNextStartElement() )
        {
            if( xmlData.name() == "show" )
                results.append( ShowDetail() );
            else if( xmlData.name() == "showid" )
                results.back().id = xmlData.readElementText().toInt();
            else if( xmlData.name() == "name" )
                results.back().name = xmlData.readElementText();
            else if( xmlData.name() == "started" )
                results.back().start = QDate::fromString( xmlData.readElementText() );
            else if( xmlData.name() == "ended" )
                results.back().end = QDate::fromString( xmlData.readElementText() );
            else if( xmlData.name() == "status" )
                results.back().status = xmlData.readElementText();
            else if( xmlData.name() == "genre" )
            {
                if( !results.back().genre.isEmpty() )
                    results.back().genre += "; ";
                results.back().genre += xmlData.readElementText();
            }
        }
    }

    netReply->deleteLater();

    return results;
}

QList< EpisodeDetail > ShowHolder::Fetch( ShowDetail const& showDetail )
{
    QEventLoop waitLoop;
    QNetworkAccessManager netManager;
    QNetworkReply* netReply = netManager.get( QNetworkRequest( QUrl( "http://services.tvrage.com/feeds/episode_list.php?sid=" + QString::number( showDetail.id ) ) ) );
    QObject::connect( netReply, SIGNAL( finished() ), &waitLoop, SLOT( quit() ) );
    waitLoop.exec();

    int currentSeason = 0;
    QList< EpisodeDetail > episodes;
    QXmlStreamReader xmlData;
    xmlData.addData( netReply->readAll() );

    while( !xmlData.atEnd() )
    {
        if( xmlData.readNextStartElement() )
        {
            if( xmlData.name() == "Season" )
                currentSeason = xmlData.attributes().value( "no" ).toString().toInt();
            else if( xmlData.name() == "episode" )
            {
                episodes.append( EpisodeDetail() );
                episodes.back().show = showDetail.name;
                episodes.back().season = currentSeason;
            }
            else if( xmlData.name() == "epnum" )
                episodes.back().show_episode = xmlData.readElementText().toInt();
            else if( xmlData.name() == "seasonnum" )
                episodes.back().episode = xmlData.readElementText().toInt();
            else if( xmlData.name() == "title" )
                episodes.back().title = xmlData.readElementText();
        }
    }

    netReply->deleteLater();

    qSort( episodes );

    return episodes;
}
