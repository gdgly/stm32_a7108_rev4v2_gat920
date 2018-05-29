/**
  *********************************************************************************************************
  * @file    socketinitialization.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   SocketЭ���ʼ��
  *********************************************************************************************************
  * @attention
  *			����:
  *			1. ��ʼ��SocketЭ��
  *			2. ��ʼ��Socket����
  *			3. SocketЭ�鴮���жϴ�����
  *			4. SocketЭ�鴦��
  *			
  *********************************************************************************************************
  */

#include "socketinitialization.h"
#include "socketconfig.h"
#include "socketfunc.h"

#ifdef SOCKET_SERIALPORT_USART1
#define SOCKET_USART		USART1
#endif
#ifdef SOCKET_SERIALPORT_USART2
#define SOCKET_USART		USART2
#endif
#ifdef SOCKET_SERIALPORT_USART3
#define SOCKET_USART		USART3
#endif

extern GPIO_TypeDef* OUTPUT_TYPE[16];
extern u16 OUTPUT_PIN[16];
extern void Delay(vu32 nCount);
#define RS485_Mode_RX()		GPIO_ResetBits( GPIOA, GPIO_Pin_1 )
#define RS485_Mode_TX()		GPIO_SetBits( GPIOA, GPIO_Pin_1 )

#define SOCKETRTCCHECKENABLE												//RTC��ʱʹ��
#define SOCKETRTCCHECKLED1												//ѡ����RTC��ʱLED1��ʾ
#define SOCKETUSARTTXIRQn												//ѡ����USART�����ж�(�Ƿ�ʹ���жϷ��ͻ���ͨ����)

/**
  * ͨ���жϽ�������2���ַ�֮���ʱ������timer�������ǲ���һ������������
  * ���2���ַ����ռ������timer,����Ϊ����1����������.Ҳ���ǳ���timerû�н��յ��κ�����,���ʾ�˴ν������
  * ���յ�������״̬
  * [15]  :  0,û�н��յ�����;  1,���յ���һ������.
  * [14:0]:  ���յ������ݳ���
  */
volatile u16 SOCKET_RX_STA = 0;											//��������״̬
#ifdef SOCKETRTCCHECKENABLE
volatile u8  SOCKET_RTC_CHECK = PACKETTYPE_RTCCHECKINIT;						//RTC��ʱУ���־  0x80�ϵ��ʱ 0x81���ж�ʱ 0x01����ʱ
#else
volatile u8  SOCKET_RTC_CHECK = PACKETTYPE_FLOWMESSAGE;
#endif

#ifdef SOCKETUSARTTXIRQn
u16 SocketTxDataLangth = 0;												//�������ݳ���
volatile u16 TxCounter = 0;												//�������ݵ�ַ
#endif

/**********************************************************************************************************
 @Function			void SOCKET_Init(void)
 @Description			��ʼ��SocketЭ��
 @Input				void
 @Return				void
**********************************************************************************************************/
void SOCKET_Init(void)
{
	socket_dev.PortSerialInit(SOCKET_USART, 115200);							//��ʼ��SocketЭ�鴮��
	SOCKET_RX_STA = 0;													//δ��������״̬
	TIM_Cmd(TIM2, DISABLE);												//�رն�ʱ��2
#ifdef SOCKETRTCCHECKLED1
	if (PlatformSockettime == SocketTime_ENABLE) {
		GPIO_SetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0]);
	}
#endif
}

