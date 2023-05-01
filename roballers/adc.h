/*
 * adc.h
 *
 *  Created on: Mar 23, 2023
 *      Author: jackmorr
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

void adc_init();
int32_t adc_getRawValue();
float adc_getVoltage();
float adc_getDistance();
float adc_convert(int16_t);


#endif /* ADC_H_ */
