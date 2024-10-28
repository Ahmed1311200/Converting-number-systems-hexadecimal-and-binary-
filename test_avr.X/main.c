#define F_CPU 8000000

#include "HAL/LCD/lcd_public.h"
#include <util/delay.h>
#include <stdint.h>




// Define pointers for hardware registers
volatile uint8_t * const DDRB = (uint8_t*) 0x37;   // Data Direction Register B
volatile uint8_t * const PORTB = (uint8_t*) 0x38;   // Port B Data Register
volatile uint8_t * const PIND = (uint8_t*) 0x30;    // Port D Input Register

// Function prototypes
void lcd_write_hex(lcd_t* lcd, uint8_t num); // Write a hexadecimal number to the LCD
void lcd_write_bin(lcd_t* lcd, uint8_t num); // Write a binary number to the LCD

int main(void) {
    uint8_t button1_pressed = 0;  // Flag for button 1 press
    uint8_t button2_pressed = 0;  // Flag for button 2 press
    uint8_t hex_number1 = 0;      // Variable to store hex number 1
    uint8_t hex_number2 = 0;      // Variable to store hex number 2
    uint8_t last_hex_number1 = 0; // Last displayed value of hex number 1
    uint8_t last_hex_number2 = 0; // Last displayed value of hex number 2
    uint8_t binary_number = 0;     // Variable to store the binary number

    *DDRB = 0x80;  // Set pin direction for DDRB
    *PORTB = 0x00; // Initialize PORTB

    // Initialize the LCD
    lcd_t lcd = {
        .data_pins = {
            .port = DIO_PORTA,
            .pins_array = {3, 4, 5, 6}, // Data pins
        },
        .rs_pin = {
            .port = DIO_PORTA,
            .pin = 1, // RS pin
        },
        .en_pin = {
            .port = DIO_PORTA,
            .pin = 2, // EN pin
        },
    };

    lcd_init(&lcd); // Initialize the LCD
    lcd_write_text(&lcd, "BIN >> HEX(1): "); // Display "BIN >> HEX(1):" on the LCD
    lcd_set_pos(&lcd, 0, 15); // Set position for hex number
    lcd_write_hex(&lcd, 0);   // Initialize hex display to 0
    
    lcd_set_pos(&lcd, 1, 0); // Set cursor position for hex display
    lcd_write_text(&lcd, "HEX >> BIN(2): "); // Display "HEX >> BIN(2):" on the LCD
    lcd_set_pos(&lcd, 1, 15); // Set position for hex number
    lcd_write_hex(&lcd, 0);   // Initialize hex display to 0

    while(1) {
        // Button 1 pressed handling
        if(((*PIND & 0x04) == 0x00) && (button1_pressed == 0)) {
            _delay_ms(50); // Debounce delay
            if(((*PIND & 0x04) == 0x00)) {
                button1_pressed = 1; // Mark button 1 as pressed
                lcd_set_pos(&lcd, 0, 15); // Set position for hex number
                lcd_write_num(&lcd, button1_pressed); // Display button 1 press state
            }
        } else if(((*PIND & 0x04) != 0) && (button1_pressed == 1)) {
            _delay_ms(50); // Debounce delay
            if(((*PIND & 0x04) != 0)) {
                button1_pressed = 0; // Mark button 1 as not pressed
            }
        }
        
        // Button 2 pressed handling
        if(((*PIND & 0x08) == 0x00) && (button2_pressed == 0)) {
            _delay_ms(50); // Debounce delay
            if(((*PIND & 0x08) == 0x00)) {
                button2_pressed = 1; // Mark button 2 as pressed
                lcd_set_pos(&lcd, 1, 15); // Set position for hex number
                lcd_write_num(&lcd, button2_pressed); // Display button 2 press state
            }
        } else if(((*PIND & 0x08) != 0) && (button2_pressed == 1)) {
            _delay_ms(50); // Debounce delay
            if(((*PIND & 0x08) != 0)) {
                button2_pressed = 0; // Mark button 2 as not pressed
            }
        }

        // State when button 1 is pressed
        if(button1_pressed == 1) {
            lcd_init(&lcd); // Initialize the LCD
            lcd_write_text(&lcd, "BIN: "); // Display "BIN: " on the LCD
            lcd_set_pos(&lcd, 1, 0); // Set cursor position for hex display
            lcd_write_text(&lcd, "HEX: "); // Display "HEX: " on the LCD
            lcd_set_pos(&lcd, 1, 5); // Set position for hex number
            lcd_write_hex(&lcd, 0);  // Initialize hex display to 0

            while(1) {
                // Button 1 pressed handling (represents 0)
                if(((*PIND & 0x04) == 0x00) && (button1_pressed == 0)) {
                    _delay_ms(50); // Debounce delay
                    if(((*PIND & 0x04) == 0x00)) {
                        button1_pressed = 1; // Mark button 1 as pressed
                        // Shift left to make space for new LSB, set LSB to 0
                        binary_number = (binary_number << 1) & 0xFF; // Shift left and clear LSB
                    }
                } else if(((*PIND & 0x04) != 0) && (button1_pressed == 1)) {
                    _delay_ms(50); // Debounce delay
                    if(((*PIND & 0x04) != 0)) {
                        button1_pressed = 0; // Mark button 1 as not pressed
                    }
                }

                // Button 2 pressed handling (represents 1)
                if(((*PIND & 0x08) == 0x00) && (button2_pressed == 0)) {
                    _delay_ms(50); // Debounce delay
                    if(((*PIND & 0x08) == 0x00)) {
                        button2_pressed = 1; // Mark button 2 as pressed
                        // Shift left to make space for new LSB, set LSB to 1
                        binary_number = ((binary_number << 1) & 0xFE) | 0x01; // Shift left and set LSB to 1
                    }
                } else if(((*PIND & 0x08) != 0) && (button2_pressed == 1)) {
                    _delay_ms(50); // Debounce delay
                    if(((*PIND & 0x08) != 0)) {
                        button2_pressed = 0; // Mark button 2 as not pressed
                    }
                }

                // Update the binary and hex displays
                lcd_set_pos(&lcd, 0, 5); // Set position for binary display
                lcd_write_bin(&lcd, binary_number); // Write binary number to LCD
                lcd_set_pos(&lcd, 1, 5); // Set position for hex display
                lcd_write_hex(&lcd, binary_number); // Write hex number to LCD
            }

        // State when button 2 is pressed
        } else if(button2_pressed == 1) {
            lcd_init(&lcd); // Initialize the LCD
            lcd_write_text(&lcd, "HEX: "); // Display "HEX: " on the LCD
            lcd_set_pos(&lcd, 1, 0); // Set cursor position for binary display
            lcd_write_text(&lcd, "BIN: "); // Display "BIN: " on the LCD
            lcd_set_pos(&lcd, 0, 6); // Set position for first hex number
            lcd_set_pos(&lcd, 0, 7); // Set position for second hex number
            lcd_set_pos(&lcd, 1, 5); // Set position for binary number
            lcd_write_bin(&lcd, 0);  // Initialize binary display to 0

            while(1) {
                // Button 1 pressed handling
                if(((*PIND & 0x04) == 0x00) && (button1_pressed == 0)) {
                    _delay_ms(50); // Debounce delay
                    if(((*PIND & 0x04) == 0x00)) {
                        button1_pressed = 1; // Mark button 1 as pressed
                        hex_number1 = (hex_number1 + 1) % 16; // Increment hex number 1

                        // Update display if the value has changed
                        if (hex_number1 != last_hex_number1) {
                            lcd_set_pos(&lcd, 0, 6); // Set position for hex number 1
                            lcd_write_hex(&lcd, hex_number1); // Write hex number 1 to LCD
                            last_hex_number1 = hex_number1; // Update last value
                        }
                    }
                } else if(((*PIND & 0x04) != 0) && (button1_pressed == 1)) {
                    _delay_ms(50); // Debounce delay
                    if(((*PIND & 0x04) != 0)) {
                        button1_pressed = 0; // Mark button 1 as not pressed
                    }
                }

                // Button 2 pressed handling
                if(((*PIND & 0x08) == 0x00) && (button2_pressed == 0)) {
                    _delay_ms(50); // Debounce delay
                    if(((*PIND & 0x08) == 0x00)) {
                        button2_pressed = 1; // Mark button 2 as pressed
                        hex_number2 = (hex_number2 + 1) % 16; // Increment hex number 2

                        // Update display if the value has changed
                        if (hex_number2 != last_hex_number2) {
                            lcd_set_pos(&lcd, 0, 7); // Set position for hex number 2
                            lcd_write_hex(&lcd, hex_number2); // Write hex number 2 to LCD
                            last_hex_number2 = hex_number2; // Update last value
                        }
                    }
                } else if(((*PIND & 0x08) != 0) && (button2_pressed == 1)) {
                    _delay_ms(50); // Debounce delay
                    if(((*PIND & 0x08) != 0)) {
                        button2_pressed = 0; // Mark button 2 as not pressed
                    }
                }

                // Write the combined binary value
                lcd_set_pos(&lcd, 1, 5); 
                uint8_t combined_hex = (hex_number1 << 4) | hex_number2; // Combine hex numbers
                lcd_write_bin(&lcd, combined_hex); // Write combined value to binary display
            }
        }
    }
}



// Function to write a hexadecimal number to the LCD
void lcd_write_hex(lcd_t* lcd, uint8_t num) {
    char hex_str[3];
    snprintf(hex_str, sizeof(hex_str), "%02X", num); // Convert number to hex string
    lcd_write_text(lcd, hex_str); // Write hex string to LCD
}

// Function to write a binary number to the LCD
void lcd_write_bin(lcd_t* lcd, uint8_t num) {
    char bin_str[9] = {0}; 
    for (int i = 7; i >= 0; i--) {
        bin_str[7 - i] = (num & (1 << i)) ? '1' : '0'; // Fill the binary string
    }
    lcd_write_text(lcd, bin_str); // Write binary string to LCD
}