/**********************************************************************************************************
 @Function			void SOCKET_UARTx_IRQ(USART_TypeDef* USARTx)
 @Description			SocketЭ�鴮���жϴ�����
 @Input				���ں�
 @Return				void
**********************************************************************************************************/
void SOCKET_UARTx_IRQ(USART_TypeDef* USARTx)
{
	u8 recvivedata;
	
	if (USART_GetITStatus(USARTx, USART_IT_TXE)) {							//�����жϴ���
#ifdef SOCKETUSARTTXIRQn
		USART_SendData(USARTx, (u8) SocketSendBuf[TxCounter++]);				//��������
		if (TxCounter == SocketTxDataLangth) {								//�����������
			USART_ITConfig(SOCKET_USART, USART_IT_TXE, DISABLE);				//�رշ����ж�
			if (SOCKET_USART == USART1) {									//485���ͽ���תΪ����ģʽ
				Delay(0x1fff);
				RS485_Mode_RX();
			}
		}
#endif
		USART_ClearITPendingBit(USARTx, USART_IT_TXE);
	}
	else if (USART_GetITStatus(USARTx, USART_IT_RXNE)) {						//�����жϴ���
		
		recvivedata = USART_ReceiveData(USARTx);
		if ((SOCKET_RX_STA & (1<<15)) == 0) 								//�������һ������,��û�б�����,���ٽ�����������
		{
			if (SOCKET_RX_STA < SOCKET_RECVIVE_SIZE)						//�����Խ�������
			{
				TIM_SetCounter(TIM2, 0);									//���������
				if (SOCKET_RX_STA == 0) {								//ʹ�ܶ�ʱ��2���ж�
					TIM_Cmd(TIM2, ENABLE);								//ʹ�ܶ�ʱ��2
				}
				SocketReceiveBuf[SOCKET_RX_STA++] = recvivedata;				//��¼���յ���ֵ
			}
			else {
				SOCKET_RX_STA |= 1<<15;									//ǿ�Ʊ�ǽ������
			}
		}
		USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
	}
}

/**********************************************************************************************************
 @Function			void SOCKET_Implement(u16 sendtime)
 @Description			SocketЭ�鴦��
 @Input				ͳ��ʱ��
 @Return				void
**********************************************************************************************************/
void SOCKET_Implement(u16 sendtime)
{
	static u16 socketsendtime = 0;										//socket����ʱ���ʱ��
	static u16 socketsendlength = 0;										//socket�������ݳ���
	static u8 receiveflag = 0;
	static u16 receivesendtime = 0;
	static u8 sendnum = 0;
	u32 rtctime = 0;
	
	socketsendtime += 1;
	if ((socketsendtime >= sendtime) && (receiveflag == 0)) {					//���﷢��ʱ��
		socketsendtime = 0;												//��ʱ����0
		SOCKET_RX_STA = 0;												//δ��������״̬
		socketsendlength = socket_dev.PackagedStorage();						//�����ݴ�����뻺����
#ifdef SOCKETUSARTTXIRQn
		SocketTxDataLangth = socketsendlength;								//��ȡ�������ݳ���
		TxCounter = 0;													//���͵�ַ��0
		if (SOCKET_USART == USART1) {										//485����
			RS485_Mode_TX();
			Delay(0x1fff);
		}
		USART_ITConfig(SOCKET_USART, USART_IT_TXE, ENABLE);					//�������ж�,�����뷢���жϷ�������
#else
		SOCKET_USARTSend(SOCKET_USART, (u8 *)SocketSendBuf, socketsendlength);	//��������
#endif
		receiveflag = 1;
	}
	if (receiveflag == 1) {
		receivesendtime += 1;
	}
	if (receivesendtime >= SOCKET_AGAINSENDTIME) {							//������ʱ��
		receivesendtime = 0;
		if (SOCKET_RX_STA & 0X8000) {										//���յ��ڴ���Ӧ����
			if ((SocketReceiveBuf[8] == PACKETTYPE_RTCCHECK) || 
			    (SocketReceiveBuf[8] == PACKETTYPE_RTCCHECKINIT)) {			//���ܵ���ʱ����
				rtctime |= SocketReceiveBuf[13] << 24;
				rtctime |= SocketReceiveBuf[14] << 16;
				rtctime |= SocketReceiveBuf[15] << 8;
				rtctime |= SocketReceiveBuf[16];
				rtctime += (8 * 3600);									//��ȡʱ������
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
				PWR_BackupAccessCmd(ENABLE);
				RTC_WaitForLastTask();
				RTC_SetCounter(rtctime);									//д��RTC�Ĵ���ֵ
				RTC_WaitForLastTask();
				SOCKET_RTC_CHECK = 0;									//��ʱ������0
#ifdef SOCKETRTCCHECKLED1
				if (PlatformSockettime == SocketTime_ENABLE) {
					GPIO_ResetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0]);
				}
#endif
			}
			if ((SocketReceiveBuf[8] == PACKETTYPE_RTCCHECK) ||				//����������ȷ
			    (SocketReceiveBuf[8] == PACKETTYPE_RTCCHECKINIT) ||
			    (SocketReceiveBuf[8] == PACKETTYPE_FLOWMESSAGE)) {
				sendnum = 0;
				receiveflag = 0;
				receivesendtime = 0;
			}
			else {
				SOCKET_RX_STA = 0;
			}
		}
		else {
#ifdef SOCKETUSARTTXIRQn
			SocketTxDataLangth = socketsendlength;							//��ȡ�������ݳ���
			TxCounter = 0;												//���͵�ַ��0
			if (SOCKET_USART == USART1) {									//485����
				RS485_Mode_TX();
				Delay(0x1fff);
			}
			USART_ITConfig(SOCKET_USART, USART_IT_TXE, ENABLE);				//�������ж�,�����뷢���жϷ�������
#else
			SOCKET_USARTSend(SOCKET_USART, (u8 *)SocketSendBuf, socketsendlength);//��������
#endif
			sendnum += 1;
			if (sendnum >= 3) {
				sendnum = 0;
				receiveflag = 0;
				receivesendtime = 0;
			}
		}
	}

	if (PlatformSockettime == SocketTime_ENABLE) {
#ifdef SOCKETRTCCHECKENABLE
		rtctime = RTC_GetCounter();										//��ȡ��ǰʱ��ֵ
		if ((rtctime % 86400) == 0) {										//�ж��Ƿ񵽴��賿0��
			SOCKET_RTC_CHECK = PACKETTYPE_RTCCHECK;							//����RTC���ж�ʱ��0x81
#ifdef SOCKETRTCCHECKLED1
			GPIO_SetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0]);
