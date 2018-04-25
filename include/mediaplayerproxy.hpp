/******************************************************************************
 * \filename
 * \brief
 *
 * \details
 *
 * \copyright (c) 2018  Thomas Ruschival <thomas@ruschival.de>
 * \license {This file is licensed under GNU PUBLIC LICENSE Version 2 or later
 * 			 SPDX-License-Identifier: GPL-2.0-or-later}
 *
 *****************************************************************************/

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <QDateTime>
#include <QMediaPlayer>
#include <QObject>
#include <memory>


namespace DigitalRooster {
class PlayableItem;
class PodcastEpisode;
class RadioStream;

/**
 * Wrapper around QMediaPlayer to allow access to the same player instance from
 * QML and C++
 */
class MediaPlayerProxy : public QObject {
    Q_OBJECT
    Q_PROPERTY(qint64 position READ get_position NOTIFY position_changed)
    Q_PROPERTY(qint64 duration READ get_duration NOTIFY duration_changed)
    Q_PROPERTY(int volume READ get_volume WRITE set_volume NOTIFY volume_changed)
    Q_PROPERTY(bool muted READ muted WRITE set_muted NOTIFY muted_changed)
    Q_PROPERTY(bool seekable READ seekable NOTIFY seekable_changed)
	Q_PROPERTY(QMediaPlayer::MediaStatus mediaStatus READ media_status NOTIFY media_status_changed)
	Q_PROPERTY(QMediaPlayer::State playbackState READ playback_state NOTIFY playback_state_changed)

public:
	MediaPlayerProxy();
    ~MediaPlayerProxy() = default;
    MediaPlayerProxy(const MediaPlayerProxy& rhs) = delete;
    MediaPlayerProxy operator=(const MediaPlayerProxy& rhs) = delete;

    /**
     * Incremental seek add increment to current position
	 * will do nothing if not seekable
	 * @param incr increment to add
	 */
	Q_INVOKABLE
	void seek(qint64 incr) {
		if (this->seekable())
			set_position(get_position()+incr);
	}
	;

	/** property access methods */
	bool seekable() const;
	bool muted() const;
	int get_volume() const;
	qint64 get_duration() const;
	qint64 get_position() const;
	QMediaPlayer::MediaStatus media_status()const;
	QMediaPlayer::State playback_state()const;
	/**
	 * Initial player volume
	 */
	const int initial_volume=5;
public slots:
    void set_media(std::shared_ptr<DigitalRooster::PlayableItem> media);
    void set_position(qint64 position);
    void set_muted(bool muted);
    void set_volume(int volume);
    void pause();
	void play();
    void stop();
signals:
    void position_changed(qint64 position);
    void duration_changed(qint64 duration);
    void media_changed(const QMediaContent& media);
    void volume_changed(int volume);
    void muted_changed(bool muted);
    void seekable_changed(bool seekable);
    void media_status_changed(QMediaPlayer::MediaStatus);
    void playback_state_changed(QMediaPlayer::State state);
	void error(QMediaPlayer::Error error);

private:
    /**
     * The actual player implementation
     */
    std::unique_ptr<QMediaPlayer> backend;
	/**
	 * currently selected media (Podcastepisode, RadioStream...)
  	 */
	std::shared_ptr<PlayableItem> current_item;
}; // Player
} // namespace DigitalRooster
#endif // _PLAYER_HPP_