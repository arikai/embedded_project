#include "aduc812.h"
#include "system.h"
#include "timer.h"
#include "led.h"
#include "sound.h"
#include "keyboard.h"

#define TESTNOTE 55
struct note __xdata notes[] = {
    { TESTNOTE, NOTE_WHOLE },
    { NOTE_REST, NOTE_HALF },
};

void main(void) {
    int8_t counter = 0;
    int i;
    struct note note;
    
    uint8_t kb_value = 0xf0;
    note.duration = NOTE_SIXTEENTH;

    init_system();

    while(1){
	while( kb_poll_event() ) {
	    leds(kb_event.key);
	    // leds(kb_event.type);
	    switch(kb_event.key) {
		case '2': note.freq = NOTECN(3, 0); play_note(&note); break;
		case '3': note.freq = NOTECN(3, 1); play_note(&note); break;
		case 'A': note.freq = NOTECN(3, 2); play_note(&note); break;
		case '5': note.freq = NOTECN(3, 3); play_note(&note); break;
		case '6': note.freq = NOTECN(3, 4); play_note(&note); break;
		case 'B': note.freq = NOTECN(3, 5); play_note(&note); break;
		case '8': note.freq = NOTECN(3, 6); play_note(&note); break;
		case '9': note.freq = NOTECN(3, 7); play_note(&note); break;
		case 'C': note.freq = NOTECN(3, 8); play_note(&note); break;
		case '0': note.freq = NOTECN(3, 9); play_note(&note); break;
                case '#': note.freq = NOTECN(3,10); play_note(&note); break;
		case 'D': note.freq = NOTECN(3,11); play_note(&note); break;
	    }
	}
    }

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
