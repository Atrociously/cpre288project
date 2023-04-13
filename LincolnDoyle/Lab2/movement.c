/*
 * movement.c
 *
 *  Created on: Feb 1, 2023
 *      Author: lincolnh
 */


#include "open_interface.h"
#include "lcd.h"
#include "Timer.h"



void turn_right(oi_t*sensor_data, double degrees){

    double angle_sum = 0;

    timer_waitMillis(300);

    oi_setWheels(-100, 100); //pivot left

    while(angle_sum > degrees) {

        oi_update(sensor_data);

        angle_sum += sensor_data -> angle;

        //corrects if over turned
        if(angle_sum < degrees) {
             angle_sum = degrees;
         }
    }

    oi_setWheels(0,0); //stop the turn
}

void turn_left(oi_t*sensor_data, double degrees){
    double angle_sum = 0;

    timer_waitMillis(300);

    oi_setWheels(100, -100); //pivot left

    while(angle_sum < degrees) {

        oi_update(sensor_data);

        angle_sum += sensor_data -> angle;

        //corrects if overturned
        if(angle_sum > degrees) {
            angle_sum = degrees;
        }
    }

    oi_setWheels(0,0); //stop the turn
}


void move_backwards(oi_t *sensor_data, double distance_mm) {
    double sum = 0;

    //moves until distance is achieved
        while(sum < distance_mm) {

            oi_setWheels(-250, -250); //move backwards at half speed

            oi_update(sensor_data);

            //update distance traveled
            sum -= sensor_data -> distance;
        }

        oi_setWheels(0, 0); //stop

}

void move_forward(oi_t *sensor_data, double distance_mm) {

    double sum = 0;

    //moves until distance is achieved
        while(sum < distance_mm) {

            oi_setWheels(250, 250); //move forward at half speed

            oi_update(sensor_data);

            //update distance travelled
            sum += sensor_data -> distance;

            lcd_printf("%.2lf", sum);

            //bumpin shit
            if(sensor_data -> bumpLeft) {
                oi_setWheels(0, 0); //stops wheels
                move_backwards(sensor_data, 150); //move back 150 mm
                turn_right(sensor_data, -90); //right turn
                move_forward(sensor_data, 250); //move forward
                turn_left(sensor_data, 90); //left turn
            }
            if(sensor_data -> bumpRight) {
                oi_setWheels(0, 0);
                move_backwards(sensor_data, 150);
                turn_left(sensor_data, 90);
                move_forward(sensor_data, 250);
                turn_right(sensor_data, -90);
            }
        }

        oi_setWheels(0, 0); //stop
}






