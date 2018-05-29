/**
  *********************************************************************************************************
  * @file    socketmodulationinitialization.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   SocketModulationЭ���ʼ��
  *********************************************************************************************************
  * @attention
  *			����:
  *
  *			
  *********************************************************************************************************
  */

#include "socketmodulationinitialization.h"
#include "socketmodulationconfig.h"
#include "socketmodulationfunc.h"
#include "rtc.h"

#ifdef SOCKET_MODULATION_SERIALPORT_USART1
#define SOCKET_MODULATION_USART		USART1
#endif
#ifdef SOCKET_MODULATION_SERIALPORT_USART2
#define SOCKET_MODULATION_USART		USART2
#endif
#ifdef SOCKET_MODULATION_SERIALPORT_USART3
#define SOCKET_MODULATION_USART		USART3
#endif

extern GPIO_TypeDef* OUTPUT_TYPE[16];
extern u16 OUTPUT_PIN[16];
extern void Delay(vu32 nCount);
#define SOCKET_MODULATION_RS485_Mode_RX()		GPIO_ResetBits( GPIOA, GPIO_Pin_1 )
#define SOCKET_MODULATION_RS485_Mode_TX()		GPIO_SetBits( GPIOA, GPIO_Pin_1 )

#define SOCKETMODULATIONUSARTTXIRQn										//ѡ����USART�����ж�(�Ƿ�ʹ���жϷ��ͻ���ͨ����)

/**
  * ͨ���жϽ�������2���ַ�֮���ʱ������timer�������ǲ���һ������������
  * ���2���ַ����ռ������timer,����Ϊ����1����������.Ҳ���ǳ���timerû�н��յ��κ�����,���ʾ�˴ν������
  * ���յ�������״̬
  * [15]  :  0,û�н��յ�����;  1,���յ���һ������.
  * [14:0]:  ���յ������ݳ���
  */
volatile u16 SOCKET_MODULATION_RX_STA = 0;									//��������״̬

