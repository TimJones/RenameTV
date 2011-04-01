#ifndef SEARCHRESULTS_HPP
#define SEARCHRESULTS_HPP

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
