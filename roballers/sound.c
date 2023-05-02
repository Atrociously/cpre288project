#include "sound.h"
#include "open_interface.h"


#define MICHAEL_MYERS 0

//loads song index of choice to the oi
void sound_load_halloweenTheme() {

    unsigned int halloweenNumNotes = 10;
    unsigned char halloweenNotes[10]    = {96,89,89,96,89,89,96,89,97,89};
    unsigned char halloweenDuration[10] = {13,13,13,13,13,13,13,13,13,13};
    oi_loadSong(MICHAEL_MYERS, halloweenNumNotes, halloweenNotes, halloweenDuration);
}

//plays a song of choice numLoops times
void sound_play_halloweenTheme() {
    oi_play_song(MICHAEL_MYERS);
    timer_waitMillis(2075);
    oi_play_song(MICHAEL_MYERS);
    timer_waitMillis(2075);
    oi_play_song(MICHAEL_MYERS);
    timer_waitMillis(2075);
}


