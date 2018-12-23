#include <stdio.h>

#include "aduc812.h"

#include "system.h"

#include "timer.h"
#include "led.h"
#include "sound.h"
#include "keyboard.h"
#include "display.h"

#include "demo.h"

XDATA(struct track) track = {
    90,       // BPM
    4, 4,     // Track signature

    TRACK_INF_REPEAT, // Repeat 1 times (play 2 times)
    {
	N(B4 + 0, 8)
	N(B4 + 6, 8)
	N(B4 + 4, 8)
	N(B4 + 0, 8)
	N(B4 + 3, 8d)
	N(B4 + 3, 8d)
	N(B4 + 4, 4)

	N(B4 + 0, 8)
	N(B4 + 4, 8)
	N(B4 + 0, 8)
	N(B4 + 3, 8d)
	N(B4 + 3, 8d)
	N(B4 + 4, 8)

	N(B4 + 0, 8)
	N(B4 + 6, 8)
	N(B4 + 4, 8)
	N(B4 + 0, 8)
	N(B4 + 3, 8d)
	N(B4 + 3, 8d)
	N(B4 + 4, 4)

	N(B4 + 0, 8)
	N(B4 + 4, 8)
	N(B4 + 8, 8)
	N(B4 + 6, 8d)
	N(B4 + 4, 8d)
	N(B4 + 6, 8)

	END
    }
};

void main(void) {
    int8_t counter = 0;
    int i = 0;

    struct note note;

    note.pitch_idx = PITCHCN(4,0);
    note.duration_64ths = NOTE_1;

    init_system();

    snd_set_bg_track(&track);

    /* Some demos from demo.c */
    // sound_test();     // Play all notes with ascending and descending pitch
    // piano_keyboard(); // Infinite loop for playing Cmaj on keyboard
    read_and_display_input("Hello, Frisk!"); // Show prompt and read and display keyboard's input
}
