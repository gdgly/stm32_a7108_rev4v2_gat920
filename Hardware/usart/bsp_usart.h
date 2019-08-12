#ifndef __BSP_USART_H
#define   __BSP_USART_H

#include "stm32f10x_lib.h"
#include "string.h"
#include "stdint.h"
#include <stdio.h>

/* ѡ�񴮿���Ϊ printf ���Ͷ˿� */
#define PRINTF_USART	USART1
//#define PRINTF_USART	USART2
//#define PRINTF_USART	USART3

#define RS485_Mode_RX()		GPIO_ResetBits( GPIOA, GPIO_Pin_1 )
#define RS485_Mode_TX()		GPIO_SetBits( GPIOA, GPIO_Pin_1 )

void USART_Config(USART_TypeDef* USARTx, uint32_t BaudRate);					//��������
void USART_SendByte(USART_TypeDef* USARTx, uint8_t data);						//���ڷ���1Byte
u32  USART_Send(USART_TypeDef* USARTx, u8 *data_arr, u32 length);				//���ڷ���
void UARTx_IRQn_Interrupt(USART_TypeDef* USARTx);								//�����жϴ���

#endif /* BSP_USART_H */
