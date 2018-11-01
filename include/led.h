#ifndef __LED__H
#define __LED__H

#define LED_MAX_BRIGHTNESS 255
#define LED_MIN_BRIGHTNESS 231 // found empirically value when flickering is hard to notice 
// #define LED_MIN_BRIGHTNESS 231 // found empirically value when flickering is 
//hard to notice 
#define LED_STEPS_NUM 6	
#define LED_BRIGHTNESS_STEP ((LED_MAX_BRIGHTNESS - LED_MIN_BRIGHTNESS)/(LED_STEPS_NUM-2))

#define LED_MAX_BRIGHTNESS 255
#define LED_MIN_BRIGHTNESS 231 // found empirically value when flickering is hard to notice 
extern unsigned char led_steps[];

#define DEC_BRIGHTNESS(b) do { b = ((b)-LED_BRIGHTNESS_STEP) < LED_MIN_BRIGHTNESS ? 0 : ((b)-LED_BRIGHTNESS_STEP); } while(0)
#define INC_BRIGHTNESS(b) do { b = ((LED_MAX_BRIGHTNESS - LED_BRIGHTNESS_STEP) <= (b) ? LED_MAX_BRIGHTNESS : (b) + LED_BRIGHTNESS_STEP) } while(0)

struct leds_state {
    unsigned char brightness[8]; 
    unsigned char state[8];
};

void leds_pwm();
void leds_static( unsigned char on );

void update_leds();

#endif //__LED__H
