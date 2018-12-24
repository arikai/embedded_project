#ifndef __UI_H__
#define __UI_H__

#include "keyboard.h"
#include "display.h"
#include "time.h"

#define MESSAGE_DELAY 50
#define MESSAGE_NDELAY 200

// Flavors of text
#define message(str)     print(str, MESSAGE_DELAY, MESSAGE_NDELAY)
#define info(str, i)     print(str, i, MESSAGE_NDELAY)
#define story(str, i, n) print(str, i, n)
#define story_number(num, i, n) print_number(num, i, n)
#define print_char(c)    dp_print_char(c)

/*
 * Print ASCIIZ string to LCD with ~delay~ ms between characters
 */
void print(char* str, 
	time_t delay, 
	time_t newline_delay
     );

/*
 * Convert number to string and ~print~
 */
void print_number(uint16_t number, 
	time_t delay, 
	time_t newline_delay);

/* Convenient macros for game.c */
#define newline()    dp_newline()
#define clear()      dp_clear()
#define clear_line() dp_clear_line()
#define home()       dp_home()
#define line_home()  dp_line_home();

/* Text printing between-characters delays */

#define DELAY_NONE 0
#define DELAY_NORMAL 50
#define DELAY_SLOW   75
#define DELAY_UNEASY 100
#define DELAY_SCARY  150
#define DELAY_IMPORTANT 300
#define DELAY_SNAIL 1000

/*
 * Read input from user
 * Prompt must be ASCIIZ string with length <= 16 characters (excluding \0)
 * ~delay~ is passed to print()
 * ~buf~ must be min(DP_MAX_X, maxlen)+1 bytes
 * ~echo controls if characters must be echoed to LCD
 */
uint8_t read_input(
	char* prompt, time_t delay,
	char* buf, uint8_t maxlen, bit echo
	);


/*
 * Read Ok/Cancel
 */
uint8_t read_ok(char* prompt);

/*
 * Read single key from keyboard
 */
uint8_t read_key(time_t timeout);

#endif /* __UI_H__ */
