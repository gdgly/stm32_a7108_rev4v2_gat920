/**
  *********************************************************************************************************
  * @file    socketxunfeiinitialization.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    SocketXunfei
  * @brief   SocketXunfei协议初始化
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *
  *********************************************************************************************************
  */

#include "socketxunfeiinitialization.h"
#include "socketxunfeiconfig.h"
#include "socketxunfeifunc.h"
#include "socketxunfeiinstantia.h"
#include "socketxunfeimessage.h"
#include "rtc.h"
#include "string.h"

#ifdef SOCKET_XUNFEI_SERIALPORT_USART1
#define SOCKET_XUNFEI_USART		USART1
#endif
#ifdef SOCKET_XUNFEI_SERIALPORT_USART2
#define SOCKET_XUNFEI_USART		USART2
#endif
#ifdef SOCKET_XUNFEI_SERIALPORT_USART3
#define SOCKET_XUNFEI_USART		USART3
#endif

extern GPIO_TypeDef* OUTPUT_TYPE[16];
extern u16 OUTPUT_PIN[16];
extern void Delay(vu32 nCount);
#define SOCKET_XUNFEI_RS485_Mode_RX()		GPIO_ResetBits( GPIOA, GPIO_Pin_1 )
#define SOCKET_XUNFEI_RS485_Mode_TX()		GPIO_SetBits( GPIOA, GPIO_Pin_1 )

#define SOCKETXUNFEILOGINENABLE											//登陆请求使能
#define SOCKETXUNFEILOGINCHECKLED1											//选择开启登陆请求LED1提示
#define SOCKETXUNFEIUSARTTXIRQn											//选择开启USART发送中断(是否使用中断发送或普通发送)

/** 通过判断接收连续2个字符之间的时间的时间差不大于timer来决定是不是一次连续的数据
  * 如果2个字符接收间隔超过timer,则认为不是1次连续数据.也就是超过timer没有接收到任何数据,则表示此次接收完毕
  * 接收到数据状态
  * [15]  : 0,没有接收到数据;  1,接收到了一批数据.
  * [14:0]: 接收到的数据长度
  */
volatile u16 SOCKET_XUNFEI_RX_STA = 0;										//接收数据状态
#ifdef SOCKETXUNFEILOGINENABLE
volatile u8  SOCKET_XUNFEI_LOGIN_CHECK = SOCKET_XUNFEI_LOGIN_REQUEST;				//Login登陆检查标志  0x01 未登陆
#else
volatile u8  SOCKET_XUNFEI_LOGIN_CHECK = SOCKET_XUNFEI_LOGIN_OVER;				//Login登陆检查标志  0x00 登陆
#endif

#ifdef SOCKETXUNFEIUSARTTXIRQn
u16 SocketXunfeiTxDataLength = 0;											//发送数据长度
volatile u16 SocketXunfeiTxCounter = 0;										//发送数据地址
#endif

/**********************************************************************************************************
 @Function			void SOCKET_Xunfei_Tr_Init(SOCKET_Xunfei_ClientsTypeDef* pClient)
 @Description			初始化SocketXunfei接口协议
 @Input				void
 @Return				void
**********************************************************************************************************/
void SOCKET_Xunfei_Tr_Init(SOCKET_Xunfei_ClientsTypeDef* pClient)
{
	SOCKET_Xunfei_PortSerialInit(pClient, SOCKET_XUNFEI_USART, 115200);			//初始化SocketXunfei协议串口
	SOCKET_XUNFEI_RX_STA = 0;											//未接收数据状态
	TIM_Cmd(TIM2, DISABLE);												//关闭定时器2
	
#ifdef SOCKETXUNFEILOGINENABLE
#ifdef SOCKETXUNFEILOGINCHECKLED1
	GPIO_SetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0]);
#endif
	SOCKET_XUNFEI_LOGIN_CHECK = SOCKET_XUNFEI_LOGIN_REQUEST;
#else
	SOCKET_XUNFEI_LOGIN_CHECK = SOCKET_XUNFEI_LOGIN_OVER;
#endif
}

