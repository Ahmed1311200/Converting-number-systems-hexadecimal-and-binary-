#include "dio_priv.h"
#include "dio_public.h"

#include <stddef.h>

static volatile dio_t * const dio_ports[DIO_PORT_MAX] = {
    PORTA_BASE_ADDR, PORTB_BASE_ADDR, PORTC_BASE_ADDR, PORTD_BASE_ADDR };

int8_t dio_pin_init(dio_port_t port, dio_pin_t pin, dio_pin_mode_t mode)
{
    int8_t retval = 0;
    
    if(port >= DIO_PORT_MAX)
    {
        retval = -1;
    }
    else if(pin >= DIO_PIN_MAX)
    {
        retval = -1;
    }
    else if(mode > DIO_PIN_MODE_OUTPUT)
    {
        retval = -1;
    }
    else
    {
        if(mode == DIO_PIN_MODE_OUTPUT)
        {
            /* Set pin direction to output */
            dio_ports[port]->ddr |= (1 << pin);
            /* Set pin initial level to low */
            dio_ports[port]->port &= ~(1 << pin);
        }
        else
        {
            dio_ports[port]->ddr &= ~(1 << pin);
            
            if(mode == DIO_PIN_MODE_INPUT_PU)
            {
                /* Enable pull-up resistor */
                dio_ports[port]->port |= (1 << pin);
            }
            else
            {
                /* Disable pull-up resistor */
                dio_ports[port]->port &= ~(1 << pin);
            }
        }
    }
    
    return retval;
}

int8_t dio_pin_write(dio_port_t port, dio_pin_t pin, dio_level_t level)
{
    int8_t retval = 0;
    
    if(port >= DIO_PORT_MAX)
    {
        retval = -1;
    }
    else if(pin >= DIO_PIN_MAX)
    {
        retval = -1;
    }
    else if((dio_ports[port]->ddr & (1 << pin)) == 0x00)
    {
        retval = -1;
    }
    else
    {
        if(level != 0)
        {
            dio_ports[port]->port |= (1 << pin);
        }
        else
        {
            dio_ports[port]->port &= ~(1 << pin);
        }
    }
    
    return retval;
}

int8_t dio_pin_toggle(dio_port_t port, dio_pin_t pin)
{
    int8_t retval = 0;
    
    if(port >= DIO_PORT_MAX)
    {
        retval = -1;
    }
    else if(pin >= DIO_PIN_MAX)
    {
        retval = -1;
    }
    else if((dio_ports[port]->ddr & (1 << pin)) == 0x00)
    {
        retval = -1;
    }
    else
    {
        dio_ports[port]->port ^= (1 << pin);
    }
    
    return retval;
}

int8_t dio_pin_read(dio_port_t port, dio_pin_t pin, dio_level_t *pin_level)
{
    int8_t retval = 0;
    
    if(pin_level == NULL)
    {
        retval = -1;
    }
    else if(port >= DIO_PORT_MAX)
    {
        retval = -1;
    }
    else if(pin >= DIO_PIN_MAX)
    {
        retval = -1;
    }
    else
    {
        *pin_level = (dio_ports[port]->pin >> pin) & 0x01;
    }
    
    return retval;
}

int8_t dio_port_write(dio_port_t port, dio_port_val_t val)
{
    int8_t retval = 0;
    
    if(port >= DIO_PORTS_NUM)
    {
        retval = -1;
    }
    else
    {
        dio_ports[port]->port = val;
    }
    
    return retval;
}

int8_t dio_port_read(dio_port_t port, dio_port_val_t* pval)
{
    int8_t retval = 0;
    
    if(port >= DIO_PORTS_NUM)
    {
        retval = -1;
    }
    else if(pval == NULL)
    {
        retval = -1;
    }
    else
    {
        *pval = dio_ports[port]->pin;
    }
    
    return retval;
}