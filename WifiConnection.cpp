/*
 * WifiConnection.cpp
 *
 *  Created on: 2017年8月7日
 *      Author: yuansmac
 */
#include "LcdTcp.h"

#define AUX_AP_SSID "LCD TCP WIFI CONFIG"

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}


void WifiConnectionInit(void)
{
	  //WiFiManager
	  //Local intialization. Once its business is done, there is no need to keep it around
	  WiFiManager wifiManager;
	  //reset settings - for testing
	  //wifiManager.resetSettings();

	  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
	  wifiManager.setAPCallback(configModeCallback);

	  //fetches ssid and pass and tries to connect
	  //if it does not connect it starts an access point with the specified name
	  //here  "AutoConnectAP"
	  //and goes into a blocking loop awaiting configuration
	  if(!wifiManager.autoConnect(AUX_AP_SSID)) {
	    Serial.println("failed to connect and hit timeout");
	    //reset and try again, or maybe put it to deep sleep
	    ESP.reset();
	    delay(1000);
	  }

	  //if you get here you have connected to the WiFi
	  Serial.println("connected...yeey :)");
	  lcd.setCursor(0,1);
	  lcd.print(WiFi.localIP());
}





