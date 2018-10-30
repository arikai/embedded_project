#ifndef  __TIMER_H__
#define __TIMER_H__

#define CPU_FREQ 11059200 // ADuC812
#define TIMER_FREQ (CPU_FREQ/12) 

#define TIMER_MS (TIMER_FREQ / 1000)
#define TIMER_MS_TICK_HIGH ((TIMER_MS >> 8) & 0xf)
#define TIMER_MS_TICK_LOW  (TIMER_MS & 0xff) 

#define TIMER_DELAY_HIGH ((unsigned char)(0xff - TIMER_MS_TICK_HIGH))
#define TIMER_DELAY_LOW  ((unsigned char)(0xff - TIMER_MS_TICK_LOW))

extern volatile unsigned long timer_ms;

void InitTimer(void);
unsigned long GetMsCounter(void);
unsigned long DTimeMs(unsigned long t0);
void DelayMs(unsigned long t);

#endif /*  __TIMER_H__ */

