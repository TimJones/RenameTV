#include "showholder.hpp"

/*! \class ShowHolder
    \brief ShowHolder: Simple container class for shows

    Blah blah blah long description here.

    \var QString ShowHolder::m_show
    \brief Holds the name of the show

    \var QString ShowHolder::m_textFormat
    \brief Holds the formatting syntax for text output of episode

    \var QList <QStringList> ShowHolder::m_episodes
    \brief This one is going to be a little harder...
*/

/*! \brief Class default constructor

    Initialises members m_show and m_textFormat
*/
ShowHolder::ShowHolder() :
        m_show( "" ),
        m_textFormat( "%show% S%ss%E%ee% %title%" )
{
}

/*! \brief Get the total number of seasons
    \return The number of seasons
*/
int ShowHolder::numSeasons() const
{
    return m_episodes.isEmpty() ? 0 : m_episodes.count();
}

/*! \brief Get the total number of episodes accross all seasons
    \return The total number of episodes
*/
int ShowHolder::numEpisodes() const
{
    int count = 0;

    for( int s = 0; s <= numSeasons(); ++s )
        count += numEpisodes( s );

    return count;
}

/*! \brief Get the number of episodes for a particular season
    \param[in] season The season to fetch an episode count from
*/
int ShowHolder::numEpisodes( const int &season ) const
{
    if( m_episodes.isEmpty() || 1 > season || numSeasons() < season || m_episodes.at( season - 1 ).isEmpty() )
        return 0;
    return m_episodes.at( season - 1 ).count();
}

/*! \brief Get the current text output format
    \return The current test output format
*/
QString ShowHolder::format() const
{
    return m_textFormat;
}

/*! \brief Get the show name
    \return The current show name
*/
QString ShowHolder::show() const
{
    return m_show;
}

/*! \brief Get the formatted episode title
    \param[in] season The season number for the episode
    \param[in] episode The episode number within the season
    \return The formatted episode title

    Get the episode title as text, as currently formatted by ShowHolder::m_textFormat
*/
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

/*! \brief Get a formatted list of all episode titles
    \return A list of the formatted episode titles
*/
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

/*! \brief Get a formatted list of all the episode titles in a season
    \param[in] season The season to get an episode list from
    \return A list of formatted episode titles
*/
QStringList ShowHolder::episodeList( const int &season ) const
{
    QStringList list;
    if( m_episodes.isEmpty() || 1 > season || numSeasons() < season - 1 )
        return list;

    for( int e = 0; e < numEpisodes( season ); ++e )
        list.append( episode( season, e + 1 ) );

    return list;
}

/*! \brief Set the name for the show
    \param[in] name The name of the show
*/
void ShowHolder::show( const QString &name )
{
    m_show = name;
}

/*! \brief Set the formatting syntax for episode titles
    \param[in] format The new style format

    This function defines how the episode titles will be formatted when retriving them.
    \n This is done with a simple string replace:
    \li \c \%show\% will be replaced with the show name (set with ShowHolder::show() )
    \li \c \%title\% will be replaced with the title of the episode
    \li \c \%s\% will be replaced with the season number (see below for more in-depth explanation)
    \li \c \%e\% will be replaced with the episode number (see below for more in-depth explanation)

    For the \c \%s\% and \c \%e\% options, the number of characters inbetween the percent marks dictate the
    minimum 'width' or 'padding' the output number should have.
    \n For example; an episode number of 23:
    \li \c \%e\% will output \c 23
    \li \c \%ee\% will output \c 23
    \li \c \%eee\% will output \c 023
*/
void ShowHolder::format( const QString &format )
{
    m_textFormat = format;
}

/*! \brief Adds an episode title to the show holder
    \param[in] season The season number of the episode
    \param[in] episode The episode number within the specified season
    \param[in] title The title of the episode
    \param[in] override \b Optional: default value: \c false \n A bool to signal if you want to force replacing any current value for the episode title
*/
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
