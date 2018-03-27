/**
  *********************************************************************************************************
  * @file    lestcfunc.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   LESTC
  *********************************************************************************************************
  * @attention
  *		
  *			
  *********************************************************************************************************
  */

#include "lestcfunc.h"

/**********************************************************************************************************
 @Function			unsigned char LestcGetCheckCode(LestcPacketDataTypeDef* PacketData)
 @Description			LestcGetCheckCode	: 获取校验值
 @Input				PacketData		: Lestc数据结构体指针
 @Return				获取的校验值
**********************************************************************************************************/
unsigned char LestcGetCheckCode(LestcPacketDataTypeDef* PacketData)
{
	unsigned char checkCode = 0;
	
	checkCode = PacketData->MessageID ^ PacketData->MessageLength ^ PacketData->PlateNum ^ PacketData->CarStatus[0] ^ PacketData->CarStatus[1] ^
			  PacketData->CarStatus[2] ^ PacketData->CarStatus[3] ^ PacketData->Reserved;
	
	return checkCode;
}

/**********************************************************************************************************
 @Function			void LestcCarInSetStatus(LestcPacketDataTypeDef* PacketData, unsigned char status)
 @Description			LestcCarInSetStatus	: 获取车辆驶入信息
 @Input				PacketData		: Lestc数据结构体指针
					status			: 0 ~ 31车辆号
 @Return				void
**********************************************************************************************************/
void LestcCarInSetStatus(LestcPacketDataTypeDef* PacketData, unsigned char status)
{
	unsigned int carStatus = 0x00000001;
	
	carStatus = carStatus << status;
	
	PacketData->CarStatus[0] |= (carStatus & 0x000000FF);
	PacketData->CarStatus[1] |= ((carStatus >> 8 ) & 0x000000FF);
	PacketData->CarStatus[2] |= ((carStatus >> 16) & 0x000000FF);
	PacketData->CarStatus[3] |= ((carStatus >> 24) & 0x000000FF);
}

/**********************************************************************************************************
 @Function			void LestcCarOutSetStatus(LestcPacketDataTypeDef* PacketData, unsigned char status)
 @Description			LestcCarOutSetStatus: 获取车辆驶离信息
 @Input				PacketData		: Lestc数据结构体指针
					status			: 0 ~ 31车辆号
 @Return				void
**********************************************************************************************************/
void LestcCarOutSetStatus(LestcPacketDataTypeDef* PacketData, unsigned char status)
{
	unsigned int carStatus = 0x00000001;
	
	carStatus = ~(carStatus << status);
	
	PacketData->CarStatus[0] &= (carStatus & 0x000000FF);
	PacketData->CarStatus[1] &= ((carStatus >> 8 ) & 0x000000FF);
	PacketData->CarStatus[2] &= ((carStatus >> 16) & 0x000000FF);
	PacketData->CarStatus[3] &= ((carStatus >> 24) & 0x000000FF);
}

/********************************************** END OF FLEE **********************************************/
