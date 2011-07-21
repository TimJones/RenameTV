#ifndef EPISODESELECTOR_HPP
#define EPISODESELECTOR_HPP

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

#include <QMap>
#include <QDialog>
#include <QStringList>

namespace Ui {
    class EpisodeSelector;
}

class EpisodeSelector : public QDialog
{
    Q_OBJECT

public:
    explicit EpisodeSelector(QWidget *parent = 0);
    ~EpisodeSelector();

    void AddSeason( int const& season );
    void AddEpisode( QString const& episode );

    bool isSeasonSelected() const;
    bool isEpisodeSelected() const;
    int GetSelectedSeason() const;
    QStringList GetSelectedEpisodes() const;

private:
    Ui::EpisodeSelector *ui;
    QMap< QString, int > m_map;
};

#endif // EPISODESELECTOR_HPP
