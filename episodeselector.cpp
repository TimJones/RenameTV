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

/*! \file episodeselector.hpp
    \brief Definition of the EpisodeSelector class
    \file episodeselector.cpp
    \brief Implimentation for the EpisodeSelector class
*/

#include "episodeselector.hpp"
#include "ui_episodeselector.h"

/*! \class EpisodeSelector
    \headerfile episodeselector.hpp
    \brief The GUI window class for selecting episode(s)
*/

/*! \brief Constructor
    \param parent QWidget pointer to the parent QWidget of the MainWindow
*/
EpisodeSelector::EpisodeSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EpisodeSelector)
{
    ui->setupUi(this);
}

/*! \brief Destructor
*/
EpisodeSelector::~EpisodeSelector()
{
    delete ui;
}

/*! \brief Adds a selectable season to the season combo box
    \param season The numerical order of the season to be added
*/
void EpisodeSelector::AddSeason( int const& season )
{
    QString seasonName = tr( "Season %1" ).arg( season );
    ui->comboSeasons->addItem( seasonName );
    m_map.insert( seasonName, season );
}

/*! \brief Adds a selectable episode to the episode list box
    \param episode A string representation of an episode title to be added
*/
void EpisodeSelector::AddEpisode( QString const& episode )
{
    ui->listEpisodes->addItem( episode );
}

/*! \brief Checks for an entire season being selected
    \return True if an entire season or all seasons are selected
*/
bool EpisodeSelector::isSeasonSelected() const
{
    return ui->radioSeason->isChecked();
}

/*! \brief Checks for an episode or selection of episodes being selected
    \return True if an episode or selection of episodes are selected
*/
bool EpisodeSelector::isEpisodeSelected() const
{
    return ui->radioEpisode->isChecked();
}

/*! \brief Get the currently selected season
    \return The index value of the selected season
*/
int EpisodeSelector::GetSelectedSeason() const
{
    return m_map.value( ui->comboSeasons->currentText(), -1 );
}

/*! \brief Checks for an episode or selection of episodes being selected
    \return A list of QStrings of episode titles selected
*/
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
