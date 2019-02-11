/******************************************************************************
 * \filename
 * \brief
 *
 * \details
 *
 * \copyright (c) 2018  Thomas Ruschival <thomas@ruschival.de>
 * \license {This file is licensed under GNU PUBLIC LICENSE Version 3 or later
 * 			 SPDX-License-Identifier: GPL-3.0-or-later}
 *
 *****************************************************************************/

#include <QDebug>
#include <QLoggingCategory>
#include <QUrl>
#include <QUuid>

#include "PlayableItem.hpp"

using namespace DigitalRooster;

static Q_LOGGING_CATEGORY(CLASS_LC, "DigitalRooster.PlayableItem");

/***********************************************************************/
PlayableItem::PlayableItem(const QUuid& uid)
    : id(uid){};

/***********************************************************************/

PlayableItem::PlayableItem(
    const QString& name, const QUrl& url, const QUuid& uid)
    : id(uid)
    , display_name(name)
    , media_url(url){};

/***********************************************************************/
void PlayableItem::set_position(qint64 newVal) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;

    if (newVal <= duration && newVal >= 0) {
        position = newVal;
        emit position_updated(newVal);
        emit data_changed();
    }
};

/***********************************************************************/
void PlayableItem::set_duration(qint64 len) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
    if (len >= 0) {
        duration = len;
        emit duration_changed(len);
        emit data_changed();
    }
}

/***********************************************************************/

void PlayableItem::set_url(const QUrl& url) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
    media_url = url;
};
/***********************************************************************/

void PlayableItem::set_title(const QString& title) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
    this->title = title;
    emit display_name_changed(do_get_display_name());
    emit data_changed();
};

/***********************************************************************/
void PlayableItem::set_publisher(const QString& publisher) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
    this->publisher = publisher;
    emit display_name_changed(do_get_display_name());
    emit data_changed();
};

/***********************************************************************/
QString PlayableItem::do_get_display_name() const {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
    if (title.isEmpty() && publisher.isEmpty()) {
        return display_name;
    }
    if (!title.isEmpty() && publisher.isEmpty()) {
        return title;
    }
    if (title.isEmpty() && !publisher.isEmpty()) {
        return publisher;
    }
    return publisher + ": " + title;
}
/***********************************************************************/


/***********************************************************************/
PodcastEpisode::PodcastEpisode(const QString& name, const QUrl& url)
    : PlayableItem(name, url) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
}

/***********************************************************************/
const QString& PodcastEpisode::get_description() const {
    return description;
}

/***********************************************************************/
void PodcastEpisode::set_description(const QString& desc) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
    description = desc;
    emit description_changed(desc);
    emit data_changed();
}

/***********************************************************************/
QString PodcastEpisode::get_guid() const {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
    if (guid.isEmpty()) {
        return get_url().toString();
    }
    return guid;
}

/***********************************************************************/
void PodcastEpisode::set_guid(const QString& uid) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;
    guid = uid;
}

/***********************************************************************/
const QDateTime& PodcastEpisode::get_publication_date() const {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;

    return publication_date;
}

/***********************************************************************/
void PodcastEpisode::set_publication_date(const QDateTime& date) {
    qCDebug(CLASS_LC) << Q_FUNC_INFO;

    if (!date.isValid()) {
        qCCritical(CLASS_LC) << "invalid QDateTime!";
        return;
    }
    publication_date = date;
    emit publication_date_changed(date);
    emit data_changed();
}
/***********************************************************************/
