#ifndef __TIME_H__
#define __TIME_H__

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

#endif /* __TIME_H__ */
