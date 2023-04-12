/*
 * adc.c
 *
 *  Created on: Mar 23, 2023
 *      Author: jackmorr
 */
#include <inc/tm4c123gh6pm.h>
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

#define VREFP 3.3
#define VREFN 0.0
#define MAX_RAW 4096.0;

#define MAGIC_POWER -1.218
#define MAGIC_M 5.8855
#define MAGIC_INVERSE 0.42

void adc_init() {
    SYSCTL_RCGCADC_R |= BIT0; // enable clocks for analog digital converter 0 (ADC0)

    SYSCTL_RCGCGPIO_R |= BIT1;

    GPIO_PORTB_AMSEL_R |= BIT4; // set analog function on pin 4
    GPIO_PORTB_DEN_R |= BIT4; // enable gpio pin 4
    GPIO_PORTB_DIR_R &= ~BIT4; // set input on pin 4
    GPIO_PORTB_DIR_R &= ~BIT4; // set input on pin 4

    ADC0_CC_R |= ADC_CC_CS_SYSPLL;
    ADC0_SSCTL3_R |= ADC_SSCTL3_IE0;
    ADC0_IM_R &= ~ADC_IM_MASK3;
    ADC0_ACTSS_R |= BIT3; // enable sample sequencer 3 (SS3) for ADC0
    ADC0_EMUX_R &= 0x0FFF; // set trigger source to software for SS3
    ADC0_SSMUX3_R |= 10; // set analog input to AIN10 which is wired to the IR
}


int16_t adc_getRawValue() {
    ADC0_PSSI_R |= BIT3;
    while (ADC0_RIS_R & BIT3 == 0) {} // wait for conversion to be complete
    int16_t result = ADC0_SSFIFO3_R & 0x00000FFF; // read the result and only keep bits 0-11
    ADC0_ISC_R |= BIT3;

    return result;
}

float adc_getVoltage() {
    float factor = (VREFP - VREFN) / MAX_RAW; // conversion factor from Tiva-Datasheet p.809
    return  factor * (float) adc_getRawValue();
}

float adc_getDistance() {
    float voltage = adc_getVoltage();
    return MAGIC_M * powf(voltage, MAGIC_POWER);
}