#ifdef SOCKETMODULATIONUSARTTXIRQn
u16 SocketModulationTxDataLangth = 0;										//�������ݳ���
volatile u16 SocketModulationTxCounter = 0;									//�������ݵ�ַ
#endif

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_Init(void)
 @Description			��ʼ��SocketModulationЭ��
 @Input				void
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_Init(void)
{
	socket_modulation_dev.PortSerialInit(SOCKET_MODULATION_USART, 115200);				//��ʼ��SocketExtendЭ�鴮��
	SOCKET_MODULATION_RX_STA = 0;													//δ��������״̬
	TIM_Cmd(TIM2, DISABLE);														//�رն�ʱ��2
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_UARTx_IRQ(USART_TypeDef* USARTx)
 @Description			SocketModulationЭ�鴮���жϴ�����
 @Input				���ں�
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_UARTx_IRQ(USART_TypeDef* USARTx)
{
	u8 recvivedata;
	
	if (USART_GetITStatus(USARTx, USART_IT_TXE)) {										//�����жϴ���
#ifdef SOCKETMODULATIONUSARTTXIRQn
		USART_SendData(USARTx, (u8) SocketModulationSendBuf[SocketModulationTxCounter++]);		//��������
		if (SocketModulationTxCounter == SocketModulationTxDataLangth) {						//�����������
			USART_ITConfig(SOCKET_MODULATION_USART, USART_IT_TXE, DISABLE);					//�رշ����ж�
			if (SOCKET_MODULATION_USART == USART1) {									//485���ͽ���תΪ����ģʽ
				Delay(0x1fff);
				SOCKET_MODULATION_RS485_Mode_RX();
			}
		}
#endif
		USART_ClearITPendingBit(USARTx, USART_IT_TXE);
	}
	else if (USART_GetITStatus(USARTx, USART_IT_RXNE)) {									//�����жϴ���
		
		recvivedata = USART_ReceiveData(USARTx);
		if ((SOCKET_MODULATION_RX_STA & (1<<15)) == 0) 									//�������һ������,��û�б�����,���ٽ�����������
		{
			if (SOCKET_MODULATION_RX_STA < SOCKET_MODULATION_RECVIVE_SIZE)					//�����Խ�������
			{
				TIM_SetCounter(TIM2, 0);												//���������
				if (SOCKET_MODULATION_RX_STA == 0) {									//ʹ�ܶ�ʱ��2���ж�
					TIM_Cmd(TIM2, ENABLE);											//ʹ�ܶ�ʱ��2
				}
				SocketModulationReceiveBuf[SOCKET_MODULATION_RX_STA++] = recvivedata;			//��¼���յ���ֵ
			}
			else {
				SOCKET_MODULATION_RX_STA |= 1<<15;										//ǿ�Ʊ�ǽ������
			}
		}
		USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
	}
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_Implement(u16 sendtime)
 @Description			SocketModulationЭ�鴦��
 @Input				ͳ��ʱ��
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_Implement(u16 sendtime)
{
	static u16 socketModulationsendtime = 0;										//socket����ʱ���ʱ��
	static u16 socketModulationsendlength = 0;										//socket�������ݳ���
	static u8 socketModulationreceiveflag = 0;
	static u16 socketModulationreceivesendtime = 0;
	static u8 socketModulationsendnum = 0;
	
	socketModulationsendtime += 1;
	if ((socketModulationsendtime >= sendtime) && (socketModulationreceiveflag == 0)) {		//���﷢��ʱ��
		socketModulationsendtime = 0;												//��ʱ����0
		SOCKET_MODULATION_RX_STA = 0;												//δ��������״̬
		socketModulationsendlength = socket_modulation_dev.PackagedStorage();				//�����ݴ�����뻺����
#ifdef SOCKETMODULATIONUSARTTXIRQn
		SocketModulationTxDataLangth = socketModulationsendlength;						//��ȡ�������ݳ���
		SocketModulationTxCounter = 0;											//���͵�ַ��0
		if (SOCKET_MODULATION_USART == USART1) {									//485����
			SOCKET_MODULATION_RS485_Mode_TX();
			Delay(0x1fff);
		}
		USART_ITConfig(SOCKET_MODULATION_USART, USART_IT_TXE, ENABLE);					//�������ж�,�����뷢���жϷ�������
#else
		SOCKET_Modulation_USARTSend(SOCKET_MODULATION_USART, (u8 *)SocketModulationSendBuf, socketModulationsendlength);	//��������
#endif
		socketModulationreceiveflag = 1;
	}
	
	if (socketModulationreceiveflag == 1) {
		socketModulationreceivesendtime += 1;
	}
	
	if (socketModulationreceivesendtime >= SOCKET_MODULATION_AGAINSENDTIME) {				//������ʱ��
		socketModulationreceivesendtime = 0;
		if (SOCKET_MODULATION_RX_STA & 0X8000) {									//���յ��ڴ���Ӧ����
			if (SocketModulationReceiveBuf[4] == SOCKET_MODULATION_PACKET_TYPE_FLOWMESSAGE_ACK) {
				socketModulationsendnum = 0;
				socketModulationreceiveflag = 0;
				socketModulationreceivesendtime = 0;
			}
			else {
				SOCKET_MODULATION_RX_STA = 0;
			}
		}
		else {
#ifdef SOCKETMODULATIONUSARTTXIRQn
			SocketModulationTxDataLangth = socketModulationsendlength;					//��ȡ�������ݳ���
			SocketModulationTxCounter = 0;										//���͵�ַ��0
			if (SOCKET_MODULATION_USART == USART1) {								//485����
				SOCKET_MODULATION_RS485_Mode_TX();
				Delay(0x1fff);
			}
			USART_ITConfig(SOCKET_MODULATION_USART, USART_IT_TXE, ENABLE);				//�������ж�,�����뷢���жϷ�������
#else
			SOCKET_Modulation_USARTSend(SOCKET_MODULATION_USART, (u8 *)SocketModulationSendBuf, socketModulationsendlength);	//��������
#endif
			socketModulationsendnum += 1;
			if (socketModulationsendnum >= 3) {
				socketModulationsendnum = 0;
				socketModulationreceiveflag = 0;
				socketModulationreceivesendtime = 0;
			}
		}
	}
}

/***************************************************************
 @Function			void SOCKET_Modulation_USARTSendByte(USART_TypeDef* USARTx, u8 data)
 @Description			USART 
 @Input				data : 8bit
 @Return				void
***************************************************************/
void SOCKET_Modulation_USARTSendByte(USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, (u8) data);
	
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Modulation_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length)
 @Description			Socket���ݴ��ڷ���
 @Input				ͳ��ʱ��
 @Return				void
**********************************************************************************************************/
u32 SOCKET_Modulation_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length)
{
	u32 i = 0;
	
	if (USARTx == USART1) {
		SOCKET_MODULATION_RS485_Mode_TX();
		Delay(0x1fff);
		while (length > 0) {
			SOCKET_Modulation_USARTSendByte(USARTx, *data_arr++);
			i++;
			length--;
		}
		Delay(0x1fff);
		SOCKET_MODULATION_RS485_Mode_RX();
	}
	else {
		while (length > 0) {
			SOCKET_Modulation_USARTSendByte(USARTx, *data_arr++);
			i++;
			length--;
		}
	}
	
	return i;
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate)
 @Description			��ʼ��SocketModulation����
 @Input				���ں�
					������
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef SOCKET_MODULATION_SERIALPORT_USART1
	/* Enable  USART1 clocks */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif
#ifdef SOCKET_MODULATION_SERIALPORT_USART2
  	/* Enable  USART2 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#endif
#ifdef SOCKET_MODULATION_SERIALPORT_USART3  
  	/* Enable  USART3 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#endif
	
	USART_DeInit(USARTx);
#ifdef SOCKET_MODULATION_SERIALPORT_USART1
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
#ifdef SOCKET_MODULATION_SERIALPORT_USART2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
#endif
#ifdef SOCKET_MODULATION_SERIALPORT_USART3
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
	
#ifdef SOCKET_MODULATION_SERIALPORT_USART1
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
#endif
#ifdef SOCKET_MODULATION_SERIALPORT_USART2
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
#endif
#ifdef SOCKET_MODULATION_SERIALPORT_USART3
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
#endif

	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);							//���������ж�
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);							//�رշ����ж�
}

/********************************************** END OF FLEE **********************************************/
