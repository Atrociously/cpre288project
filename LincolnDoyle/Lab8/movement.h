/*
 * movement.h
 *
 *  Created on: Feb 1, 2023
 *      Author: lincolnh
 */

#include "open_interface.h"

#ifndef MOVEMENT_H_
#define MOVEMENT_H_


void move_forward(oi_t *sensor_data, double distance_mm);

void turn_right(oi_t*sensor_data, double degrees);

void turn_left(oi_t*sensor_data, double degrees);

void move_backwards(oi_t *sensor_data, double distance_mm);

#endif /* MOVEMENT_H_ */
