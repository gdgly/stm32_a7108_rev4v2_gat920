#ifndef __LESTC_CONFIG_H
#define   __LESTC_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

#define	LestcProtocolHeadLength		5
#define	LestcCarStatusNumber		4
#define	LestcMessageID				0x03
#define	LestcPlateNumber			0x90

#define	LestcSendTime				125									//LESTC发送时间(MS)

typedef __packed struct
{
	unsigned char	ProtocolHead[LestcProtocolHeadLength];
	unsigned char	MessageID;
	unsigned char	MessageLength;
	unsigned char	PlateNum;
	unsigned char	CarStatus[LestcCarStatusNumber];
	unsigned char	Reserved;
	unsigned char	CheckCode;
}LestcPacketDataTypeDef;

extern LestcPacketDataTypeDef		LestcPacketData;
extern u32 LestcSendTimeTick;

void LestcInitPackData(void);												//LESTC初始化

#endif
