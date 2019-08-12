#ifndef __SOCKET_XUNFEI_INITIALIZATION_H
#define   __SOCKET_XUNFEI_INITIALIZATION_H

#include "stm32f10x_lib.h"
#include "socketxunfeiconfig.h"

extern volatile u16 SOCKET_XUNFEI_RX_STA;															//接收数据状态
extern volatile u8  SOCKET_XUNFEI_LOGIN_CHECK;														//Login登陆检查标志

void SOCKET_Xunfei_Tr_Init(SOCKET_Xunfei_ClientsTypeDef* pClient);										//初始化SocketXunfei接口协议
void SOCKET_Xunfei_PortSerialInit(SOCKET_Xunfei_ClientsTypeDef* pClient, USART_TypeDef* USARTx, u32 BaudRate);	//初始化SocketXunfei串口
void SOCKET_Xunfei_UARTx_IRQ(SOCKET_Xunfei_ClientsTypeDef* pClient, USART_TypeDef* USARTx);					//SocketXunfei串口中断处理函数
void SOCKET_Xunfei_Implement(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 sendtime);							//SocketXunfei协议处理

void SOCKET_Xunfei_USARTSendByte(USART_TypeDef* USARTx, u8 data);
u32  SOCKET_Xunfei_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length);

#endif
