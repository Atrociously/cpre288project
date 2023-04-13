/**
 * @file lab9_template.c
 * @author lincoln hatlestad and doyle chism
 * Template file for CprE 288 Lab 9
 */

#include "ping_template.h"
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"

// Uncomment or add any include directives that are needed



int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	ping_init();

	// YOUR CODE HERE
    char str[50];

	while(1)
	{
	    // YOUR CODE HERE

	    sprintf(str, "Distance: %.2f", ping_getDistance());
	    lcd_printf(str);
	}

}
