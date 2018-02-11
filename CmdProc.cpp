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
	FRAME_PARSER_STATUS_RECV_CMD_LEN,

} EVENT_FRAME_PARSER_STATUS;

EVENT_FRAME_PARSER_STATUS frameParseStatus;

uint8_t cmdBuf[256];

void Protocol_Process(unsigned char *Buf)
{
	unsigned int i;

	switch (Buf[0])
	{
	case CMD_LCD_INIT:
		lcd = LiquidCrystal_I2C(0x3F, Buf[1], Buf[2], LCD_5x8DOTS);
		lcd.begin();
		lcd.clear();
		Serial.printf("CMD_LCD_INIT.X=%d,Y=%d\n", Buf[1], Buf[2]);

		break;

	case CMD_LCD_SETBACKLGIHT:
		Serial.printf("CMD_LCD_SETBACKLGIHT.Level=%d\n", Buf[1]);
		if (Buf[1])
		{
			lcd.backlight();
		}
		else
		{
			lcd.noBacklight();
		}

		break;

	case CMD_LCD_SETCONTRAST:
		Serial.printf("CMD_LCD_SETCONTRAST.Level=%d\n", Buf[1]);

		break;

	case CMD_LCD_SETBRIGHTNESS:
		Serial.printf("CMD_LCD_SETCONTRAST.Level=%d\n", Buf[1]);

		break;

	case CMD_LCD_WRITEDATA:
		for (i = 0; i < Buf[1]; i++)
		{
			lcd.write(Buf[2 + i]);
			//Serial.write(Buf[2 + i]);
		}

		//Serial.println("");

		break;

	case CMD_LCD_SETCURSOR:
		//Serial.printf("CMD_LCD_SETCURSOR.X=%d,Y=%d\n",Buf[1], Buf[2]);
		//set_cursor(Buf[1], Buf[2]);
		lcd.setCursor(Buf[1], Buf[2]);

		break;

	case CMD_LCD_CUSTOMCHAR:
		Serial.println("CMD_LCD_CUSTOMCHAR.");
		lcd.createChar(Buf[1], &Buf[2]);
		break;

	case CMD_LCD_WRITECMD:

		break;

	case CMD_ENTER_BOOT:

	default:
		break;
	}
}
unsigned short crc16(const unsigned char *data_p, unsigned char length)
{
	unsigned char x;
	unsigned short crc = 0xFFFF;

	while (length--)
	{
		x = crc >> 8 ^ *data_p++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
	}
	return crc;
}
int ParseEventFrameStream(CircularBuffer<char, 1024> *client)
{
	uint8_t streamByte;
	static uint8_t cmdLen = 0;

	switch (frameParseStatus)
	{
	case FRAME_PARSER_STATUS_IDLE:
	{
		if (client->size())
		{
			streamByte = client->shift();
			if (streamByte == ((uint8_t)(0xFF & EVENT_FRAME_FLAG)))
			{
				frameParseStatus = FRAME_PARSER_STATUS_SOF_LO;
			}
		}
		else
		{
			return 1;
		}
	}
	break;
	case FRAME_PARSER_STATUS_SOF_LO:
	{
		if (client->size())
		{
			streamByte = client->shift();
			if (streamByte == ((uint8_t)(0xFF & (EVENT_FRAME_FLAG >> 8))))
			{
				frameParseStatus = FRAME_PARSER_STATUS_SOF_HI;
			}
			else
			{
				return 1;
			}
		}
	}
	break;
	case FRAME_PARSER_STATUS_SOF_HI:
	{
		if (client->size())
		{
			streamByte = client->shift();

			cmdLen = streamByte;
			frameParseStatus = FRAME_PARSER_STATUS_RECV_CMD_LEN;
		}
		else
		{
			return 1;
		}
	}
	break;

	case FRAME_PARSER_STATUS_RECV_CMD_LEN:
	{
		if (client->size() >= cmdLen)
		{
			for (int i = 0; i < cmdLen; i++)
			{
				cmdBuf[i] = client->shift();
			}
			//client->read(cmdBuf, cmdLen);
			uint16_t crc = crc16(cmdBuf, cmdLen - 2);
			uint16_t recv_crc = cmdBuf[cmdLen - 2] | (cmdBuf[cmdLen - 1] << 8);
			if (crc == recv_crc)
				Protocol_Process(cmdBuf);
			else
				Serial.printf("CRC ERROR\n");

			frameParseStatus = FRAME_PARSER_STATUS_IDLE;
			cmdLen = 0;
			return 1;
		}
		else
		{
			return 1;
		}
	}
	break;

	default:
		break;
	}
	return 0;
}
