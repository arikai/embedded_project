#ifndef __INTERRUPT__H
#define __INTERRUPT__H
#include "aduc812.h"

void set_vector(XDATA(unsigned char) *address, void* vector);

#endif /* __INTERRUPT__H */
