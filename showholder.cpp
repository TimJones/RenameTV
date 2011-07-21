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

/*! \file showholder.hpp
    \brief Definition for ShowHolder class and EpisodeDetail and ShowDetail structs
    \file showholder.cpp
    \brief Implimentation for ShowHolder class

    \struct ShowDetail
    \brief Holds summary information of a show
    \property ShowDetail::id
    \brief An ID of the particular show
    \property ShowDetail::name
    \brief The show name
    \property ShowDetail::start
    \brief A string representation of the show start date
    \property ShowDetail::end
    \brief A string representation of the show end date
    \property ShowDetail::status
    \brief The current status of the show (airing, renewed, ended, etc)
    \property ShowDetail::genre
    \brief The genre(s) of the show

    \struct EpisodeDetail
    \brief Holds summary information of an episode
    \property EpisodeDetail::show
    \brief The name of the show the episode is from
    \property EpisodeDetail::title
    \brief The title of the episode
    \property EpisodeDetail::season
    \brief The season of the show to which the episode belongs
    \property EpisodeDetail::episode
    \brief The index of the episode within the its season
    \property EpisodeDetail::show_episode
    \brief The index of the episode within the entire show (all seasons)S
*/

#include "showholder.hpp"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QtAlgorithms>

/*! \class ShowHolder
    \headerfile showholder.hpp
    \brief An interface class to store and retrieve show and episode information
*/

/*! \brief Searches for a given show name
    \param showName The show name to search for
    \return A QList of ShowDetail containing the show information of search results
    \sa ShowDetail

    Using QNetworkAccessManager and QNetworkReply, a blocking connection is made
    to an XML service provider (in this case TVRage), for the results of the user
    submitted search.
    Once the XML data is retrived, it is parsed with QXmlStreamReader and
    necessary ShowDetial objects are contructed and returned.
*/
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

/*! \brief Retrieves the episode data for a given series
    \param showDetail A ShowDetail object for which to retrieve an episode listing
    \return A QList of EpisodeDetail for the show requested
    \sa ShowDetail EpisodeDetail

    Using QNetworkAccessManager and QNetworkReply, a blocking connection is made
    to an XML service provider (in this case TVRage), for the episode listing of
    the show.
    Once the XML data is retrived, it is parsed with QXmlStreamReader and
    necessary EpisodeDetial objects are contructed and returned.
*/
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
