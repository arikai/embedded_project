#include "aduc812.h"
#include "timer.h"  
#include "led.h"
#include "interrupt.h"
#include "common.h"

volatile unsigned char led_mode = LED_MODE_ANIMATION;

static unsigned long next_frame = 0;
#define FRAME_DELAY 10
void main(void)
{
    init_interrupts();
    init_timers();

    while(1){
	if( led_mode == LED_MODE_ANIMATION ) update_leds();
	sleep(FRAME_DELAY);
    }
}
