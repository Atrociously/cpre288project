/*
 * servo.h
 *
 *  Created on: Apr 11, 2023
 *      Author: jackmorr
 */

#ifndef SERVO_H_
#define SERVO_H_
#include <inc/tm4c123gh6pm.h>

static double servo_pwm_max = 2.25;
static double servo_pwm_min = 0.5;

void servo_init(void);
void servo_move(float);


#endif /* SERVO_H_ */
