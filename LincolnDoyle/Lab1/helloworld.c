/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"


int main (void) {

    char msg[] = "                    Microcontrollers are lots of fun!";
    char shiftedMsg[20];
    int i, n;


	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	// Print "Hello, world" on the LCD


	//while loop to wait for 300 mill
	while(1) {

	    n = 1;

	    while(n < 53) {

	        //make substring shiftedMsg
	        for(i = 0; i < 20; i++) {
	            shiftedMsg[i] = msg[i + n];
	        }

	        timer_waitMillis(300);

	        //print
	        lcd_printf(shiftedMsg);


	        n++;
	    }
	}

	// lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h
}
