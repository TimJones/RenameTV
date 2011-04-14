#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

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

    bool CanRename() const;
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
