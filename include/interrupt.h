#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include "aduc812.h"
#include "common.h"

/* Convert interrupt number to User Vector Address */
#define INTNUM_TO_UVEC(d) (0x2000 | ((d) * 8) + 3)

/* Interrupt vector addresses */
#define IE0_USER_VECTOR         ((XDATA(unsigned char) *) INTNUM_TO_UVEC(IE0_VECTOR))
#define TF0_USER_VECTOR         ((XDATA(unsigned char) *) INTNUM_TO_UVEC(TF0_VECTOR))
#define IE1_USER_VECTOR         ((XDATA(unsigned char) *) INTNUM_TO_UVEC(IE1_VECTOR))
#define TF1_USER_VECTOR         ((XDATA(unsigned char) *) INTNUM_TO_UVEC(TF1_VECTOR))
#define RI_TI_USER_VECTOR       ((XDATA(unsigned char) *) INTNUM_TO_UVEC(SIO_VECTOR))
#define TF2_EXF2_USER_VECTOR    ((XDATA(unsigned char) *) INTNUM_TO_UVEC(TF2_VECTOR))
#define ADCI_USER_VECTOR        ((XDATA(unsigned char) *) INTNUM_TO_UVEC(ADC_VECTOR))
#define I2CI_ISPI_USER_VECTOR   ((XDATA(unsigned char) *) INTNUM_TO_UVEC(I2C_SPI_VECTOR))
#define PSMI_USER_VECTOR        ((XDATA(unsigned char) *) INTNUM_TO_UVEC(PSM_VECTOR))

void init_interrupts();
void set_vector(XDATA(unsigned char) *address, void* vector);

#endif /* __INTERRUPT_H__ */ 
