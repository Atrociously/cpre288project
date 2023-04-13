/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Lincoln Hatlestad and Doyle chism
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
#include "servo.h"
#include "button.h"
#include "cybot_Scan.h"

int main (void) {

    //INITIALIZATION
       timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                       // and enables time functions (e.g. timer_waitMillis)
       lcd_init();   // Initialize the LCD screen.  This also clears the screen.

       servo_init(); //initializes servo

       button_init(); //initializes SW1-4

//       int degrees = 90;
//       int direction = 0; //counter clockwise is 0 and clockwise is 1
//       int button_pressed;
//
//       servo_move(degrees);
//
       while(1) {
//           button_pressed = button_getButton();
//
//           //could do nested if statements for (degrees - x, for clockwise)
//           // within 0-180 degrees
//           if(button_pressed == 1) { // SW1: move 1 degree
//               if(direction == 0) {
//                   servo_move(degrees + 1);
//                   degrees += 1;
//               }
//               else {
//                   servo_move(degrees - 1);
//                   degrees -= 1;
//               }
//           }
//           if(button_pressed == 2){ // SW2: move 5 degrees
//               if(direction == 0) {
//                   servo_move(degrees + 5);
//                   degrees += 5;
//               }
//               else {
//                   servo_move(degrees - 5);
//                   degrees -= 5;
//               }
//           }
//           if(button_pressed == 3){ // SW3: switch between counterclockwise and clockwise
//               if(direction == 0) { //switch from counterclockwise to clockwise
//                   direction = 1;
//               }
//               else {               //switch from clockwise to counterclockwise
//                   direction = 0;
//               }
//           }
//           if(button_pressed == 4){ //SW4: Move 5 degrees in clockwise mode. Move 175 degrees in counterclockwise
//               if(direction == 0) {
//                   servo_move(175);
//                   degrees = 175;
//               }
//               else {
//                   servo_move(5);
//                   degrees = 5;
//               }
//           }
           // print out the match value and direction on LCD


           servo_move(90);
           servo_move(30);
           servo_move(150);
           servo_move(0);
       }
	}
