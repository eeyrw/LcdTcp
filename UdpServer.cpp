#include "LcdTcp.h"

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1
#define LOCAL_PORT 2400

WiFiUDP udp;
CircularBuffer<char, 1024> queue;
char incomingPacket[255];

void UdpServerInit(void)
{
	Serial.println("UDP SERVER BEGIN WITH:");
	Serial.print("IP:");
	Serial.println(WiFi.localIP());

	Serial.println("Starting UDP");
	udp.begin(LOCAL_PORT);
	Serial.print("Local port: ");
	Serial.println(udp.localPort());
	queue.clear();
}

void UdpServerProc(void)
{

	int cb = udp.parsePacket();
	if (!cb)
	{
		ParseEventFrameStream(&queue);
	}
	else
	{
		//Serial.print("packet received, length=");
		//Serial.println(cb);
		// We've received a packet, read the data from it

		//the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, esxtract the two words:
		//if(ParseEventFrameStream(&udp)!=1)
		//goto par;

		// receive incoming UDP packets
		int len = udp.read(incomingPacket, cb);

		for (int i = 0; i < cb; i++)
		{
			queue.push(incomingPacket[i]);
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
}
