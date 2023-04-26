#include "music.h"



int main() {
    oi_init();

    oi_load_song(RICK_ROLL, rickrollNumNotes, rickrollNotes, rickrollDuration);

    cyproto_read_command();
}