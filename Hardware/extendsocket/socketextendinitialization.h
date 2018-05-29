#ifndef __SOCKET_EXTEND_INITIALIZATION_H
#define   __SOCKET_EXTEND_INITIALIZATION_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

extern volatile u16 SOCKET_EXTEND_RX_STA;									//接收数据状态
extern volatile u8  SOCKET_EXTEND_RTC_CHECK;									//RTC对时校验标志

void SOCKET_Extend_Init(void);											//初始化SocketExtend协议
void SOCKET_Extend_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate);			//初始化SocketExtend串口
void SOCKET_Extend_UARTx_IRQ(USART_TypeDef* USARTx);							//SocketExtend协议串口中断处理函数
void SOCKET_Extend_Implement(u16 sendtime);									//SocketExtend协议处理

void SOCKET_Extend_USARTSendByte(USART_TypeDef* USARTx, u8 data);
u32  SOCKET_Extend_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length);

#endif
