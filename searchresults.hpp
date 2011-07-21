#ifndef SEARCHRESULTS_HPP
#define SEARCHRESULTS_HPP

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

#include <QDialog>
#include <QList>

namespace Ui {
    class SearchResults;
}

class SearchResults : public QDialog
{
    Q_OBJECT

public:
    explicit SearchResults(QWidget *parent = 0);
    explicit SearchResults( QList< ShowDetail > const& shows, QWidget *parent = 0 );
    ~SearchResults();

    void SetShows( QList< ShowDetail > const& shows );
    int GetSelectedShowIndex() const;

private:
    Ui::SearchResults *ui;
    QList< ShowDetail > m_shows;
    int m_selectedShow;

private slots:
    void on_comboNames_currentIndexChanged(int index);
};

#endif // SEARCHRESULTS_HPP
