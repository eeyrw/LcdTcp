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
#define CMD_ECHO 0x09
#define CMD_GET_VER_INFO 0x0A
#define CMD_LCD_DE_INIT 0x0B
#define CMD_ENTER_BOOT 0x19

#define CMD_OFFEST 0x03

typedef enum _EVENT_FRAME_PARSER_STATUS {
	FRAME_PARSER_STATUS_IDLE = 0,
	FRAME_PARSER_STATUS_SOF_LO,
	FRAME_PARSER_STATUS_SOF_HI,
	FRAME_PARSER_STATUS_RECV_CMD_LEN,

} EVENT_FRAME_PARSER_STATUS;

EVENT_FRAME_PARSER_STATUS frameParseStatus;

uint8_t cmdBuf[256];
uint8_t cmdRetBuf[256];

uint8_t *GetCmdDataPtr(uint8_t *buffer)
{
	return buffer + CMD_OFFEST;
}

int Protocol_Process(unsigned char *Buf)
{
	unsigned int i;
	int retByteNum = 0;
	uint8_t *rbf;

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
		}

		break;

	case CMD_LCD_SETCURSOR:
		Serial.printf("CMD_LCD_SETCURSOR.X=%d,Y=%d\n", Buf[1], Buf[2]);
		lcd.setCursor(Buf[1], Buf[2]);

		break;

	case CMD_LCD_CUSTOMCHAR:
		Serial.println("CMD_LCD_CUSTOMCHAR.");
		lcd.createChar(Buf[1], &Buf[2]);
		break;

	case CMD_LCD_WRITECMD:
		Serial.printf("CMD_LCD_COMMAND: 0x%X.\n", Buf[1]);
		lcd.command(Buf[1]);

		break;

	case CMD_ECHO:
		rbf = GetCmdDataPtr(cmdRetBuf);
		rbf[0] = CMD_ECHO;
		for (i = 1; i < Buf[1] + 1 + 1; i++)
		{
			rbf[i] = Buf[i];
		}
		retByteNum = Buf[1] + 1 + 1;
		break;

	case CMD_GET_VER_INFO:
		rbf = GetCmdDataPtr(cmdRetBuf);
		rbf[0] = CMD_GET_VER_INFO;
		rbf[1] = sprintf((char *)&rbf[2], "Date: %s, Time: %s, Git: %s, Env: %s", _BuildInfo.date, _BuildInfo.time, _BuildInfo.src_version, _BuildInfo.env_version);
		retByteNum = rbf[1] + 1 + 1;
		break;
	case CMD_LCD_DE_INIT:
		LcdShowStandby();

		break;
	case CMD_ENTER_BOOT:

	default:
		break;
	}
	return retByteNum;
}

void ParseEventFrameStream(WiFiClient *client)
{
	uint8_t streamByte;
	static uint8_t cmdLen = 0;

	switch (frameParseStatus)
	{
	case FRAME_PARSER_STATUS_IDLE:
	{
		if (client->available())
		{
			streamByte = client->read();
			if (streamByte == ((uint8_t)(0xFF & EVENT_FRAME_FLAG)))
			{
				frameParseStatus = FRAME_PARSER_STATUS_SOF_LO;
			}
		}
	}
	break;
	case FRAME_PARSER_STATUS_SOF_LO:
	{
		if (client->available())
		{
			streamByte = client->read();
			if (streamByte == ((uint8_t)(0xFF & (EVENT_FRAME_FLAG >> 8))))
			{
				frameParseStatus = FRAME_PARSER_STATUS_SOF_HI;
			}
		}
	}
	break;
	case FRAME_PARSER_STATUS_SOF_HI:
	{
		if (client->available())
		{
			streamByte = client->read();
			cmdLen = streamByte;
			frameParseStatus = FRAME_PARSER_STATUS_RECV_CMD_LEN;
		}
	}
	break;

	case FRAME_PARSER_STATUS_RECV_CMD_LEN:
	{
		if (client->available() >= cmdLen)
		{
			client->read(cmdBuf, cmdLen);
			int retByteNum = Protocol_Process(cmdBuf);
			if (retByteNum > 0)
			{
				cmdRetBuf[0] = (uint8_t)(0xFF & EVENT_FRAME_FLAG);
				cmdRetBuf[1] = (uint8_t)(0xFF & (EVENT_FRAME_FLAG >> 8));
				cmdRetBuf[2] = retByteNum;
				retByteNum += 3;
				client->write(cmdRetBuf, retByteNum);
			}

			frameParseStatus = FRAME_PARSER_STATUS_IDLE;
			cmdLen = 0;
		}
	}
	break;

	default:
		break;
	}
}
