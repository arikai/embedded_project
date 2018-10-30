#include "interrupt.h"

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

