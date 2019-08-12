/**
  *********************************************************************************************************
  * @file    socketxunfeimessage.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socketXunfei消息队列
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "socketxunfeimessage.h"
#include "fifomessage.h"
#include "string.h"

#define MESSAGE_SOCKETXUNFEI_SENDPARKNUM_MAX			64
#define MESSAGE_SOCKETXUNFEI_RECVPARKNUM_MAX			20
#define MESSAGE_SOCKETXUNFEI_SENDPARKSIZE_MAX		4096
#define MESSAGE_SOCKETXUNFEI_RECVPARKSIZE_MAX		1024

MessageFifoTypeDef		NETSocketXunfeiFifoMessageSendPark;
MessageFifoTypeDef		NETSocketXunfeiFifoMessageRecvPark;

unsigned char			SocketXunfeiFifoMessageSendBuf[MESSAGE_SOCKETXUNFEI_SENDPARKSIZE_MAX];
unsigned char			SocketXunfeiFifoMessageRecvBuf[MESSAGE_SOCKETXUNFEI_RECVPARKSIZE_MAX];

/**********************************************************************************************************
 @Function			void NET_SocketXunfei_FifoSendMessageInit(void)
 @Description			NET_SocketXunfei_FifoSendMessageInit		: 发送数据Fifo初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_SocketXunfei_FifoSendMessageInit(void)
{
	netMessageFifoInit(&NETSocketXunfeiFifoMessageSendPark, SocketXunfeiFifoMessageSendBuf, sizeof(SocketXunfeiFifoMessageSendBuf), MESSAGE_SOCKETXUNFEI_SENDPARKNUM_MAX);
}

/**********************************************************************************************************
 @Function			void NET_SocketXunfei_FifoRecvMessageInit(void)
 @Description			NET_SocketXunfei_FifoRecvMessageInit		: 接收数据Fifo初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_SocketXunfei_FifoRecvMessageInit(void)
{
	netMessageFifoInit(&NETSocketXunfeiFifoMessageRecvPark, SocketXunfeiFifoMessageRecvBuf, sizeof(SocketXunfeiFifoMessageRecvBuf), MESSAGE_SOCKETXUNFEI_RECVPARKNUM_MAX);
}

/**********************************************************************************************************
 @Function			void NET_SocketXunfei_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_SocketXunfei_Message_SendDataEnqueue	: 发送数据写入队列
 @Input				dataBuf	 		 					: 需写入数据
					dataLength							: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_SocketXunfei_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	netMessageFifoEnqueue(&NETSocketXunfeiFifoMessageSendPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			void NET_SocketXunfei_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_SocketXunfei_Message_RecvDataEnqueue	: 接收数据写入队列
 @Input				dataBuf	 		 					: 需写入数据
					dataLength							: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_SocketXunfei_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	netMessageFifoEnqueue(&NETSocketXunfeiFifoMessageRecvPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_SocketXunfei_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_SocketXunfei_Message_SendDataDequeue	: 发送数据读出队列
 @Input				dataBuf	 		 					: 需读出数据地址
					dataLength							: 需读出数据长度地址
 @Return				true									: 未空
					false								: 已空
**********************************************************************************************************/
bool NET_SocketXunfei_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	return netMessageFifoDequeue(&NETSocketXunfeiFifoMessageSendPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_SocketXunfei_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_SocketXunfei_Message_RecvDataDequeue	: 接收数据读出队列
 @Input				dataBuf	 		 					: 需读出数据地址
					dataLength							: 需读出数据长度地址
 @Return				true									: 未空
					false								: 已空
**********************************************************************************************************/
bool NET_SocketXunfei_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	return netMessageFifoDequeue(&NETSocketXunfeiFifoMessageRecvPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_SocketXunfei_Message_SendDataOffSet(void)
 @Description			NET_SocketXunfei_Message_SendDataOffSet		: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true									: 未空
					false								: 已空
**********************************************************************************************************/
bool NET_SocketXunfei_Message_SendDataOffSet(void)
{
	return netMessageFifoDiscard(&NETSocketXunfeiFifoMessageSendPark);
}

/**********************************************************************************************************
 @Function			bool NET_SocketXunfei_Message_RecvDataOffSet(void)
 @Description			NET_SocketXunfei_Message_RecvDataOffSet		: 接收数据队列(队列头偏移1)
 @Input				void
 @Return				true									: 未空
					false								: 已空
**********************************************************************************************************/
bool NET_SocketXunfei_Message_RecvDataOffSet(void)
{
	return netMessageFifoDiscard(&NETSocketXunfeiFifoMessageRecvPark);
}

/**********************************************************************************************************
 @Function			unsigned char NET_SocketXunfei_Message_SendDataRear(void)
 @Description			NET_SocketXunfei_Message_SendDataRear		: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_SocketXunfei_Message_SendDataRear(void)
{
	return netMessageFifoRear(&NETSocketXunfeiFifoMessageSendPark);
}

/**********************************************************************************************************
 @Function			unsigned char NET_SocketXunfei_Message_RecvDataRear(void)
 @Description			NET_SocketXunfei_Message_RecvDataRear		: 接收数据队尾值
 @Input				void
 @Return				接收数据队尾值
**********************************************************************************************************/
unsigned char NET_SocketXunfei_Message_RecvDataRear(void)
{
	return netMessageFifoRear(&NETSocketXunfeiFifoMessageRecvPark);
}

/********************************************** END OF FLEE **********************************************/
