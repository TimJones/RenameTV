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

#include "searchresults.hpp"
#include "ui_searchresults.h"

SearchResults::SearchResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchResults)
{
    ui->setupUi(this);
}

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

SearchResults::~SearchResults()
{
    delete ui;
}

int SearchResults::GetSelectedShowIndex() const
{
    return m_selectedShow;
}

void SearchResults::on_comboNames_currentIndexChanged( int index )
{
    ui->labelStatus->setText( tr( "Status: %1" ).arg( m_shows[ index ].status ) );
    ui->labelStart->setText( tr( "Start: %1" ).arg( m_shows[ index ].start ) );
    ui->labelEnd->setText( tr( "End: %1" ).arg( m_shows[ index ].end ) );
    ui->labelGenre->setText( tr( "Genre(s): %1" ).arg( m_shows[ index ].genre ) );
    m_selectedShow = index;
}
