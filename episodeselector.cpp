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

#include "episodeselector.hpp"
#include "ui_episodeselector.h"

EpisodeSelector::EpisodeSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EpisodeSelector)
{
    ui->setupUi(this);
}

EpisodeSelector::~EpisodeSelector()
{
    delete ui;
}

void EpisodeSelector::AddSeason( int const& season )
{
    QString seasonName = tr( "Season %1" ).arg( season );
    ui->comboSeasons->addItem( seasonName );
    m_map.insert( seasonName, season );
}

void EpisodeSelector::AddEpisode( QString const& episode )
{
    ui->listEpisodes->addItem( episode );
}

bool EpisodeSelector::isSeasonSelected() const
{
    return ui->radioSeason->isChecked();
}

bool EpisodeSelector::isEpisodeSelected() const
{
    return ui->radioEpisode->isChecked();
}

int EpisodeSelector::GetSelectedSeason() const
{
    return m_map.value( ui->comboSeasons->currentText(), -1 );
}

QStringList EpisodeSelector::GetSelectedEpisodes() const
{
    QStringList list;
    // Problem:
    // QListWidget.selectedItems() returns the order the user selected
    // the items, not the oder they appear in the list. Hence the need
    // for this construct :-S

    for( int i = 0; i < ui->listEpisodes->count(); i++ )
    {
        if( ui->listEpisodes->item( i )->isSelected() )
            list.append( ui->listEpisodes->item( i )->text() );
    }

    return list;
}
