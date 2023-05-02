/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
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

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){

  // YOUR CODE HERE
    SYSCTL_RCGCGPIO_R |= BIT1;
    SYSCTL_RCGCTIMER_R |= BIT3;

    while ((SYSCTL_PRGPIO_R & BIT1) == 0) {}
    while ((SYSCTL_PRTIMER_R & BIT3) == 0) {}

    GPIO_PORTB_DEN_R |= BIT3;
    GPIO_PORTB_DIR_R |= BIT3;
    GPIO_PORTB_PCTL_R |= 0x7000;

    TIMER3_CTL_R &= ~BIT8;
    TIMER3_CFG_R |= 0x04;
    TIMER3_TBMR_R |= 0b00000111;
    TIMER3_TBMR_R &= ~BIT4;
    TIMER3_CTL_R |= (0x3 << 10);

    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;

    // Configure and enable the timer
    TIMER3_ICR_R |= BIT10;
    TIMER3_IMR_R |= BIT10;
    TIMER3_CTL_R |= BIT8;

    NVIC_PRI9_R = (NVIC_PRI9_R & 0xFFFFFF0F) | 0x0000020;
    NVIC_EN1_R |= BIT4;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();
}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~BIT8;
    TIMER3_IMR_R &= ~BIT10;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~BIT3;

    // trigger start
    GPIO_PORTB_DATA_R |= BIT3;
    timer_waitMicros(2);
    GPIO_PORTB_DATA_R &= ~BIT3;

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= BIT10;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= BIT3;
    TIMER3_IMR_R |= BIT10;
    TIMER3_CTL_R |= BIT8;
}

float ping_distance(void) {
    ping_trigger();

    while (STATE != DONE) {}

    STATE = LOW;
    TIMER3_CTL_R &= ~BIT8;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBV_R |= 0xFFFF;

    // num cycles * ms_per_cycle * speed of sound in cm/ms / 2 for round trip
    return ((0.0625 * (START_TIME - END_TIME)) * 0.0343) / 2.0;
}

void TIMER3B_Handler(void) {
    if (TIMER3_MIS_R & BIT10) {
        uint32_t prescale = TIMER3_TBPV_R << 15;
        uint32_t value = TIMER3_TBV_R;
        uint32_t timer_val = prescale | value;

        if (STATE == LOW) {
            START_TIME = timer_val;
            STATE = HIGH;
        } else if (STATE == HIGH) {
            END_TIME = timer_val;
            STATE = DONE;
        } else {
            // cry
            STATE = LOW;
        }
        TIMER3_ICR_R |= BIT10;
    }

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.

}
