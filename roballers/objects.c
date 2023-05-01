/*
 * objects.c
 *
 *  Created on: May 1, 2023
 *      Author: jackmorr
 */
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "Timer.h"
#include "objects.h"
#include <math.h>

typedef struct {
    int16_t ir_raw;
    float ir_dist;
} Scan_t;

int IR_validated(int raw_val) {
    if (raw_val <= 800 || raw_val >= 3000) {
        return -1;
    } else {
        return raw_val;
    }
}

void scan_around(Scan_t output[], size_t len) {
    uint16_t i = 0;
    uint16_t angle = 0;
    for (i = 0; i < len; i++) {
        Scan_t data;
        servo_move(angle);
        data.ir_raw = adc_getRawValue();
        data.ir_dist = adc_convert(data.ir_raw);
        timer_waitMillis(100);
        output[i] = data;
        angle += 180 / len;
    }
}

size_t scan_objects(Obj_t objects[], size_t max) {
    adc_init();
    ping_init();
    servo_init();
    timer_init();

    #define DATA_LEN 180
    Scan_t data[DATA_LEN];
    scan_around(data, DATA_LEN);

    int angle = 0;
    int last_angle = 0;
    size_t objIdx = 0;
    char inObject = 0;
    Obj_t current;
    // create objects
    for (; angle < DATA_LEN; angle++) {
        int valid = IR_validated(data[angle].ir_raw);
        char msg[100];
        sprintf(msg, "Angle: %d Val: %d\r\n", angle, valid);
        uart_sendStr(msg);
//        float last_dist;
//        if (angle >= 1) {
//            last_dist = data[angle-1].ir_dist;
//        } else {
//            last_dist = 80;
//        }
        // add more logic for validation


        if (!inObject && valid > 0) {
            current = (Obj_t) { .right = angle, .left = -1, .arc_len = -1 };
            inObject = 1;
        } else if (inObject && (valid < 0 || angle == DATA_LEN - 1) && last_angle - current.right < 3) {
            inObject = 0;
        } else if (inObject && (valid < 0 || angle == DATA_LEN - 1) && objIdx < max) {
            current.left = last_angle;
            objects[objIdx++] = current;
            inObject = 0;
        }
        last_angle = angle;
    }

    int i;
    for (i = 0; i < objIdx; i++) {
        Obj_t* obj = &objects[i];
        int angular_width = obj->left - obj->right;
        int center_point = obj->right + (angular_width / 2);

        servo_move(center_point);
        timer_waitMillis(500); // wait for servo to move
        float dist = data[center_point].ir_dist; // get the ping distance
        obj->distance = dist;

        float arc_length = angular_width * (M_PI / 180.0) * dist;
        obj->arc_len = arc_length;
    }

    return objIdx;
}
