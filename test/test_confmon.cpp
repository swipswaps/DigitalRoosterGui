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
#include <QSignalSpy>

#include "confmon.hpp"
#include "appconstants.hpp"

using namespace DigitalRooster;

TEST(ConfMon, config_file_path) {
    DigitalRooster::ConfMon cm;
    ASSERT_EQ(cm.getCfgPath(), DigitalRooster::SYSTEM_CONFIG_PATH);
}

TEST(ConfMon, sig_conf_changed_emitted) {
    DigitalRooster::ConfMon cm;

    QSignalSpy spy(&cm, SIGNAL(config_changed()));

    cm.sendSignal();

    ASSERT_EQ(spy.count(), 1);
}
