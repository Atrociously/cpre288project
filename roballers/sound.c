#include "sound.h"
#include "open_interface.h"


#define MICHAEL_MYERS 0

//loads the michael myers halloween theme to oi
void sound_load_halloweenTheme() {

    unsigned int halloweenNumNotes = 10;
    unsigned char halloweenNotes[10]    = {96,89,89,96,89,89,96,89,97,89}; //96 is C; 89 is F; 97 is C#
    unsigned char halloweenDuration[10] = {13,13,13,13,13,13,13,13,13,13}; //each note playsfor 13/64 of a second
    oi_loadSong(MICHAEL_MYERS, halloweenNumNotes, halloweenNotes, halloweenDuration); //loads song to oi
}

//plays a song of choice for 4 measures
void sound_play_halloweenTheme() {
    oi_play_song(MICHAEL_MYERS);
    timer_waitMillis(2075); //time it takes to complete oi_play_song(MICHAEL_MYERS) function
    oi_play_song(MICHAEL_MYERS);
    timer_waitMillis(2075);
    oi_play_song(MICHAEL_MYERS);
    timer_waitMillis(2075);
}


