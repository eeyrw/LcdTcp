// Do not remove the include below
#include "LcdTcp.h"

void setup()
{

  Serial.begin(230400);
  LcdInit();
  WifiConnectionInit();
  TcpServerInit();
}

void loop()
{
  TcpServerProc();
}
