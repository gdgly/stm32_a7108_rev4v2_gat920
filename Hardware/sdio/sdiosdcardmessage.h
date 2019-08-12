#ifndef __SDIO_SDCARD_MESSAGE_H
#define   __SDIO_SDCARD_MESSAGE_H

#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "stdbool.h"

void NET_Sdcard_FifoSendMessageInit(void);											//发送数据Fifo初始化
void NET_Sdcard_FifoRecvMessageInit(void);											//接收数据Fifo初始化

void NET_Sdcard_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);	//发送数据写入队列
void NET_Sdcard_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);	//接收数据写入队列

bool NET_Sdcard_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);	//发送数据读出队列
bool NET_Sdcard_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);	//接收数据读出队列

bool NET_Sdcard_Message_SendDataOffSet(void);										//发送数据队列(队列头偏移1)
bool NET_Sdcard_Message_RecvDataOffSet(void);										//接收数据队列(队列头偏移1)

unsigned char NET_Sdcard_Message_SendDataRear(void);									//发送数据队尾值
unsigned char NET_Sdcard_Message_RecvDataRear(void);									//接收数据队尾值

#endif
