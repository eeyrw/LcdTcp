/*
 * LcdInterface.cpp
 *
 *  Created on: 2017年8月7日
 *      Author: yuansmac
 */

#include "LcdTcp.h"

void LcdInit(void)
{
	  lcd_init();
	  lcd_clear();
	  set_cursor(0,0);
	  lcd_print("THE LCD WORKS.");
}



