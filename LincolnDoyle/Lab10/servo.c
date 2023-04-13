/*
 * servo.c
 *
 *  Created on: Apr 5, 2021
 *      Author: lincoln hatlestad and doyle chism
 */


#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

/*
 * initialize and configure PWM timer
 */
void servo_init(void) {

    //GPIO INITIALIZATION
    SYSCTL_RCGCGPIO_R |= 0x02; // activate clock for Port B
    while((SYSCTL_PRGPIO_R & 0x02) != 0x02) {}; // for stabilization
    SYSCTL_RCGCTIMER_R |= 0x02; //activate system timer
    while((SYSCTL_RCGCTIMER_R & 0x02) != 0x02) {}; //wait for stability
    GPIO_PORTB_DEN_R |= 0x20; // enable PB5
    GPIO_PORTB_DIR_R |= 0x20; // make PB5 an output
    GPIO_PORTB_AFSEL_R |= 0x20; //  enable alternate function on PB5
    GPIO_PORTB_PCTL_R |= 0x700000;

    //TIMER INITIALIZATION
    SYSCTL_RCGCTIMER_R |= 0x02; //duplicate code
    TIMER1_CTL_R &= ~0x100; //disable timer (might just be 0x100)
    TIMER1_CFG_R |= 0x4; //configs 16-bit timer
    TIMER1_TBMR_R |= 0xA;  //counts down, PWM mode enabled, etc.


    TIMER1_TBILR_R |= 0xE200;  //loads start val
    TIMER1_TBPR_R |= 0x4;     //loads prescaler
    TIMER1_TBMATCHR_R = 0x48400; //loads end val
    TIMER1_TBPMR_R = 0x4;
    TIMER1_CTL_R |= 0x100; //enables timer B and PWM out is inverted
}

/*
 * move function
 * might need a direction parameter to make it go clockwise or counter clockwise
 */
void servo_move(uint16_t degrees) {

    float pulse_width;

    //y = (1/180)x + 1
    pulse_width = (1.0 / 180.0) * degrees + 1; //is in milliseconds

    uint32_t high = (pulse_width / 1000.0) * 16000000 ;
    uint32_t low = 320000 - high;

    //set match values
    TIMER1_TBMATCHR_R = low & 0xFFFF;
    TIMER1_TBPMR_R |= low >> 16; //converts to 16 bit val

    //wait
    timer_waitMillis(500);
}


