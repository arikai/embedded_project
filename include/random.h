#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "common.h"

/*
 * Get pseudo-random byte
 */
uint8_t random_byte();

/*
 * Get pseudo-random number in range [a;b)
 * Note: may block for ranges with length not equals to 2**k
 */
uint8_t random(uint8_t a, uint8_t b);


/*
 * Return 1 with ~probability~
 */
bit random_bool(uint8_t probability);

#endif /* __RANDOM_H__ */
