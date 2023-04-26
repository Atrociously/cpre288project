#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"
#include "open_interface.h"
#include "music.h"

#define RICK_ROLL   0
//#define IMERPIAL_MARCH      1
//#define MARIO_UNDERWORLD    3
//#define MARIO_UNDERWATER    7
#define JAWS    4


int main() {

    oi_t *sensor_data = oi_alloc();

    oi_init(sensor_data);

    //load_songs();

    int rickrollNumNotes = 11;
    unsigned char rickrollNotes[11]    = {53, 55, 48, 55, 57, 60, 58, 57, 53, 55, 48};
    unsigned char rickrollDuration[11] = {48, 64, 16, 48, 48, 8,  8,  8,  48, 64, 64};
    oi_loadSong(RICK_ROLL, rickrollNumNotes, rickrollNotes, rickrollDuration);

//    int jawsNumNotes = 16;
//    unsigned char jawsNotes[16] = {72, 72, 72, 72, 74, 74, 74, 76, 76, 76, 77, 77, 77, 79, 79, 72};
//    unsigned char jawsDuration[16] = {64, 21, 21, 21, 64, 21, 21, 64, 21, 21, 64, 21, 21, 64, 21, 64};
//    oi_loadSong(JAWS, jawsNumNotes, jawsNotes, jawsDuration);

//    int mario1NumNotes = 49;
//    unsigned char mario1Notes[49]    = {48, 60, 45, 57, 46, 58,  0, 48, 60, 45, 57, 46, 58,  0, 41, 53, 38, 50, 39, 51,  0, 41, 53, 38, 50, 39, 51,  0, 51, 50, 49, 48, 51, 50, 44, 43, 49, 48, 54, 53, 52, 58, 57, 56, 51, 47, 46, 45, 44 };
//    unsigned char mario1Duration[49] = {12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 48,  8,  8,  8, 24, 24, 24, 24, 24, 24,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16, 16 };
//    oi_loadSong(MARIO_UNDERWORLD, mario1NumNotes, mario1Notes, mario1Duration);

    while(1) {
        timer_waitMillis(8000);
        oi_play_song(RICK_ROLL);
    }

    //oi_play_song(JAWS);

    //cyproto_read_command();

}
