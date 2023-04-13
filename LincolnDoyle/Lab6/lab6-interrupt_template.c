/**
 * lab6-interrupt_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "uart-interrupt.h"

// Uncomment or add any include directives that you want to use
// #include "open_interface.h"
// #include "movement.h"
// #include "button.h"

// Your code can use the global variables defined in uart-interrupt.c
// They are declared with the extern qualifier in uart-interrupt.h, which makes the variables visible to this file.

//void frontal_scan(void) { //helper for later labs
//    cyBOT_Scan_t Mark; //make a cyBOT named Mark
//
//    char str[50];
//    double array[90]; //send it back to your PuTTY to see array[] data
//    int i, n;
//
//     //gets distance data and sends it to the PuTTY
//    for(i = 0; i <= 180; i += 2) {
//        timer_waitMillis(100);
//        cyBOT_Scan(i, &Mark); //pointer to Mark
//        if(Mark.sound_dist < 100.00) {
//            array[i/2] = Mark.sound_dist; //fills array with data
//        }
//        else {
//            array[i/2] = 100;
//        }
//
//}
//}



int main(void) {
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    uart_interrupt_init();
    cyBOT_init_Scan(0b0111);

    cyBOT_Scan_t Mark; //make a cyBOT named Mark

    // OPTIONAL
    //assign a value to command_byte if you want to know whether that ASCII code is received
    //note that command_byte is global shared variable read by the ISR
    //for example, try using a tab character as a command from PuTTY

    char str[50];
    double array[90]; //send it back to your PuTTY to see array[] data
    int i, n;

    while(1)
    {


    }
}
