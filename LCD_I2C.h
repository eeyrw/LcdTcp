#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>

extern void lcd_init       (void);
extern void lcd_clear      (void);
extern void lcd_putchar    (uint8_t c);
extern void set_cursor     (int32_t column, int32_t line);
extern void lcd_print      (char *string);
extern void lcd_bargraph   (int32_t value, int32_t size);
extern void lcd_bargraphXY (int32_t pos_x, int32_t pos_y, int32_t value,int32_t length);
extern void lcd_write_cmd (uint8_t c);
#endif

/******************************************************************************/

