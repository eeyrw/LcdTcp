/*
 * LcdInterface.cpp
 *
 *  Created on: 2017年8月7日
 *      Author: yuansmac
 */

#include "LcdTcp.h"
LiquidCrystal_I2C lcd(I2C_LCD_ADDR, 16, 2, LCD_5x8DOTS);
void LcdInit(void)
{
	lcd.begin();
	// Print a message to the LCD.
	lcd.clear();

}
void LcdShowFetchIP(void)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Acquiring IP..");
}


void LcdShowStandby(void)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Standby.");
	lcd.setCursor(0, 1);
	lcd.print("IP:");
	lcd.print(WiFi.localIP());
}
