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
 * SOME special characters for HD44780
 */

#define CHAR_ARROW_UP    0x18
#define CHAR_ARROW_DOWN  0x19
#define CHAR_TRIANG_UP   0x1e
#define CHAR_TRIANG_DOWN 0x1f
/*
 * Initialize LCD
 */
void dp_init(void);

/*
 * Clear LCD
 */
void dp_clear(void);

/*
 * Clear current line
 */
void dp_clear_line(void);

/*
 * Return cursor to global home position
 */
void dp_home(void);

/*
 * Return cursor to line's home position
 */
void dp_line_home(void);

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

/*
 * Shift cursor for 1 character right (1) or left (0)
 */
void dp_shift_cursor(bit right);

/*
 * Shift display up
 */
void dp_newline(void);

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
