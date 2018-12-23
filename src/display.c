#include "display.h"
#include "system.h"
#include "timer.h"

/*
 * Registers and constants for Read/Write
 */

#define C_IND_DATA_START         0x01 // E register (strobe)
#define C_IND_DATA_END           0x00
#define C_IND_E_MASK             0x01

#define C_IND_WRITE    0x00  // RW register
#define C_IND_READ     0x02

#define C_IND_DATA     0x04  // RS register
#define C_IND_COMMAND  0x00

#define DP_COMMAND     C_IND_COMMAND  
#define DP_DATA        C_IND_DATA     


/* 
 * COMMAND CONSTANTS
 * One block describes one command and it's options
 * */

/* Clear screen and return cursor */
#define DP_CMD_CLEAR           0x01    //requires delay cylce of min 57 NOPs


/* Return cursor (w/o clearing display) */
#define DP_CMD_HOME            0x02


/* How AC changes when character code is written or red from DDRAM */
#define DP_CMD_ENTRY_MODE      0x04

#define DP_OPT_CURSOR_INC      0x02 // Increment AC
#define DP_OPT_CURSOR_DEC      0x00 // Decrement AC

#define DP_OPT_SETSH_DISPLAY   0x01 // Shift not cursor but whole display (visually)
#define DP_OPT_SETSH_CURSOR    0x00 // Shift cursor


/* Controls for Display and cursor */
#define DP_CMD_CONTROL         0x08

#define DP_OPT_DISPLAY_ON      0x04    // Enable display
#define DP_OPT_DISPLAY_OFF     0x00    // Disable display

#define DP_OPT_CURSOR_ON       0x02    // Enable cursor
#define DP_OPT_CURSOR_OFF      0x00    // Enable cursor

#define DP_OPT_BLINK           0x01    // Enable blinking cursor
#define DP_OPT_NO_BLINK        0x00

/* Cursor shift controls */
#define DP_CMD_SHIFT           0x10

#define DP_OPT_DISPLAY         0x08    // Shift not cursor but display contents 
				       // ( DP_OPT_SETSH_DISPLAY must be set )
#define DP_OPT_CURSOR          0x00    // Shift not cursor but display contents 

#define DP_OPT_RIGHT           0x04    // Shift direction


/* Set Display Functioning controls */ 
#define DP_CMD_FUNCTION_SET    0x20

#define DP_OPT_EIGHT_BITS      0x10    // Data length of 8 bits
#define DP_OPT_FOUR_BITS       0x00    // or 4 bits

#define DP_OPT_ONE_LINE        0x00    // Display lines used: one line
#define DP_OPT_TWO_LINE        0x08    // or two lines

#define DP_OPT_SIZE_5x8        0x40    // Character size: 5x8
#define DP_OPT_SIZE_5x11       0x00    // or 5x11


/* Set DD RAM address (DB5-0) */
#define DP_CMD_RAM_CG          0x40
#define DP_OPT_RAM_CG_MASK     0x3f


/* Set CG RAM address (DB6-0) */
#define DP_CMD_RAM_DD          0x80
#define DP_OPT_RAM_DD_MASK     0x7f


/* 
Other commands:
| RS | R/W | Data (DB7-DB0)     | Description                                  |
|  0 |  1  | BF(1) AC(7)        | Read Busy Flag (BF) and Address Counter (AC) |
|  1 |  0  | DD/CG RAM data (8) | Write data to DD/CG RAM                      |
|  1 |  1  | DD/CG RAM data (8) | Read data from DD/CG RAM                     |
*/


/* 
 * Module-level functions and macros 
 */

/* Wait for Busy Flag (BF) to be cleared by hardware, so it would accept next 
 * command
 * */
// Doesn't work for some reason
// Only CLEAR/HOME commands have significant execution times (~2ms)
// While other commands have exec times < 1ms (37us)
// Just make those commands sleep for 2 ms instead
// #define dp_wait() while( 0x80 & dp_read(DP_DATA) ) 

// Macros for commands
#define dp_command(data) dp_write(data, DP_COMMAND)
#define dp_write_data(data) dp_write(data, DP_DATA)


void dp_write(uint8_t data, uint8_t mode)
{
    mode |= C_IND_WRITE;

    write_max(DATA_IND, data);
    write_max(C_IND, C_IND_DATA_START | mode);
    write_max(C_IND, (~C_IND_E_MASK) & mode);
}

