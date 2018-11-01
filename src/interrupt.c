#include "max.h"
#include "common.h"
#include "interrupt.h"

void int0_handler() interrupt(IE0_VECTOR);

// Enable INT0 at ENA register to enable Ext. Interrupt 0
void ena_int0(){
#define ENA_ADDRESS ((XDATA(unsigned char)*) 4)
#define ENA_INT0_MASK 0x20
    write_max(ENA_ADDRESS, ENA_INT0_MASK | read_max(ENA_ADDRESS));
}

void init_interrupts(){
    
    IE = 0; // Clear Interrupt Registers
    IE2 = 0;
    IP = 0; // Interrupt Priority

    // Mode switcher
    ena_int0();
    set_vector(IE0_USER_VECTOR, (void*) int0_handler);   
    EX0 = 1; // Enable External Interrupt 0
    EA = 1; // Enable interrupts
}

void int0_handler() interrupt(IE0_VECTOR)
{
    led_mode ^= 1;
}

void set_vector(XDATA(unsigned char) *address, void* vector)
{
    XDATA(unsigned char) *tmp_vector;

    *address = 0x02; //ljump

    tmp_vector = (XDATA(unsigned char) *) (address + 1);
    *tmp_vector = (unsigned char) ((unsigned short)vector >> 8); // put MS byte

    ++tmp_vector;
    *tmp_vector = (unsigned char) vector; // put LS byte
    // now there's instruction "ljump {vector}" at {address}
}

