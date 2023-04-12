/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */
#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "Timer.h"

int main (void) {

    oi_t *sd = oi_alloc(); // allocate memory
    // initialize stuff
    lcd_init();
    oi_init(sd);

    double distance = mvmt_drive(sd, 1000, 500);
    lcd_printf("collision: %.2lf", distance);
    mvmt_drive(sd, -150, 200);
    mvmt_turn(sd, -90, 200);
    mvmt_drive(sd, 250, 200);
    mvmt_turn(sd, 90, 200);
    mvmt_drive(sd, 1000 - distance, 500);
//    mvmt_turn(sd, -90, 100);
//    mvmt_drive(sd, 500, 500);
//    mvmt_turn(sd, -90, 100);
//    mvmt_drive(sd, 500, 500);
//    mvmt_turn(sd, -90, 100);
//    mvmt_drive(sd, 500, 500);
//    mvmt_turn(sd, -90, 100);

    oi_free(sd);

    return 0;
}
