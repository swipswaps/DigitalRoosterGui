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
#include <QList>
#include <QMediaPlayer>
#include <QSignalSpy>
#include <QUrl>
#include <QVariant>
#include <memory>

#include <gtest/gtest.h>
#include <mediaplayerproxy.hpp>

#include "PlayableItem.hpp"
#include "appconstants.hpp"

using namespace DigitalRooster;

class PlayerFixture : public virtual ::testing::Test {
public:
    PlayerFixture()
        : podcast(
              std::make_unique<DigitalRooster::PodcastEpisode>("TestEpisode",
                  QUrl::fromLocalFile(TEST_FILE_PATH + "/testaudio.mp3"))) {
    }

protected:
    std::shared_ptr<PodcastEpisode> podcast;
    const qint64 desired_pos = 10000; // 10 seconds
    MediaPlayerProxy dut;
};
/*****************************************************************************/
TEST_F(PlayerFixture, emitMediaChanged) {
    QSignalSpy spy(&dut, SIGNAL(media_changed(const QMediaContent&)));
    ASSERT_TRUE(spy.isValid());
    dut.set_media(podcast);
    ASSERT_EQ(spy.count(), 1);
}


/*****************************************************************************/
TEST_F(PlayerFixture, emitStateChanged) {
    QSignalSpy spy(&dut, SIGNAL(playback_state_changed(QMediaPlayer::State)));
    ASSERT_TRUE(spy.isValid());
    dut.set_media(podcast);
    dut.play();
    spy.wait(500);
    dut.pause();
    ASSERT_EQ(spy.count(), 2); // 1 play, 2 pause
}

/*****************************************************************************/
TEST_F(PlayerFixture, stop) {
    QSignalSpy spy(&dut, SIGNAL(playback_state_changed(QMediaPlayer::State)));
    ASSERT_TRUE(spy.isValid());
    dut.set_media(podcast);
    dut.play();
    spy.wait(500);
    ASSERT_EQ(dut.playback_state(), QMediaPlayer::PlayingState);
    dut.stop();
    ASSERT_EQ(spy.count(), 2); // 1 play, 2 stop
    ASSERT_EQ(dut.playback_state(), QMediaPlayer::StoppedState);
}

/*****************************************************************************/
TEST_F(PlayerFixture, setMuted) {
    QSignalSpy spy(&dut, SIGNAL(muted_changed(bool)));
    ASSERT_TRUE(spy.isValid());

    dut.set_media(podcast);
    dut.play();
    dut.set_muted(true);
    dut.pause();
    spy.wait(500);
    ASSERT_TRUE(dut.muted());
    dut.set_muted(false);
    ASSERT_EQ(spy.count(), 2);
    ASSERT_FALSE(dut.muted());
}

/*****************************************************************************/
TEST_F(PlayerFixture, setVolume) {
    QSignalSpy spy(&dut, SIGNAL(volume_changed(int)));
    ASSERT_TRUE(spy.isValid());

    dut.set_media(podcast);
    dut.play();
    dut.set_volume(23);
    ASSERT_EQ(spy.count(), 1);
    ASSERT_LE(
        std::abs(dut.get_volume() - 23LL), 1); // account for rounding errors
}

/*****************************************************************************/
TEST_F(PlayerFixture, checkSeekable) {
	QSignalSpy seekspy(&dut, SIGNAL(seekable_changed(bool)));
	ASSERT_TRUE(seekspy.isValid());
    dut.set_media(podcast);

    dut.play();
    seekspy.wait(1000);
    ASSERT_EQ(seekspy.count(),1);
    ASSERT_TRUE(dut.seekable());

    QSignalSpy spy(&dut, SIGNAL(position_changed(qint64)));
    ASSERT_TRUE(spy.isValid());
    dut.seek(500);
    spy.wait(1000);
    spy.wait(1000);
    ASSERT_GE(spy.count(), 2); // some times position changed was emitted
    ASSERT_GE(dut.get_position(),500); // at least what we seeked
}

/*****************************************************************************/
TEST_F(PlayerFixture, setPositionForward) {
    dut.set_media(podcast);
    dut.play();

    QSignalSpy spy(&dut, SIGNAL(position_changed(qint64)));
    ASSERT_TRUE(spy.isValid());
    auto pos = dut.get_position();
    dut.set_position(pos+500);
    spy.wait(1000);
    spy.wait(1000);
    ASSERT_GE(spy.count(), 2); // some times position changed was emitted
    ASSERT_GE(dut.get_position(),500); // less than 50ms delta
}




