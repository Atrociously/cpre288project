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
#include "cyBot_uart.h"
#include "cybot_Scan.h"


//int frontal_scan(void) { //helper for later labs
//    cyBOT_Scan_t Mark; //make a cyBOT named Mark
//
//    char str[50];
//    double array[90]; //send it back to your PuTTY to see array[] data
//    int i, n;
//
//    //gets distance data and sends it to the PuTTY
//    for(i = 0; i <= 180; i += 2) { //maybe i = i + 2
//        cyBOT_Scan(i, &Mark); //pointer to Mark
//        array[i] = Mark.sound_dist; //fills array with data
//        sprintf(str ,"Degrees: %d\tDistance: %.2lf\n\r", i, array[i]);
//
//        n = 0; //iterates through the index of the str
//        while(n < strlen(str)) { //prints data message to the PuTTY
//            cyBot_sendByte(str[n]);
//            n++;
//        }
//    }
//
//    return 0;
//}

   typedef struct {
        int start;
        int end;
        double distance; //cm
        int width; // in degrees
        int angle;
    } object_t;


int main(void) {

    cyBot_uart_init();//initializes cybot

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
    // and enables time functions (e.g. timer_waitMillis)
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    cyBOT_init_Scan(0b0111);
    //cyBOT_SERVO_cal();
    //calibrates the 0 - 180 degrees
    right_calibration_value = 264250; //gotta set these on each cybot
    left_calibration_value = 1240750;

    //    oi_t *sensor_data = oi_alloc();
    //    oi_init(sensor_data);


    cyBOT_Scan_t Mark; //make a cyBOT named Mark

    char str[50];
    //double array[3][90]; //send it back to your PuTTY to see array[] data
    double array[90];
    int i, n, m; //loop variables
    //i also is degrees
    int num_objects = 0;
    object_t objects[20];

    //initialize objects
    for(m = 0; m < 20; m++) {
        objects[m].start = -1;
        objects[m].end = -1;
        objects[m].width = -1;
        objects[m].angle = -1;
    }

    //gets distance data and sends it to the PuTTY
    for(i = 0; i <= 180; i += 2) {
        timer_waitMillis(100);
        cyBOT_Scan(i, &Mark); //pointer to Mark
        if(Mark.sound_dist < 100.00) {
            array[i/2] = Mark.sound_dist; //fills array with data
        }
        else {
            array[i/2] = 100;
        }

        //calculate objects
        //integrate to ignore walls (ignore values more than like 80)

        if((array[i/2] >= (array[(i/2) - 1]) + 10) || (array[i/2] <= (array[(i/2) - 1]) - 10)) { //checks for jumps in distance
            //new object
            if(objects[num_objects].start == -1) {
                //give a start value
                objects[num_objects].start = i;
            }
            else {
                //give an end value
                objects[num_objects].end = i;
                objects[num_objects].width = objects[num_objects].end - objects[num_objects].start;
                objects[num_objects].angle = (objects[num_objects].end + objects[num_objects].start) / 2;
                num_objects++;
            }
        }

        //kill order
        if(num_objects > 20) {
            return 0;
        }

        //print stuff
        sprintf(str ,"Degrees: %d\tDistance: %.2lf, Num of Objects: %d\n\r", i, array[i/2], num_objects);
        n = 0; //iterates through the index of the str
        while(n < strlen(str)) { //prints data message to the PuTTY
            cyBot_sendByte(str[n]);
            n++;
        }
    }



    //print object data
    m = 0;
    while(m < num_objects) {
        sprintf(str, "Object[%d]: Width = %d\tAngle = %d\n\r", m,  objects[m].width, objects[m].angle);
        n = 0; //iterates through the index of the str
        while(n < strlen(str)) { //prints data message to the PuTTY
            cyBot_sendByte(str[n]);
            n++;
        }
        m++;
    }

    //point at smallest object
    object_t smallest_object;
    smallest_object.width = 1000;
    for(m = 0; m < num_objects; m++) {
        if(objects[m].width > 3) {
            if(objects[m].width <= smallest_object.width) {
                smallest_object = objects[m];
            }
        }
    }

    cyBOT_Scan(smallest_object.angle, &Mark);

    return 0;
}



//void movement_WAXD() {
//    while(1) {
//        char letter = cyBot_getByte();
//        lcd_putc(letter);//prints to lcd
//        cyBot_sendByte(letter); //sends back to PuTTY
//
//        while(letter == 'w') {
//            oi_setWheels(250,250);
//            letter = cyBot_getByte();
//        }
//        oi_setWheels(0,0); //stops after new letter
//
//        while(letter == 'x') {
//            oi_setWheels(-250,-250);
//            letter = cyBot_getByte();
//        }
//        oi_setWheels(0,0); //stops after new letter
//
//        if(letter == 'a') {
//            turn_left(sensor_data,90);
//            while(letter == 'a') {
//                oi_setWheels(250,250);
//                letter = cyBot_getByte();
//            }
//        }
//        oi_setWheels(0,0); //stops after new letter stops
//
//        if(letter == 'd') {
//            turn_right(sensor_data, -90);
//            while(letter == 'd') {
//                oi_setWheels(250,250);
//                letter = cyBot_getByte();
//            }
//        }
//        oi_setWheels(0,0); //stops after new letter stops
//    }
//}


