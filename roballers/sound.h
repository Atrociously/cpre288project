#ifndef SOUND_H_
#define SOUND_H_

#define RICK_ROLL   0
//#define IMERPIAL_MARCH      1
//#define MARIO_UNDERWORLD    3
//#define MARIO_UNDERWATER    7
#define JAWS    1


//include these lines for for proper use
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);



//plays loaded song for specified amount of time
void sound_load_song(int time);

//plays a song of choice numLoops times

void sound_play_song(int songIndex, int numLoops);


#endif
