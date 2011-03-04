#include "showholder.hpp"

ShowHolder::ShowHolder() :
        m_textFormat( "%show% S%ss%E%ee% %title%" )
{
}

int ShowHolder::numSeasons() const
{
    return m_episodes.isEmpty() ? 0 : m_episodes.count();
}

int ShowHolder::numEpisodes() const
{
    int count = 0;

    for( int s = 0; s <= numSeasons(); ++s )
        count += numEpisodes( s );

    return count;
}

int ShowHolder::numEpisodes( const int &season ) const
{
    if( m_episodes.isEmpty() || 1 > season || numSeasons() < season || m_episodes.at( season - 1 ).isEmpty() )
        return 0;
    return m_episodes.at( season - 1 ).count();
}

QString ShowHolder::format() const
{
    return m_textFormat;
}

QString ShowHolder::show() const
{
    return m_show;
}

QString ShowHolder::episode( const int &season, const int &episode ) const
{
    if( m_episodes.isEmpty() || 1 > season || numSeasons() < season || m_episodes.at( season - 1 ).isEmpty() || m_episodes.at( season - 1 ).count() < episode - 1 )
        return QString( "" );

    int index = 0;
    QString formattedName = m_textFormat;

    formattedName.replace( "%show%", m_show );
    formattedName.replace( "%title%", m_episodes.at( season - 1 ).at( episode - 1 ) );

    // Replace any occurance of %eee% (with any number of 'e's e.g. %e% %ee% %eeeeeeeeee%) with the padded episode number
    // e.g. %e%     => 1 or 23 or 456
    //      %ee%    => 01 or 23 or 456
    //      %eeeee% => 00001 or 00023 or 00456
    while( ( index = formattedName.indexOf( QRegExp( "%e+%" ) ) ) != -1 )
    {
        int width = 0;
        while( formattedName.at( index + width + 1 ) != '%' )
            width++;

        formattedName.replace( index, width + 2, QString( "%1" ).arg( episode, width, 10, QChar( '0' ) ) );
    }

    // and the same with the season number
    while( ( index = formattedName.indexOf( QRegExp( "%s+%" ) ) ) != -1 )
    {
        int width = 0;
        while( formattedName.at( index + width + 1 ) != '%' )
            width++;

        formattedName.replace( index, width + 2, QString( "%1" ).arg( season, width, 10, QChar( '0' ) ) );
    }

    return formattedName;
}

QStringList ShowHolder::episodeList() const
{
    QStringList all;

    for( int s = 0; s < numSeasons(); ++s )
    {
        if( !m_episodes.at( s ).isEmpty() )
            all.append( episodeList( s + 1 ) );
    }

    return all;
}

QStringList ShowHolder::episodeList( const int &season ) const
{
    QStringList list;
    if( m_episodes.isEmpty() || 1 > season || numSeasons() < season - 1 )
        return list;

    for( int e = 0; e < numEpisodes( season ); ++e )
        list.append( episode( season, e + 1 ) );

    return list;
}

void ShowHolder::show( const QString &name )
{
    m_show = name;
}

void ShowHolder::format( const QString &format )
{
    m_textFormat = format;
}

void ShowHolder::addEpisode( const int &season, const int &episode, const QString &title, const bool &override )
{
    if( season < 1 || episode < 1 )
        return;

    QString emptyTitle( "" );
    QStringList emptyList;

    if( m_episodes.isEmpty() )
        m_episodes.append( emptyList );

    if( m_episodes.count() <= season - 1 )
    {
        for( int s = m_episodes.count(); s <= season - 1; ++s )
        {
            m_episodes.append( emptyList );
            m_episodes.last().append( emptyTitle );
        }
    }

    if( m_episodes.at( season - 1 ).count() <= episode - 1 )
    {
        for( int e = m_episodes.at( season - 1 ).count(); e <= episode - 1; ++e )
            m_episodes[ season - 1].append( emptyTitle );
    }

    if( override || m_episodes.at( season - 1 ).at( episode - 1).isEmpty() )
        m_episodes[ season - 1 ][ episode - 1 ] = title;
}
