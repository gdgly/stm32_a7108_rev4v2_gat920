/**
  *********************************************************************************************************
  * @file    lestcconfig.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   LESTC
  *********************************************************************************************************
  * @attention
  *		
  *			
  *********************************************************************************************************
  */

#include "lestcconfig.h"
#include "lestcfunc.h"
#include "string.h"

LestcPacketDataTypeDef		LestcPacketData;
u32 LestcSendTimeTick;

static void LestcUsartInit(void)
{
	GPIO_InitTypeDef GPIO_struct;
	USART_InitTypeDef USART_struct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO Config */
	/* Config USART1 TX ( PA.9 ) */
	GPIO_struct.GPIO_Pin = GPIO_Pin_9;
	GPIO_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_struct);
	/* Config USART1 RX ( PA.10 ) */
	GPIO_struct.GPIO_Pin = GPIO_Pin_10;
	GPIO_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_struct);
	/* Config USART1_RS485 DE ( PA.1 ) */
	GPIO_struct.GPIO_Pin = GPIO_Pin_1;
	GPIO_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_struct);
	
	/* USART MODE Config */
	USART_struct.USART_BaudRate = 9600;
	USART_struct.USART_WordLength = USART_WordLength_9b;
	USART_struct.USART_StopBits = USART_StopBits_1;
	USART_struct.USART_Parity = USART_Parity_Even;
	USART_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_struct);
	
	USART_Cmd(USART1, ENABLE);
}

/**********************************************************************************************************
 @Function			void LestcInitPackData(void)
 @Description			LestcInitPackData	: ≥ı ºªØ
 @Input				void
 @Return				void
**********************************************************************************************************/
void LestcInitPackData(void)
{
	memset((void*)&LestcPacketData, 0x0, sizeof(LestcPacketData));
	
	LestcPacketData.ProtocolHead[0] = 0xFE;
	LestcPacketData.ProtocolHead[1] = 0xFE;
	LestcPacketData.ProtocolHead[2] = 0xFE;
	LestcPacketData.ProtocolHead[3] = 0xFE;
	LestcPacketData.ProtocolHead[4] = 0xFE;
	
	LestcPacketData.MessageID = LestcMessageID;
	LestcPacketData.MessageLength = sizeof(LestcPacketData) - sizeof(LestcPacketData.ProtocolHead) - sizeof(LestcPacketData.CheckCode);
	
	LestcPacketData.PlateNum = LestcPlateNumber;
	
	LestcUsartInit();
}

/********************************************** END OF FLEE **********************************************/
