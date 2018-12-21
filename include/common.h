#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

/* 
 * SDCC Trickery
 */

// Starting from version 2.9 or so, xdata definition changed
#if SDCC_REVISION > 8008 || __SDCC_REVISION > 8008
    #define XDATA(type) __xdata type
    #define interrupt __interrupt
    #define bit __bit
#else 
    #define XDATA(type) type xdata // 
#endif

/*
 * Useful macros
 */

/* Static array length */
#define ALEN(n) (sizeof(n)/(sizeof(*n)))

#endif /* __COMMON_H__ */
