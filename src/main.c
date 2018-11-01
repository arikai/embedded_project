#include "aduc812.h"
#include "timer.h"  
#include "led.h"
#include "interrupt.h"
#include "common.h"

volatile unsigned char led_mode = 0;

static unsigned long next_frame = 0;
#define FRAME_DELAY 50
void main(void)
{
    // int i;
    int counter = 0;

    init_interrupts();
    init_timers();

    while(1)
	if( get_time() >= next_frame ){
	    next_frame = get_time() + FRAME_DELAY;
	    update_leds();
	}
}
