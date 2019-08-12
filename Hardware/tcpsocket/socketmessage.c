/**
  *********************************************************************************************************
  * @file    socketmessage.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socket消息队列
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "socketmessage.h"
#include "fifomessage.h"
#include "string.h"

#define MESSAGE_SOCKET_SENDPARKNUM_MAX			64
#define MESSAGE_SOCKET_RECVPARKNUM_MAX			20
#define MESSAGE_SOCKET_SENDPARKSIZE_MAX			2048
#define MESSAGE_SOCKET_RECVPARKSIZE_MAX			32

MessageFifoTypeDef		NETSocketFifoMessageSendPark;
MessageFifoTypeDef		NETSocketFifoMessageRecvPark;

unsigned char			SocketFifoMessageSendBuf[MESSAGE_SOCKET_SENDPARKSIZE_MAX];
unsigned char			SocketFifoMessageRecvBuf[MESSAGE_SOCKET_RECVPARKSIZE_MAX];

/**********************************************************************************************************
 @Function			void NET_Socket_FifoSendMessageInit(void)
 @Description			NET_Socket_FifoSendMessageInit		: 发送数据Fifo初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_Socket_FifoSendMessageInit(void)
{
	netMessageFifoInit(&NETSocketFifoMessageSendPark, SocketFifoMessageSendBuf, sizeof(SocketFifoMessageSendBuf), MESSAGE_SOCKET_SENDPARKNUM_MAX);
}

/**********************************************************************************************************
 @Function			void NET_Socket_FifoRecvMessageInit(void)
 @Description			NET_Socket_FifoRecvMessageInit		: 接收数据Fifo初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_Socket_FifoRecvMessageInit(void)
{
	netMessageFifoInit(&NETSocketFifoMessageRecvPark, SocketFifoMessageRecvBuf, sizeof(SocketFifoMessageRecvBuf), MESSAGE_SOCKET_RECVPARKNUM_MAX);
}

/**********************************************************************************************************
 @Function			void NET_Socket_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_Socket_Message_SendDataEnqueue		: 发送数据写入队列
 @Input				dataBuf	 		 				: 需写入数据
					dataLength						: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_Socket_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	netMessageFifoEnqueue(&NETSocketFifoMessageSendPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			void NET_Socket_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_Socket_Message_RecvDataEnqueue		: 接收数据写入队列
 @Input				dataBuf	 		 				: 需写入数据
					dataLength						: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_Socket_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	netMessageFifoEnqueue(&NETSocketFifoMessageRecvPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_Socket_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_Socket_Message_SendDataDequeue		: 发送数据读出队列
 @Input				dataBuf	 		 				: 需读出数据地址
					dataLength						: 需读出数据长度地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_Socket_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	return netMessageFifoDequeue(&NETSocketFifoMessageSendPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_Socket_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_Socket_Message_RecvDataDequeue		: 接收数据读出队列
 @Input				dataBuf	 		 				: 需读出数据地址
					dataLength						: 需读出数据长度地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_Socket_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	return netMessageFifoDequeue(&NETSocketFifoMessageRecvPark, dataBuf, dataLength);
}

/**********************************************************************************************************
 @Function			bool NET_Socket_Message_SendDataOffSet(void)
 @Description			NET_Socket_Message_SendDataOffSet		: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_Socket_Message_SendDataOffSet(void)
{
	return netMessageFifoDiscard(&NETSocketFifoMessageSendPark);
}

/**********************************************************************************************************
 @Function			bool NET_Socket_Message_RecvDataOffSet(void)
 @Description			NET_Socket_Message_RecvDataOffSet		: 接收数据队列(队列头偏移1)
 @Input				void
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_Socket_Message_RecvDataOffSet(void)
{
	return netMessageFifoDiscard(&NETSocketFifoMessageRecvPark);
}

/**********************************************************************************************************
 @Function			unsigned char NET_Socket_Message_SendDataRear(void)
 @Description			NET_Socket_Message_SendDataRear		: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_Socket_Message_SendDataRear(void)
{
	return netMessageFifoRear(&NETSocketFifoMessageSendPark);
}

/**********************************************************************************************************
 @Function			unsigned char NET_Socket_Message_RecvDataRear(void)
 @Description			NET_Socket_Message_RecvDataRear		: 接收数据队尾值
 @Input				void
 @Return				接收数据队尾值
**********************************************************************************************************/
unsigned char NET_Socket_Message_RecvDataRear(void)
{
	return netMessageFifoRear(&NETSocketFifoMessageRecvPark);
}

/**********************************************************************************************************
 @Function			int NET_Socket_Message_SendDataStatus(void)
 @Description			NET_Socket_Message_SendDataStatus		: 发送数据队列已用大小
 @Input				void
 @Return				发送数据队列已用大小
**********************************************************************************************************/
int NET_Socket_Message_SendDataStatus(void)
{
	return netMessageFifoStatus(&NETSocketFifoMessageSendPark);
}

/**********************************************************************************************************
 @Function			int NET_Socket_Message_RecvDataStatus(void)
 @Description			NET_Socket_Message_RecvDataStatus		: 接收数据队列已用大小
 @Input				void
 @Return				接收数据队列已用大小
**********************************************************************************************************/
int NET_Socket_Message_RecvDataStatus(void)
{
	return netMessageFifoStatus(&NETSocketFifoMessageRecvPark);
}

/**********************************************************************************************************
 @Function			int NET_Socket_Message_SendDataFree(void)
 @Description			NET_Socket_Message_SendDataFree		: 发送数据队列剩余大小
 @Input				void
 @Return				发送数据队列已用大小
**********************************************************************************************************/
int NET_Socket_Message_SendDataFree(void)
{
	return netMessageFifoFree(&NETSocketFifoMessageSendPark);
}

/**********************************************************************************************************
 @Function			int NET_Socket_Message_RecvDataFree(void)
 @Description			NET_Socket_Message_RecvDataFree		: 接收数据队列剩余大小
 @Input				void
 @Return				接收数据队列已用大小
**********************************************************************************************************/
int NET_Socket_Message_RecvDataFree(void)
{
	return netMessageFifoFree(&NETSocketFifoMessageRecvPark);
}

/********************************************** END OF FLEE **********************************************/
