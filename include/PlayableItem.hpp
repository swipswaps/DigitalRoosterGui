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

#ifndef _PLAYABLEITEM_HPP_
#define _PLAYABLEITEM_HPP_

#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QUuid>

namespace DigitalRooster {
/**
 * Interface of Playable Item : self contained information to play
 */
class PlayableItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QString display_name READ get_display_name NOTIFY display_name_changed)
    Q_PROPERTY(QString title READ get_title WRITE set_title NOTIFY
            display_name_changed)
    Q_PROPERTY(QString publisher READ get_publisher WRITE set_publisher NOTIFY
            display_name_changed)
    Q_PROPERTY(QUrl url READ get_url WRITE set_url)
    Q_PROPERTY(QUuid id READ get_id)
    Q_PROPERTY(int position READ get_position WRITE set_position)
    Q_PROPERTY(int duration READ get_duration WRITE set_duration NOTIFY
            duration_changed)
public:
    /**
     * Default Constructor
     */
    explicit PlayableItem(const QUuid& uid = QUuid::createUuid());

    /**
     * Convenience Constructor
     * @param name display_name
     * @param url media_url
     */
    PlayableItem(const QString& name, const QUrl& url,
        const QUuid& uid = QUuid::createUuid());

    /**
     * Information on title, publisher ,etc.
     * @return  display_name
     */
    QString get_display_name() const {
        return do_get_display_name();
    };

    /**
     * Stream source URI
     * @return media uri
     */
    const QUrl& get_url() const {
        return media_url;
    };

    void set_url(const QUrl& url);

    /**
     * last stored position
     */
    qint64 get_position() const {
        return position;
    };
    /**
     * update position of already listened content
     * @param newVal current position in stream
     */
    void set_position(qint64 newVal);

    /**
     * Title for Playable item
     * @return title
     */
    QString get_title() const {
        return title;
    };
    void set_title(const QString& title);

    /**
     * Publisher/Radio station of this playable item
     * @return publisher
     */
    QString get_publisher() const {
        return publisher;
    };
    void set_publisher(const QString& publisher);

    /**
     * unique id for this Item
     * @return
     */
    QUuid get_id() const {
        return id;
    }

    /**
     * Total length of media in ms
     * @return
     */
    qint64 get_duration() const {
        return duration;
    }

    /**
     * Update length in ms only used for display purpose
     * @param len >= 0
     */
    void set_duration(qint64 len);

signals:
    void display_name_changed(const QString& info);
    void duration_changed(qint64 duration);
    void position_updated(qint64 newpos);
    /**
     * Any information changed
     */
    void data_changed();

private:
    /**
     * 'unique' id for this object, info managed by configuration_manager
     */
    const QUuid id;

    /** initial human assigned display name*/
    QString display_name;

    /** Title */
    QString title;

    /** publisher/author/station name */
    QString publisher;

    /** Media URL */
    QUrl media_url;

    /**
     * Current Position in stream
     */
    qint64 position = 0;

    /**
     * Total length in ms
     */
    qint64 duration = 0;

    /**
     * Human information dynamically build
     * @return
     */
    virtual QString do_get_display_name() const;
};

/**
 * PodcastEpisode = item of RSS feed
 */
class PodcastEpisode : public PlayableItem {
    Q_OBJECT
    Q_PROPERTY(
        QString description READ get_description NOTIFY description_changed)
public:
    PodcastEpisode() = default;

    /**
     * Convenience constructor
     * @param name
     * @param url
     */
    PodcastEpisode(const QString& name, const QUrl& url);

    virtual ~PodcastEpisode() = default;

    /**
     * Human readable description written by Episode publisher assinged during
     * RSS parsing
     * @param desc description
     */
    void set_description(const QString& desc);
    const QString& get_description() const;

    /**
     * Podcast publisher assinged unique ID can be URL or UUID formatted string
     * @param uid new unique ID
     */
    void set_guid(const QString& uid);
    QString get_guid() const;

    /**
     * Publication date of podcast episode
     * @param date date&time
     */
    void set_publication_date(const QDateTime& date);
    const QDateTime& get_publication_date() const;

signals:
    void description_changed(const QString& desc);
    void publication_date_changed(const QDateTime& datetime);

private:
    /**
     * Synopsis of this episode
     */
    QString description;
    /**
     * Global Unique ID of podcast, assigned by publisher in RSS
     * Do not confuse with PlayableItem.id
     */
    QString guid;
    /**
     * Release date of episode (item)
     */
    QDateTime publication_date;
};
};     // namespace DigitalRooster
#endif // _PLAYABLEITEM_HPP_
