#include "timer.h"
#include "aduc812.h"

volatile unsigned long timer_ms;

void InitTimer(void)
{
    /*
    |                   Timer 0 (16 bit)
    |   T1 interrupt -> increment Counter 1 (16 bit) 
    | INT0 interrupt -> change mode of LED display
    */

    timer_ms = 500;
    TCON = 0;
    TMOD = 0;
    // IE = 0;
    EA = 1; // Enable interrupts

    // Timer 0 - 8-bit timer 
    TH0 = TIMER_DELAY_HIGH;
    TL0 = TIMER_DELAY_LOW;
    
    TMOD |= TMOD_T0_TYPE_TIMER | TMOD_T0_MODE_16BIT;
    TCON |= TCON_T0_ENABLE | TCON_IE0_TYPE_FRONT;
    // TCON |= TCON_IE0_TYPE_FRONT;

    ET0 = 1; // Enable Timer 0 Interrupt
    EX0 = 1; // Enable External Interrupt 0

    // Timer 1 - IE1 Front counter 
    TH1 = 0;
    TL1 = 0;
    TMOD |= TMOD_T1_TYPE_COUNTER | TMOD_T1_MODE_16BIT;
    TCON |= TCON_T1_ENABLE; // | TCON_IE1_TYPE_FRONT;
}

unsigned long GetMsCounter(void)
{
    return timer_ms;
}

unsigned long DTimeMs(unsigned long t0)
{
    return timer_ms - t0;
}

void DelayMs(unsigned long t)
{
    unsigned long target = t + timer_ms;
    while( timer_ms != target );
}

