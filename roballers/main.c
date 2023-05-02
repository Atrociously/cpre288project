#include "cyproto_compat.h"
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"
#include "open_interface.h"
#include "movement.h"
#include "objects.h"
#include "uart_int.h"
//#include "cyproto.h"

#define RICK_ROLL   0
//#define IMERPIAL_MARCH      1
//#define MARIO_UNDERWORLD    3
//#define MARIO_UNDERWATER    7
#define JAWS    4

int main() {

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                    // and enables time functions (e.g. timer_waitMillis)

    //lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    uart_interrupt_init(); //Initialize uart interrupts

    oi_t *sensor_data = oi_alloc(); //Initializes the open interface functions and sensor data
    oi_init(sensor_data);           //(needed for movement and stuff)



//    uint8_t data_buffer[]; //buffer used for sending data from cybot to UI
//    uint8_t *buffer[] = (uint8_t)malloc(cyproto_buffer_size);
    CommandRequest comm;


    while(1) { //always read commands from UI
        //1) waits for command to be read in
        comm = read_command(); //returns type CommandRequest
        uint8_t data_buffer[cyproto_buffer_size()];
        //3) execute command
        if(comm.tag == Drive) { //drive command
            //fill structs from cyproto
           DriveDone drive_data;

            if (comm.drive.distance < 0) {
                drive_data = move_backwards(sensor_data, abs(comm.drive.distance), comm.drive.speed);
             } else {
                 drive_data = move_forward(sensor_data, abs(comm.drive.distance), comm.drive.speed);
             }

            cyproto_drive_done(drive_data ,data_buffer); //buffer used for sending data from cybot to UI
        }
        else if(comm.tag == Turn) { //turn command
            //fill structs from cyproto
            TurnDone turn_data;

            if(comm.turn.angle < 0) {
                turn_data = turn_right(sensor_data, comm.turn.angle, comm.turn.speed);
            } else {
                turn_data = turn_left(sensor_data, comm.turn.angle, comm.turn.speed);
            }

            cyproto_turn_done(turn_data ,data_buffer);
        }
        else if(comm.tag == Scan) { //scan command
            //fill structs from cyproto
            ScanDone scan_data;

            Obj_t objects[21];
            ObjectData found[21];
            size_t amt = scan_objects(objects, 21);

            int i = 0;
            for (i = 0; i < amt; i++) {
                found[i].angle = objects[i].right + ((objects[i].left - objects[i].right) / 2);
                found[i].distance = objects[i].distance;
                found[i].width = objects[i].arc_len;
            }
            scan_data.objects = found;
            scan_data.size = amt;

            size_t v = cyproto_scan_done(scan_data ,data_buffer);
            scan_data.size = v;
        }
        else { //command error
            lcd_printf("command error!");
        }


        //5) send response back to UI
        send_response(data_buffer);
    }




//
//    //SCARY FACE FOR ROBOT
//    lcd_gotoLine(1);
//    lcd_puts("    .\\   /.");
//    lcd_gotoLine(2);
//    lcd_puts("       >");
//    lcd_gotoLine(3);
//    lcd_puts("      000");
//    lcd_gotoLine(4);
//    lcd_puts("DIE HUMANS!!");

//    Obj_t arr[42];
//    size_t amt = scan_objects(arr, 42);

    oi_free(sensor_data); //frees the oi so robot isn't locked up
}

