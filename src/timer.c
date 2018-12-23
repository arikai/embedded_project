#include "timer.h"
#include "sound.h" // Sound Timer (Timer 0) 
#include "aduc812.h"
#include "interrupt.h"
#include "keyboard.h"

#include "led.h"

/*
 * ADuC's Timers Usage:
 *   Timer 0:
 *   Timer 1: System Time
 *   Timer 2:
 */

#define TIMER_MS (TIMER_FREQ / 1000)
#define TIMER_MS_TICK_HIGH ((TIMER_MS >> 8) & 0xff)
#define TIMER_MS_TICK_LOW  (TIMER_MS & 0xff) 

#define TIMER_DELAY_HIGH ((unsigned char)(0xff - TIMER_MS_TICK_HIGH))
#define TIMER_DELAY_LOW  ((unsigned char)(0xff - TIMER_MS_TICK_LOW))

static unsigned long timer_ms;

void init_sound_timer(void);
void sound_int_handler(void) interrupt(TF0_VECTOR);

void init_system_timer(void);
void system_timer_int_handler(void) interrupt(TF1_VECTOR);

void init_timers(void)
{
    TCON = 0;
    TMOD = 0;
    T2CON = 0;

    init_sound_timer();
    init_system_timer();
}

/* Init Sound Timer (Timer 0) */
void init_sound_timer(void){
    // ET0 = 1;

    TH0 = 0xFF;
    TL0 = 0xFF;

    PT0 = 1; // High Priority for "smooth" sound

    TMOD |= TMOD_T0_TYPE_TIMER | TMOD_T0_MODE_16BIT;
    TCON |= TCON_T0_ENABLE;

    set_vector(TF0_USER_VECTOR, (void*) sound_int_handler);
}

/* Sound Timer (Timer 0) Interrupt Handler */
void sound_int_handler(void) interrupt(TF0_VECTOR)
{
    TL0 = note_period[0];
    TH0 = note_period[1];

    beep();
}

/* Init System Timer (Timer 1) */
void init_system_timer(void){
    ET1 = 1;

    TH1 = TIMER_DELAY_HIGH;
    TL1 = TIMER_DELAY_LOW;

    TMOD |= TMOD_T1_TYPE_TIMER | TMOD_T1_MODE_16BIT;
    TCON |= TCON_T1_ENABLE;

    set_vector(TF1_USER_VECTOR, (void*) system_timer_int_handler);
}

/* Timer 1 Interrupt Handler */
void system_timer_int_handler(void) interrupt(TF1_VECTOR)
{
    ++timer_ms;
    TH1 = TIMER_DELAY_HIGH;
    TL1 = TIMER_DELAY_LOW;

    /* Keyboard Events handler */
    kb_process();
}

unsigned long get_time(void)
{
    unsigned long local = timer_ms; // Avoid possible interruption
    return local;
}

unsigned long delta_time(unsigned long t0)
{
    return get_time() - t0;
}

void sleep(unsigned long t)
{
    unsigned long target = t + get_time();
    sleep_till(target);
}

void sleep_till(unsigned long target)
{
    while( get_time() <= target );
}