#endif
		}
#endif
	}
}

/***************************************************************
 @Function			void USART_SendByte(USART_TypeDef* USARTx, uint8_t data)
 @Description			USART 
 @Input				data : 8bit
 @Return				void
***************************************************************/
void SOCKET_USARTSendByte(USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, (u8) data);
	
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/**********************************************************************************************************
 @Function			u32 SOCKET_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length)
 @Description			Socket���ݴ��ڷ���
 @Input				ͳ��ʱ��
 @Return				void
**********************************************************************************************************/
u32 SOCKET_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length)
{
	u32 i = 0;
	
	if (USARTx == USART1) {
		RS485_Mode_TX();
		Delay(0x1fff);
		while (length > 0) {
			SOCKET_USARTSendByte(USARTx, *data_arr++);
			i++;
			length--;
		}
		Delay(0x1fff);
		RS485_Mode_RX();
	}
	else {
		while (length > 0) {
			SOCKET_USARTSendByte(USARTx, *data_arr++);
			i++;
			length--;
		}
	}
	
	return i;
}

/**********************************************************************************************************
 @Function			void SOCKET_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate)
 @Description			��ʼ��Socket����
 @Input				���ں�
					������
 @Return				void
**********************************************************************************************************/
void SOCKET_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef SOCKET_SERIALPORT_USART1
	/* Enable  USART1 clocks */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif
#ifdef SOCKET_SERIALPORT_USART2
  	/* Enable  USART2 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#endif
#ifdef SOCKET_SERIALPORT_USART3  
  	/* Enable  USART3 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#endif
	
	USART_DeInit(USARTx);
#ifdef SOCKET_SERIALPORT_USART1
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
#ifdef SOCKET_SERIALPORT_USART2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
#endif
#ifdef SOCKET_SERIALPORT_USART3
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
	
#ifdef SOCKET_SERIALPORT_USART1
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
#endif
#ifdef SOCKET_SERIALPORT_USART2
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
#endif
#ifdef SOCKET_SERIALPORT_USART3
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
#endif

	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);							//���������ж�
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);							//�رշ����ж�
}

/********************************************** END OF FLEE **********************************************/
