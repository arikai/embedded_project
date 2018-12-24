#ifndef __TIME_H__
#define __TIME_H__

typedef unsigned long time_t;

/*
 * Get time
 */
time_t get_time(void);

/*
 * Get difference between current system time and t0
 */
time_t delta_time(time_t t0);

/*
 * Sleep for t milliseconds
 */
void sleep(time_t t);

/*
 * Wait till time specified
 */
void sleep_till(time_t target);

#endif /* __TIME_H__ */
