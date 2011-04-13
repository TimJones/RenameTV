#include "episodeselector.hpp"
#include "ui_episodeselector.h"

EpisodeSelector::EpisodeSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EpisodeSelector)
{
    ui->setupUi(this);
}

EpisodeSelector::~EpisodeSelector()
{
    delete ui;
}

void EpisodeSelector::AddSeason( int const& season )
{
    QString seasonName = tr( "Season %1" ).arg( season );
    ui->comboSeasons->addItem( seasonName );
    m_map.insert( seasonName, season );
}

void EpisodeSelector::AddEpisode( QString const& episode )
{
    ui->listEpisodes->addItem( episode );
}

bool EpisodeSelector::isSeasonSelected() const
{
    return ui->radioSeason->isChecked();
}

bool EpisodeSelector::isEpisodeSelected() const
{
    return ui->radioEpisode->isChecked();
}

int EpisodeSelector::GetSelectedSeason() const
{
    return m_map.value( ui->comboSeasons->currentText(), -1 );
}

QStringList EpisodeSelector::GetSelectedEpisodes() const
{
    QStringList list;
    // Problem:
    // QListWidget.selectedItems() returns the order the user selected
    // the items, not the oder they appear in the list. Hence the need
    // for this construct :-S

    for( int i = 0; i < ui->listEpisodes->count(); i++ )
    {
        if( ui->listEpisodes->item( i )->isSelected() )
            list.append( ui->listEpisodes->item( i )->text() );
    }

    return list;
}
