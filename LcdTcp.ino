// Do not remove the include below
#include "LcdTcp.h"

void setup()
{

  Serial.begin(230400);
  LcdInit();
  WifiConnectionInit();
  if (!MDNS.begin("YUAN_STA"))
  {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
  MDNS.addService("esp", "tcp", 2400); // Announce esp tcp service on port 2400
  TcpServerInit();
}

void loop()
{
  TcpServerProc();
}
