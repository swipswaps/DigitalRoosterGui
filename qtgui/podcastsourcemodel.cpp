/*************************************************************************************
 * \filename
 * \brief
 *
 * \details
 *
 * \author ruschi
 *
 *************************************************************************************/
#include <QByteArray>
#include <QHash>

#include "PodcastSource.hpp"
#include "configuration_manager.hpp"
#include "podcastsourcemodel.hpp"

using namespace DigitalRooster;
/*************************************************************************************/

PodcastSourceModel::PodcastSourceModel(ConfigurationManager* confman, QObject* parent)
    : QAbstractListModel(parent)
    , cm(confman) {
	auto v = cm->get_podcast_sources();
	for(auto ps : v){
		ps->set_updateInterval(100);
		connect(ps.get(),SIGNAL(newDataAvailable()),this,SLOT(newDataAvailable()));
	}
}
/*************************************************************************************/

QHash<int, QByteArray> PodcastSourceModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DisplayNameRole] = "display_name";
    roles[DisplayUrlRole] = "link";
    roles[DisplayCountRole] = "episode_count";
    roles[DescriptionRole] = "description";
    roles[ImageRole] = "logo_image";
    return roles;
}
/*************************************************************************************/

int PodcastSourceModel::rowCount(const QModelIndex& /*parent */) const {
	return cm->get_podcast_sources().size();
}

/*************************************************************************************/

void PodcastSourceModel::newDataAvailable() {
     emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0));
}
/*************************************************************************************/

QVariant PodcastSourceModel::data(const QModelIndex& index, int role) const {
    auto v = cm->get_podcast_sources();
    if (index.row() < 0 || index.row() >= v.size())
        return QVariant();

    auto ps = v[index.row()];

    switch(role){
    case DisplayNameRole:
        return QVariant(ps->get_title());
    case DisplayUrlRole:
        return ps->get_url();
    case DisplayCountRole:
    	return ps->get_episodes().size();
    case DescriptionRole:
        return ps->get_description();
    case ImageRole:
           return ps->get_image_uri();
    }
    return QVariant();
}