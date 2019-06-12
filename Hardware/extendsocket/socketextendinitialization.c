/**
  *********************************************************************************************************
  * @file    socketextendinitialization.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   SocketExtendЭ���ʼ��
  *********************************************************************************************************
  * @attention
  *			����:
  *			1. ��ʼ��SocketExtendЭ��
  *			2. ��ʼ��SocketExtend����
  *			3. SocketExtendЭ�鴮���жϴ�����
  *			3. SocketExtendЭ�鴦��
  *
  *********************************************************************************************************
  */

#include "socketextendinitialization.h"
#include "socketextendconfig.h"
#include "socketextendfunc.h"
#include "rtc.h"

#ifdef SOCKET_EXTEND_SERIALPORT_USART1
#define SOCKET_EXTEND_USART		USART1
#endif
#ifdef SOCKET_EXTEND_SERIALPORT_USART2
#define SOCKET_EXTEND_USART		USART2
#endif
#ifdef SOCKET_EXTEND_SERIALPORT_USART3
#define SOCKET_EXTEND_USART		USART3
#endif

extern GPIO_TypeDef* OUTPUT_TYPE[16];
extern u16 OUTPUT_PIN[16];
extern void Delay(vu32 nCount);
#define SOCKET_EXTEND_RS485_Mode_RX()		GPIO_ResetBits( GPIOA, GPIO_Pin_1 )
#define SOCKET_EXTEND_RS485_Mode_TX()		GPIO_SetBits( GPIOA, GPIO_Pin_1 )

#define SOCKETEXTENDRTCCHECKENABLE											//RTC��ʱʹ��
#define SOCKETEXTENDRTCCHECKLED1											//ѡ����RTC��ʱLED1��ʾ
#define SOCKETEXTENDUSARTTXIRQn											//ѡ����USART�����ж�(�Ƿ�ʹ���жϷ��ͻ���ͨ����)

/**
  * ͨ���жϽ�������2���ַ�֮���ʱ������timer�������ǲ���һ������������
  * ���2���ַ����ռ������timer,����Ϊ����1����������.Ҳ���ǳ���timerû�н��յ��κ�����,���ʾ�˴ν������
  * ���յ�������״̬
  * [15]  :  0,û�н��յ�����;  1,���յ���һ������.
  * [14:0]:  ���յ������ݳ���
  */
volatile u16 SOCKET_EXTEND_RX_STA = 0;										//��������״̬
#ifdef SOCKETEXTENDRTCCHECKENABLE
volatile u8  SOCKET_EXTEND_RTC_CHECK = SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT;		//RTC��ʱУ���־  0x04��ʱ
#else
volatile u8  SOCKET_EXTEND_RTC_CHECK = SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE;		//RTC��ʱУ���־  0x01����ʱ
#endif

#ifdef SOCKETEXTENDUSARTTXIRQn
u16 SocketExtendTxDataLangth = 0;											//�������ݳ���
volatile u16 SocketExtendTxCounter = 0;										//�������ݵ�ַ
#endif

/**********************************************************************************************************
 @Function			void SOCKET_Extend_Init(void)
 @Description			��ʼ��Socket_ExtendЭ��
 @Input				void
 @Return				void
**********************************************************************************************************/
void SOCKET_Extend_Init(void)
{
	socket_extend_dev.PortSerialInit(SOCKET_EXTEND_USART, 115200);				//��ʼ��SocketExtendЭ�鴮��
	SOCKET_EXTEND_RX_STA = 0;											//δ��������״̬
	TIM_Cmd(TIM2, DISABLE);												//�رն�ʱ��2

	if (PlatformSocketExtendtime == SocketExtendTime_ENABLE) {
#ifdef SOCKETEXTENDRTCCHECKLED1
		GPIO_SetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0]);
#endif
		SOCKET_EXTEND_RTC_CHECK = SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT;
	}
	else {
		SOCKET_EXTEND_RTC_CHECK = SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE;
	}
}

