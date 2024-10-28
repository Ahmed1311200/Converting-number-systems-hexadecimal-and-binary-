#ifndef __LCD_H__

#define __LCD_H__

#include "../../MCAL/DIO/dio_public.h"

#include <stdint.h>

#define LCD_DATA_PINS_NUM   4


typedef enum
{
    LCD_SHIFT_LEFT,
    LCD_SHIFT_RIGHT,
} lcd_shift_dir_t;

typedef struct
{
    struct
    {
        dio_port_t port;
        dio_pin_t pins_array[LCD_DATA_PINS_NUM];
    } data_pins;
    
    struct {
        dio_port_t port;
        dio_pin_t pin;
    } rs_pin;
    
    struct {
        dio_port_t port;
        dio_pin_t pin;
    } en_pin;
    
} lcd_t;


/**
 * 
 * @param p_lcd
 * @return 
 */
int8_t lcd_init(const lcd_t* p_lcd);

/**
 * 
 * @param p_lcd
 * @param text
 * @return 
 */
int8_t lcd_write_text(const lcd_t* p_lcd, const char* text);




int8_t lcd_write_num(const lcd_t* p_lcd, int num);

/**
 * 
 * @param p_lcd
 * @param dir
 * @param count
 * @return 
 */
int8_t lcd_shift(const lcd_t *p_lcd, lcd_shift_dir_t dir, uint8_t count);

/**
 * 
 * @param p_lcd
 * @param line
 * @param pos
 * @return 
 */
int8_t lcd_set_pos(const lcd_t* p_lcd, uint8_t line, uint8_t pos);

#endif /* __LCD_H__ */

