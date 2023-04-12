/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */
#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "Timer.h"
#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include <string.h>
#include "buttons.h"
//#include "uart_com.h"
#include "uart_int.h"

void avoid_collision(oi_t *sd) {
    double distance = mvmt_drive(sd, 1000, 500);
    lcd_printf("collision: %.2lf", distance);
    mvmt_drive(sd, -150, 200);
    mvmt_turn(sd, -90, 200);
    mvmt_drive(sd, 250, 200);
    mvmt_turn(sd, 90, 200);
    mvmt_drive(sd, 1000 - distance, 500);
}

void scan_around(cyBOT_Scan_t output[], size_t len) {
    uint16_t i = 0;
    uint16_t angle = 0;
    for (i = 0; i < len; i++) {
        cyBOT_Scan_t data;
        cyBOT_Scan(angle, &data);
        output[i] = data;
        angle += 180 / len;
    }
}

int IR_validated(int raw_val) {
    if (raw_val <= 600 || raw_val >= 3000) {
        return -1;
    } else {
        return raw_val;
    }
}

float scan_until(float start_angle, int8_t turn_by, uint16_t thresh_cm) {
    float angle = start_angle;
    uint8_t thresh_met = false;
    cyBOT_Scan_t last;
    cyBOT_Scan(angle, &last);
    while (angle < 180 && angle >= 0 && !thresh_met) {
        cyBOT_Scan_t curr;
        cyBOT_Scan(angle, &curr);
        lcd_printf("Angle: %.0f Dist: %d", angle, curr.sound_dist);
        timer_waitMillis(500);
        angle += turn_by;
        if (last.sound_dist - curr.sound_dist > thresh_cm) {
            thresh_met = true;
        }
    }
    if (!thresh_met) {
        return 0;
    }
    return start_angle - angle;
}

//float convert_raw_ir_val(int IR_raw_val) {
//    float raw_float = (float) IR_raw_val;
//
//    float voltage_start = 0.4;
//    float voltage_end = 2.6;
//    float raw_start = 600.0;
//    float raw_end = 1800.0;
//    float inverse_start = 0.012;
//    float inverse_end = 0.111;
//
//    if (raw_float < raw_start || raw_float > raw_end) {
//        return -3.0;
//    }
//
//    float voltage_conversion = (voltage_end - voltage_start) / (raw_end - raw_start);
//    float distance_conversion = (inverse_end - inverse_start) / (voltage_end - voltage_start);
//
//    float voltage_val = voltage_start + voltage_conversion * (raw_float - raw_start);
//    float inverse_val = inverse_start + distance_conversion * (voltage_val - voltage_start);
//
//    return 1.0 / inverse_val + 0.42;
//}

static int calibrated = 0;

typedef struct {
    int right;
    int left;
    float arc_len;
} Obj_t;

typedef struct {
    int index;
    Obj_t data;
} Result_t;

Result_t scan_smallest() {
    #define DATA_LEN 180
    #define MAX_OBJS 50
    cyBOT_Scan_t data[DATA_LEN];
        scan_around(data, DATA_LEN);

        Obj_t objects[MAX_OBJS];
        int angle = 0;
        int last_angle = 0;
        int objIdx = 0;
        char inObject = 0;
        Obj_t current;
        // create objects
        for (; angle < DATA_LEN; angle++) {
            int ir_dist = IR_validated(data[angle].IR_raw_val);
            char msg[100];
            sprintf(msg, "%d IR: %d\r\n", angle, ir_dist);
            uart_sendString(msg);

            if (!inObject && ir_dist > 0) {
                current = (Obj_t) { .right = angle, .left = -1, .arc_len = -1 };
                inObject = 1;
            } else if (inObject && (ir_dist < 0 || angle == DATA_LEN - 1) && last_angle - current.right < 3) {
                inObject = 0;
            } else if (inObject && (ir_dist < 0 || angle == DATA_LEN - 1) && objIdx < MAX_OBJS) {
                current.left = last_angle;
                objects[objIdx++] = current;
                inObject = 0;
            }
            last_angle = angle;
        }

        uart_sendString("Done Scanning!\r\n");
        int smallest = -1;
        size_t i = 0;
        if (objIdx == 1) {
            return (Result_t) { .index = 0, .data = objects[0] };
        }
        // find smallest object
        for (; i < objIdx; i++) {
            Obj_t* obj = &objects[i];
            char msg[100];
            sprintf(msg, "Object %d: left: %d right: %d\r\n", i+1, obj->left, obj->right);
            uart_sendString(msg);
            //if (obj->left - obj->right <= 2) { continue; }
//            if (smallest < 0) {
//                smallest = i;
//            }

            int angular_width = obj->left - obj->right;
            int center_point = obj->right + (angular_width / 2);

            cyBOT_Scan_t scan;
            cyBOT_Scan(center_point, &scan);
            timer_waitMillis(500); // wait to make sure
            //if (IR_validated(scan.IR_raw_val) < 0) {
            //    continue;
            //}
            float arc_length = angular_width * (M_PI / 180.0) * scan.sound_dist;
            sprintf(msg, "dist: %.2f arc_len: %.2f\r\n", scan.sound_dist, arc_length);
            uart_sendString(msg);
            //if (arc_length < 0.5) {
            //    continue;
            //}
            obj->arc_len = arc_length;
            if (smallest < 0 || (arc_length > 0 && (objects[smallest].arc_len < 0 || arc_length < objects[smallest].arc_len))) {
                smallest = i;
            }

            // TODO: get distance with ping
            // TODO: caluculate arc witdh using angular width and distance
            // TODO: get thinnest object
            // TODO: navigate to thinnest object
        }
        return (Result_t) { .index = smallest, .data = objects[smallest] };
}

int main (void) {
    //uart_interrupt_init();
    lcd_init();
    oi_t *sd = oi_alloc();
    oi_init(sd);
    //oi_free(sd);
    cyBOT_init_Scan(0b0111);
    uart_init();

    if (!calibrated) {
        cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
        right_calibration_value = /*248500;*/ calibration.right;
        left_calibration_value = /*1214500;*/ calibration.left;
        calibrated = 1;
    }

    int done = 0;
    while (!done) {
        Result_t smallest = scan_smallest();

        while (smallest.index < 0) {
            smallest = scan_smallest();
        }
        Obj_t obj = smallest.data;

        int width = obj.left - obj.right;
        int center = obj.right + (width / 2);

        cyBOT_Scan_t scan;
        cyBOT_Scan(center, &scan);
        timer_waitMillis(500); // wait to make sure
        double target = ((double) scan.sound_dist - 8) * 10;
        if (target < 80) { // was 7 and 70
            done = 1;
            break;
        }


        mvmt_turn(sd, center - 90, 200);
        double traveled = mvmt_drive(sd, target, 200);
        if (traveled >= target) {
            uart_sendString("Done!");
            done = 1;
        } else {
            uart_sendString("EVADE!");
            // evade
            mvmt_turn(sd, -90, 200);
            mvmt_drive(sd, 100, 200);
            mvmt_turn(sd, 90, 200);
        }
    }

    oi_free(sd);
    return 0;
}
