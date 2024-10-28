/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __DIO_PRIV_H__
#define	__DIO_PRIV_H__

#include <stdint.h>

#define DIO_PORT_MAX    4
#define DIO_PIN_MAX     8

#define PORTA_BASE_ADDR ((volatile dio_t*) 0x39)
#define PORTB_BASE_ADDR ((volatile dio_t*) 0x36)
#define PORTC_BASE_ADDR ((volatile dio_t*) 0x33)
#define PORTD_BASE_ADDR ((volatile dio_t*) 0x30)

typedef struct {
    uint8_t pin;
    uint8_t ddr;
    uint8_t port;
} dio_t;

#endif	/* __DIO_PRIV_H__ */

