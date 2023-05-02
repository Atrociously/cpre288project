/*
 * movement.h
 *
 *  Created on: Feb 1, 2023
 *      Author: lincolnh
 */



#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"
#include "cyproto.h"


DriveDone move_forward(oi_t *sensor_data, double distance_mm, int speed);

TurnDone turn_right(oi_t*sensor_data, double degrees, int speed);

TurnDone turn_left(oi_t*sensor_data, double degrees, int speed);

DriveDone move_backwards(oi_t *sensor_data, double distance_mm, int speed);

#endif /* MOVEMENT_H_ */
