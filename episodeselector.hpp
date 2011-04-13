#ifndef EPISODESELECTOR_HPP
#define EPISODESELECTOR_HPP

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
};

#endif // EPISODESELECTOR_HPP
