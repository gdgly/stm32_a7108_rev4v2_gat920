/**
  *********************************************************************************************************
  * @file    platform_config.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "stm32f10x_lib.h"
#include "platform_config.h"


PlatFormSocketTime			PlatformSockettime = SocketTime_DISABLE;			//Socket��ʱ��
PlatFormSocket				PlatformSocket = Socket_DISABLE;					//Socketʹ����
PlatFormSocketExtendTime		PlatformSocketExtendtime = SocketExtendTime_DISABLE;	//SocketExtend��ʱ��
PlatFormSocketExtend		PlatformSocketExtend = SocketExtend_DISABLE;			//SocketExtendʹ����
PlatFormSocketModulation		PlatformSocketModulation = SocketModulation_DISABLE;	//SocketModulationʹ����
PlatFormGat920				PlatformGat920 = Gat920_DISABLE;					//GAT920ʹ����
PlatFormModBusUSART			PlatformModbusUsart = Modbus_USART2;				//Modbus���Ӵ�����
PlatFormGat920USART			PlatformGat920Usart = Gat920_USART1;				//Gat920���Ӵ�����
PlatFormGPRS				PlatformGPRS = GPRS_DISABLE;						//GPRSʹ����
PlatFormRJ45				PlatformRJ45 = RJ45_DISABLE;						//RJ45ʹ����
PlatFormLESTC				PlatformLESTC = LESTC_DISABLE;					//LESTCʹ����

/**********************************************************************************************************
 @Function			void PlatForm_SnToFunction(u32 Sn)
 @Description			����SN��ѡ������Ӧ������
 @Input				SN
 @Return				void
**********************************************************************************************************/
void PlatForm_SnToFunction(u32 SN)
{
	u8 index = 0;
	
	if ((SN == 0xFFFFFFFF) || (SN == 0x12345678)) {							//SNδ����
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_DISABLE;										//RJ45��ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		return;
	}
	
	index = (SN % 1000) / 100;
	
	switch (index)
	{
	case 0:
		/* Modbus --> USART2, Gat920 --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, GPRS --> DISABLE, RJ45 --> DISABLE */
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_DISABLE;										//RJ45��ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	
	case 1:
		/* Modbus --> USART1, Gat920 --> USART2, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, GPRS --> DISABLE, RJ45 --> DISABLE */
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART1;								//Modbus���Ӵ���1
		PlatformGat920Usart = Gat920_USART2;								//Gat920���Ӵ���2
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_DISABLE;										//RJ45��ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	
	case 2:
		/* Modbus --> USART2, Gat920 --> USART1, Socket --> ENABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, GPRS --> DISABLE, RJ45 --> ENABLE */
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_ENABLE;									//Socket����
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_ENABLE;										//RJ45ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	
	case 3:
		/* Modbus --> USART2, Gat920 --> USART1, Socket --> ENABLE, SocketTime --> ENABLE, Gat920 --> DISABLE, LESTC --> DISABLE, GPRS --> DISABLE, RJ45 --> ENABLE */
		PlatformSockettime = SocketTime_ENABLE;								//Socket��ʱ����
		PlatformSocket = Socket_ENABLE;									//Socket����
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_ENABLE;										//RJ45ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	
	case 4:
		/* Modbus --> USART2, Gat920 --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> ENABLE, LESTC --> DISABLE, GPRS --> DISABLE, RJ45 --> DISABLE */
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_ENABLE;									//Gat920����
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_DISABLE;										//RJ45��ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	
	case 5:
		/* Modbus --> USART1, Gat920 --> USART2, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> ENABLE, LESTC --> DISABLE, GPRS --> DISABLE, RJ45 --> DISABLE */
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_ENABLE;									//Gat920����
		PlatformModbusUsart = Modbus_USART1;								//Modbus���Ӵ���1
		PlatformGat920Usart = Gat920_USART2;								//Gat920���Ӵ���2
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_DISABLE;										//RJ45��ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	
	case 6:
		/* Modbus --> USART2, Gat920 --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, RJ45 --> ENABLE, SocketModulation --> ENABLE */
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_ENABLE;					//SocketModulation����
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_ENABLE;										//RJ45ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	
	case 7:
		/* Modbus --> USART2, Gat920 --> USART1, Socket --> ENABLE, SocketTime --> ENABLE, Gat920 --> DISABLE, LESTC --> DISABLE, GPRS --> ENABLE, RJ45 --> DISABLE */
		PlatformSockettime = SocketTime_ENABLE;								//Socket��ʱ����
		PlatformSocket = Socket_ENABLE;									//Socket����
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_ENABLE;										//GPRSʹ��
		PlatformRJ45 = RJ45_DISABLE;										//RJ45��ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	
	case 8:
		/* Modbus --> USART2, Gat920 --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> ENABLE, GPRS --> DISABLE, RJ45 --> DISABLE */
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_DISABLE;										//RJ45��ʹ��
		PlatformLESTC = LESTC_ENABLE;										//LESTCʹ��
		break;
	
	case 9:
		/* Modbus --> USART2, Gat920 --> USART1, SocketExtend --> ENABLE,  SocketExtendTime --> ENABLE,  Gat920 --> DISABLE, LESTC --> DISABLE, GPRS --> DISABLE, RJ45 --> ENABLE */
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_ENABLE;					//SocketExtend��ʱ����
		PlatformSocketExtend = SocketExtend_ENABLE;							//SocketExtend����
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_ENABLE;										//RJ45ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
		
	default :
		PlatformSockettime = SocketTime_DISABLE;							//Socket��ʱ�ر�
		PlatformSocket = Socket_DISABLE;									//Socket�ر�
		PlatformSocketExtendtime = SocketExtendTime_DISABLE;					//SocketExtend��ʱ�ر�
		PlatformSocketExtend = SocketExtend_DISABLE;							//SocketExtend�ر�
		PlatformSocketModulation = SocketModulation_DISABLE;					//SocketModulation�ر�
		PlatformGat920 = Gat920_DISABLE;									//Gat920�ر�
		PlatformModbusUsart = Modbus_USART2;								//Modbus���Ӵ���2
		PlatformGat920Usart = Gat920_USART1;								//Gat920���Ӵ���1
		PlatformGPRS = GPRS_DISABLE;										//GPRS��ʹ��
		PlatformRJ45 = RJ45_DISABLE;										//RJ45��ʹ��
		PlatformLESTC = LESTC_DISABLE;									//LESTC��ʹ��
		break;
	}
}

/********************************************** END OF FLEE **********************************************/
