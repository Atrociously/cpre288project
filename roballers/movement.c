/*
 * movement.c
 *
 *  Created on: Feb 1, 2023
 *      Author: lincolnh
 */

//
#include "open_interface.h"
#include "lcd.h"
#include "Timer.h"
#include "movement.h"
#include "cyproto.h"


TurnDone turn_right(oi_t*sensor_data, double degrees, int speed){

    double angle_sum = 0;
    TurnDone data;

    timer_waitMillis(300);

    oi_setWheels(-speed, speed); //pivot left

    while(angle_sum > degrees) {

        oi_update(sensor_data);

        angle_sum += sensor_data -> angle;
    }

    oi_setWheels(0,0); //stop the turn

    //initialize struct
    data.total_angle = angle_sum;

    return data;
}

TurnDone turn_left(oi_t*sensor_data, double degrees, int speed){

    double angle_sum = 0;
    TurnDone data;

    timer_waitMillis(300);

    oi_setWheels(speed, -speed); //pivot left

    while(angle_sum < degrees) {

        oi_update(sensor_data);

        angle_sum += sensor_data -> angle;
    }

    oi_setWheels(0,0); //stop the turn

    //initialize struct
    data.total_angle = angle_sum;

    return data;
}


//MAKE BOTH MOVEMENTS RETURN A DRIVEDONE STRUTT
DriveDone move_backwards(oi_t *sensor_data, double distance_cm, int speed) {
    oi_update(sensor_data);
    oi_update(sensor_data);

    double distance_mm = distance_cm * 10;
    double sum = 0;
    DriveDone data;

    //initialize the struct
    data.bump_detected = false;
    data.cliff_detected = false;

    //moves until distance is achieved
        while(sum < distance_mm) {

            //cliff detection
            if(sensor_data -> wheelDropLeft || sensor_data -> wheelDropRight) { //wheeldrop
                oi_setWheels(0, 0); //stops wheels and awaits instructions
                data.cliff_detected = true;
                break;
            }

            //movement
            oi_setWheels(-speed, -speed); //move backwards at declared speed

            oi_update(sensor_data);

            //update distance traveled
            sum -= sensor_data -> distance;


        }

        oi_setWheels(0, 0); //stop

        //initialize the struct
        data.total_distance = -sum;

        return data;

}

DriveDone move_forward(oi_t *sensor_data, double distance_cm, int speed) {
    oi_update(sensor_data);
    oi_update(sensor_data);

    double distance_mm = distance_cm * 10;
    double sum = 0;
    DriveDone data;

    //initialize the struct
    data.bump_detected = false;
    data.cliff_detected = false;

    //moves until distance is achieved
        while(sum < distance_mm) {
            //cliff detection
            //both cliff sensors and wheel drop sensors
            if(sensor_data -> cliffLeft || sensor_data -> cliffFrontLeft || sensor_data -> cliffRight || sensor_data -> cliffFrontRight) { //cliff
                oi_setWheels(0, 0); //stops wheels and awaits instructions
                data.cliff_detected = true;
                break;
            }
            if(sensor_data -> wheelDropLeft || sensor_data -> wheelDropRight) { //wheeldrop
                oi_setWheels(0, 0); //stops wheels and awaits instructions
                data.cliff_detected = true;
                break;
            }

            //boundary detection
            //the cliffSignal variables all track the "strength" (color) of the cliff, which will let us identify the boundary lines
            //values vary from 0-4095
//            if(sensor_data -> cliffLeftSignal > 2700 || sensor_data -> cliffFrontLeftSignal > 2700
//                    || sensor_data -> cliffRightSignal > 2700 || sensor_data -> cliffFrontRightSignal > 2700) {
//                oi_setWheels(0, 0); //stops wheels and waits for instruction
//                data.cliff_detected = true;
//                break;
//            }

            //bump detection (continues to move forward)
            if(sensor_data -> bumpLeft) { //left sensor
                oi_setWheels(0, 0); //stops wheels and awaits further instructions
                data.bump_detected = true;
                break;
            }
            if(sensor_data -> bumpRight) { //right sensor
                oi_setWheels(0, 0);
                data.bump_detected = true;
                break;
            }

            //movement
            oi_setWheels(speed, speed); //move forward at declared speed

            oi_update(sensor_data);

            //update distance traveled
            sum += sensor_data -> distance;

            //lcd_printf("%.2lf", sum); //displays distance traveled for testing purposes

            //prints cliff signal data
            //front right signal is really small for some reason, probably bot specific
            //lcd_printf("%d\t%d\t%d\t%d\t", sensor_data -> cliffLeftSignal, sensor_data -> cliffFrontLeftSignal, sensor_data -> cliffFrontRightSignal, sensor_data -> cliffRightSignal);
        }

        oi_setWheels(0, 0); //stops once the distance is reached

        //initialize the struct
        data.total_distance = sum;

        return data;
}






