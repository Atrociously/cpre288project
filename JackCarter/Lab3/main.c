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

int16_t IR_dist_cm(int raw_val) {
    return 1 / ((raw_val - 1125) / 137500);
}

float scan_until(float start_angle, int8_t turn_by, uint16_t thresh_cm) {
    float angle = start_angle;
    uint8_t thresh_met = false;
    cyBOT_Scan_t last;
    cyBOT_Scan(angle, &last);
    while (angle < 180 && angle >= 0 && !thresh_met) {
        cyBOT_Scan_t curr;
        cyBOT_Scan(angle, &curr);
        lcd_printf("Angle: %.0f Dist: %d", angle, curr.IR_raw_val);
        timer_waitMillis(500);
        angle += turn_by;
        if (last.IR_raw_val - curr.IR_raw_val > thresh_cm) {
            thresh_met = true;
        }
    }
    if (!thresh_met) {
        return 0;
    }
    return start_angle - angle;
}

void uart_sendString(const char *s) {
    int i = 0;
    while (i < strlen(s)) {
        cyBot_sendByte(s[i++]);
    }
}

size_t lowest(cyBOT_Scan_t arr[], size_t start, size_t end) {
    size_t i = start;
    size_t lowIdx = start;
    cyBOT_Scan_t low = arr[0];
    for (; i < end; i++) {
        if (arr[i].IR_raw_val > low.IR_raw_val) {
            low = arr[i];
            lowIdx = i;
        }
    }
    return lowIdx;
}

int main (void) {
    static int calibrated = 0;

    oi_t *sd = oi_alloc(); // allocate memory
    // initialize stuff
    lcd_init();
    oi_init(sd);
    cyBot_uart_init();

    cyBOT_init_Scan(0b0111);
    if (!calibrated) {
        //cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
        right_calibration_value = 295750; //calibration.right;
        left_calibration_value = 1251250; //calibration.left;
        calibrated = 1;
    }

    size_t vals_len = 90;
    cyBOT_Scan_t vals[vals_len];
    scan_around(vals, vals_len);

    int i = 0;
    int leftSum = 0;
    int rightSum = 0;
    int mid = vals_len / 2;
    for (i = 0; i < mid; i++) {
        rightSum += vals[i].IR_raw_val;
        leftSum += vals[i + mid].IR_raw_val;
    }


    char data[50];
    sprintf(data, "Left: %d Right: %d\n", leftSum, rightSum);
    uart_sendString(data);
    cyBOT_Scan_t empty;
    if (leftSum > rightSum) {
        size_t angle = lowest(vals, 45, vals_len)  * 2;
        mvmt_turn(sd, -(90.0 - angle), 200);
        mvmt_drive(sd, 1000, 200);
    } else {
        size_t angle = lowest(vals, 0, 45) * 2;
        mvmt_turn(sd, -(90.0 - angle), 200);
        mvmt_drive(sd, 1000, 200);
    }


    oi_free(sd);

    return 0;
}
