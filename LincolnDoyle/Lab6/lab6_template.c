/**
 * lab6_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "uart.h"

// Uncomment or add any include directives that are needed
// #include "open_interface.h"
// #include "movement.h"
// #include "button.h"




int frontal_scan(void) { //helper for later labs
    cyBOT_Scan_t Mark; //make a cyBOT named Mark

    char str[50];
    double array[90]; //send it back to your PuTTY to see array[] data
    int i, n;

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

        n = 0; //iterates through the index of the str
        while(n < strlen(str)) { //prints data message to the PuTTY
            cyBot_sendByte(str[n]);
            n++;
        }
    }

    return 0;
}


//#warning "Possible unimplemented functions"
//#define REPLACEME 0


int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_init();
    cyBOT_init_Scan(0b0111); //initialize scan

	char c;

	while(1)
	{

//      c = uart_receive();
//
//      if(c == 'g') {
//          frontal_scan();
//      }


	}

}
