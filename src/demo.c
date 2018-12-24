#include <stdio.h>
#include "demo.h"

#include "sound.h"
#include "keyboard.h"
#include "led.h"
#include "display.h"
#include "time.h"
#include "ui.h"

/*
 * Commented to cut binary size => speed up upload size
 */

/*
void piano_keyboard(void)
{
    struct note note;
    uint8_t octave = 4;
    const char* note_names[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    char buf[17];
    uint8_t last_note = 0;
    uint8_t cur_note;

    note.duration = NOTE_16TH;

#define NOTE_CASE(key, oct, note_num, led)				    \
	KB_CASE2(key, KB_PRESS, KB_HOLD):                                   \
	    note.freq = NOTECN(oct, (note_num));                            \
	    leds((led));			                            \
	    snd_play_note(&note);			                            \
	    cur_note = oct<<4 | note_num;                                   \
	    if(last_note != cur_note) {                                     \
		sprintf(buf, "Note: %s%d", note_names[note_num], oct);      \
	    	dp_home();						    \
		dp_print_string(buf);					    \
	    }

    while( kb_poll_event() ) {
	KB_SWITCH(kb_event) {
	    // C Major notes
	    NOTE_CASE('2', octave,   0, 1 << 0); break;
	    NOTE_CASE('3', octave,   2, 1 << 1); break;
	    NOTE_CASE('A', octave,   4, 1 << 2); break;
	    NOTE_CASE('5', octave,   5, 1 << 3); break;
	    NOTE_CASE('6', octave,   7, 1 << 4); break;
	    NOTE_CASE('B', octave,   9, 1 << 5); break;
	    NOTE_CASE('8', octave,  11, 1 << 6); break;
	    NOTE_CASE('9', octave+1, 0, 1 << 7); break;

	    // Controls
	    // Reset octave transposition
	    KB_CASE('0', KB_PRESS): octave = 4; leds(octave); break;
	    // Transpose by octaveave lower
	    KB_CASE('#', KB_PRESS): --octave; if (octave < 2) octave = 2; leds(octave); break;
	    // Transpose by octaveave higher
	    KB_CASE('D', KB_PRESS): ++octave; if (octave > 6) octave = 6; leds(octave); break; 
	}
    }
#undef NOTE_CASE
}
*/

/*
void sound_test(void)
{
    struct note note;
    int i;

    note.duration = NOTE_128TH;

    for( i = 0; i < ALEN(note_table); ++i ){
	note.freq = NOTE(i);
	leds(i);
	snd_play_note(&note);
    }
    for( i = ALEN(note_table)-1; i >= 0 ; --i ){
	note.freq = NOTE(i);
	leds(i);
	snd_play_note(&note);
    }
}
*/


/*
void read_and_display_input(const char* str) // read and display keyboard's input
{
    dp_clear();
    dp_cursor_style(CURSOR_BLINK);

    dp_print_string(str);

    dp_move_cursor(0, 1);
    while(1)
    {
	while(kb_poll_event())
	{
	    if(kb_event.type == KB_PRESS){
		dp_print_char(kb_event.key);
		if( dp_gety() == 0)
		    dp_move_cursor(0, 1);
	    }
	}
    }
}
*/

/*
void login(void)
{
    char buf_login[40];
    char buf_pwd[40];
    uint8_t input_len;

    while(1){
	input_len = read_input("Your name:", 100, buf_login, 16, 1);
	if( buf_login[0] == '\0' )
	    break;

	input_len = read_input("Password:", 100, buf_pwd, 16, 0);
	dp_clear();

	if( buf_login[0] == '2' && buf_login[1] == 'B' && buf_login[2] == '\0' &&
	    buf_pwd[0] == '9' && buf_pwd[1] == 'C' && buf_pwd[2] == '\0' )
	{
	    print("Hello, ", 50);
	    print(buf_login, 50);
	    print("!", 50);
	}

	else {
	    print("Who are you?", 250);
	}

	sleep(2000);
    }
}
*/