/**********************************************************************************************************
 @Function			void SOCKET_Xunfei_UARTx_IRQ(SOCKET_Xunfei_ClientsTypeDef* pClient, USART_TypeDef* USARTx)
 @Description			SocketXunfei协议串口中断处理函数
 @Input				串口号
 @Return				void
**********************************************************************************************************/
void SOCKET_Xunfei_UARTx_IRQ(SOCKET_Xunfei_ClientsTypeDef* pClient, USART_TypeDef* USARTx)
{
	u8 recvivedata;
	
	if (USART_GetITStatus(USARTx, USART_IT_TXE)) {											//发送中断处理
#ifdef SOCKETXUNFEIUSARTTXIRQn
		USART_SendData(USARTx, (u8) pClient->Sendbuf[SocketXunfeiTxCounter++]);					//发送数据
		if (SocketXunfeiTxCounter == SocketXunfeiTxDataLength) {								//发送数据完成
			USART_ITConfig(SOCKET_XUNFEI_USART, USART_IT_TXE, DISABLE);							//关闭发送中断
			if (SOCKET_XUNFEI_USART == USART1) {											//485发送结束转为接收模式
				Delay(0x1fff);
				SOCKET_XUNFEI_RS485_Mode_RX();
			}
		}
#endif
		USART_ClearITPendingBit(USARTx, USART_IT_TXE);
	}
	else if (USART_GetITStatus(USARTx, USART_IT_RXNE)) {										//接收中断处理
		
		recvivedata = USART_ReceiveData(USARTx);
		if ((SOCKET_XUNFEI_RX_STA & (1<<15)) == 0)											//接收完一批数据,还未处理,则不再接收其他数据
		{
			if (SOCKET_XUNFEI_RX_STA < SOCKET_XUNFEI_RECV_BUFFER_SIZE)							//还可以接收数据
			{
				TIM_SetCounter(TIM2, 0);													//计数器清空
				if (SOCKET_XUNFEI_RX_STA == 0) {											//使能定时器2的中断
					TIM_Cmd(TIM2, ENABLE);												//使能定时器2
				}
				pClient->Recvbuf[SOCKET_XUNFEI_RX_STA++] = recvivedata;						//记录接收到的值
			}
			else {
				SOCKET_XUNFEI_RX_STA |= 1<<15;											//强制标记接收完成
			}
		}
		USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
	}
}

