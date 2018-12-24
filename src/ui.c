#include "ui.h"

#include "system.h"
#include "time.h"
#include "display.h"
#include "led.h"

void print(char* str, unsigned long interval)
{
    dp_cursor_style(CURSOR_OFF);
    while(*str)
    {
	dp_print_char(*str++);
	if( interval )
	    sleep(interval);
    }
}


uint8_t read_input(
	char* prompt, unsigned long interval,
	char* buf, uint8_t maxlen, bit echo
	)
{
    uint8_t i;
    uint8_t key;

    maxlen = (maxlen > DP_MAX_X ) ? DP_MAX_X : maxlen;

    dp_clear();

    if( prompt )
	print(prompt, interval);

    dp_move_cursor(0,1);
    dp_cursor_style(CURSOR_BLINK);
    for( i = 0;; )
    {
	kb_poll_event();

	if(KB_IS( kb_event, KB_IS_PRESSED ))
	{
	    key = kb_event.key;
	    switch( key )
	    {
		case 'D':
		    if( i > 0 ) 
		    {
			leds(0xff);
			--i;
			if( echo ){
			    dp_shift_cursor(0);
			    dp_print_char(' ');
			    dp_shift_cursor(0);
			}
		    }
		    break;

		case '#': 
		    buf[i] = '\0';
		    return i;

		default:
		    if( i < (maxlen-1) )
		    {
			if(echo) 
			    dp_print_char(key);
			buf[i++] = key;
			break;
		    }
	    }
	}
    }
    return 0;
}
	

