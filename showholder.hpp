#ifndef SHOWHOLDER_HPP
#define SHOWHOLDER_HPP

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

#include <QDate>
#include <QList>
#include <QString>

struct ShowDetail
{
    int id;
    QString name;
    QString start;
    QString end;
    QString status;
    QString genre;
};

struct EpisodeDetail
{
    QString show;
    QString title;
    int season;
    int episode;
    int show_episode;

    bool operator<( EpisodeDetail const& rhs ) const
    {
        return show_episode < rhs.show_episode || season < rhs.season || ( season == rhs.season && episode < rhs.episode );
    }
};

class ShowHolder
{
public:
    ShowHolder() {}

    QList< ShowDetail > Search( QString const& showName );
    QList< EpisodeDetail > Fetch( ShowDetail const& showDetail );
};

#endif // SHOWHOLDER_HPP
