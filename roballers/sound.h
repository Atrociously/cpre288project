#ifndef SOUND_H_
#define SOUND_H_

//no clue how this will work yet
//loads song onto cybot
//returns true if successful
//false if not successful
bool sound_load_song(int song);

//plays loaded song for specified amount of time
void sound_play_song(int time);