/**********************************************************************************************************
 @Function			void SOCKET_Extend_UARTx_IRQ(USART_TypeDef* USARTx)
 @Description			SocketExtendЭ�鴮���жϴ�����
 @Input				���ں�
 @Return				void
**********************************************************************************************************/
void SOCKET_Extend_UARTx_IRQ(USART_TypeDef* USARTx)
{
	u8 recvivedata;
	
	if (USART_GetITStatus(USARTx, USART_IT_TXE)) {							//�����жϴ���
#ifdef SOCKETEXTENDUSARTTXIRQn
		USART_SendData(USARTx, (u8) SocketExtendSendBuf[SocketExtendTxCounter++]);	//��������
		if (SocketExtendTxCounter == SocketExtendTxDataLangth) {				//�����������
			USART_ITConfig(SOCKET_EXTEND_USART, USART_IT_TXE, DISABLE);			//�رշ����ж�
			if (SOCKET_EXTEND_USART == USART1) {							//485���ͽ���תΪ����ģʽ
				Delay(0x1fff);
				SOCKET_EXTEND_RS485_Mode_RX();
			}
		}
#endif
		USART_ClearITPendingBit(USARTx, USART_IT_TXE);
	}
	else if (USART_GetITStatus(USARTx, USART_IT_RXNE)) {						//�����жϴ���
		
		recvivedata = USART_ReceiveData(USARTx);
		if ((SOCKET_EXTEND_RX_STA & (1<<15)) == 0) 							//�������һ������,��û�б�����,���ٽ�����������
		{
			if (SOCKET_EXTEND_RX_STA < SOCKET_EXTEND_RECVIVE_SIZE)				//�����Խ�������
			{
				TIM_SetCounter(TIM2, 0);									//���������
				if (SOCKET_EXTEND_RX_STA == 0) {							//ʹ�ܶ�ʱ��2���ж�
					TIM_Cmd(TIM2, ENABLE);								//ʹ�ܶ�ʱ��2
				}
				SocketExtendReceiveBuf[SOCKET_EXTEND_RX_STA++] = recvivedata;	//��¼���յ���ֵ
			}
			else {
				SOCKET_EXTEND_RX_STA |= 1<<15;							//ǿ�Ʊ�ǽ������
			}
		}
		USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
	}
}

