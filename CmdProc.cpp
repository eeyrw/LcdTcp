#include "LcdTcp.h"

#define EVENT_FRAME_FLAG 0x776E //ASCII:"wn"

#define CMD_LCD_INIT 0x01
#define CMD_LCD_SETBACKLGIHT 0x02
#define CMD_LCD_SETCONTRAST 0x03
#define CMD_LCD_SETBRIGHTNESS 0x04
#define CMD_LCD_WRITEDATA 0x05
#define CMD_LCD_SETCURSOR 0x06
#define CMD_LCD_CUSTOMCHAR 0x07
#define CMD_LCD_WRITECMD 0x08
#define CMD_ENTER_BOOT 0x19

typedef enum _EVENT_FRAME_PARSER_STATUS {
	FRAME_PARSER_STATUS_IDLE = 0,
	FRAME_PARSER_STATUS_SOF_LO,
	FRAME_PARSER_STATUS_SOF_HI,
	FRAME_PARSER_STATUS_SOF_SYS_LO,
	FRAME_PARSER_STATUS_SOF_SYS_HI,
} EVENT_FRAME_PARSER_STATUS;

EVENT_FRAME_PARSER_STATUS frameParseStatus;

void Protocol_Process(unsigned char* Buf) {
	unsigned int i;

	switch (Buf[0]) {
	case CMD_LCD_INIT:

		//lcd_init(Buf[1], Buf[2]);
   lcd_init();
		Serial.println("Lcd init.");

		break;

	case CMD_LCD_SETBACKLGIHT:
		Serial.println("CMD_LCD_SETBACKLGIHT.");

		break;

	case CMD_LCD_SETCONTRAST:
		Serial.println("CMD_LCD_SETCONTRAST.");

		break;

	case CMD_LCD_SETBRIGHTNESS:
		Serial.println("CMD_LCD_SETBRIGHTNESS.");

		break;

	case CMD_LCD_WRITEDATA:
		for (i = 0; i < Buf[1]; i++) {
			lcd_putchar(Buf[2 + i]);
			Serial.write(Buf[2 + i]);

		}

		Serial.println("");

		break;

	case CMD_LCD_SETCURSOR:

		set_cursor(Buf[1], Buf[2]);

		break;

	case CMD_LCD_CUSTOMCHAR:

//		if (Is_Daul) {
//			CurrentPanel = 1;
//			lcd_write_cmd(0x40 | 8 * Buf[1]);
//
//			for (i = 0; i < 8; i++)
//				lcd_putchar(Buf[2 + i]);
//		}
//		CurrentPanel = 0;
		lcd_write_cmd(0x40 | 8 * Buf[1]);

		for (i = 0; i < 8; i++)
			lcd_putchar(Buf[2 + i]);

		break;

	case CMD_LCD_WRITECMD:

		break;

	case CMD_ENTER_BOOT:

	default:
		break;

	}
}

void ParseEventFrameStream(WiFiClient* client) {
	uint8_t streamByte;

	switch (frameParseStatus) {
	case FRAME_PARSER_STATUS_IDLE: {
		if (client->available()) {
			streamByte = client->read();
			if (streamByte == ((uint8_t) (0xFF & EVENT_FRAME_FLAG))) {
				frameParseStatus = FRAME_PARSER_STATUS_SOF_LO;
			}
		}
	}
		break;
	case FRAME_PARSER_STATUS_SOF_LO: {
		if (client->available()) {
			streamByte = client->read();
			if (streamByte == ((uint8_t) (0xFF & (EVENT_FRAME_FLAG >> 8)))) {
				frameParseStatus = FRAME_PARSER_STATUS_SOF_HI;
			}
		}
	}
		break;
	case FRAME_PARSER_STATUS_SOF_HI: {
		uint8_t buf[62];
		if (client->available() >= 62) {
			client->read(buf, sizeof(buf));
			Protocol_Process(buf);
			frameParseStatus = FRAME_PARSER_STATUS_IDLE;
		}
	}
		break;

	default:
		break;
	}
}
