#ifndef __SOCKET_MODULATION_INITIALIZATION_H
#define   __SOCKET_MODULATION_INITIALIZATION_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

extern volatile u16 SOCKET_MODULATION_RX_STA;								//接收数据状态

void SOCKET_Modulation_Init(void);											//初始化SocketModulation协议
void SOCKET_Modulation_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate);		//初始化SocketModulation串口
void SOCKET_Modulation_UARTx_IRQ(USART_TypeDef* USARTx);						//SocketModulation协议串口中断处理函数
void SOCKET_Modulation_Implement(u16 sendtime);								//SocketModulation协议处理

void SOCKET_Modulation_USARTSendByte(USART_TypeDef* USARTx, u8 data);
u32  SOCKET_Modulation_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length);

#endif
