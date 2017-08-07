#include "LcdTcp.h"

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1
#define SRV_PORT 2400


WiFiServer server(SRV_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];


void TcpServerInit(void)
{
	Serial.println("TCP SERVER BEGIN WITH:");
	  server.begin();
	  server.setNoDelay(true);
	  Serial.print("IP:");
	  Serial.println(WiFi.localIP());
}

void TcpServerProc(void)
{
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
