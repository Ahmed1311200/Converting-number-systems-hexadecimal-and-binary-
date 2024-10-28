/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __DIO_DRIVER_H__
#define	__DIO_DRIVER_H__

#include <stdint.h>

#define DIO_PINS_NUM    8

typedef int8_t dio_pin_t;
typedef uint8_t dio_port_val_t;


typedef enum
{
    DIO_PORTA = 0,
    DIO_PORTB,
    DIO_PORTC,
    DIO_PORTD,
    DIO_PORTS_NUM,
} dio_port_t;

typedef enum {
    DIO_PIN_MODE_INPUT,
    DIO_PIN_MODE_INPUT_PU,
    DIO_PIN_MODE_OUTPUT,
} dio_pin_mode_t;

typedef enum
{
    DIO_LEVEL_LOW = 0,
    DIO_LEVEL_HIGH,
} dio_level_t;
/**
 * Initialize DIO pin
 * 
 * @param port Port number (0,1,....)
 * @param pin Pin number (0,1,....)
 * @param mode Pin initial mode
 * @return 0 on success, -1 otherwise
 */
int8_t dio_pin_init(dio_port_t port, dio_pin_t pin, dio_pin_mode_t mode);

/**
 * Output digital level to DIO pin
 * 
 * @param port Port number
 * @param pin Pin number
 * @param level Digital level to output
 * 
 * @return 0 if pin is output pin, -1 otherwise
 */
int8_t dio_pin_write(dio_port_t port, dio_pin_t pin, dio_level_t level);

/**
 * Toggle DIO pin state
 * 
 * @param port Port number
 * @param pin Pin number
 * 
 * @return 0 if pin is output pin, -1 otherwise
 */
int8_t dio_pin_toggle(dio_port_t port, dio_pin_t pin);

/**
 * Write port at once
 * 
 * @param port Port number
 * @param val Port value
 * 
 * @return 0 if all pins on the port are output, -1 otherwise
 */
int8_t dio_port_write(dio_port_t port, dio_port_val_t val);

/**
 * Get DIO pin physical level
 * 
 * @param port Port number
 * @param pin Pin number
 * @param pin_level Pointer to store pin level
 * 
 * @return 0 for logical low, 1 for logical high
 */
int8_t dio_pin_read(dio_port_t port, dio_pin_t pin, dio_level_t *pin_level);

/**
 * Read DIO port at once
 * 
 * @param port Port number
 * 
 * @return Port read value
 */
int8_t dio_port_read(dio_port_t port, dio_port_val_t* pval);

#endif /* __DIO_DRIVER_H__ */

