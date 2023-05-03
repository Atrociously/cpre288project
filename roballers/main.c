#include "cyproto_compat.h"
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"
#include "open_interface.h"
#include "movement.h"
#include "objects.h"
#include "uart_int.h"
#include "servo.h"
#include "sound.h"

int main() {
    // servo calibration
    servo_pwm_max = 2.24; // default 2.25
    servo_pwm_min = 0.49; // defult 0.5

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                    // and enables time functions (e.g. timer_waitMillis)

    uart_interrupt_init(); //Initialize uart interrupts

    oi_t *sensor_data = oi_alloc(); //Initializes the open interface functions and sensor data
    oi_init(sensor_data);           //(needed for movement and stuff)

    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    //SCARY FACE FOR ROBOT
    lcd_gotoLine(1);
    lcd_puts("    .\\   /.");
    lcd_gotoLine(2);
    lcd_puts("       >");
    lcd_gotoLine(3);
    lcd_puts("      000");
    lcd_gotoLine(4);
    lcd_puts("DIE HUMANS!!");

//    uint8_t *buffer[] = (uint8_t)malloc(cyproto_buffer_size);
    CommandRequest comm;


    while(1) { //always read commands from UI
        //1) waits for command to be read in
        comm = read_command(); //returns type CommandRequest
        uint8_t data_buffer[cyproto_buffer_size()];
        //3) execute command
        if(comm.tag == Drive) { //drive command
            //fill structs from cyproto
            //movement functions return structs that initialize drive data
           DriveDone drive_data;

           //moves cybot and updates data
            if (comm.drive.distance < 0) {
                drive_data = move_backwards(sensor_data, abs(comm.drive.distance), comm.drive.speed);
             } else {
                 drive_data = move_forward(sensor_data, abs(comm.drive.distance), comm.drive.speed);
             }

            cyproto_drive_done(drive_data ,data_buffer); //buffer used for sending data from cybot to UI
        }
        else if(comm.tag == Turn) { //turn command
            //fill structs from cyproto
            //movement functions return structs that initialize drive data
            TurnDone turn_data;

            if(comm.turn.angle < 0) {
                turn_data = turn_right(sensor_data, comm.turn.angle, comm.turn.speed);
            } else {
                turn_data = turn_left(sensor_data, comm.turn.angle, comm.turn.speed);
            }

            //sends data from cybot to UI
            cyproto_turn_done(turn_data ,data_buffer);
        }
        else if(comm.tag == Scan) { //scan command
            //fill structs from cyproto
            //movement functions return structs that initialize drive data
            ScanDone scan_data;

            //tracks max of 21 objects
            //calls scan to cybot
            Obj_t objects[21];
            ObjectData found[21]; //tracks found objects data
            size_t amt = scan_objects(objects, 21);

            //updates data of objects based on scan from cybot
            int i = 0;
            for (i = 0; i < amt; i++) {
                found[i].angle = objects[i].right + ((objects[i].left - objects[i].right) / 2);
                found[i].distance = objects[i].distance;
                found[i].width = objects[i].arc_len;
            }
            scan_data.objects = found; //once data is updated object is "found"
            scan_data.size = amt;

            //scan function returns struct that initializes scan data to be sent to UI
            size_t v = cyproto_scan_done(scan_data ,data_buffer);
            scan_data.size = v;
        }
        else { //command error
            lcd_printf("command error!");
        }


        //5) send response back to UI
        send_response(data_buffer);
    }


    oi_free(sensor_data); //frees the oi so robot isn't locked up
}

