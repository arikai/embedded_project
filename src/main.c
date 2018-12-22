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
    uint8_t oct = 4;
    
    uint8_t kb_value = 0xf0;
    note.duration = NOTE_SIXTEENTH;

    init_system();

    while(1){
	while( kb_poll_event() ) {
	    // leds(kb_event.type);
	    if(kb_event.type == KB_PRESS)
		switch(kb_event.key) {
		    case '2': note.freq = NOTECN(oct, 0); play_note(&note);  leds(1 << 0); break;
		    case '3': note.freq = NOTECN(oct, 2); play_note(&note);  leds(1 << 1); break;
		    case 'A': note.freq = NOTECN(oct, 4); play_note(&note);  leds(1 << 2); break;
		    case '5': note.freq = NOTECN(oct, 5); play_note(&note);  leds(1 << 3); break;
		    case '6': note.freq = NOTECN(oct, 7); play_note(&note);  leds(1 << 4); break;
		    case 'B': note.freq = NOTECN(oct, 9); play_note(&note);  leds(1 << 5); break;
		    case '8': note.freq = NOTECN(oct, 11); play_note(&note); leds(1 << 6); break;
		    case '9': note.freq = NOTECN(oct, 12); play_note(&note); leds(1 << 7); break;

		    case '0': oct = 4; leds(oct); break;
		    case '#': --oct; if (oct < 2) oct = 2; leds(oct); break;
		    case 'D': ++oct; if (oct > 6) oct = 6; leds(oct); break; 
		    // case 'C': note.freq = NOTECN(4, 8); play_note(&note); break;
		    // case '0': note.freq = NOTECN(4, 9); play_note(&note); break;
		    // case '#': note.freq = NOTECN(4,10); play_note(&note); break;
		    // case 'D': note.freq = NOTECN(4,11); play_note(&note); break;
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
