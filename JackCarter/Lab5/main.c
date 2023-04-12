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
#include "uart_com.h"

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

//void uart_sendString(const char *s) {
//    int i = 0;
//    while (i < strlen(s)) {
//        cyBot_sendByte(s[i++]);
//    }
//}

int main (void) {
    lcd_init();
    uart_init();
    while (true) {
        char c = uart_recieve();
        uart_send(c);
        lcd_putc(c);
    }
    //cyBot_uart_init();
    //lcd_init();
    //btn_init();


//    while (true) {
//        btns_t state = btn_get_buttons();
//        while (state == BTN_NONE) {
//            state = btn_get_buttons();
//        }
//
//        if (state & BUTTON1) {
//            uart_sendString("Button 1!\r\n");
//        }
//        if (state & BUTTON2) {
//            uart_sendString("Button 2!\n");
//        }
//        if (state & BUTTON3) {
//            uart_sendString("Button 3!\n");
//        }
//        if (state & BUTTON4) {
//            uart_sendString("Button 4!\n");
//        }
//        if (state != BTN_NONE) {
//            timer_waitMillis(200);
//        }
//    }

    return 0;
}
