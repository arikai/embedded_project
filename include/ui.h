#ifndef __UI_H__
#define __UI_H__

#include "keyboard.h"

/*
 * Print ASCIIZ string to LCD with ~interval~ ms between characters
 */
void print(char* str, unsigned long interval);

/*
 * Read input from user
 * Prompt must be ASCIIZ string with length <= 16 characters (excluding \0)
 * ~interval~ is passed to print()
 * ~buf~ must be min(DP_MAX_X, maxlen)+1 bytes
 * ~echo controls if characters must be echoed to LCD
 */
uint8_t read_input(
	char* prompt, unsigned long interval,
	char* buf, uint8_t maxlen, bit echo
	);

#endif /* __UI_H__ */
