#ifndef SHOWHOLDER_HPP
#define SHOWHOLDER_HPP

#include <QList>
#include <QString>
#include <QStringList>

class ShowHolder
{
public:
    ShowHolder();

    int numSeasons() const;
    int numEpisodes() const;
    int numEpisodes( const int &season ) const;
    QString episode( const int &season, const int &episode ) const;
    QStringList episodeList() const;
    QStringList episodeList( const int &season ) const;

    void addEpisode( const int &season, const int &episode, const QString &title, const bool &override = false );

protected:
    QList <QStringList> m_episodes;
};

#endif // SHOWHOLDER_HPP