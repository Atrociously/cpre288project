/*
 * servo.h
 *
 *  Created on: Apr 5, 2023
 *      Author: lincoln hatlestad and doyle chism
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

/*
 *  initialize and configure PWM timer
 */
void servo_init(void);

/*
 * move function
 */
void servo_move(uint16_t degrees);


#endif /* SERVO_H_ */
