#ifndef __SOCKET_EXTEND_INITIALIZATION_H
#define   __SOCKET_EXTEND_INITIALIZATION_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

extern volatile u16 SOCKET_EXTEND_RX_STA;									//��������״̬
extern volatile u8  SOCKET_EXTEND_RTC_CHECK;									//RTC��ʱУ���־

void SOCKET_Extend_Init(void);											//��ʼ��SocketExtendЭ��
void SOCKET_Extend_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate);			//��ʼ��SocketExtend����
void SOCKET_Extend_UARTx_IRQ(USART_TypeDef* USARTx);							//SocketExtendЭ�鴮���жϴ�����
void SOCKET_Extend_Implement(u16 sendtime);									//SocketExtendЭ�鴦��

void SOCKET_Extend_USARTSendByte(USART_TypeDef* USARTx, u8 data);
u32  SOCKET_Extend_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length);

#endif
