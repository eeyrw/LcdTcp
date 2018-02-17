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
3. Install libraries (`WiFiManager` by inside library manager of Arduino IDE.
4. Open the LcdTcp.ino with Arduino IDE and specify the board `NodeMCU 1.0 ESP-12E` and other parameters should be set accordingly.
5. Click verify button.

Also, if you are interested in building with [makeEspArduino](https://github.com/plerup/makeEspArduino) , you can check the https://github.com/plerup/makeEspArduino and config.mk in the project root. Generally, you can clone the makeEspArduino to the project root and run `make -f ./makeEspArduino/makeEspArduino.mk -j7` to build.

**Upload firmware**

If you choose Arduino IDE, the correct COM port should be set. After that, click upload button. You can upload binary through esptool in the scenario that you just want to upload bin from release page to ESP8266 . The following steps can be taken:
1. If you have not python 3, then get it. Because upload tool is written in python.
2. Run `pip install esptool` .  Installing this tool manually is also feasible and check https://github.com/espressif/esptool to learn how to do .
3. Run `esptool --port YOUR_COM_PORT --baud YOUR_BAUD_RATE write_flash 0x0 BIN_PATH`  




<!--stackedit_data:
eyJoaXN0b3J5IjpbLTcyNjM3MjMwMywyMDc4Mzg4ODg3XX0=
-->