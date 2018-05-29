#ifndef __SOCKET_MODULATION_INITIALIZATION_H
#define   __SOCKET_MODULATION_INITIALIZATION_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

extern volatile u16 SOCKET_MODULATION_RX_STA;								//��������״̬

void SOCKET_Modulation_Init(void);											//��ʼ��SocketModulationЭ��
void SOCKET_Modulation_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate);		//��ʼ��SocketModulation����
void SOCKET_Modulation_UARTx_IRQ(USART_TypeDef* USARTx);						//SocketModulationЭ�鴮���жϴ�����
void SOCKET_Modulation_Implement(u16 sendtime);								//SocketModulationЭ�鴦��

void SOCKET_Modulation_USARTSendByte(USART_TypeDef* USARTx, u8 data);
u32  SOCKET_Modulation_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length);

#endif
