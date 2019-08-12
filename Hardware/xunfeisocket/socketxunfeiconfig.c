/**
  *********************************************************************************************************
  * @file    socketxunfeiconfig.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   例化各个外部函数接口
  *********************************************************************************************************
  * @attention
  *		config 		--> ok
  *		func   		--> ok
  *		initialization 
  *		instantia		
  *********************************************************************************************************
  */

#include "socketxunfeiconfig.h"
#include "socketxunfeifunc.h"
#include "socketxunfeiinstantia.h"
#include "socketxunfeiinitialization.h"
#include "socketxunfeimessage.h"

unsigned char Socket_Xunfei_SendBuf[SOCKET_XUNFEI_SEND_BUFFER_SIZE];
unsigned char Socket_Xunfei_RecvBuf[SOCKET_XUNFEI_RECV_BUFFER_SIZE];
unsigned char Socket_Xunfei_DataStack[SOCKET_XUNFEI_BUFFER_SIZW];

/**********************************************************************************************************
 @Function			void SOCKET_Xunfei_Client_Init(SOCKET_Xunfei_ClientsTypeDef* pClient)
 @Description			SOCKET_Xunfei_Client_Init				: 初始化Socket Xunfei客户端
 @Input				pClient								: Socket Xunfei客户端实例
 @Return				void
**********************************************************************************************************/
void SOCKET_Xunfei_Client_Init(SOCKET_Xunfei_ClientsTypeDef* pClient)
{
	pClient->Sendbuf										= Socket_Xunfei_SendBuf;
	pClient->Recvbuf										= Socket_Xunfei_RecvBuf;
	pClient->Sendbuf_size									= sizeof(Socket_Xunfei_SendBuf);
	pClient->Recvbuf_size									= sizeof(Socket_Xunfei_RecvBuf);
	pClient->Sendlen										= 0;
	pClient->Recvlen										= 0;
	pClient->DataProcessStack								= Socket_Xunfei_DataStack;
	pClient->DataProcessStack_size							= sizeof(Socket_Xunfei_DataStack);
	pClient->DataProcessStack_len								= 0;
	
	pClient->MsgId											= 1;
	
	SOCKET_Xunfei_Tr_Init(pClient);
	
	NET_SocketXunfei_FifoSendMessageInit();
	NET_SocketXunfei_FifoRecvMessageInit();
}

/********************************************** END OF FLEE **********************************************/
