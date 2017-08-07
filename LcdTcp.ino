// Do not remove the include below
#include "LcdTcp.h"


//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1


WiFiServer server(2400);
WiFiClient serverClients[MAX_SRV_CLIENTS];



void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void WifiManagerSetup(){
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
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
}

void setup() {
  Serial.begin(230400);

  lcd_init();
  lcd_clear();
  set_cursor(0,0);
  lcd_print("THE LCD WORKS.");

  WifiManagerSetup();
  //start UART and the server
  server.begin();
  server.setNoDelay(true);

  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");
}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        //get data from the telnet client and push it to the UART
    	  ParseEventFrameStream(&serverClients[i]);
      }
    }
  }
  //check UART for data
//  if(Serial.available()){
//    size_t len = Serial.available();
//    uint8_t sbuf[len];
//    Serial.readBytes(sbuf, len);
//    //push UART data to all connected telnet clients
//    for(i = 0; i < MAX_SRV_CLIENTS; i++){
//      if (serverClients[i] && serverClients[i].connected()){
//        serverClients[i].write(sbuf, len);
//        delay(1);
//      }
//    }
//  }
}