uint8_t dp_read(uint8_t mode)
{
    uint8_t data;
    mode |= C_IND_READ;

    write_max(C_IND, C_IND_DATA_START | mode);
    data = read_max(DATA_IND);
    write_max(C_IND, ~C_IND_E_MASK & mode);
    return data;
}


/* 
 * API 
 */

static uint8_t x;
static bit y;

uint8_t dp_getx()
{
    return x;
}

bit dp_gety()
{
    return y;
}

void dp_clear(void)
{
    dp_command( DP_CMD_CLEAR );
    x = y = 0;
    sleep(2);
}

void dp_home(void)
{
    dp_command( DP_CMD_HOME );
    x = y = 0;
    sleep(2);
}

void dp_print_char(char c)
{
    dp_write_data(c);

    if( x >= DP_MAX_X )
    {
	x = 0;
	y = !y; // Only lines 0 and 1
	dp_move_cursor(x, y);
    }
    else
    {
	++x;
    }
}

void dp_print_string(char str[])
{
    while(*str)
	dp_print_char(*str++);
}

static bit display_on = 1;
void dp_power(bit on)
{
    dp_command(DP_CMD_CONTROL
	       | ( on ? DP_OPT_DISPLAY_ON : DP_OPT_DISPLAY_OFF )
	    );
    display_on = on;
}

void dp_cursor_style(enum cursor_style style)
{
    dp_command(DP_CMD_CONTROL
	       | ( display_on ? DP_OPT_DISPLAY_ON : DP_OPT_DISPLAY_OFF )
	       | (( style & CURSOR_UNDERSCORE ) ? DP_OPT_CURSOR_ON : DP_OPT_CURSOR_OFF)
	       | (( style & CURSOR_BLINK )      ? DP_OPT_BLINK     : DP_OPT_NO_BLINK  )
	    );
}

void dp_move_cursor(uint8_t nx, bit ny)
{
    dp_command(DP_CMD_RAM_DD | nx+(ny ? 40 : 0));
    x = nx;
    y = ny;
}

void dp_init(void)
{
     dp_command( DP_CMD_FUNCTION_SET 
 	        | DP_OPT_EIGHT_BITS
 		| DP_OPT_TWO_LINE
 		| DP_OPT_SIZE_5x11
 		);

    dp_clear();

    return;
}

// void dp_init_by_instruction()
// {
//     /*
//      * Must not be used in normal circumstances
//      *
//      * From HD44780U Specification:
//      * If the power supply conditions for correctly operating the internal reset 
//      * circuit are not met, initialization by instructions becomes necessary.
//      */
// 
//     /*
//      * HD44780U (LCD-II) Initializing by Instruction (see documentation)
//      * Using 8-bit interface
//      */
// 
//     sleep(41); // Wait for LCD to power on 
// 	       // (>15 ms after Vcc rises to 4.5 V)
// 	       // (>40 ms after Vcc rises to 2.7 V)
// 
//     /* cmd = 0x30 */
//     dp_command( DP_CMD_FUNCTION_SET
// 		| DP_OPT_EIGHT_BITS );
//     sleep(5);
// 
//     /* cmd = 0x30 */
//     dp_command( DP_CMD_FUNCTION_SET
// 	        | DP_OPT_EIGHT_BITS );
//     sleep(1);
// 
//     /* cmd = 0x30 */
//     dp_command(DP_CMD_FUNCTION_SET
// 	        | DP_OPT_EIGHT_BITS );
// 
// 
//     /* Now BF can and must be checked */
// 
//     // Set Display parameters
//     // Cannot be changed afterwards
//     dp_command( DP_CMD_FUNCTION_SET 
// 	        | DP_OPT_EIGHT_BITS
// 		| DP_OPT_TWO_LINE
// 		| DP_OPT_SIZE_5x8
// 		);
// 
//     /* cmd = 0x08 */
//     dp_command( DP_CMD_CONTROL
// 	        | DP_OPT_DISPLAY_OFF
// 		// | DP_OPT_BLINK
// 		);
// 
//     dp_command( DP_CMD_CLEAR );
// 
//     // Cursor parameters
//     dp_command( DP_CMD_ENTRY_MODE
// 	        | DP_OPT_CURSOR_INC );
// 
//     dp_command( DP_CMD_CONTROL
// 	        | DP_OPT_DISPLAY_ON );
// }
