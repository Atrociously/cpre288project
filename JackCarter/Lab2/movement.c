/*
 * movement.c
 *
 *  Created on: Feb 1, 2023
 *      Author: jackmorr
 */

#include "open_interface.h"
#include <stdint.h>

double __signof(double t) {
    return (t > 0 ? 1 : (t < 0) ? -1 : 0);
}

double lerp(double a, double b, double frac) {
    return a * (1.0 - frac) + (b * frac);
}

/// positive is counter-clockwise
double mvmt_turn(oi_t *sensor_data, double degrees, uint8_t speed) {
    if (speed == 0) {
        return -1;
    }

    double turned = 0;

    int16_t realSpeed = (int16_t) speed * (int) __signof(degrees);
    oi_setWheels(realSpeed, realSpeed * -1);

    while (fabs(turned) < fabs(degrees)) {
        oi_update(sensor_data);
        double angle_degrees = sensor_data -> angle;
        turned += angle_degrees;
//        if (fabs(degrees - turned) < (speed / 10)) {
//            realSpeed = speed - lerp(turned, degrees, 0.5);
//            oi_setWheels(realSpeed, realSpeed * -1);
//        }
    }

    oi_setWheels(0, 0);
    return turned;
}

double mvmt_drive(oi_t *sensor_data, double distance, uint16_t speed) {
    if (speed == 0) {
        return -1;
    }
    double traveled = 0;

    oi_setWheels(speed, speed);
    while (abs(traveled) < abs(distance)) {
        oi_update(sensor_data);
        traveled += sensor_data -> distance;

        if (sensor_data -> bumpLeft || sensor_data -> bumpRight) {
            oi_setWheels(0, 0);
            return traveled;
        }
    }

    oi_setWheels(0, 0);
    return traveled;
}