/**********************************************************************************************************
 @Function			void SOCKET_Extend_Implement(u16 sendtime)
 @Description			SocketExtendЭ�鴦��
 @Input				ͳ��ʱ��
 @Return				void
**********************************************************************************************************/
void SOCKET_Extend_Implement(u16 sendtime)
{
	static u16 socketExtendsendtime = 0;									//socketExtend����ʱ���ʱ��
	static u16 socketExtendsendlength = 0;									//socketExtend�������ݳ���
	static u8 socketExtendreceiveflag = 0;
	static u16 socketExtendreceivesendtime = 0;
	static u8 socketExtendsendnum = 0;
	u32 rtctime = 0;
	
	socketExtendsendtime += 1;
	if ((socketExtendsendtime >= sendtime) && (socketExtendreceiveflag == 0)) {	//���﷢��ʱ��
		socketExtendsendtime = 0;										//��ʱ����0
		SOCKET_EXTEND_RX_STA = 0;										//δ��������״̬
		/* �������뻺�� */
#ifdef SOCKETEXTENDRTCCHECKENABLE
		if (PlatformSocketExtendtime == SocketExtendTime_ENABLE) {
			/* ��δ�Ժ�ʱ�� */
			if (SOCKET_EXTEND_RTC_CHECK == SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT) {
				socketExtendsendlength = socket_extend_dev.RtcCheckPackagedStorage();		//�����ݴ�����뻺����(��ʱ��)
			}
			/* �Ժ�ʱ�� */
			else {
				socketExtendsendlength = socket_extend_dev.PackagedStorage();			//�����ݴ�����뻺����(������)
			}
		}
		else {
			socketExtendsendlength = socket_extend_dev.PackagedStorage();				//�����ݴ�����뻺����(������)
		}
#else
		socketExtendsendlength = socket_extend_dev.PackagedStorage();					//�����ݴ�����뻺����(������)
#endif
		/* ���ݷ��� */
#ifdef SOCKETEXTENDUSARTTXIRQn
		SocketExtendTxDataLangth = socketExtendsendlength;							//��ȡ�������ݳ���
		SocketExtendTxCounter = 0;												//���͵�ַ��0
		if (SOCKET_EXTEND_USART == USART1) {										//485����
			SOCKET_EXTEND_RS485_Mode_TX();
			Delay(0x1fff);
		}
		USART_ITConfig(SOCKET_EXTEND_USART, USART_IT_TXE, ENABLE);									//�������ж�,�����뷢���жϷ�������
#else
		SOCKET_Extend_USARTSend(SOCKET_EXTEND_USART, (u8 *)SocketExtendSendBuf, socketExtendsendlength);	//��������
#endif
		socketExtendreceiveflag = 1;
	}
	
	if (socketExtendreceiveflag == 1) {
		socketExtendreceivesendtime += 1;
	}
	
#ifdef SOCKETEXTENDRTCCHECKENABLE
	if (PlatformSocketExtendtime == SocketExtendTime_ENABLE) {
		/* ��δ�Ժ�ʱ�� */
		if (SOCKET_EXTEND_RTC_CHECK == SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT) {
			if (socketExtendreceivesendtime >= SOCKET_EXTEND_AGAINSENDTIME) {						//������ʱ��
				socketExtendreceivesendtime = 0;
				if (SOCKET_EXTEND_RX_STA & 0X8000) {											//���յ��ڴ���Ӧ����
					if (SocketExtendReceiveBuf[1] == (SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT | SOCKET_EXTEND_SERVERTYPE_CROSS)) {	//���ܵ���ʱ����
						RTC_Time_Adjust(SocketExtendReceiveBuf[16], SocketExtendReceiveBuf[17], SocketExtendReceiveBuf[18], 
									 SocketExtendReceiveBuf[19], SocketExtendReceiveBuf[20], SocketExtendReceiveBuf[21]);
						SOCKET_EXTEND_RTC_CHECK = SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE;				//��ʱ������1
#ifdef SOCKETEXTENDRTCCHECKLED1
						if (PlatformSocketExtendtime == SocketExtendTime_ENABLE) {
							GPIO_ResetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0]);
						}
#endif
						socketExtendsendnum = 0;
						socketExtendreceiveflag = 0;
						socketExtendreceivesendtime = 0;
					}
					else {
						SOCKET_EXTEND_RX_STA = 0;
					}
				}
				else {
					/* ���ݷ��� */
#ifdef SOCKETEXTENDUSARTTXIRQn
					SocketExtendTxDataLangth = socketExtendsendlength;							//��ȡ�������ݳ���
					SocketExtendTxCounter = 0;												//���͵�ַ��0
					if (SOCKET_EXTEND_USART == USART1) {										//485����
						SOCKET_EXTEND_RS485_Mode_TX();
						Delay(0x1fff);
					}
					USART_ITConfig(SOCKET_EXTEND_USART, USART_IT_TXE, ENABLE);									//�������ж�,�����뷢���жϷ�������
#else
					SOCKET_Extend_USARTSend(SOCKET_EXTEND_USART, (u8 *)SocketExtendSendBuf, socketExtendsendlength);	//��������
#endif
					socketExtendsendnum += 1;
					if (socketExtendsendnum >= 3) {
						socketExtendsendnum = 0;
						socketExtendreceiveflag = 0;
						socketExtendreceivesendtime = 0;
					}
				}
			}
		}
		/* �Ժ�ʱ�� */
		else {
			if (socketExtendreceivesendtime >= SOCKET_EXTEND_AGAINSENDTIME) {						//������ʱ��
				socketExtendreceivesendtime = 0;
				if (SOCKET_EXTEND_RX_STA & 0X8000) {											//���յ��ڴ���Ӧ����
					if (SocketExtendReceiveBuf[1] == (SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE | SOCKET_EXTEND_SERVERTYPE_CROSS)) {	//���ܵ�����
						if (SocketExtendReceiveBuf[16] == 0x99) {
							socketExtendsendnum = 0;
							socketExtendreceiveflag = 0;
							socketExtendreceivesendtime = 0;
						}
						else {
							SOCKET_EXTEND_RX_STA = 0;
						}
					}
					else {
						SOCKET_EXTEND_RX_STA = 0;
					}
				}
				else {
					/* ���ݷ��� */
#ifdef SOCKETEXTENDUSARTTXIRQn
					SocketExtendTxDataLangth = socketExtendsendlength;							//��ȡ�������ݳ���
					SocketExtendTxCounter = 0;												//���͵�ַ��0
					if (SOCKET_EXTEND_USART == USART1) {										//485����
						SOCKET_EXTEND_RS485_Mode_TX();
						Delay(0x1fff);
					}
					USART_ITConfig(SOCKET_EXTEND_USART, USART_IT_TXE, ENABLE);									//�������ж�,�����뷢���жϷ�������
#else
					SOCKET_Extend_USARTSend(SOCKET_EXTEND_USART, (u8 *)SocketExtendSendBuf, socketExtendsendlength);	//��������
#endif
					socketExtendsendnum += 1;
					if (socketExtendsendnum >= 3) {
						socketExtendsendnum = 0;
						socketExtendreceiveflag = 0;
						socketExtendreceivesendtime = 0;
					}
				}
			}
		}
	}
	else {
		if (socketExtendreceivesendtime >= SOCKET_EXTEND_AGAINSENDTIME) {							//������ʱ��
			socketExtendreceivesendtime = 0;
			if (SOCKET_EXTEND_RX_STA & 0X8000) {												//���յ��ڴ���Ӧ����
				if (SocketExtendReceiveBuf[1] == (SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE | SOCKET_EXTEND_SERVERTYPE_CROSS)) {		//���ܵ�����
					if (SocketExtendReceiveBuf[16] == 0x99) {
						socketExtendsendnum = 0;
						socketExtendreceiveflag = 0;
						socketExtendreceivesendtime = 0;
					}
					else {
						SOCKET_EXTEND_RX_STA = 0;
					}
				}
				else {
					SOCKET_EXTEND_RX_STA = 0;
				}
			}
			else {
				/* ���ݷ��� */
#ifdef SOCKETEXTENDUSARTTXIRQn
				SocketExtendTxDataLangth = socketExtendsendlength;								//��ȡ�������ݳ���
				SocketExtendTxCounter = 0;													//���͵�ַ��0
				if (SOCKET_EXTEND_USART == USART1) {											//485����
					SOCKET_EXTEND_RS485_Mode_TX();
					Delay(0x1fff);
				}
				USART_ITConfig(SOCKET_EXTEND_USART, USART_IT_TXE, ENABLE);										//�������ж�,�����뷢���жϷ�������
#else
				SOCKET_Extend_USARTSend(SOCKET_EXTEND_USART, (u8 *)SocketExtendSendBuf, socketExtendsendlength);		//��������
#endif
				socketExtendsendnum += 1;
				if (socketExtendsendnum >= 3) {
					socketExtendsendnum = 0;
					socketExtendreceiveflag = 0;
					socketExtendreceivesendtime = 0;
				}
			}
		}
	}
