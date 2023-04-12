#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <stdint.h>
#include "open_interface.h"

double mvmt_turn(oi_t *sensor_data, double degrees, uint8_t speed);
double mvmt_drive(oi_t *sensor_data, double distance, uint16_t speed);

#endif
