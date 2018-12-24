#include <stdio.h>

#include "aduc812.h"

#include "system.h"

#include "time.h"
#include "led.h"
#include "sound.h"
#include "keyboard.h"
#include "display.h"
#include "ui.h"
#include "random.h"

#include "game.h"
#include "demo.h"

void main(void) {
    int8_t counter = 0;
    int i = 0;

    struct note note;

    note.pitch_idx = C4;
    note.duration_64ths = NOTE_1;

    init_system();

    game_start();
    while(1);

    /* Some demos from demo.c */
    // sound_test();     // Play all notes with ascending and descending pitch
    // piano_keyboard(); // Infinite loop for playing Cmaj on keyboard
    // read_and_display_input("Hello, Frisk!"); // Show prompt and read and 
    //display keyboard's input
    // login(); // login demo
}
