/*
 * servo.c
 *
 *  Created on: Apr 11, 2023
 *      Author: jackmorr
 */

#include "servo.h"
#include <stdint.h>
#include <math.h>
#include "Timer.h"

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

#define PULSE_TIME_MS 20.0
#define MICROS_PER_CYCLE 6.25e-5

void set_pwm(float pulse_time_ms, float high_time_ms) {
    uint32_t low_cycles = floorf(pulse_time_ms / MICROS_PER_CYCLE);
    uint32_t high_cycles = floorf(high_time_ms / MICROS_PER_CYCLE);

    // calculate the timer value at which the pwm signal should go low
    uint32_t low_point = low_cycles - high_cycles;

    // this sets the maximum pulse time
    // set prescale start
    TIMER1_TBPR_R = (low_cycles >> (4 * 4));
    // set start value
    TIMER1_TBILR_R = (low_cycles & 0xFFFF);

    // this sets the time to go low at
    // set the prescale match
    TIMER1_TBPMR_R = (low_point >> (4 * 4));
    // set the match value
    TIMER1_TBMATCHR_R = (low_point & 0xFFFF);
}

float angle_to_pwm(float angle) {
    #define ANGLE_MAX 180
    #define ANGLE_MIN 0
    angle = angle > ANGLE_MAX ? ANGLE_MAX : angle;
    angle = angle < ANGLE_MIN ? ANGLE_MIN : angle;

    float angle_range = (ANGLE_MAX - ANGLE_MIN);
    float pwm_range = (servo_pwm_max - servo_pwm_min);

    return (((angle - ANGLE_MIN) * pwm_range) / angle_range) + servo_pwm_min;
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

    set_pwm(PULSE_TIME_MS, 0.5);
    TIMER1_CTL_R |= BIT8; // re-enable timer
}

void servo_move(float angle) {
    TIMER1_CTL_R &= ~BIT8; // disable timer
    set_pwm(PULSE_TIME_MS, angle_to_pwm(angle));
    TIMER1_CTL_R |= BIT8; // re-enable timer
    timer_waitMillis(10);
}
