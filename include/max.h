#ifndef __MAX__H
#define __MAX__H

#include "aduc812.h"

//Названия регистров
#define KB          ((XDATA(unsigned char) *) 0x0)
#define DATA_IND    ((XDATA(unsigned char) *) 0x1)
#define EXT_LO      ((XDATA(unsigned char) *) 0x2)
#define EXT_HI      ((XDATA(unsigned char) *) 0x3)
#define ENA         ((XDATA(unsigned char) *) 0x4)
#define C_IND       ((XDATA(unsigned char) *) 0x6)
#define SV          ((XDATA(unsigned char) *) 0x7)

void write_max( XDATA(unsigned char) *regnum, unsigned char val );
unsigned char read_max( XDATA(unsigned char) *regnum );

#endif //__MAX__H
