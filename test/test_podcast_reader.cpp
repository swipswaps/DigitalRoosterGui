/*************************************************************************************
 * \filename
 * \brief
 *
 * \details
 *
 * \author ruschi
 *
 *************************************************************************************/

#include "PodcastSourceReader.hpp"
#include <QMap>
#include <config.h>
#include <gtest/gtest.h>
#include <stdexcept> // std::system_error

using namespace DigitalRooster;

TEST(PodcastSourceReader, parseInfo_good) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos.rss");
    update_podcast(ps);

    EXPECT_EQ(ps.get_title(), QString("Alternativlos"));
    EXPECT_EQ(ps.get_link().toString(), QString("http://www.alternativlos.org/"));
    EXPECT_STREQ(ps.get_description().toStdString().substr(0, 58).c_str(),
        "Alternativlos ist der Boulevard-Podcast von Frank und Fefe");
}

TEST(PodcastSourceReader, parseInfo_good_element_count) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos.rss");
    update_podcast(ps);
    const auto& episodes = ps.get_episodes();
    EXPECT_EQ(episodes.size(), 27);
}

TEST(PodcastSourceReader, parseInfo_episode_length) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos.rss");
    update_podcast(ps);
    const auto episodes = ps.get_episodes();
    auto title = episodes[20];
    EXPECT_EQ(title->get_length(), 82174997);
}

TEST(PodcastSourceReader, parseInfo_episode_display_name) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos.rss");
    update_podcast(ps);
    const auto episodes = ps.get_episodes();
    auto title = episodes[19];
    EXPECT_EQ(title->get_display_name(),
        QString("ALT022: Korruption"));
}

TEST(PodcastSourceReader, parseInfo_episode_pubdate) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos.rss");
    update_podcast(ps);
    const auto episodes = ps.get_episodes();
    auto title = episodes[19]; // ALT22
    auto testtime = QDateTime::fromString(
        "Mon, 05 Mar 2012 22:00:00 GMT", Qt::DateFormat::RFC2822Date);
    EXPECT_EQ(title->get_publication_date().toTime_t(), testtime.toTime_t());
}

TEST(PodcastSourceReader, parseInfo_episode_url) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos.rss");
    update_podcast(ps);

    const auto episodes = ps.get_episodes();
    EXPECT_STREQ(episodes[1]->get_url().toString().toStdString().c_str(),
        "http://alternativlos.cdn.as250.net/alternativlos-40.mp3");
}

TEST(PodcastSourceReader, parseInfo_bad_nonexistent) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./doesnotexist.rss");
    EXPECT_THROW(update_podcast(ps), std::system_error);
}

TEST(PodcastSourceReader, parseInfo_bad_malformatted) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos_malformatted.rss");
    /* all exceptions should be caught */
    EXPECT_NO_THROW(update_podcast(ps));
}

TEST(PodcastSourceReader, parseInfo_bad_missing_title) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos_bad.rss");
    update_podcast(ps);
    const auto& episodes = ps.get_episodes();
    EXPECT_EQ(episodes.size(), 25);
}

TEST(PodcastSourceReader, parseInfo_bad_missing_url) {
    PodcastSource ps("https://alternativlos.org/alternativlos.rss");
    ps.set_rss_file("./alternativlos_bad.rss");
    update_podcast(ps);
    const auto& episodes = ps.get_episodes();
    EXPECT_EQ(episodes.size(), 25);
}

TEST(PodcastSourceReader, maxEpisodesReached) {
    const size_t maxepisodes = 14;
    PodcastSource ps(
        "https://alternativlos.org/alternativlos.rss"); // has many episodes
    ps.set_rss_file("./alternativlos.rss");
    ps.set_max_episodes(maxepisodes);
    update_podcast(ps);

    const auto& episodes = ps.get_episodes();
    ASSERT_GT(episodes.size(), 1);
    ASSERT_LE(episodes.size(), maxepisodes);
}


