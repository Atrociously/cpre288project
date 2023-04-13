/*
 * main.c
 *
 *  Created on: Feb 1, 2023
 *      Author: lincolnh and dchism
 */

#include "movement.h"
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"
#include "cybot_Scan.h"

#define round 100

typedef struct {
    int start; //start angle
    int start_IR; //IR Val at the start edge
    int end; //end angle
    int width; // in degrees
    int angle; //center angle
} object_t;




int main(void) {

    //cyBot_uart_init();//initializes cybot

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
    // and enables time functions (e.g. timer_waitMillis)
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    uart_interrupt_init(); //Initialize the sending and receiving methods from uart

    cyBOT_init_Scan(0b0111);
    //cyBOT_SERVO_cal();
    //calibrates the 0 - 180 degrees. Set on each cybot
    right_calibration_value = 269500;
    left_calibration_value = 1240750;

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); //initialize sensor data




    //INSTANCE VARIABLES
    cyBOT_Scan_t Mark; //make a cyBOT named Mark
    char str[50];
    int array[3][91];
    int avg_array[91];
    int degrees, i, j, m; //loop variables, i is also degrees
    int num_objects = 0;
    object_t objects[20];
    double leftover = 0;


    //initialize objects
    for(m = 0; m < 20; m++) {
        objects[m].start = -1;
        objects[m].start_IR = -1;
        objects[m].end = -1;
        objects[m].width = -1;
        objects[m].angle = -1;
    }


    //IR scans 3 times and fills and gets the avg and prints out values
    for(degrees = 0; degrees <= 180; degrees += 2) { //goes through all degrees

        i = degrees / 2; //takes a measurement every 2 degrees

        for(j = 0; j < 3; j++) { //fills array[][]
            //timer_waitMillis(50);
            cyBOT_Scan(degrees, &Mark); //pointer to Mark

            //maxes IR at 2 meters
            if(Mark.IR_raw_val < 800.00) {
                array[j][i] = 800.00;
            }
            else {
                array[j][i] = Mark.IR_raw_val;
            }
        }

        //calculates avg IR val
        avg_array[i] = (array[0][i] + array[1][i] + array[2][i]) / 3.0;

        //round avg_array[i]
        leftover = avg_array[i] % round;

        if(leftover > round / 2) {
            avg_array[i] = (avg_array[i] - leftover) + round;
        }
        else {
            avg_array[i] = avg_array[i] - leftover;
        }


        //detects and sets start edge
        //if there isn't a current start edge
        if(objects[num_objects].start == -1) {
            if(avg_array[i] > 800) {
                objects[num_objects].start = degrees;
                objects[num_objects].start_IR = avg_array[i];
            }
        }
        //detects end edge and completes object
        else if(avg_array[i] < objects[num_objects].start_IR) {
            objects[num_objects].end = degrees;
            objects[num_objects].width = objects[num_objects].end - objects[num_objects].start;
            objects[num_objects].angle = (objects[num_objects].end + objects[num_objects].start) / 2;
            num_objects++;
        }


        //kill order so memory doesn't get jacked up
        if(num_objects > 20) {
            return 0;
        }


        //send data to PuTTY
        sprintf(str,"Degrees: %d\tIRval: %d\tNum Objects: %d\n\r", degrees, avg_array[i], num_objects);
        uart_sendStr(str);


    }


    //print object data
    m = 0;
    while(m < num_objects) {
        sprintf(str, "Object[%d]: Width = %d\tAngle = %d\n\r", m,  objects[m].width, objects[m].angle);
        uart_sendStr(str);
        m++;
    }

    //calculates smallest object
    object_t smallest_object;
    smallest_object.width = 1000; //arbitrarily large value
    for(m = 0; m < num_objects; m++) {
        if(objects[m].width > 8) {
            if(objects[m].width <= smallest_object.width) {
                smallest_object = objects[m];
            }
        }
    }


    //point to smallest object
    cyBOT_Scan(smallest_object.angle, &Mark);

    //print smallest object data
    sprintf(str, "\n\rSmallest object: Width = %d\tAngle = %d\tDistance = %.2lf\n\r", smallest_object.width, smallest_object.angle, Mark.sound_dist);
    uart_sendStr(str);


    //MOVING TO THE OBJECT
    //point to smallest object
    cyBOT_Scan(smallest_object.angle, &Mark);

    //turn to smallest object
    if(smallest_object.angle > 90) { //object is to the left
        turn_left(sensor_data, smallest_object.angle - 90);
    }
    else { //object is to the right
        turn_right(sensor_data, smallest_object.angle - 90);
    }

    //drive to object
    move_forward(sensor_data, (Mark.sound_dist * 10.0) - 50);

}



