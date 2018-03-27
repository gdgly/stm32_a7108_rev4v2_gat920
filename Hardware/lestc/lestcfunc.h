#ifndef __LESTC_FUNC_H
#define   __LESTC_FUNC_H

#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "lestcconfig.h"

unsigned char LestcGetCheckCode(LestcPacketDataTypeDef* PacketData);				//获取校验值
void LestcCarInSetStatus(LestcPacketDataTypeDef* PacketData, unsigned char status);	//获取车辆驶入信息
void LestcCarOutSetStatus(LestcPacketDataTypeDef* PacketData, unsigned char status);	//获取车辆驶离信息

#endif
