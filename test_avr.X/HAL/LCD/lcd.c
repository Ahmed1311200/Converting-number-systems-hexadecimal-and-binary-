#include "lcd_priv.h"
#include "lcd_public.h"

#include <stddef.h>
#include <stdio.h>
#include <util/delay.h>


static void lcd_output_nibble(const lcd_t* p_lcd, uint8_t nibble);
static void lcd_write_cmd(const lcd_t* p_lcd, uint8_t cmd);
static void lcd_write_data(const lcd_t* p_lcd, uint8_t data);

int8_t lcd_init(const lcd_t* p_lcd)
{
    uint8_t pin_indx;
    int8_t retval = 0;
    uint8_t cmd;
    
    if(p_lcd == NULL)
    {
        retval = -1;
    }
    else
    {
        for(pin_indx = 0; pin_indx < LCD_DATA_PINS_NUM; pin_indx++)
        {
            retval = dio_pin_init(p_lcd->data_pins.port, p_lcd->data_pins.pins_array[pin_indx], 
                                                                            DIO_PIN_MODE_OUTPUT);
            if(retval != 0)
            {
                return retval;
            }
        }
        
        retval = dio_pin_init(p_lcd->rs_pin.port, p_lcd->rs_pin.pin, DIO_PIN_MODE_OUTPUT);
        if(retval != 0)
        {
            return retval;
        }
        
        retval = dio_pin_init(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_PIN_MODE_OUTPUT);
        if(retval != 0)
        {
            return retval;
        }
        
    }
        
    CMD_RETURN_HOME(cmd);
    lcd_write_cmd(p_lcd, cmd);
    
    CMD_CLEAR_DISP(cmd);
    lcd_write_cmd(p_lcd, cmd);
    
    CMD_FUNC_SET(cmd, 0, 1, 0);
    lcd_write_cmd(p_lcd, cmd);
    
    CMD_DISP_CTRL(cmd, 1, 1, 1);
    lcd_write_cmd(p_lcd, cmd);
    
    CMD_ENTRY_MODE(cmd, 1, 0);
    lcd_write_cmd(p_lcd, cmd);
    
    
    return retval;
}

int8_t lcd_write_text(const lcd_t* p_lcd, const char* text)
{
    int8_t retval = 0;
    
    if(p_lcd == NULL)
    {
        retval = -1;
    }
    else if(text == NULL)
    {
        retval = -1;
    }
    else
    {
        while(*text != '\0')
        {
            lcd_write_data(p_lcd, *text);
            text++;
        }
    }
    
    return retval;
}



int8_t lcd_write_num(const lcd_t* p_lcd, int num)
{
    int8_t retval = 0;
    char buffer[12];
    
    if(p_lcd == NULL)
    {
        retval = -1;
    }
    else
    {
        sprintf(buffer, "%d", num);
        retval = lcd_write_text(p_lcd, buffer);
    }
    
    return retval;
}

int8_t lcd_shift(const lcd_t *p_lcd, lcd_shift_dir_t dir, uint8_t count)
{
    uint8_t cmd;
    
    for(uint8_t i = 0; i < count; i++)
    {
        if(dir == LCD_SHIFT_LEFT)
        {
            CMD_DISP_SHIFT(cmd, 1, 0);
        }
        else
        {
            CMD_DISP_SHIFT(cmd, 1, 1);
        }
        
        lcd_write_cmd(p_lcd, cmd);
        
        _delay_ms(LCD_SHIFT_DELAY_MS);
    }
    
    return 0;
}

int8_t lcd_set_pos(const lcd_t* p_lcd, uint8_t line, uint8_t pos)
{
    uint8_t cmd;
    int8_t retval = 0;
    
    if(p_lcd == NULL)
    {
        retval = -1;
    }
    else if(line > 1)
    {
        retval = -1;
    }
    else if(pos > 39)
    {
        retval = -1;
    }
    else
    {
        if(line == 0)
        {
            CMD_DDRAM_ADDR(cmd, pos);
        }
        else
        {
            CMD_DDRAM_ADDR(cmd, pos | 0x40);
        }
        
        lcd_write_cmd(p_lcd, cmd);
    }
    
    return retval;
}

static void lcd_write_cmd(const lcd_t* p_lcd, uint8_t cmd)
{ 
    // RS = 0
    dio_pin_write(p_lcd->rs_pin.port, p_lcd->rs_pin.pin, DIO_LEVEL_LOW);
    
    // Put command most significant nibble on data pin
    lcd_output_nibble(p_lcd, cmd >> 4);
    
    // EN = 1
    dio_pin_write(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_LEVEL_HIGH);
    
    // EN = 0
    dio_pin_write(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_LEVEL_LOW);
        
    // Put command most significant nibble on data pin
    lcd_output_nibble(p_lcd, cmd);
    
    // EN = 1
    dio_pin_write(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_LEVEL_HIGH);
    
    // EN = 0
    dio_pin_write(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_LEVEL_LOW);
    
    // Wait for command to complete
    _delay_ms(LCD_CMD_DELAY_MS);
}

static void lcd_write_data(const lcd_t* p_lcd, uint8_t data)
{ 
    // RS = 1
    dio_pin_write(p_lcd->rs_pin.port, p_lcd->rs_pin.pin, DIO_LEVEL_HIGH);
    
    // Put command most significant nibble on data pin
    lcd_output_nibble(p_lcd, data >> 4);
    
    // EN = 1
    dio_pin_write(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_LEVEL_HIGH);
    
    // EN = 0
    dio_pin_write(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_LEVEL_LOW);
        
    // Put command most significant nibble on data pin
    lcd_output_nibble(p_lcd, data);
    
    // EN = 1
    dio_pin_write(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_LEVEL_HIGH);
    
    // EN = 0
    dio_pin_write(p_lcd->en_pin.port, p_lcd->en_pin.pin, DIO_LEVEL_LOW);
    
    // Wait for command to complete
    _delay_ms(LCD_CMD_DELAY_MS);
}

static void lcd_output_nibble(const lcd_t* p_lcd, uint8_t nibble)
{
    uint8_t indx;
    
    for(indx = 0; indx < LCD_DATA_PINS_NUM; indx++)
    {
        if(nibble & 0x01)
        {
            dio_pin_write(p_lcd->data_pins.port, p_lcd->data_pins.pins_array[indx], DIO_LEVEL_HIGH);
        }
        else
        {
            dio_pin_write(p_lcd->data_pins.port, p_lcd->data_pins.pins_array[indx], DIO_LEVEL_LOW);
        }
        
        nibble = nibble >> 1;
    }
}
