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
#include "adc.h"
#include "math.h"
//#include "cybot_Scan.h"


int main(void) {

    //Your program should be set up to repeatedly take samples of the analog input from the IR sensor.

    //May want to slow down how fast samples are taken as well as how frequently the display is updated (in main loop)



    //INITIALIZATION
    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                    // and enables time functions (e.g. timer_waitMillis)
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    uart_interrupt_init(); //Initialize the sending and receiving methods from uart

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); //initialize sensor data

    adc_init();

   //INSTANCE  VARIABLES
    char str[50];
    uint16_t result;
    double estimated_distance;

    //MAIN LOOPs
    while(1) {
        if(command_flag == 1) {
            result = adc_read(); //Reads IR_val
            estimated_distance = 9.55E+06 * pow(result, -1.75); // 9.55E+06x^-1.75

            sprintf(str, "IR_val: %d\tEstimated Distance: %.2lf\n\r", result, estimated_distance);
            uart_sendStr(str);
            command_flag = 0;
        }

    }


}



