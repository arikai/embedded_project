#include "aduc812.h"
#include "max.h"
#include "led.h"

static unsigned char old_led = 0;   

void leds_pwm( struct leds_state* s )
{
    unsigned char i, new_led = 0, out;
    for( i = 0 ; i < 8 ; ++i ){
        out = 0;
        if( s->brightness[i] != 0 ){
            if( s->state[i] == 0 ){
                out = 1;
                s->state[i] = LED_MAX_BRIGHTNESS - s->brightness[i];
            }
            else
                --s->state[i];
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