/**********************************************************************************************************
 @Function			void SOCKET_Xunfei_Implement(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 sendtime)
 @Description			SocketXunfei协议处理
 @Input				统计时长
 @Return				void
**********************************************************************************************************/
void SOCKET_Xunfei_Implement(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 sendtime)
{
	static u16 socketXunfeiSendTime = 0;													//统计时间计时器
	
	static u8  socketXunfeiRecvFlag = 0;													//等待接收标志
	static u8  socketXunfeiRecvTime = 0;													//等待接收计时器
	
	static u16 socketXunfeiSendLen  = 0;													//发送数据长度
	
	/* 1.采集数据填入缓存 */
	socketXunfeiSendTime += 1;
	if (socketXunfeiSendTime >= sendtime) {													//统计时间到达
		socketXunfeiSendTime = 0;														//统计时间清空
		SOCKET_Xunfei_GetFillData(pClient);												//获取各个车道数据
		for (int lanNum = 0; lanNum < OUTPUT_MAX; lanNum++) {									//循环例化各车道数据并填入缓存
			if (pClient->OutputID[lanNum] != 0) {
				memset((void*)pClient->DataProcessStack, 0x00, pClient->DataProcessStack_size);
				pClient->DataProcessStack_len = 0;
				SOCKET_Xunfei_Serialize_LenDataRequest(pClient, pClient->OutputID[lanNum], lanNum);
				NET_SocketXunfei_Message_SendDataEnqueue(pClient->DataProcessStack, pClient->DataProcessStack_len);
			}
		}
	}
	
	/* 2.登陆请求 */
	if (SOCKET_XUNFEI_LOGIN_CHECK == SOCKET_XUNFEI_LOGIN_REQUEST) {
		/* 2.1. 发送登陆请求 */
		if (socketXunfeiRecvFlag == 0) {
			SOCKET_XUNFEI_RX_STA = 0;													//未接收数据状态
			/* 例化获取登陆请求数据 */
			memset((void*)pClient->DataProcessStack, 0x00, pClient->DataProcessStack_size);
			pClient->DataProcessStack_len = 0;
			memset((void*)pClient->Sendbuf, 0x00, pClient->Sendbuf_size);
			pClient->Sendlen = 0;
			memset((void*)pClient->Recvbuf, 0x00, pClient->Recvbuf_size);
			pClient->Recvlen = 0;
			SOCKET_Xunfei_Serialize_LoginRequest(pClient, SOCKET_XUNFEI_LOGIN_ACCOUNT, SOCKET_XUNFEI_LOGIN_PASSWD);
			memcpy(pClient->Sendbuf, pClient->DataProcessStack, pClient->DataProcessStack_len);
			pClient->Sendlen = pClient->DataProcessStack_len;
			socketXunfeiSendLen = pClient->DataProcessStack_len;
			/* 数据发送 */
#ifdef SOCKETXUNFEIUSARTTXIRQn
			SocketXunfeiTxDataLength = socketXunfeiSendLen;									//获取发送数据长度
			SocketXunfeiTxCounter = 0;													//发送地址归0
			USART_ITConfig(SOCKET_XUNFEI_USART, USART_IT_TXE, ENABLE);							//开发送中断,并进入发送中断发送数据
#else
			SOCKET_Xunfei_USARTSend(SOCKET_XUNFEI_USART, pClient->Sendbuf, socketXunfeiSendLen);		//发送数据
#endif
			socketXunfeiRecvFlag = 1;
		}
		
		/* 2.2. 等待接收时间累加 */
		if (socketXunfeiRecvFlag) socketXunfeiRecvTime += 1;
		
		/* 2.3. 接收登陆请求 */
		if (socketXunfeiRecvTime >= SOCKET_XUNFEI_AGAINSENDTIME) {								//到达检测时间
			socketXunfeiRecvTime = 0;
			if (SOCKET_XUNFEI_RX_STA & 0X8000) {											//接收到期待应答结果
				/* 2.3.1 校验接收数据 */
				pClient->Recvlen = SOCKET_XUNFEI_RX_STA & 0x7FFF;
				if (SOCKET_Xunfei_Deserialize_LoginRequest(pClient, (char*)pClient->Recvbuf, pClient->Recvlen) == SocketXunfei_OK) {
					SOCKET_XUNFEI_LOGIN_CHECK = SOCKET_XUNFEI_LOGIN_OVER;						//已登陆
#ifdef SOCKETXUNFEILOGINCHECKLED1
					GPIO_ResetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0]);
#endif
					socketXunfeiRecvFlag = 0;
					socketXunfeiRecvTime = 0;
				}
				else {
					SOCKET_XUNFEI_RX_STA = 0;
					socketXunfeiRecvFlag = 0;
					socketXunfeiRecvTime = 0;
				}
				
			}
			else {																	//未接收到应答结果
				/* 2.3.2 重发数据 */
#ifdef SOCKETXUNFEIUSARTTXIRQn
				SocketXunfeiTxDataLength = socketXunfeiSendLen;								//获取发送数据长度
				SocketXunfeiTxCounter = 0;												//发送地址归0
				USART_ITConfig(SOCKET_XUNFEI_USART, USART_IT_TXE, ENABLE);						//开发送中断,并进入发送中断发送数据
#else
				SOCKET_Xunfei_USARTSend(SOCKET_XUNFEI_USART, pClient->Sendbuf, socketXunfeiSendLen);	//发送数据
#endif
				socketXunfeiRecvTime = 0;
			}
		}
		
		return;
	}
	
	/* 3.车道交通流量数据 */
	if (NET_SocketXunfei_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short*)&pClient->Sendlen) == true) {
		/* 3.1. 发送车道交通流量数据 */
		if (socketXunfeiRecvFlag == 0) {
			SOCKET_XUNFEI_RX_STA = 0;													//未接收数据状态
			/* 读取车道交通流量数据 */
			memset((void*)pClient->Recvbuf, 0x00, pClient->Recvbuf_size);
			pClient->Recvlen = 0;
			socketXunfeiSendLen = pClient->Sendlen;
			/* 数据发送 */
#ifdef SOCKETXUNFEIUSARTTXIRQn
			SocketXunfeiTxDataLength = socketXunfeiSendLen;									//获取发送数据长度
			SocketXunfeiTxCounter = 0;													//发送地址归0
			USART_ITConfig(SOCKET_XUNFEI_USART, USART_IT_TXE, ENABLE);							//开发送中断,并进入发送中断发送数据
#else
			SOCKET_Xunfei_USARTSend(SOCKET_XUNFEI_USART, pClient->Sendbuf, socketXunfeiSendLen);		//发送数据
#endif
			socketXunfeiRecvFlag = 1;
		}
		
		/* 3.2. 接收车道交通流量数据应答 */
		if (socketXunfeiRecvFlag > 0) {													//到达检测时间
			if (SOCKET_XUNFEI_RX_STA & 0X8000) {											//接收到期待应答结果
				/* 2.3.1 校验接收数据 */
				pClient->Recvlen = SOCKET_XUNFEI_RX_STA & 0x7FFF;
				if (SOCKET_Xunfei_Deserialize_LenDataRequest(pClient, (char*)pClient->Recvbuf, pClient->Recvlen) == SocketXunfei_OK) {
					NET_SocketXunfei_Message_SendDataOffSet();								//已送达
					socketXunfeiRecvFlag = 0;
				}
				else {
					SOCKET_XUNFEI_RX_STA = 0;
					socketXunfeiRecvFlag = 0;
				}
			}
			else {																	//未接收到应答结果
				/* 2.3.2 重发数据 */
#ifdef SOCKETXUNFEIUSARTTXIRQn
				SocketXunfeiTxDataLength = socketXunfeiSendLen;								//获取发送数据长度
				SocketXunfeiTxCounter = 0;												//发送地址归0
				USART_ITConfig(SOCKET_XUNFEI_USART, USART_IT_TXE, ENABLE);						//开发送中断,并进入发送中断发送数据
#else
				SOCKET_Xunfei_USARTSend(SOCKET_XUNFEI_USART, pClient->Sendbuf, socketXunfeiSendLen);	//发送数据
#endif
			}
		}
	}
}

