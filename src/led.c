#include "aduc812.h"
#include "max.h"
#include "led.h"

struct leds_state state = { {0}, {0} };

static unsigned char old_led = 0;   

void leds_pwm()
{
    unsigned char i, new_led = 0, out;
    for( i = 0 ; i < 8 ; ++i ){
	out = 0;
	if( state.brightness[i] != LED_MIN_BRIGHTNESS ) {
	    ++state.state[i];
	    if( state.state[i] >= LED_MAX_BRIGHTNESS ){
		out = 1;
		state.state[i] = state.brightness[i];
	    }
	}
        new_led = (new_led << 1) | out;
    }
    leds_static(new_led);
}

void leds_static( unsigned char on )
{
    write_max( SV, on );     
    old_led = on;
}

// Variant 7
void update_leds(){
    static char direction = 1; // -1 - left, 1 - right
    static char led_leader = 1; // index of led that has max brightness
                                         // "leader led" moves around,
                                         // leaving fading "led-tail"

    unsigned char i;

    led_leader += direction;

    for( i = 0 ; i < 8; ++i ){
	if( i != led_leader ){
	    DEC_BRIGHTNESS(state.brightness[i]);
	}
	else
	    state.brightness[i] = LED_MAX_BRIGHTNESS;
    }

    if( !led_leader || led_leader == 7 )
        direction = -direction;
}

