/*
 * servo.c
 *
 *  Created on: Apr 11, 2023
 *      Author: jackmorr
 */

#include "servo.h"
#include <stdint.h>
#include <math.h>

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000

#define MICROS_PER_CYCLE 6.25e-5

void set_pwm(float high_millis, float low_millis) {
    uint32_t high_cycles = floorf(high_millis / MICROS_PER_CYCLE);
    uint32_t low_cycles = floorf(low_millis / MICROS_PER_CYCLE);

    uint32_t high_start = low_cycles + high_cycles;
    uint32_t low_start = low_cycles;

    lcd_printf("%x, %x", high_start, low_start);

    // set prescaler start
    TIMER1_TBPR_R = (high_start >> 4);
    // set timer start
    TIMER1_TBILR_R = (high_start & 0xFFFF);

    // set match value for low time
    // set prescale match
    TIMER1_TBPMR_R = (low_start >> 4);
    // set timer match
    TIMER1_TBMATCHR_R = (low_start & 0xFFFF);

    //lcd_printf("%x, %x", high_start, low_start);
}

void servo_init(void) {
    SYSCTL_RCGCGPIO_R |= BIT1;
    SYSCTL_RCGCTIMER_R |= BIT1;

    while ((SYSCTL_PRGPIO_R & BIT1) != BIT1) {}
    while ((SYSCTL_PRTIMER_R & BIT1) != BIT1) {}

    GPIO_PORTB_DEN_R |= BIT5;
    GPIO_PORTB_DIR_R |= BIT5;
    GPIO_PORTB_AFSEL_R |= BIT5;
    GPIO_PORTB_PCTL_R |= 0x700000;


    // configure timer for pwm
    TIMER1_CTL_R &= ~BIT8; // disable timer
    TIMER1_CFG_R |= 0x4;
    TIMER1_TBMR_R |= 0xA;

    TIMER1_TBILR_R = 0xE200;
    TIMER1_TBPR_R = 0x4;
    TIMER1_TBMATCHR_R = 0xE200;
    TIMER1_TBPMR_R = 0x4;

    //set_pwm(0.75, 20.0);
    TIMER1_CTL_R |= BIT8; // re-enable timer
}

void servo_move(float high) {
    TIMER1_CTL_R &= ~BIT8; // disable timer
    set_pwm(high, 20.0);
    TIMER1_CTL_R |= BIT8; // re-enable timer

    timer_waitMillis(2000);
}
