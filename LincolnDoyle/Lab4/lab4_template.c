/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

//#define REPLACEME 0



int main(void) {
    button_init();
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    // Don't forget to initialize the cyBot UART before trying to use it
    cyBot_uart_init();


    int button_pressed;
    char str[20];
    int n;

    while(1)
    {
        button_pressed = button_getButton();
        lcd_printf("%d", button_pressed);

        //printing to putty
        sprintf(str ,"button pressed %d\n\r", button_pressed);
        n = 0; //iterates through the index of the str
        while(n < strlen(str)) { //prints data message to the PuTTY
            cyBot_sendByte(str[n]);
            n++;
        }

    }
}
