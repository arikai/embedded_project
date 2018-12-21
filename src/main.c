#include "aduc812.h"
#include "system.h"
#include "timer.h"
#include "led.h"
#include "sound.h"

#define TESTNOTE 55
struct note __xdata notes[] = {
    { TESTNOTE, NOTE_WHOLE },
    { NOTE_REST, NOTE_HALF },
};

void main(void) {
    int8_t counter = 0;
    int i;
    struct note note;
    note.duration = NOTE_SIXTEENTH;

    init_system();

    while(1) {
        for( i = 0; i < ALEN(note_table); ++i ){
	    note.freq = NOTE(i);
	    leds(i);
	    play_note(&note);
        }
        for( i = ALEN(note_table)-1; i >= 0 ; --i ){
	    note.freq = NOTE(i);
	    leds(i);
	    play_note(&note);
        }
    }
    // while(1) { sleep(100); leds(counter++); }
}
