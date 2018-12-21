#include "system.h"
#include "interrupt.h"

void int0_handler() interrupt(IE0_VECTOR);

// Enable INT0 at ENA register to enable Ext. Interrupt 0
void ena_int0(){
    #define ENA_INT0_MASK 0x20
    write_max(ENA, ENA_INT0_MASK | read_max(ENA));
}

void init_interrupts(){
    
    IE = 0;       // Clear Interrupt Registers
    IE2 = 0;      // 
    IP = 0;       // Interrupt Priority

    /*
    ena_int0();
    set_vector(IE0_USER_VECTOR, (void*) int0_handler);   
    EX0 = 1; // Enable External Interrupt 0
    */

    EA = 1; // Enable interrupts
}

// void int0_handler() interrupt(IE0_VECTOR)
// {
//     led_mode ^= 1;
// }

void set_vector(XDATA(uint8_t) *address, void* vector)
{
    XDATA(uint8_t) *tmp_vector;

    *address = 0x02; //ljump

    tmp_vector = (XDATA(uint8_t) *) (address + 1);
    *tmp_vector = (uint8_t) ((uint16_t)vector >> 8); // put MS byte

    ++tmp_vector;
    *tmp_vector = (uint8_t) vector; // put LS byte
    // now there's instruction "ljump {vector}" at {address}
}

