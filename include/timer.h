#ifndef  __TIMER_H__
#define __TIMER_H__

#define CPU_FREQ 11059200 // ADuC812
#define TIMER_FREQ (CPU_FREQ/12)  // Frequency of Timers 0 and 1
#define TIMER2_FREQ (CPU_FREQ/2)  // Frequency of Timer 2

/* TMOD Bits */
#define TMOD_T1_GC 0x80						// Gate Control: Timer_enabled = ~INT1 && TR1 if set to 1, = TR1 if cleared
#define TMOD_T1_TYPE_COUNTER 0x40
#define TMOD_T1_TYPE_TIMER 0x00
#define TMOD_T1_MODE_OFF 0x30
#define TMOD_T1_MODE_8BIT_AUTORELOAD 0x20 // TH is reloaded into TL each time TL overflows
#define TMOD_T1_MODE_16BIT 0x10
#define TMOD_T1_MODE_8BIT_PRESCALER 0x00 // TH - 8bit counter, 5 bits of TL - prescaler

#define TMOD_T0_GC 0x08						// Gate Control: Timer_enabled = ~INT0 && TR0 if set to 1, = TR-0 if cleared
#define TMOD_T0_TYPE_COUNTER 0x04
#define TMOD_T0_TYPE_TIMER 0x00
#define TMOD_T0_MODE_OFF 0x03
#define TMOD_T0_MODE_8BIT_AUTORELOAD 0x02 // TH is reloaded into TL each time TL overflows
#define TMOD_T0_MODE_16BIT 0x01
#define TMOD_T0_MODE_8BIT_PRESCALER 0x00 // TH - 8bit counter, 5 bits of TL - prescaler

/* TCON Bits */
#define TCON_T1_OVERFLOW 0x80
#define TCON_T1_ENABLE 0x40
#define TCON_T0_OVERFLOW 0x20
#define TCON_T0_ENABLE 0x10
#define TCON_IE1_FLAG 0x08
#define TCON_IE1_TYPE_FRONT 0x04
#define TCON_IE1_TYPE_LEVEL 0x00
#define TCON_IE0_FLAG 0x02
#define TCON_IE0_TYPE_FRONT 0x01
#define TCON_IE0_TYPE_LEVEL 0x00

#define T2CON_T2_OVERFLOW 0x80
#define T2CON_T2_EXT_FLAG 0x40
#define T2CON_T2_REC_CLK  0x20
#define T2CON_T2_TNS_CLK  0x10
#define T2CON_T2_EXT_ENA  0x08
#define T2CON_T2_ENABLE   0x04
#define T2CON_T2_COUNTER  0x02
#define T2CON_T2_TIMER    0x00
#define T2CON_T2_CAP_SEL  0x01

/*
 * Initialize system timer
 */
void init_timers(void);

/*
 * Get time
 */
unsigned long get_time(void);

/*
 * Get difference between current system time and t0
 */
unsigned long delta_time(unsigned long t0);

/*
 * Sleep for t milliseconds
 */
void sleep(unsigned long t);

/*
 * Wait till time specified
 */
void sleep_till(unsigned long target);
#endif /*  __TIMER_H__ */
