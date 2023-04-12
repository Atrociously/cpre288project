/*
 * buttons.c
 *
 *  Created on: Feb 15, 2023
 *      Author: jackmorr
 */

#include "buttons.h"

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80
#define BTN_PINS    (BIT0 | BIT1 | BIT2 | BIT3)

static int initialized = 0;

void btn_init() {
    if (!initialized) {
        SYSCTL_RCGCGPIO_R |= BIT4;
        while (SYSCTL_PRGPIO_R & ~BIT4 == 0) {}

        GPIO_PORTE_DIR_R &= ~BTN_PINS;
        GPIO_PORTE_DEN_R |= BTN_PINS;
        initialized = 1;
    }

}

btns_t btn_get_buttons() {
    btns_t state = BTN_NONE;
    uint32_t data = GPIO_PORTE_DATA_R;
    if ((data & BIT0) == 0) {
        state |= BUTTON1;
    }
    if ((data & BIT1) == 0) {
        state |= BUTTON2;
    }
    if ((data & BIT2) == 0) {
        state |= BUTTON3;
    }
    if ((data & BIT3) == 0) {
        state |= BUTTON4;
    }

    return state;
}
