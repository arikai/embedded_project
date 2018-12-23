#include <stdio.h>

#include "aduc812.h"

#include "system.h"

#include "timer.h"
#include "led.h"
#include "sound.h"
#include "keyboard.h"
#include "display.h"

#include "demo.h"

#define NOTEBN(o, n) NOTECN(o+1, n-1)
struct note __xdata track[] = {
    { NOTEBN(4, 0), NOTE_8TH },
    { NOTE_REST   , NOTE_32TH },
    { NOTEBN(4, 6), NOTE_8TH },
    { NOTE_REST   , NOTE_32TH },
    { NOTEBN(4, 4), NOTE_8TH },
    { NOTE_REST   , NOTE_32TH },
    { NOTEBN(4, 0), NOTE_8TH },

    { NOTE_REST   , NOTE_16TH },
    { NOTEBN(4, 3), NOTE_8TH },

    { NOTE_REST   , NOTE_8TH },
    { NOTEBN(4, 3), NOTE_8TH },

    { NOTE_REST   , NOTE_8TH },
    { NOTEBN(4, 4), NOTE_8TH },

    { NOTE_TRACK_END }
};

void main(void) {
    int8_t counter = 0;
    int i = 0;

    init_system();
    play_track(track);

    /* Some demos from demo.c */
    // sound_test();     // Play all notes with ascending and descending pitch
    // piano_keyboard(); // Infinite loop for playing Cmaj on keyboard
    // read_and_display_input(); // read and display keyboard's input
}
