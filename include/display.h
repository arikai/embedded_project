#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "common.h"

enum cursor_style {
    CURSOR_OFF        = 0,
    CURSOR_UNDERSCORE = 1,
    CURSOR_BLINK      = 2,
    CURSOR_BOTH       = 3
};

/*
 * Initialize LCD
 */
void dp_init(void);

/*
 * Clear LCD
 */
void dp_clear(void);

/*
 * Return cursor to home position
 */
void dp_home(void);

/*
 * Enable/disable LCD display
 */
void dp_power(bit on);

/*
 * Change cursor style. 
 * cursor = 0/1 - disable/enable cursor
 *  blink = 0/1 - disable/enable blinking cursor;
*/
void dp_cursor_style(enum cursor_style);

/*
 * Move cursor to (x;y)
 */
void dp_move_cursor(uint8_t x, bit y);

#define DP_MAX_X 15
#define DP_MAX_Y 1

/*
 * Get X of current position of cursor
 */
uint8_t dp_getx();

/*
 * Get Y of current position of cursor
 */
bit dp_gety();

/*
 * Print character on current cursor position
 */
void dp_print_char(char);

/*
 * Print ASCIIZ string on LCD
 */
void dp_print_string(char str[]);

#endif /* __DISPLAY_H__ */
