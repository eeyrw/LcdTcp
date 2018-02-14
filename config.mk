THIS_DIR := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))
ROOT := $(THIS_DIR)/..
LIBS = $(ESP_LIBS)/SPI \
  $(ESP_LIBS)/Wire \
  $(ESP_LIBS)/ESP8266WiFi \
  $(ESP_LIBS)/ESP8266WebServer \
  $(ESP_LIBS)/DNSServer \
  $(ESP_LIBS)/DNSServer \
  $(ARDUINO_LIBS)/WiFiManager \
  $(ARDUINO_LIBS)/CircularBuffer
  
BOARD = nodemcuv2
UPLOAD_PORT = COM7
UPLOAD_SPEED = 1500000
FLASH_DEF=4M3M
#VERBOSE=1
