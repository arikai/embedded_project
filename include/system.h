#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "aduc812.h"
#include "common.h"

// Register names
#define KB          ((XDATA(unsigned char)*) 0x0)
#define DATA_IND    ((XDATA(unsigned char)*) 0x1)
#define EXT_LO      ((XDATA(unsigned char)*) 0x2)
#define EXT_HI      ((XDATA(unsigned char)*) 0x3)
#define ENA         ((XDATA(unsigned char)*) 0x4)
#define C_IND       ((XDATA(unsigned char)*) 0x6)
#define SV          ((XDATA(unsigned char)*) 0x7)

/* Write byte ~val~ to register with address/number ~regnum~ */
void write_max( XDATA(uint8_t *) regnum, uint8_t val );

/* Read byte from register with address/number ~regnum~ */
uint8_t read_max( XDATA(uint8_t *) regnum );

#define DIP ~read_max(EXT_LO)

/* Init System: Timers, Keyboard (?), etc. */
void init_system();

#define ENA_EN_LO 0x01
#define ENA_EN_HI 0x02
#define ENA_SND0  0x04
#define ENA_SND1  0x08
#define ENA_SND2  0x10
#define ENA_INT0  0x20
#define ENA_KB    0x40
uint8_t get_ena( void );
void set_ena( uint8_t );
void xor_ena( uint8_t );
void or_ena( uint8_t );

#endif /* __SYSTEM_H__ */
