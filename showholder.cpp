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

#include "showholder.hpp"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QtAlgorithms>

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
                results.back().start = xmlData.readElementText();
            else if( xmlData.name() == "ended" )
                results.back().end = xmlData.readElementText();
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
            if( xmlData.name() == "Special" )
                xmlData.skipCurrentElement();
            else if( xmlData.name() == "Season" )
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
