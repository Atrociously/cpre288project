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
#include <string.h>

#define LCD_WIDTH 20

void bannerPrint(char *text, unsigned int offset) {
    unsigned int length = strlen(text);
    if (length <= LCD_WIDTH) {
        lcd_clear();
        lcd_puts(text);
    } else if (offset < length) {
        unsigned int idx = 0;
        lcd_clear();
        while (idx < LCD_WIDTH && idx + offset < length) {
            lcd_putc(text[offset + idx++]);
        }
        int i = 0;
        while (i < LCD_WIDTH - idx) {
            lcd_putc(text[i++]);
        }
    }
}

void rotatingBanner(char *text) {
    uint8_t offset = 0;
    while (1) {
        bannerPrint(text, offset);
        timer_waitMillis(300);
        offset += 1;
        if (offset >= strlen(text)) {
            offset = 0;
        }
    }
}

int main (void) {

	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	rotatingBanner("A demonstration of my banner -- ");

	return 0;
}
