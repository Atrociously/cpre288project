/**
 * Driver for ping sensor
 * @file ping_template.c
 * @author lincoln hatlestad and doyle chism
 */

#include "ping_template.h"
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){


    //part 1 GPIO initilization
    SYSCTL_RCGCGPIO_R |= 0x02; //activate clock for PB (0x02, bit 2)
    while((SYSCTL_PRGPIO_R & 0x02) != 0x02) {}; //more clock shit
    SYSCTL_RCGCTIMER_R |= 0x08; // enables the clock time
    while((SYSCTL_PRTIMER_R & 0x08) == 0){}; // wait for TImer 3B to initialize

    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_DIR_R |= 0x08; //PB3
    GPIO_PORTB_AFSEL_R |= 0x08; //~0X80 &=01111111
    GPIO_PORTB_DATA_R |= 0x02; //pin acts as low-high-low

    //GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0x00007000) | 0x00007000; // clears then sets the alt to the T3CCP1
    GPIO_PORTB_PCTL_R |= 0x7000;

    //Part 2 Timer config + NVIC config
     //Configure and enable the timer functions
     TIMER3_CTL_R &= ~(0x100); // disables timer 3B
     TIMER3_CFG_R |= 0x04; // sets the timer
     TIMER3_TBMR_R |= 0x007; // set to edge time mode
     TIMER3_CTL_R |= 0x100; // re-enables timer 3B

     TIMER3_TBPR_R |= 0xFF; //prescaler stuff
     TIMER3_TBILR_R |= 0xFFFF; //loads timer interval

     TIMER3_ICR_R |= 0x400; // clears
     TIMER3_IMR_R |= 0x400; // interrupt mask something

     //NVIC stuff
     NVIC_EN1_R = 0x00000010; // enables the interrupt for NVIC

     NVIC_PRI9_R = (NVIC_PRI9_R & 0xFFFFFF0F) | 0x00000020; // sets the priority to timer 3B

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();



}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100; //disables timer 3
    TIMER3_IMR_R &= ~0x400; //disables interrupt
    GPIO_PORTB_AFSEL_R &= ~0x08;// Disable alternate function (disconnect timer from port pin)

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DIR_R |= 0x08;//set value in pin 3 to 1
    GPIO_PORTB_DATA_R &= ~0x08; //make it low writes a 0

    timer_waitMicros(5);//wait 5 microseconds
    GPIO_PORTB_DATA_R |= 0x08; //make it  high writes a 1

    timer_waitMicros(5);//wait 5 microseconds
    GPIO_PORTB_DATA_R &= ~0x08; //make it low writes a 0

    GPIO_PORTB_DIR_R &= ~0x08;//PB3 set to input

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400; // clears the interrupt
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_CTL_R |= 0x100;
    TIMER3_IMR_R |= 0x400;
}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.


    //need flag? status or command?\
    //STATE is our flag

    //process a capture event in input edge-time mode
    if (TIMER3_MIS_R & 0x400)
    {
        TIMER3_ICR_R |= 0x400;

    if(STATE == LOW){
        START_TIME = TIMER3_TBR_R;
        STATE = HIGH;
    }
    else if(STATE == HIGH){
        END_TIME = TIMER3_TBR_R;
        STATE = DONE;
        }
    }
}

float ping_getDistance (void){

    // YOUR CODE HERE

    float pulse_width;
    float distance;
    long int clock_cycles; //use later

    ping_trigger(); // triggers the pulse

    while(STATE != DONE){};  // waits for interrupt (was getting stuck in here)

    pulse_width = (START_TIME - END_TIME) / 16000000; //gives us the pulse width (milliseconds?)

    // if(pulse_width < 0) { //overflow occurs
    //handle calculations with overflow
    //(unsigned long) 0xFFFFFF is the maximum value
    //
    // }
    // else { //no overflow occurs
    //maybe multiply by 62.5 nanoseconds
    pulse_width = (START_TIME - END_TIME) / 16000000; //gives us the pulse width (milliseconds?)
    distance = pulse_width * 343; //convert to a distance in cm
    //}

    //print statement
//    sprintf(str, "Distance: %.2f", distanceS);
//    lcd_printf(str);

    return distance;
}
