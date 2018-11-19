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
 * Hardware init
 * @return
 */

int setup_hardware();

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

#ifdef __cplusplus
} //extern "C"
#endif


#endif /* _HAL_H_ */