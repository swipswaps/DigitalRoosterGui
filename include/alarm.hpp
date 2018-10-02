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

#ifndef _ALARM_HPP_
#define _ALARM_HPP_

#include <QDateTime>
#include <QObject>
#include <chrono>
#include <memory>

namespace DigitalRooster {

class PlayableItem;
class TimeProvider;
/**
 * A single alarm
 */
class Alarm : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ is_enabled WRITE enable NOTIFY enabled_changed)
public:
    /**
     * Alarm periodicity
     */
    enum Period {
        Once = 0,   //!< next time the time of day occurs (within 24 hrs)
        Daily,		//!< every day
        Weekend ,	//!< Saturdays & Sundays
        Workdays	//!< Monday through Friday
    };
    Q_ENUM(Period)

    /**
     * Construct an alarm for given time
     * @param media what to play
     * @param timepoint time of day - any time of day
     * @param period periodicity
     * @param enabled activated/deactivated
     * @param parent obligatory QObject parent
     */
    Alarm(const QUrl& media, const QTime& timepoint,
        Alarm::Period period = Alarm::Daily, bool enabled = true,
        QObject* parent = nullptr);

	 /**
     * Construct an alarm for given time and date
     * @param media what to play
     * @param timepoint particular time & date
     * @param period periodicity
     * @param enabled activated/deactivated
     * @param parent obligatory QObject parent
     */
    Alarm(const QUrl& media, const QDateTime& timepoint,
        Alarm::Period period = Alarm::Once, bool enabled = true,
        QObject* parent = nullptr);


    /**
     * Need Default constructor to register with QML
     */
    Alarm()
        : media(nullptr)
        , period(Alarm::Daily)
        , enabled(true){};

    /**
     * next trigger instant when the alarm is to be triggered
     * @return
     */
    const QDateTime& get_next_trigger();

    /**
     * set time of day when the alarm should occur
     * @param timeofday 00:00 to 23:59
     * @param period periodicity
     */
    void set_trigger(const QTime& timeofday, Alarm::Period period);

    /**
     * Set the alarm to trigger once at given timeinsance
     * @param timeinstance when to trigger alarm
     */
    void set_trigger(const QDateTime& timeinstance);

    /**
	 * Trigger time
     * @return time of day when alarm is due
     */
    const QTime get_time() const {
        return trigger_instant.time();
    }
    /**
     * Periodicity of Alarm
     * @return period
     */
    Alarm::Period get_period() const {
        return period;
    }

	/**
	 * Convert the periodicity into a human readable string
	 * @return string representation for enum
	 */
    QString get_period_string() const;

    /**
     * Change periodicity
     */
    void set_period(Alarm::Period period) {
        this->period = period;
        update_trigger();
    };

    /**
     * Volume for this Alarm
     * @return
     */
    int get_volume() {
        return volume;
    }
    /**
     * Update Volume for this Alarm
     * @param volume
     */
    void set_volume(int volume) {
        this->volume = volume;
    }

    /**
     * Duration for alarm to stop automatically
     * @return time in minutes
     */
    std::chrono::minutes get_timeout() {
        return alarmtimeout;
    }
    /**
     * Duration for alarm to stop automatically
     * @param  timeout in minutes
     */
    void set_timeout(std::chrono::minutes timeout) {
        alarmtimeout = timeout;
    }

    /**
     * Alarm media
     */
    std::shared_ptr<PlayableItem> get_media() const {
        return media;
    }

    /**
     * is this alarm set
     * @return state
     */
    bool is_enabled() const {
        return enabled;
    }

public slots:
    /**
     * enable alarm to play next time
     * @param state
     */

    void enable(bool state) {
        enabled = state;
        emit dataChanged();
        emit enabled_changed(true);
    }

signals:
    void enabled_changed(bool state);

	/**
	 * Generic event, some data of this object changed
	 */
	void dataChanged();

private:
    /**
     * What to play when alarm triggers
     */
    std::shared_ptr<PlayableItem> media;

	/**
     * when alarm is repeated
     */
    Alarm::Period period;

    /**
     * Time point when this alarm will trigger
     * The date part of is ignored except for period "Once"
     */
    QDateTime trigger_instant;

    /**
     * Will it trigger?
     */
    bool enabled;
    /**
     * Duration for alarm to stop automatically
     */
    std::chrono::minutes alarmtimeout;

    /**
     * Default volume for alarm
     */
    int volume = 40;
    /**
     * Calculate the upcoming timepoint when the alarm should trigger
     */
    void update_trigger();
};

/**
 * Map a Alarm period literal to enum value
 * @param literal "daily", "once", "weekend" or "workdays"
 * @return enum value
 */
Alarm::Period json_string_to_alarm_period(const QString& literal);

/**
 * Get String for period
 * @param period Alarm::Once "1".... etc
 * @return QString "Once"
 */
QString alarm_period_to_json_string(const Alarm::Period period);

};     // namespace DigitalRooster
#endif // _ALARM_HPP_