#else
	socketExtendsendnum = 0;
	socketExtendreceiveflag = 0;
	socketExtendreceivesendtime = 0;
#endif
	
	if (PlatformSocketExtendtime == SocketExtendTime_ENABLE) {
#ifdef SOCKETEXTENDRTCCHECKENABLE
		rtctime = RTC_GetCounter();										//��ȡ��ǰʱ��ֵ
		if ((rtctime % 86400) == 0) {										//�ж��Ƿ񵽴��賿0��
			SOCKET_EXTEND_RTC_CHECK = SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT;		//RTC��ʱУ���־
#ifdef SOCKETEXTENDRTCCHECKLED1
			GPIO_SetBits(OUTPUT_TYPE[0], OUTPUT_PIN[0]);
#endif
		}
#endif
	}
}

/***************************************************************
 @Function			void SOCKET_Extend_USARTSendByte(USART_TypeDef* USARTx, u8 data)
 @Description			USART 
 @Input				data : 8bit
 @Return				void
***************************************************************/
void SOCKET_Extend_USARTSendByte(USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, (u8) data);
	
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length)
 @Description			Socket���ݴ��ڷ���
 @Input				ͳ��ʱ��
 @Return				void
**********************************************************************************************************/
u32 SOCKET_Extend_USARTSend(USART_TypeDef* USARTx, u8 *data_arr, u32 length)
{
	u32 i = 0;
	
	if (USARTx == USART1) {
		SOCKET_EXTEND_RS485_Mode_TX();
		Delay(0x1fff);
		while (length > 0) {
			SOCKET_Extend_USARTSendByte(USARTx, *data_arr++);
			i++;
			length--;
		}
		Delay(0x1fff);
		SOCKET_EXTEND_RS485_Mode_RX();
	}
	else {
		while (length > 0) {
			SOCKET_Extend_USARTSendByte(USARTx, *data_arr++);
			i++;
			length--;
		}
	}
	
	return i;
}

/**********************************************************************************************************
 @Function			void SOCKET_Extend_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate)
 @Description			��ʼ��SocketExtend����
 @Input				���ں�
					������
 @Return				void
**********************************************************************************************************/
void SOCKET_Extend_PortSerialInit(USART_TypeDef* USARTx, u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef SOCKET_EXTEND_SERIALPORT_USART1
	/* Enable  USART1 clocks */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif
#ifdef SOCKET_EXTEND_SERIALPORT_USART2
  	/* Enable  USART2 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#endif
#ifdef SOCKET_EXTEND_SERIALPORT_USART3  
  	/* Enable  USART3 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#endif
	
	USART_DeInit(USARTx);
#ifdef SOCKET_EXTEND_SERIALPORT_USART1
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
#ifdef SOCKET_EXTEND_SERIALPORT_USART2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
#endif
#ifdef SOCKET_EXTEND_SERIALPORT_USART3
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
	
#ifdef SOCKET_EXTEND_SERIALPORT_USART1
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
#endif
#ifdef SOCKET_EXTEND_SERIALPORT_USART2
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
#endif
#ifdef SOCKET_EXTEND_SERIALPORT_USART3
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
#endif

	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);							//���������ж�
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);							//�رշ����ж�
}

/********************************************** END OF FLEE **********************************************/
