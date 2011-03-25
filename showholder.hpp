#ifndef SHOWHOLDER_HPP
#define SHOWHOLDER_HPP

#include <QDate>
#include <QList>
#include <QString>

struct ShowDetail
{
    int id;
    QString name;
    QDate start;
    QDate end;
    QString status;
    QString genre;
};

struct EpisodeDetail
{
    QString show;
    QString title;
    int season;
    int episode;
    int show_episode;

    bool operator<( EpisodeDetail const& rhs ) const
    {
        return show_episode < rhs.show_episode || season < rhs.season || ( season == rhs.season && episode < rhs.episode);
    }
};

class ShowHolder
{
public:
    ShowHolder() {}

    QList< ShowDetail > Search( QString const& showName );
    QList< EpisodeDetail > Fetch( ShowDetail const& showDetail );
};

#endif // SHOWHOLDER_HPP
