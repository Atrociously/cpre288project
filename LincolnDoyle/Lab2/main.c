/*
 * main.c
 *
 *  Created on: Feb 1, 2023
 *      Author: lincolnh
 */

#include "movement.h"
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"


void main(void) {


    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                  // and enables time functions (e.g. timer_waitMillis)

    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    oi_t *sensor_data = oi_alloc();

    oi_init(sensor_data);

    //move forward 2 meters
    move_forward(sensor_data, 2000);

    oi_free(sensor_data);

}

