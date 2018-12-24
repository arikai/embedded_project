#include "ui.h"

#include "system.h"
#include "display.h"
#include "led.h"
#include "bindings.h"

void print(char* str, 
	time_t delay, 
	time_t newline_delay
     )
{
    dp_cursor_style(CURSOR_OFF);
    while(*str != '\0')
    {
	if( *str == '\n' )
	{
	    if( newline_delay )
		sleep(newline_delay);
	    dp_newline();
	}
	else
	{
	    if( delay && *str != ' ' )
		sleep(delay);
	    dp_print_char(*str);
	}
	str++;
    }
}

void print_number(uint16_t number, 
	time_t delay, 
	time_t newline_delay)
{
    char buf[10];
    uint16_t digit;
    uint8_t i = 0;
    if( digit = number / 10000 ) buf[i++] = digit + '0';
    if( ( digit = ( number  % 10000 ) / 1000 ) || i ) buf[i++] = digit + '0';
    if( ( digit = ( number  % 1000 )  / 100  ) || i ) buf[i++] = digit + '0';
    if( ( digit = ( number  % 100 )   / 10   ) || i ) buf[i++] = digit + '0';
    buf[i++] = (number % 100) / 10 + '0';
    buf[i] = '\0';

    print(buf, delay, newline_delay);
}

uint8_t read_input(
	char* prompt, time_t delay,
	char* buf, uint8_t maxlen, bit echo
	)
{
    uint8_t i;
    uint8_t key;

    maxlen = (maxlen > DP_MAX_X ) ? DP_MAX_X : maxlen;

    dp_clear();

    if( prompt )
	print(prompt, delay, 0);

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
		case BIND_BACKSPACE:
		    if( i > 0 ) 
		    {
			--i;
			if( echo ){
			    dp_shift_cursor(0);
			    dp_print_char(' ');
			    dp_shift_cursor(0);
			}
		    }
		    break;

		case BIND_ENTER:
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
}
	
uint8_t read_ok(char* prompt)
{
    clear();
    info(prompt, DELAY_NONE);
    newline();
    sleep(500);
    info(BIND_OK_S " - Yes, " BIND_CANCEL_S " - No ", DELAY_NORMAL);

    while(1) {
	switch(read_key(0)) 
	{
	    case BIND_OK:     return 1;
	    case BIND_CANCEL: return 0;
	}
    }
}
uint8_t read_key(time_t timeout)
{
    time_t timeout_time;
    bit timeout_active;

    timeout_active = ( timeout != 0 );
    if( timeout_active )
	timeout_time = get_time() + timeout;

    while(1)
    {
	if( kb_poll_event() == KB_PRESS )
	    return kb_event.key;

	if( timeout_active && (timeout_time <= get_time()) )
	    return 0;
    }
}

