#ifndef __SDIO_SDCARD_APP_H
#define   __SDIO_SDCARD_APP_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

#define SDCARD_DATA_START_CODE			0x55

#define SDCARD_LED1

#ifdef SDCARD_LED1
#define SDCARD_LED1_ON()		GPIO_SetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0])
#define SDCARD_LED1_OFF()	GPIO_ResetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0])
#endif

typedef __packed struct
{
     unsigned char            CrossID[8];
     unsigned char            PacketType;
     unsigned int             PacketInfo;
}SocketPacketHeadTypeDef;

typedef __packed struct
{
     unsigned char            DeviceType;
     unsigned short           Interval;
     unsigned char            LaneNo[3];
     unsigned char            DateTime[20];
     unsigned short           Volume;
     unsigned short           Volume1;
     unsigned short           Volume2;
     unsigned short           Volume3;
     unsigned short           Volume4;
     unsigned short           Volume5;
     unsigned short           AvgOccupancy;
     unsigned short           AvgHeadTime;
     float                    AvgLength;
     float                    AvgSpeed;
     unsigned char            Saturation;
     unsigned short           Density;
     unsigned short           Pcu;
     float                    AvgQueueLength;
}SocketPacketDataTypeDef;

typedef __packed struct
{
     unsigned char            ManuCode;
     unsigned char            CheckCode;
}SocketPacketLastTypeDef;

typedef __packed struct
{
	unsigned char			StartCode;
	unsigned int			FrontAddr;
	unsigned int			ReartAddr;
	unsigned char			SumCheck;
}SdcardDataHeadTypeDef;

void SDCard_APP_Initialized(void);

void SDCard_APP_RunEvent(void);

#ifdef SDCARD_LED1
extern unsigned short	OUTPUT_PIN[16];
extern GPIO_TypeDef*	OUTPUT_TYPE[16];
#endif












#endif
