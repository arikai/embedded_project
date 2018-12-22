#include "keyboard.h"
#include "system.h"

#include "led.h"

uint8_t keys[KEYNUM] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D',
};

void kb_init(void){
    // or_ena(ENA_KB);
}

struct kb_event internal_kb_event; 

#define PRESS_THRESHOLD 20
#define RELEASE_THRESHOLD 30

void kb_process(void){
    static uint8_t col_mask; // Mask for KB register. ALSO used as a flag that 
			     // some key was pressed recently
    static uint8_t key_mask; // Mask to check if watched key is pressed
    static uint8_t key;      // Key Pressed (ASCII char)

    static uint8_t press_time;
    static uint8_t release_time;

    uint8_t col_num;

    uint8_t key_pressed;

    if( !col_mask ){ // No key was pressed recently
	kb_set_mask(0xf0); // Read whole keyboard again
	key_pressed = kb_any_key_pressed();

	if( !key_pressed ){
	    return;
	}
	else 
	{
	    for( col_num = 0; col_num < 4; ++col_num ){
		col_mask = (0xff ^ (1 << col_num));
		kb_set_mask(col_mask); // Read only col_num column's keys
		key_mask = ~kb_raw() & 0xf0;
		if( key_mask ) // If any key of this column is pressed
		{
		    switch(key_mask)
		    {
			case 0x10: key = keys[0 + col_num]; break;
			case 0x20: key = keys[4 + col_num]; break;
			case 0x40: key = keys[8 + col_num]; break;
			case 0x80: key = keys[12 + col_num]; break;
		    }
		    press_time = 1;
		    release_time = 0;
		    kb_set_mask(col_mask); // Read only specific column now
		    return;
		}
	    }
	}
    }
    else { // Some key was pressed recently and is watched
	key_pressed = ~kb_raw() & key_mask; // If watched key is pressed

	if( key_pressed ){
	    if( press_time < PRESS_THRESHOLD )
	    {
		++press_time;
	    }
	    else if( press_time == PRESS_THRESHOLD )
	    {
		kb_push_event(key, KB_PRESS);
		release_time = 0;
	    }
	}
	else
	{
	    if( release_time < RELEASE_THRESHOLD )
	    {
		++release_time;
	    }
	    else if( release_time == RELEASE_THRESHOLD )
	    {
		kb_push_event(key, KB_RELEASE);
		press_time = 0;
		release_time = 0;
		col_mask = 0;
		// key_mask = 0; // Ideally must be cleared
		// key = 0;      // But CURRENTLY is not used as a flag
	    }
	}
    }

}

struct kb_event kb_event; // Current event. Externally visible. 
			  // Filled after kb_poll_event()

void kb_push_event( uint8_t key, enum kb_event_type type )
{
    internal_kb_event.key = key;
    internal_kb_event.type = type;
}

enum kb_event_type kb_poll_event(void)
{
    kb_event.key  = internal_kb_event.key;
    kb_event.type = internal_kb_event.type;
    internal_kb_event.type = KB_NONE;
    return kb_event.type;
}
