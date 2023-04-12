/*
 * buttons.h
 *
 *  Created on: Feb 15, 2023
 *      Author: jackmorr
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

typedef enum {
    BTN_NONE = 0x00,
    BUTTON1 = 0x01,
    BUTTON2 = 0x02,
    BUTTON3 = 0x04,
    BUTTON4 = 0x08,
} btns_t;

void btn_init();

btns_t btn_get_buttons();

#endif /* BUTTONS_H_ */
