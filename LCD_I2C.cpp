#include <stdio.h>
#include "LCD_I2C.h"
#include "LcdTcp.h"

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be 
 increased by factor 2^N by this constant                                   */
#define DELAY_2N     2
/* 8 user defined characters to be loaded into CGRAM (used for bargraph)      */
const uint8_t UserFont[8][8] = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00 }, { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 }, { 0x18,
		0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }, { 0x1C, 0x1C, 0x1C, 0x1C,
		0x1C, 0x1C, 0x1C, 0x1C }, { 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
		0x1E }, { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }, { 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00 } };

/*******************************************************************************
 * Delay in while loop cycles                                                   *
 *   Parameter:    cnt:    number of while cycles to delay                      *
 *   Return:                                                                    *
 *******************************************************************************/

void delay_lcd_i2c(uint32_t cnt) {
	cnt <<= DELAY_2N;

	while (cnt--)
		;
}

/*******************************************************************************
 * Write command to LCD controller                                              *
 *   Parameter:    c:      command to be written                                *
 *   Return:                                                                    *
 *******************************************************************************/

void lcd_write_cmd(uint8_t c) {

	Wire.beginTransmission(0x3c);
	/*ѡ�� SLAVE ADDRESS*/
	Wire.write(0x80); /*�����ֽڣ�Co=1,A0=0,��ʾ���´���� 1 ���ֽ���ָ��*/
	Wire.write(c);
	Wire.endTransmission();
}

/*******************************************************************************
 * Write data to LCD controller                                                 *
 *   Parameter:    c:      data to be written                                   *
 *   Return:                                                                    *
 *******************************************************************************/

void lcd_write_data(uint8_t c) {

	Wire.beginTransmission(0x3c);
	Wire.write(0x40); /*�����ֽڣ�Co=0,A0=1,��ʾ���´���� n ���ֽ�������*/
	Wire.write(c);
	Wire.endTransmission();
}

/*******************************************************************************
 * Set cursor position on LCD display                                           *
 *   Parameter:    column: column position                                      *
 *                 line:   line position                                        *
 *   Return:                                                                    *
 *******************************************************************************/

void set_cursor(int32_t column, int32_t line) {
	uint8_t address;

	address = (line * 0x40) + column;
	address = 0x80 + (address & 0x7F);
	lcd_write_cmd(address); /* Set DDRAM address counter to 0     */

}

/*******************************************************************************
 * Clear the LCD display                                                        *
 *   Parameter:                                                                 *
 *   Return:                                                                    *
 *******************************************************************************/

void lcd_clear(void) {
	lcd_write_cmd(0x01); /* Display clear                      */
	delay_lcd_i2c(100);
	set_cursor(0, 0);
	delay_lcd_i2c(200);
}

/*******************************************************************************
 * Print Character to current cursor position                                   *
 *   Parameter:    c:      character to be printed                              *
 *   Return:                                                                    *
 *******************************************************************************/

void lcd_putchar(uint8_t c) {
	lcd_write_data(c);
}

/*******************************************************************************
 * Print sting to LCD display                                                   *
 *   Parameter:    string: pointer to output string                             *
 *   Return:                                                                    *
 *******************************************************************************/

void lcd_print(char *string) {
	Wire.beginTransmission(0x3c);
	Wire.write(0x40); /*�����ֽڣ�Co=0,A0=1,��ʾ���´���� n ���ֽ�������*/
	while (*string) {
		Wire.write(*string++);
	}
	Wire.endTransmission();
}

void lcd_ld_GCRAM(uint8_t* UserFont, uint8_t len) {
	uint32_t i;

	Wire.beginTransmission(0x3c);
	/*?? SLAVE ADDRESS*/
	Wire.write(0x80); /*????:Co=1,A0=0,??????????? 1 ??????*/
	Wire.write(0x40); //?? XGRAM ADDRESS:??? CGRAM,0X40 ??? 0 ??
	Wire.write(0x40); /*????:Co=0,A0=1,??????? n ??????*/
	for (i = 0; i < len; i++) {
		Wire.write(*UserFont);

		UserFont++;
	}

	Wire.endTransmission();

}

/*******************************************************************************
 * Print a bargraph to LCD display                                              *
 *   Parameter:     val:  value 0..100 %                                        *
 *                  size: size of bargraph 1..16                                *
 *   Return:                                                                    *
 *******************************************************************************/
void lcd_bargraph(int32_t value, int32_t size) {
	int32_t i;

	value = value * size / 20; /* Display matrix 5 x 8 pixels       */
	for (i = 0; i < size; i++) {
		if (value > 5) {
			lcd_putchar(0x05);
			value -= 5;
		} else {
			lcd_putchar(value);
			break;
		}
	}
}

/*******************************************************************************
 * Display bargraph on LCD display                                              *
 *   Parameter:     pos_x: horizontal position of bargraph start                *
 *                  pos_y: vertical position of bargraph                        *
 *                  value: size of bargraph active field (in pixels)            *
 *   Return:                                                                    *
 *******************************************************************************/

void lcd_bargraphXY(int32_t pos_x, int32_t pos_y, int32_t value,
		int32_t length) {
	int32_t i;

	set_cursor(pos_x, pos_y);
	for (i = 0; i < length; i++) {
		if (value > 5) {
			lcd_putchar(0x05);
			value -= 5;
		} else {
			lcd_putchar(value);
			while (++i < length)
				lcd_putchar(0);
		}
	}
}

void lcd_init() {

	Wire.begin();
	delay_lcd_i2c(1000);
	Wire.beginTransmission(0x3c);
	/*ѡ�� SLAVE ADDRESS*/
	Wire.write(0x00);
	/*�����ֽڣ�Co=0,A0=0,��ʾ���´���� N ���ֽ���ָ��*/
	Wire.write(0x38); //Function set
	Wire.write(0x0c); //Display ON/OFF
	Wire.write(0x01); //Clear display
	Wire.write(0x06); //Entry mode set
	Wire.endTransmission();
	/*������־*/
	delay_lcd_i2c(300);
	lcd_ld_GCRAM((uint8_t*) UserFont, 64);
	delay_lcd_i2c(300);

}

