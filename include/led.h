#ifndef __LED__H
#define __LED__H

#define LED_MAX_BRIGHTNESS 5
#define LED_MIN_BRIGHTNESS 0 // found empirically value when flickering is hard to notice 

#define DEC_BRIGHTNESS(v) do { (v) = ((v) == LED_MIN_BRIGHTNESS) ? LED_MIN_BRIGHTNESS : (v)-1; } while(0)
#define INC_BRIGHTNESS(v) do { (v) = ((v) == LED_MAX_BRIGHTNESS)) ? LED_MAX_BRIGHTNESS : (v)+1; } while(0)

struct leds_state {
    unsigned char brightness[8]; 
    unsigned char state[8];
};

void leds_pwm();
void leds_static( unsigned char on );

void update_leds();

#endif //__LED__H
