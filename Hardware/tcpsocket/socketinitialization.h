#ifndef __SOCKET_INITIALIZATION_H
#define   __SOCKET_INITIALIZATION_H

#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "stdbool.h"

extern volatile u16 SOCKET_RX_STA;											//��������״̬
extern volatile u8  SOCKET_RTC_CHECK;										//RTC��ʱУ���־
extern volatile u8  SOCKET_PARK_EVENT;

#if SDIO_SDCARD_TYPE
extern volatile bool SOCKET_ONLINE;
#endif

void SOCKET_Init(void);													//��ʼ��SocketЭ��
void SOCKET_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate);					//��ʼ��Socket����
void SOCKET_UARTx_IRQ(USART_TypeDef* USARTx);								//SocketЭ�鴮���жϴ�����
void SOCKET_Implement(u16 sendtime);										//SocketЭ�鴦��
void SOCKET_Park_Implement(void);											//SocketЭ�鴦��
void SOCKET_Park_ImplementEvent(void);										//SocketЭ�鴦��

void SOCKET_USARTSendByte(USART_TypeDef* USARTx, u8 data);
u32  SOCKET_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length);


#endif
