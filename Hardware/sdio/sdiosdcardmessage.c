/**
  *********************************************************************************************************
  * @file    sdiosdcardmessage.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   sdiosdcard应用层消息队列
  *********************************************************************************************************
  * @attention
  *		
  *
  *
  *
  *********************************************************************************************************
  */

#include "sdiosdcardmessage.h"
#include "fifomessage.h"
#include "string.h"

#define MESSAGE_SDCARD_SENDPARKNUM_MAX			64
#define MESSAGE_SDCARD_RECVPARKNUM_MAX			10
#define MESSAGE_SDCARD_SENDPARKSIZE_MAX			1024
#define MESSAGE_SDCARD_RECVPARKSIZE_MAX			16

MessageFifoTypeDef		NETSdcardFifoMessageSendPark;
MessageFifoTypeDef		NETSdcardFifoMessageRecvPark;

unsigned char			SdcardFifoMessageSendBuf[MESSAGE_SDCARD_SENDPARKSIZE_MAX];
unsigned char			SdcardFifoMessageRecvBuf[MESSAGE_SDCARD_RECVPARKSIZE_MAX];

static bool			SdcardFifoSendInit = false;
static bool			SdcardFifoRecvInit = false;

/**********************************************************************************************************
 @Function			void NET_Sdcard_FifoSendMessageInit(void)
 @Description			NET_Sdcard_FifoSendMessageInit		: 发送数据Fifo初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_Sdcard_FifoSendMessageInit(void)
{
	netMessageFifoInit(&NETSdcardFifoMessageSendPark, SdcardFifoMessageSendBuf, sizeof(SdcardFifoMessageSendBuf), MESSAGE_SDCARD_SENDPARKNUM_MAX);
	
	SdcardFifoSendInit = true;
}

/**********************************************************************************************************
 @Function			void NET_Sdcard_FifoRecvMessageInit(void)
 @Description			NET_Sdcard_FifoRecvMessageInit		: 接收数据Fifo初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_Sdcard_FifoRecvMessageInit(void)
{
	netMessageFifoInit(&NETSdcardFifoMessageRecvPark, SdcardFifoMessageRecvBuf, sizeof(SdcardFifoMessageRecvBuf), MESSAGE_SDCARD_RECVPARKNUM_MAX);
	
	SdcardFifoRecvInit = true;
}

/**********************************************************************************************************
 @Function			void NET_Sdcard_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_Sdcard_Message_SendDataEnqueue		: 发送数据写入队列
 @Input				dataBuf	 		 				: 需写入数据
					dataLength						: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_Sdcard_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if (SdcardFifoSendInit != true) return;
	
	netMessageFifoEnqueue(&NETSdcardFifoMessageSendPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			void NET_Sdcard_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_Sdcard_Message_RecvDataEnqueue		: 接收数据写入队列
 @Input				dataBuf	 		 				: 需写入数据
					dataLength						: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_Sdcard_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if (SdcardFifoRecvInit != true) return;
	
	netMessageFifoEnqueue(&NETSdcardFifoMessageRecvPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_Sdcard_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_Sdcard_Message_SendDataDequeue		: 发送数据读出队列
 @Input				dataBuf	 		 				: 需读出数据地址
					dataLength						: 需读出数据长度地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_Sdcard_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	if (SdcardFifoSendInit != true) return false;
	
	return netMessageFifoDequeue(&NETSdcardFifoMessageSendPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_Sdcard_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_Sdcard_Message_RecvDataDequeue		: 接收数据读出队列
 @Input				dataBuf	 		 				: 需读出数据地址
					dataLength						: 需读出数据长度地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_Sdcard_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	if (SdcardFifoRecvInit != true) return false;
	
	return netMessageFifoDequeue(&NETSdcardFifoMessageRecvPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_Sdcard_Message_SendDataOffSet(void)
 @Description			NET_Sdcard_Message_SendDataOffSet		: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_Sdcard_Message_SendDataOffSet(void)
{
	if (SdcardFifoSendInit != true) return false;
	
	return netMessageFifoDiscard(&NETSdcardFifoMessageSendPark);
}

/**********************************************************************************************************
 @Function			bool NET_Sdcard_Message_RecvDataOffSet(void)
 @Description			NET_Sdcard_Message_RecvDataOffSet		: 接收数据队列(队列头偏移1)
 @Input				void
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_Sdcard_Message_RecvDataOffSet(void)
{
	if (SdcardFifoRecvInit != true) return false;
	
	return netMessageFifoDiscard(&NETSdcardFifoMessageRecvPark);
}

/**********************************************************************************************************
 @Function			unsigned char NET_Sdcard_Message_SendDataRear(void)
 @Description			NET_Sdcard_Message_SendDataRear		: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_Sdcard_Message_SendDataRear(void)
{
	if (SdcardFifoSendInit != true) return 0;
	
	return netMessageFifoRear(&NETSdcardFifoMessageSendPark);
}

/**********************************************************************************************************
 @Function			unsigned char NET_Sdcard_Message_RecvDataRear(void)
 @Description			NET_Sdcard_Message_RecvDataRear		: 接收数据队尾值
 @Input				void
 @Return				接收数据队尾值
**********************************************************************************************************/
unsigned char NET_Sdcard_Message_RecvDataRear(void)
{
	if (SdcardFifoRecvInit != true) return 0;
	
	return netMessageFifoRear(&NETSdcardFifoMessageRecvPark);
}

/********************************************** END OF FLEE **********************************************/
