
/*
 * scan.c
 *
 *  Created on: Feb 1, 2023
 *      Author: lincolnh and dchism
 */



#include "scan.h"
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "uart_int.h"
#include "adc.h" //LINCOLNDOYLE
#include "servo.h" //JACKCARTER

#define round 100

/**
 * returns an array of objects when called. scans using ADC and PING))) values.
 */
void scan() {

    //object structure containing all necessary object data
    typedef struct {
        int start; //start angle
        int start_dist; //IR Val at the start edge
        int end; //end angle
        int width; // in degrees
        int angle; //center angle
        double distance; //distance in centimeters (measured with PING))))
    } object_t;


    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                    // and enables time functions (e.g. timer_waitMillis)
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    uart_interrupt_init(); //Initialize the sending and receiving methods from uart

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); //initialize sensor data

    servo_init(); //initializes servo

    adc_init(); //initializes adc


    //INSTANCE VARIABLES
    char str[50];
    uint16_t result;
    double adc_distance;
    int array[3][91];
    int avg_array[91];
    int degrees, i, j, m; //loop variables, i is also degrees
    int num_objects = 0;
    object_t objects[20]; //create realloc


    //initialize objects
    for(m = 0; m < 20; m++) {
        objects[m].start = -1;
        objects[m].start_dist = -1;
        objects[m].end = -1;
        objects[m].width = -1;
        objects[m].angle = -1;
        objects[m].distance = -1;
    }


    //IR scans 3 times and fills and gets the avg and prints out values
    for(degrees = 0; degrees <= 180; degrees += 2) { //goes through all degrees

        i = degrees / 2; //takes a measurement every 2 degrees

        //need to calibrate scanner
        servo_move(degrees); //turns scanner to degrees

        for(j = 0; j < 3; j++) { //fills array[][]
            //replaces cyBot_scan

            result = adc_read(); //reads IR val from ADC
            adc_distance = 9.55E+06 * pow(result, -1.75); //converts IR value to a distance value

            //maxes IR at 80 cm
            if(adc_distance > 80.0) {
                array[j][i] = 80.0;
            }
            else {
                array[j][i] = adc_distance;
            }
        }

        //calculates avg distance
        avg_array[i] = (array[0][i] + array[1][i] + array[2][i]) / 3.0;

//
//
//        //detects and sets start edge
//        //if there isn't a current start edge
//        if(objects[num_objects].start == -1) {
//            if(avg_array[i] > 80.0) {
//                objects[num_objects].start = degrees;
//                objects[num_objects].start_dist = avg_array[i];
//            }
//        }
//        //detects end edge and completes object
//        else if(avg_array[i] < objects[num_objects].start_dist) {
//            objects[num_objects].end = degrees;
//            objects[num_objects].width = objects[num_objects].end - objects[num_objects].start;
//            objects[num_objects].angle = (objects[num_objects].end + objects[num_objects].start) / 2;
//            num_objects++;
//        }
//
//
//        //kill order so memory doesn't get jacked up
//        if(num_objects > 20) {
//            return 0;
//        }
//
//        //send data to PuTTY
//        sprintf(str,"Degrees: %d\tDistance(cm): %d\tNum Objects: %d\n\r", degrees, avg_array[i], num_objects);
//        uart_sendStr(str);

        //send data to PuTTY
                sprintf(str,"Degrees: %d\tDistance(cm): %d\n\r", degrees, avg_array[i]);
                uart_sendStr(str);
    }



//    //print object data
//    m = 0;
//    while(m < num_objects) {
//        sprintf(str, "Object[%d]: Width = %d\tAngle = %d\n\r", m,  objects[m].width, objects[m].angle);
//        uart_sendStr(str);
//        m++;
//    }
}

