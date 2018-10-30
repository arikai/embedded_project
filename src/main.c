#include "aduc812.h"
#include "timer.h"  
#include "led.h"
#include "max.h"
#include "interrupt.h"

static volatile char mode_flag = 0;

void timer_int_handler() interrupt(TF0_VECTOR)
{
    ++timer_ms;
    TH0 = TIMER_DELAY_HIGH;
    TL0 = TIMER_DELAY_LOW;
}

void int0_handler() interrupt(IE0_VECTOR)
{
    leds_static(0x55);
    mode_flag ^= 1;
}

#define ENA_ADDRESS 4
#define ENA_INT0_MASK 0x20

void ena_int0(){
    write_max(ENA_ADDRESS, ENA_INT0_MASK | read_max(ENA_ADDRESS));
    // write_max(ENA_ADDRESS, 0x38);
}

// Variant 7
void update_leds(struct leds_state* s){
    static char direction = 1; // -1 - left, 1 - right
    static char led_leader = 1; // index of led that has max brightness
                                         // "leader led" moves around,
                                         // leaving fading "led-tail"

    unsigned char i;
    // update brightness of all LEDs
    for( i = 0 ; i < 8; ++i ){
        DEC_BRIGHTNESS(s->brightness[i]);
        s->state[i] = s->brightness[i];
    }
    // progress leader
    led_leader += direction;
    if( !led_leader || led_leader == 7 )
        direction = -direction;

    // update brightness of leader
    s->state[led_leader] = s->brightness[led_leader] = LED_MAX_BRIGHTNESS;
}

#define ANIMATION_TIMEOUT 500 //

void main(void)
{
    int counter = 0;

    struct leds_state state = { {0}, {0} };

    set_vector(TF0_USER_VECTOR, (void*) timer_int_handler);
    set_vector(IE0_USER_VECTOR, (void*) int0_handler);   
    InitTimer();

    ena_int0();
    while( 1 ){
        switch( mode_flag & 1 ){
            case 0: leds_static(TL1); break;
            case 1: 
                leds_pwm(&state); 
                
                counter--;
                if(counter < 0){
                    update_leds(&state);
                    counter = ANIMATION_TIMEOUT; 
                }

                // if( !(timer_ms % ANIMATION_TIMEOUT) )
                    // update_leds(&state);
                break;
        }
    }
}    

