/*
 * adc.c
 *
 *  Created on: Mar 22, 2023
 *      Author: lincolnh
 */


#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "adc.h"

//USING SAMPLE SEQUENCE 3

void adc_init(void) {
    SYSCTL_RCGCADC_R |= 0x0001; // 1) activate ADC0
    while((SYSCTL_PRADC_R & 0x0001) != 0x0001){}; //wait for ADC to be ready
    SYSCTL_RCGCGPIO_R |= 0x02; // 2) activate clock for Port B
    while((SYSCTL_PRGPIO_R & 0x02) != 0x02) {}; // 3 for stabilization
    GPIO_PORTB_DIR_R &= ~0x10; // 4) make PB4 input
    GPIO_PORTB_AFSEL_R |= 0x10; // 5) enable alternate function on PB4
    GPIO_PORTB_DEN_R &= ~0x10; // 6) disable digital I/O on PB4
    GPIO_PORTB_AMSEL_R |= 0x10; // 7) enable analog functionality on PB4

    ADC0_ACTSS_R &= ~0x0008; // 10) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000; // 11) seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R |= 0xA; // 12) set channel
    ADC0_SSCTL3_R = 0x0006; // 13) no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008; // 14) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008; // 15) enable sample sequencer 3
}

uint16_t adc_read(void) {
    uint16_t result;
    ADC0_PSSI_R |= 0x8;
    while((ADC0_RIS_R & 0x8) == 0) {}; //waits until the raw interrupt flag goes is detected
    result = ADC0_SSFIFO3_R & 0x0FFF; //assign result with the SSFIFO(sample sequence result
    ADC0_ISC_R |= 0x8; //interrupt status and clear

    return result;
}



