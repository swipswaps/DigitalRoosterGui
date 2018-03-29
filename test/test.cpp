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

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#include <QLoggingCategory>

TEST(Basics,always_true)
{
	ASSERT_TRUE(true);
}

/**
 * see : https://stackoverflow.com/questions/33829949/how-to-use-qtimers-in-googletest
 */

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QLoggingCategory::setFilterRules("*.debug=true");

    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();

    QTimer exitTimer;
    QObject::connect(&exitTimer, &QTimer::timeout, &app, QCoreApplication::quit);
    exitTimer.start();
    app.exec();
    return ret;
}
