/******************************************************************************
 * \filename
 * \brief		Hardware abstraction interface functions
 *
 * \details		implementaion is target specific
 *
 * \copyright (c) 2018  Thomas Ruschival <thomas@ruschival.de>
 * \license {This file is licensed under GNU PUBLIC LICENSE Version 3 or later
 * 			 SPDX-License-Identifier: GPL-3.0-or-later}
 *
 *****************************************************************************/

#ifndef _HAL_H_
#define _HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Simplified linux/input event to allow portable code
 */
struct ScrollEvent {
    enum Direction {
        DOWN = -1,
		UP = 1
    };
    Direction dir;
    int code;
    int value;
    int type;
};


/**
 * Hardware init
 * @return
 */

int setup_hardware();

/**
 * Setup GPIO pin for push_button
 * @param gpio number for button
 */
int setup_gpio_pushbutton(int gpio);

/**
 * Power management functions
 */
int system_reboot();
int system_poweroff();

/**
 * Set brightness of display on target
 * @param brightness 0..100 %
 */
int set_brightness(int brightness);

/**
 * Read scroll_event data from filedescriptor
 */
ScrollEvent get_scroll_event(int filedescriptor);

/**
 * Read value of push-button (rotary switch)
 * @param filehandle
 * @return 0/1
 */
int get_pushbutton_value(int filedescriptor);


#ifdef __cplusplus
} // extern "C"
#endif


#endif /* _HAL_H_ */
