#ifndef __LCD_PRIV_H__

#define __LCD_PRIV_H__

#define F_CPU   8000000

#define LCD_CMD_DELAY_MS        5
#define LCD_SHIFT_DELAY_MS      1000

// Clear display
#define CMD_CLEAR_DISP(cmd)  ((cmd) = 0x01)

// Return home
#define CMD_RETURN_HOME(cmd) ((cmd) = 0x02)

// Display on/off control(00001100)
#define CMD_DISP_CTRL(cmd, on, cursor, blinking)   \
                        ((cmd) = 0x08 | ((on) << 2) | ((cursor) << 1) | (blinking))

// Display function set (00101000)
#define CMD_FUNC_SET(cmd, dl, lines, font)  \
                        ((cmd) = 0x20 | ((dl) << 4) | ((lines) << 3) | ((font) << 2))

// Set DDRAM address
#define CMD_DDRAM_ADDR(cmd, address)    \
                        ((cmd) = 0x80 | (address))

// Entry mode set(00000110)
#define CMD_ENTRY_MODE(cmd, inc, shift) \
                        ((cmd) = 0x04 | ((inc) << 1) | ((shift)))

// Cursor or display shift
#define CMD_DISP_SHIFT(cmd, shift, right)   \
                        ((cmd) = 0x10 | ((shift) << 3) | ((right) << 2))


#endif /* __LCD_PRIV_H__*/