// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _wifi_H_
#define _wifi_H_
#include "Arduino.h"
//add your includes for the project wifi here
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager
#include <Wire.h>
#include "LCD_I2C.h"


//end of add your includes here


//add your function definitions for the project wifi here

extern void ParseEventFrameStream(WiFiClient* client);


//Do not add code below this line
#endif /* _wifi_H_ */
