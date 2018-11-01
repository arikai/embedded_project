#include "common.h"
#include "timer.h"
#include "aduc812.h"
#include "interrupt.h"

#include "led.h" // TODO: remove temporary

unsigned long timer_ms;

static next_frame = 0;
#define FRAME_DELAY 500 // ms

void init_timer();
void init_counter();
void init_system_timer();
void timer_int_handler() interrupt(TF0_VECTOR);
void timer2_int_handler() interrupt(TF2_VECTOR);

void init_timers(void)
{
    /*
     *                   Timer 0 (16 bit)
     *   T1 interrupt -> increment Counter 1 (16 bit) 
     * INT0 interrupt -> change mode of LED display
     *		      -> Timer 2 (16 bit) - system timer
     *
    */

    TCON = 0;
    TMOD = 0;
    T2CON = 0;

    init_timer();
    init_counter();
    init_system_timer();
}

// Timer 0 - 8-bit timer 
void init_timer(){
    set_vector(TF0_USER_VECTOR, (void*) timer_int_handler);

    ET0 = 1; // Enable Timer 0 Interrupt

    TH0 = TIMER_DELAY_HIGH;
    TL0 = TIMER_DELAY_LOW;
    
    TMOD |= TMOD_T0_TYPE_TIMER | TMOD_T0_MODE_16BIT;
    TCON |= TCON_T0_ENABLE | TCON_IE0_TYPE_FRONT;
}

// Timer 1 - IE1 Front counter 
void init_counter(){
    TH1 = 0;
    TL1 = 0;

    TMOD |= TMOD_T1_TYPE_COUNTER | TMOD_T1_MODE_16BIT;
    TCON |= TCON_T1_ENABLE | TCON_IE1_TYPE_FRONT;
    // TCON |= TCON_T1_ENABLE; // | TCON_IE1_TYPE_FRONT;
}

void init_system_timer(){
    set_vector(TF2_EXF2_USER_VECTOR, (void*) timer2_int_handler);

    timer_ms = 0;

    ET2 = 1; // Enable interrupt
    PT2 = 1; // Set High Priority

    T2CON |= T2CON_T2_ENABLE | T2CON_T2_TIMER;

    RCAP2H = TH2 = 0xea;
    RCAP2L = TL2 = 0x66;
}

/* HANDLERS */
void timer_int_handler() interrupt(TF0_VECTOR)
{
    static unsigned long time;
    time = get_time();

    TH0 = TIMER_DELAY_HIGH;
    TL0 = TIMER_DELAY_LOW;
}

void timer2_int_handler() interrupt(TF2_VECTOR)
{
    static char err_count = 0;

    ++err_count;
    timer_ms += 1 + err_count&1;
    if( err_count == 5 )
	err_count = 0;
    // ++timer_ms;
    // leds_static(timer_ms/1000);
    leds_pwm();
}

unsigned long get_time(void)
{
    return timer_ms;
}

unsigned long delta_time(unsigned long t0)
{
    return timer_ms - t0;
}

void sleep(unsigned long t)
{
    unsigned long target = t + timer_ms;
    while( timer_ms > target );
}

