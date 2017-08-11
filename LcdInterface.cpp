/*
 * LcdInterface.cpp
 *
 *  Created on: 2017年8月7日
 *      Author: yuansmac
 */

#include "LcdTcp.h"
	LiquidCrystal_I2C lcd(0x3F,16,2,LCD_5x8DOTS);
void LcdInit(void)
{
	  //lcd_init();

	  lcd.begin();
	  // Print a message to the LCD.
	  lcd.clear();
	  lcd.setCursor(0,0);
	  lcd.print("Hello, world!");

	  //lcd_clear();
	  //set_cursor(0,0);
	  //lcd_print("THE LCD WORKS.");
}



