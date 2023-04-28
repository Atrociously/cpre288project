#include "sound.h"


//loads song index of choice to the oi
void sound_load_songs() {
    //Never Gonna Give You UP
    int rickrollNumNotes = 11;
    unsigned char rickrollNotes[11]    = {53, 55, 48, 55, 57, 60, 58, 57, 53, 55, 48};
    unsigned char rickrollDuration[11] = {48, 64, 16, 48, 48, 8,  8,  8,  48, 64, 64};
    oi_loadSong(RICK_ROLL, rickrollNumNotes, rickrollNotes, rickrollDuration);

    //Jaws Theme
}

//plays a song of choice numLoops times
void sound_play_song(int songIndex, int numLoops) {

    int n; //loop iterator

    while(n < numLoops) {
        timer_waitMillis(8000);
        oi_play_song(songIndex);
        n++;
    }
}