/***************************************************************
 @Function			void SOCKET_Xunfei_USARTSendByte(USART_TypeDef* USARTx, u8 data)
 @Description			USART 
 @Input				data : 8bit
 @Return				void
***************************************************************/
void SOCKET_Xunfei_USARTSendByte(USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, (u8) data);
	
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length)
 @Description			Socket数据串口发送
 @Input				void
 @Return				void
**********************************************************************************************************/
u32 SOCKET_Xunfei_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length)
{
	u32 i = 0;
	
	if (USARTx == USART1) {
		SOCKET_XUNFEI_RS485_Mode_TX();
		Delay(0x1fff);
		while (length > 0) {
			SOCKET_Xunfei_USARTSendByte(USARTx, *data_arr++);
			i++;
			length--;
		}
		Delay(0x1fff);
		SOCKET_XUNFEI_RS485_Mode_RX();
	}
	else {
		while (length > 0) {
			SOCKET_Xunfei_USARTSendByte(USARTx, *data_arr++);
			i++;
			length--;
		}
	}
	
	return i;
}

/**********************************************************************************************************
 @Function			void SOCKET_Xunfei_PortSerialInit(SOCKET_Xunfei_ClientsTypeDef* pClient, USART_TypeDef* USARTx, u32 BaudRate)
 @Description			初始化SocketXunfei串口
 @Input				串口号
					波特率
 @Return				void
**********************************************************************************************************/
void SOCKET_Xunfei_PortSerialInit(SOCKET_Xunfei_ClientsTypeDef* pClient, USART_TypeDef* USARTx, u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef SOCKET_XUNFEI_SERIALPORT_USART1
	/* Enable  USART1 clocks */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif
#ifdef SOCKET_XUNFEI_SERIALPORT_USART2
  	/* Enable  USART2 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#endif
#ifdef SOCKET_XUNFEI_SERIALPORT_USART3
  	/* Enable  USART3 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#endif
	
	USART_DeInit(USARTx);
#ifdef SOCKET_XUNFEI_SERIALPORT_USART1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
#endif
#ifdef SOCKET_XUNFEI_SERIALPORT_USART2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
#endif
#ifdef SOCKET_XUNFEI_SERIALPORT_USART3
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
	
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
#ifdef SOCKET_XUNFEI_SERIALPORT_USART1
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
#endif
#ifdef SOCKET_XUNFEI_SERIALPORT_USART2
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
#endif
#ifdef SOCKET_XUNFEI_SERIALPORT_USART3
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
#endif
	
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);							//开启接收中断
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);							//关闭发送中断
}

/********************************************** END OF FLEE **********************************************/
