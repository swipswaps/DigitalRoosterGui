/*************************************************************************************
 * \filename
 * \brief
 *
 * \details
 *
 * \author ruschi
 *
 *
 *************************************************************************************/

#ifndef _PLAYABLEITEM_HPP_
#define _PLAYABLEITEM_HPP_

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QUrl>

namespace DigitalRooster {
/**
 * Interface of Playable Item : self contained information to play
 */
class PlayableItem : public QObject{
	Q_OBJECT
	Q_PROPERTY(QString display_name READ get_display_name WRITE set_display_name)
	Q_PROPERTY(QUrl url READ get_url WRITE set_url)
public:
    /**
     * Default Constructor
     */
    PlayableItem() = default;

    /**
     * Convenience Constructor
     * @param name display_name
     * @param url media_url
     */
    PlayableItem(const QString& name, const QUrl& url)
        : display_name(name)
        , media_url(url){};

    const QString& get_display_name() {
        return display_name;
    };

    const QUrl& get_url() {
        return media_url;
    };

    void set_display_name(const QString& name) {
        display_name = name;
    };


    void set_url(const QUrl& url) {
        media_url = url;
    };

private:
    /** human readable name*/
    QString display_name;

    /** Media URL */
    QUrl media_url;
};

/**
 * Media source with random access in time, player can go back and forth
 */
class SeekablePlayableItem : public PlayableItem {

public:
    SeekablePlayableItem() = default;

    /**
     * Convienience Constructor
     * @param name
     * @param url
     */
    SeekablePlayableItem(const QString& name, const QUrl& url)
        : PlayableItem(name, url){

          };
    /**
     * Has it been played completely?
     */
    virtual bool completed() = 0;
    /**
     * Percentage of already listened content
     */
    int get_position_percent() {
        return position;
    };
    /**
     * update percentage of already listened content
     * @param newVal current position in stream
     */
    void set_position_percent(int newVal) {
        position = newVal;
    };


    /**
     * Total length of media in ms
     * @return
     */
    int get_length() {
        return length;
    }

    /**
     * Update length in ms
     * @param len >= 0
     */
    void set_length(int len) {
        if (len >= 0)
            length = len;
    }

    virtual ~SeekablePlayableItem() = default;

private:
    /**
     * Current Position in stream
     */
    int position = 0;

    /**
     * Total length in ms
     */
    int length = 0;
};


class RadioStream : public PlayableItem {
public:
    RadioStream(const QString& name, const QUrl& url)
        : PlayableItem(name, url){

          };
    virtual ~RadioStream() = default;
};


/**
 * PodcastEpisode = item of RSS feed
 */
class PodcastEpisode : public SeekablePlayableItem {

public:
    PodcastEpisode() = default;

    /**
     * Convenience constructor
     * @param name
     * @param url
     */
    PodcastEpisode(const QString& name, const QUrl& url)
        : SeekablePlayableItem(name, url){

          };
    virtual ~PodcastEpisode() = default;


    /**
     * Has it been played completely?
     */
    virtual bool completed() override {
        return false; // TODO
    };


    void set_author(const QString& newAuthor) {
        author = newAuthor;
    };


    const QString& get_author() {
        return author;
    }


    const QString& get_description() {
        return description;
    }

    void set_description(const QString& desc) {
        description = desc;
    };

    QString get_guid() {
    	if(guid.isEmpty()){
    		return get_url().toString();
    	}
        return guid;
    }

    void set_guid(const QString& uid) {
        guid = uid;
    };

    const QDateTime& get_publication_date() {
        return publication_date;
    }

    void set_publication_date(const QDateTime& date) {
        if (date.isValid())
            publication_date = date;
    };

private:
    /**
     * Author of the episode
     */
    QString author;
    /**
     * Synopsis of this episode
     */
    QString description;
    /**
     * Global Unique ID of item
     */
    QString guid;
    /**
     * Release date of episode (item)
     */
    QDateTime publication_date;
};
};
#endif // _PLAYABLEITEM_HPP_