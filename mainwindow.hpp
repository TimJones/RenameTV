#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

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

#include <QMainWindow>
#include <QModelIndex>
#include <QListWidgetItem>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString m_lastDir;
    QList< EpisodeDetail > m_episodes;

    QString FormatEpisode( EpisodeDetail const& episode ) const;

private slots:
    void on_listFiles_itemSelectionChanged();
    void on_buttonRename_clicked();
    void on_buttonRemoveEpisode_clicked();
    void on_buttonEditEpisode_clicked();
    void on_listEpisodes_itemSelectionChanged();
    void on_buttonAddEpisodes_clicked();
    void on_editName_textChanged(QString const& text);
    void on_buttonRemoveFiles_clicked();
    void on_buttonAddFiles_clicked();
    void on_buttonSearch_clicked();
};

#endif // MAINWINDOW_HPP
