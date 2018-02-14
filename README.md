# Wifi2LCD
The wifi text lcd device based on ESP8266. 

## Hardware
Check the following link :
https://lceda.cn/eeyrw/wifi-lcd

## Firmware [![Build Status](https://travis-ci.org/eeyrw/LcdTcp.svg?branch=master)](https://travis-ci.org/eeyrw/LcdTcp)
The firmware is based on ESP8266 Arduino core and several libraries. Currently the firmware is built by travis-ci and release the binary in the GitHub release page.
If you want to compile by yourself, you may do something like this:
1. Install Arduino IDE. 
2. Install ESP8266 core ( Check this [link](https://github.com/esp8266/Arduino) about how to do ) .
3. Install libraries (`CircularBuffer`, `WiFiManager` by inside library manager of Arduino IDE.
4. Open the LcdTcp.ino with Arduino IDE and specify the board `NodeMCU 1.0 ESP-12E` and other param

<!--stackedit_data:
eyJoaXN0b3J5IjpbMjY3MzY3MjNdfQ==
-->