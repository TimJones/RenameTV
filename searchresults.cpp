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

/*! \file searchresults.hpp
    \brief Definition of the SearchResults class
    \file searchresults.cpp
    \brief Implimentation of the SearchResults class
*/

#include "searchresults.hpp"
#include "ui_searchresults.h"

/*! \class SearchResults
    \headerfile searchresults.hpp
    \brief A Window class to show a summery of seach results
*/

/*! \brief Constructor
    \param parent QWidget pointer to the parent QWidget of the MainWindow


*/
SearchResults::SearchResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchResults)
{
    ui->setupUi(this);
}


/*! \brief Constructor
    \param shows A QList of ShowDetail objects to display
    \param parent QWidget pointer to the parent QWidget of the MainWindow
*/
SearchResults::SearchResults( QList< ShowDetail > const& shows, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::SearchResults),
    m_shows( shows )
{
    ui->setupUi(this);
    ui->labelNumber->setText( tr( "Number of results: %1" ).arg( m_shows.count() ) );
    foreach( ShowDetail const& show, m_shows )
        ui->comboNames->addItem( show.name );
    ui->comboNames->setCurrentIndex( 0 );
}

/*! \brief Destructor
*/
SearchResults::~SearchResults()
{
    delete ui;
}

/*! \brief Gets the index of the currently selected show
    \return Integer index of the currently selected show
*/
int SearchResults::GetSelectedShowIndex() const
{
    return m_selectedShow;
}

/*! \brief Called when the selected item in comboName chages
    \param index The newly selected index in comboNames
*/
void SearchResults::on_comboNames_currentIndexChanged( int index )
{
    ui->labelStatus->setText( tr( "Status: %1" ).arg( m_shows[ index ].status ) );
    ui->labelStart->setText( tr( "Start: %1" ).arg( m_shows[ index ].start ) );
    ui->labelEnd->setText( tr( "End: %1" ).arg( m_shows[ index ].end ) );
    ui->labelGenre->setText( tr( "Genre(s): %1" ).arg( m_shows[ index ].genre ) );
    m_selectedShow = index;
}
