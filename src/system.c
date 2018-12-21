#include "aduc812.h"
#include "system.h"
#include "timer.h"
#include "interrupt.h"

#define MAXBASE 0x8 // Ќомер страницы внешней пам€ти (xdata), куда отображаютс€
		    // регистры расширител€.

void write_max( XDATA(uint8_t *) regnum, uint8_t val )
{
    uint8_t oldDPP = DPP;

    DPP     = MAXBASE;
    *regnum = val;
    DPP     = oldDPP;
}

uint8_t read_max( XDATA(uint8_t *) regnum )
{
    uint8_t oldDPP = DPP;
    uint8_t val;

    DPP = MAXBASE;
    val = *regnum;
    DPP = oldDPP;

    return val;
}

static uint8_t current_ena = 0;

void clear_registers() {
    write_max(ENA, 0);
}

uint8_t get_ena()
{
    return current_ena;
}

void set_ena(uint8_t new_ena)
{
    current_ena = new_ena;
    write_max(ENA, current_ena);
}

void xor_ena(uint8_t xor_value)
{
    current_ena ^= xor_value;
    write_max(ENA, current_ena);
}

void or_ena(uint8_t or_value)
{
    current_ena |= or_value;
    write_max(ENA, current_ena);
}

// DO NOT CHANGE THE ORDER OF CALLS
void init_system(){
    
    // 0. Clear some registers (ENA,...)
    clear_registers();

    // 1. Interrupts must be configured properly
    init_interrupts();
    
    // 2. Init timers.
    init_timers();
}
