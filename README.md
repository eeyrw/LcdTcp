# Wifi2LCD
The wifi text lcd device based on ESP8266. 

## Hardware
Check the following link :
https://lceda.cn/eeyrw/wifi-lcd

## Firmware [![Build Status](https://travis-ci.org/eeyrw/LcdTcp.svg?branch=master)](https://travis-ci.org/eeyrw/LcdTcp)
The firmware is based on ESP8266 Arduino core and several libraries. Currently the firmware is built by travis-ci and release the binary in the GitHub release page.

**Build firmware**

1. Install Arduino IDE. 
2. Install ESP8266 core ( Check this [link](https://github.com/esp8266/Arduino) about how to do ) .
3. Install libraries (`CircularBuffer`, `WiFiManager` by inside library manager of Arduino IDE.
4. Open the LcdTcp.ino with Arduino IDE and specify the board `NodeMCU 1.0 ESP-12E` and other parameters should be set accordingly.
5. Click verify button.

Also, if you are interested in building with [makeEspArduino](https://github.com/plerup/makeEspArduino) , you can check the https://github.com/plerup/makeEspArduino and config.mk in the project root.
**Upload firmware**
6. If you have not python 3, then get it. Because upload tool is written in python.
7. `pip install esptool` Setup by yourself is also feasible and check https://github.com/espressif/esptool .
8. esptool --port YOUR_COM_PORT --baud YOUR_BAUD write_flash 0x0 f:/Users/yuan/Desktop/LcdTcp.bin


<!--stackedit_data:
eyJoaXN0b3J5IjpbMzA0OTIzMDI3XX0=
-->