#include "keyboard.h"
#include "system.h"

uint8_t keys[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

void kb_init(void){
    // or_ena(ENA_KB);
}

struct kb_event internal_kb_event;

void kb_process(void){
    uint8_t row_num;
    uint8_t col_num;
    uint8_t kb_value;
    uint8_t key;

    for( col_num = 0; col_num < 4; ++col_num ){
	kb_set_mask(0xff ^ (1 << col_num)); // Read only col_num column's keys
	kb_value = ~kb_raw();
	if (kb_value & 0xf0 ){
	    for( row_num = 0; row_num < 4; ++row_num ) {
		if (kb_value & (0x10 << row_num)) {
		    key = keys[(row_num<<2) + col_num];
		    kb_push_event(key, KB_PRESS);
		    goto end;
		}
	    }
	}
    }
end:
    kb_set_mask(0xf0); // Read whole keyboard again
}

struct kb_event kb_event;

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
