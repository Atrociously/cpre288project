#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"
#include "open_interface.h"
#include "movement.h"
#include "scan.h"

#define RICK_ROLL   0
//#define IMERPIAL_MARCH      1
//#define MARIO_UNDERWORLD    3
//#define MARIO_UNDERWATER    7
#define JAWS    4


int main() {

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                    // and enables time functions (e.g. timer_waitMillis)

    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    oi_t *sensor_data = oi_alloc(); //Initializes the open interface functions and sensor data
    oi_init(sensor_data);           //(needed for movement and stuff)

    //SCARY FACE FOR ROBOT
    lcd_gotoLine(1);
    lcd_puts("    .\\   /.");
    lcd_gotoLine(2);
    lcd_puts("       >");
    lcd_gotoLine(3);
    lcd_puts("      000");
    lcd_gotoLine(4);
    lcd_puts("DIE HUMANS!!");

    scan();

    oi_free(sensor_data); //frees the oi so robot isn't locked up
}


////print object data
//m = 0;
//while(m < num_objects) {
//    sprintf(str, "Object[%d]: Width = %d\tAngle = %d\n\r", m,  objects[m].width, objects[m].angle);
//    uart_sendStr(str);
//    m++;
//}
