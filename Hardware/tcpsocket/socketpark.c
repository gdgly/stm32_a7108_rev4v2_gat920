/**
  *********************************************************************************************************
  * @file    socketpark.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   Socketͣ�������ݴ���
  *********************************************************************************************************
  * @attention
  *			
  *
  *			
  *********************************************************************************************************
  */

#include "socketpark.h"
#include "socketconfig.h"
#include "socketfunc.h"
#include "socketinstantia.h"
#include "calculationconfig.h"
#include "calculationavgspeed.h"
#include "socketinitialization.h"


#ifdef SOCKET_SERIALPORT_USART1
#define SOCKET_USART		USART1
#endif
#ifdef SOCKET_SERIALPORT_USART2
#define SOCKET_USART		USART2
#endif
#ifdef SOCKET_SERIALPORT_USART3
#define SOCKET_USART		USART3
#endif

extern unsigned short 		output_ID[OUTPUT_MAX];

Socket_Packet_Head			SocketParkHeadPacket;							//Socketͣ�������ݰ�ͷ
Socket_Packet_Data			SocketParkDataPacket;							//Socketͣ�������ݰ�


/**********************************************************************************************************
 @Function			void SOCKET_ParkImplement(u8 laneNo, u16 CarNum, u8 CarInorOut)
 @Description			Socket����ʵʱ�ϴ�
 @Input				laneNo		: ������
					CarNum 		: ����ֵ
					CarInorOut	: ����������뿪
 @Return				void
**********************************************************************************************************/
void SOCKET_ParkImplement(u8 laneNo, u16 CarNum, u8 CarInorOut)
{
	u8 indexA = 0;
	u8 indexB = 0;
	
	u8 i = 0;
	u8 outputnum = 0;
	u16 headlength = 0;
	u16 datalength = 0;
	
	indexA = laneNo;
	
	for (i = 0; i < OUTPUT_MAX; i++) {																		//��ȡ������ID�����ֵ
		if (SocketDataPacket[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) {																		//�жϵ��شŲ��ٻ���˫�شŲ���
		/* ˫�ش� */
		if (indexA < SPEEDLANNUMMAX) {																	//���ش����ȡ����
			for (indexB = indexA; indexB > 0; indexB--) {													//Ѱ�ұ�������һ��ͬID����
				if (SocketDataPacket[indexA].OutputID == SocketDataPacket[indexB - 1].OutputID) {
					SocketDataPacket[indexA].DeviceType = SocketDataPacket[indexB - 1].DeviceType;						//д��DeviceType
					SocketDataPacket[indexA].Interval = SocketDataPacket[indexB - 1].Interval;							//д��Interval
					SocketDataPacket[indexA].LaneNo = indexA + 1;												//д��LaneNo
					SocketDataPacket[indexA].DateTime = SocketDataPacket[indexB - 1].DateTime;							//д��DateTime
					SocketDataPacket[indexA].Volume = SocketDataPacket[indexB - 1].Volume;							//д��Volume
					SocketDataPacket[indexA].Volume1 = SocketDataPacket[indexB - 1].Volume1;							//д��Volume1
					SocketDataPacket[indexA].Volume2 = SocketDataPacket[indexB - 1].Volume2;							//д��Volume2
					SocketDataPacket[indexA].Volume3 = SocketDataPacket[indexB - 1].Volume3;							//д��Volume3
					SocketDataPacket[indexA].Volume4 = SocketDataPacket[indexB - 1].Volume4;							//д��Volume4
					SocketDataPacket[indexA].Volume5 = SocketDataPacket[indexB - 1].Volume5;							//д��Volume5
					SocketDataPacket[indexA].AvgOccupancy = SocketDataPacket[indexB - 1].AvgOccupancy;					//д��AvgOccupancy
					SocketDataPacket[indexA].AvgHeadTime = SocketDataPacket[indexB - 1].AvgHeadTime;					//д��AvgHeadTime
					SocketDataPacket[indexA].AvgSpeed = SocketDataPacket[indexB - 1].AvgSpeed;							//д��AvgSpeed
					SocketDataPacket[indexA].AvgLength = SocketDataPacket[indexB - 1].AvgLength;						//д��AvgLength
					SocketDataPacket[indexA].Saturation = SocketDataPacket[indexB - 1].Saturation;						//д��Saturation
					SocketDataPacket[indexA].Density = SocketDataPacket[indexB - 1].Density;							//д��Density
					SocketDataPacket[indexA].Pcu = SocketDataPacket[indexB - 1].Pcu;									//д��Pcu
					SocketDataPacket[indexA].AvgQueueLength = SocketDataPacket[indexB - 1].AvgQueueLength;				//д��AvgQueueLength
					break;
				}
			}
			if (indexB == 0) {																			//�����ݰ���ͬID���ݰ�,��ȡ����
				SocketDataPacket[indexA].DeviceType = socket_dev.GetDeviceType(SocketDataPacket[indexA].OutputID);			//д��DeviceType
				SocketDataPacket[indexA].Interval = socket_dev.GetInterval(SocketDataPacket[indexA].OutputID);				//д��Interval
				SocketDataPacket[indexA].LaneNo = indexA + 1;													//д��LaneNo
				SocketDataPacket[indexA].DateTime = socket_dev.GetDateTime(SocketDataPacket[indexA].OutputID);				//д��DateTime
				SocketDataPacket[indexA].Volume = socket_dev.GetVolume(SocketDataPacket[indexA].OutputID);				//д��Volume
				SocketDataPacket[indexA].Volume1 = socket_dev.GetVolume1(SocketDataPacket[indexA].OutputID);				//д��Volume1
				SocketDataPacket[indexA].Volume2 = socket_dev.GetVolume2(SocketDataPacket[indexA].OutputID);				//д��Volume2
				SocketDataPacket[indexA].Volume3 = socket_dev.GetVolume3(SocketDataPacket[indexA].OutputID);				//д��Volume3
				SocketDataPacket[indexA].Volume4 = socket_dev.GetVolume4(SocketDataPacket[indexA].OutputID);				//д��Volume4
				SocketDataPacket[indexA].Volume5 = socket_dev.GetVolume5(SocketDataPacket[indexA].OutputID);				//д��Volume5
				SocketDataPacket[indexA].AvgOccupancy = socket_dev.GetAvgOccupancy(SocketDataPacket[indexA].OutputID);		//д��AvgOccupancy
				SocketDataPacket[indexA].AvgHeadTime = socket_dev.GetAvgHeadTime(SocketDataPacket[indexA].OutputID);		//д��AvgHeadTime
				SocketDataPacket[indexA].AvgSpeed = 0;															//д��AvgSpeed
				SocketDataPacket[indexA].AvgLength = 0;															//д��AvgLength
				SocketDataPacket[indexA].Saturation = socket_dev.GetSaturation(SocketDataPacket[indexA].OutputID);			//д��Saturation
				SocketDataPacket[indexA].Density = socket_dev.GetDensity(SocketDataPacket[indexA].OutputID);				//д��Density
				SocketDataPacket[indexA].Pcu = socket_dev.GetPcu(SocketDataPacket[indexA].OutputID);						//д��Pcu
				SocketDataPacket[indexA].AvgQueueLength = socket_dev.GetAvgQueueLength(SocketDataPacket[indexA].OutputID);	//д��AvgQueueLength
			}
		}
		else {																						//���شŻ�ȡ����
			if (SocketDataPacket[indexA - SPEEDLANNUMMAX].OutputID != 0) {										//���شŶ�Ӧ���ش�������
				SocketDataPacket[indexA].DeviceType = socket_dev.GetDeviceType(SocketDataPacket[indexA].OutputID);			//д��DeviceType
				SocketDataPacket[indexA].Interval = socket_dev.GetInterval(SocketDataPacket[indexA].OutputID);				//д��Interval
				SocketDataPacket[indexA].LaneNo = indexA + 1;													//д��LaneNo
				SocketDataPacket[indexA].DateTime = socket_dev.GetDateTime(SocketDataPacket[indexA].OutputID);				//д��DateTime
				SocketDataPacket[indexA].Volume = socket_dev.GetVolume(SocketDataPacket[indexA].OutputID);				//д��Volume
				SocketDataPacket[indexA].Volume1 = socket_dev.GetVolume1(SocketDataPacket[indexA].OutputID);				//д��Volume1
				SocketDataPacket[indexA].Volume2 = socket_dev.GetVolume2(SocketDataPacket[indexA].OutputID);				//д��Volume2
				SocketDataPacket[indexA].Volume3 = socket_dev.GetVolume3(SocketDataPacket[indexA].OutputID);				//д��Volume3
				SocketDataPacket[indexA].Volume4 = socket_dev.GetVolume4(SocketDataPacket[indexA].OutputID);				//д��Volume4
				SocketDataPacket[indexA].Volume5 = socket_dev.GetVolume5(SocketDataPacket[indexA].OutputID);				//д��Volume5
				SocketDataPacket[indexA].AvgOccupancy = socket_dev.GetAvgOccupancy(SocketDataPacket[indexA].OutputID);		//д��AvgOccupancy
				SocketDataPacket[indexA].AvgHeadTime = socket_dev.GetAvgHeadTime(SocketDataPacket[indexA].OutputID);		//д��AvgHeadTime
				SocketDataPacket[indexA].AvgSpeed = socket_dev.GetAvgSpeed(SocketDataPacket[indexA].OutputID);				//д��AvgSpeed
				SocketDataPacket[indexA].AvgLength = socket_dev.GetAvgLength(SocketDataPacket[indexA].OutputID);			//д��AvgLength
				SocketDataPacket[indexA].Saturation = socket_dev.GetSaturation(SocketDataPacket[indexA].OutputID);			//д��Saturation
				SocketDataPacket[indexA].Density = socket_dev.GetDensity(SocketDataPacket[indexA].OutputID);				//д��Density
				SocketDataPacket[indexA].Pcu = socket_dev.GetPcu(SocketDataPacket[indexA].OutputID);						//д��Pcu
				SocketDataPacket[indexA].AvgQueueLength = socket_dev.GetAvgQueueLength(SocketDataPacket[indexA].OutputID);	//д��AvgQueueLength
			}
			else {																					//���شŶ�Ӧ���ش�������
				for (indexB = indexA; indexB > SPEEDLANNUMMAX; indexB--) {										//Ѱ�ұ�������һ��ͬID����
					if (SocketDataPacket[indexA].OutputID == SocketDataPacket[indexB - 1].OutputID) {
						SocketDataPacket[indexA].DeviceType = SocketDataPacket[indexB - 1].DeviceType;						//д��DeviceType
						SocketDataPacket[indexA].Interval = SocketDataPacket[indexB - 1].Interval;							//д��Interval
						SocketDataPacket[indexA].LaneNo = indexA + 1;												//д��LaneNo
						SocketDataPacket[indexA].DateTime = SocketDataPacket[indexB - 1].DateTime;							//д��DateTime
						SocketDataPacket[indexA].Volume = SocketDataPacket[indexB - 1].Volume;							//д��Volume
						SocketDataPacket[indexA].Volume1 = SocketDataPacket[indexB - 1].Volume1;							//д��Volume1
						SocketDataPacket[indexA].Volume2 = SocketDataPacket[indexB - 1].Volume2;							//д��Volume2
						SocketDataPacket[indexA].Volume3 = SocketDataPacket[indexB - 1].Volume3;							//д��Volume3
						SocketDataPacket[indexA].Volume4 = SocketDataPacket[indexB - 1].Volume4;							//д��Volume4
						SocketDataPacket[indexA].Volume5 = SocketDataPacket[indexB - 1].Volume5;							//д��Volume5
						SocketDataPacket[indexA].AvgOccupancy = SocketDataPacket[indexB - 1].AvgOccupancy;					//д��AvgOccupancy
						SocketDataPacket[indexA].AvgHeadTime = SocketDataPacket[indexB - 1].AvgHeadTime;					//д��AvgHeadTime
						SocketDataPacket[indexA].AvgSpeed = SocketDataPacket[indexB - 1].AvgSpeed;							//д��AvgSpeed
						SocketDataPacket[indexA].AvgLength = SocketDataPacket[indexB - 1].AvgLength;						//д��AvgLength
						SocketDataPacket[indexA].Saturation = SocketDataPacket[indexB - 1].Saturation;						//д��Saturation
						SocketDataPacket[indexA].Density = SocketDataPacket[indexB - 1].Density;							//д��Density
						SocketDataPacket[indexA].Pcu = SocketDataPacket[indexB - 1].Pcu;									//д��Pcu
						SocketDataPacket[indexA].AvgQueueLength = SocketDataPacket[indexB - 1].AvgQueueLength;				//д��AvgQueueLength
						break;
					}
				}
				if (indexB == SPEEDLANNUMMAX) {															//�����ݰ���ͬID���ݰ�,��ȡ����
					SocketDataPacket[indexA].DeviceType = socket_dev.GetDeviceType(SocketDataPacket[indexA].OutputID);			//д��DeviceType
					SocketDataPacket[indexA].Interval = socket_dev.GetInterval(SocketDataPacket[indexA].OutputID);				//д��Interval
					SocketDataPacket[indexA].LaneNo = indexA + 1;													//д��LaneNo
					SocketDataPacket[indexA].DateTime = socket_dev.GetDateTime(SocketDataPacket[indexA].OutputID);				//д��DateTime
					SocketDataPacket[indexA].Volume = socket_dev.GetVolume(SocketDataPacket[indexA].OutputID);				//д��Volume
					SocketDataPacket[indexA].Volume1 = socket_dev.GetVolume1(SocketDataPacket[indexA].OutputID);				//д��Volume1
					SocketDataPacket[indexA].Volume2 = socket_dev.GetVolume2(SocketDataPacket[indexA].OutputID);				//д��Volume2
					SocketDataPacket[indexA].Volume3 = socket_dev.GetVolume3(SocketDataPacket[indexA].OutputID);				//д��Volume3
					SocketDataPacket[indexA].Volume4 = socket_dev.GetVolume4(SocketDataPacket[indexA].OutputID);				//д��Volume4
					SocketDataPacket[indexA].Volume5 = socket_dev.GetVolume5(SocketDataPacket[indexA].OutputID);				//д��Volume5
					SocketDataPacket[indexA].AvgOccupancy = socket_dev.GetAvgOccupancy(SocketDataPacket[indexA].OutputID);		//д��AvgOccupancy
					SocketDataPacket[indexA].AvgHeadTime = socket_dev.GetAvgHeadTime(SocketDataPacket[indexA].OutputID);		//д��AvgHeadTime
					SocketDataPacket[indexA].AvgSpeed = socket_dev.GetAvgSpeed(SocketDataPacket[indexA].OutputID);				//д��AvgSpeed
					SocketDataPacket[indexA].AvgLength = socket_dev.GetAvgLength(SocketDataPacket[indexA].OutputID);			//д��AvgLength
					SocketDataPacket[indexA].Saturation = socket_dev.GetSaturation(SocketDataPacket[indexA].OutputID);			//д��Saturation
					SocketDataPacket[indexA].Density = socket_dev.GetDensity(SocketDataPacket[indexA].OutputID);				//д��Density
					SocketDataPacket[indexA].Pcu = socket_dev.GetPcu(SocketDataPacket[indexA].OutputID);						//д��Pcu
					SocketDataPacket[indexA].AvgQueueLength = socket_dev.GetAvgQueueLength(SocketDataPacket[indexA].OutputID);	//д��AvgQueueLength
				}
			}
		}
	}
	else {
		/* ���ش� */
		for (indexB = indexA; indexB > 0; indexB--) {														//Ѱ�ұ�������һ��ͬID����
			if (SocketDataPacket[indexA].OutputID == SocketDataPacket[indexB - 1].OutputID) {
				SocketDataPacket[indexA].DeviceType = SocketDataPacket[indexB - 1].DeviceType;						//д��DeviceType
				SocketDataPacket[indexA].Interval = SocketDataPacket[indexB - 1].Interval;							//д��Interval
				SocketDataPacket[indexA].LaneNo = indexA + 1;												//д��LaneNo
				SocketDataPacket[indexA].DateTime = SocketDataPacket[indexB - 1].DateTime;							//д��DateTime
				SocketDataPacket[indexA].Volume = SocketDataPacket[indexB - 1].Volume;							//д��Volume
				SocketDataPacket[indexA].Volume1 = SocketDataPacket[indexB - 1].Volume1;							//д��Volume1
				SocketDataPacket[indexA].Volume2 = SocketDataPacket[indexB - 1].Volume2;							//д��Volume2
				SocketDataPacket[indexA].Volume3 = SocketDataPacket[indexB - 1].Volume3;							//д��Volume3
				SocketDataPacket[indexA].Volume4 = SocketDataPacket[indexB - 1].Volume4;							//д��Volume4
				SocketDataPacket[indexA].Volume5 = SocketDataPacket[indexB - 1].Volume5;							//д��Volume5
				SocketDataPacket[indexA].AvgOccupancy = SocketDataPacket[indexB - 1].AvgOccupancy;					//д��AvgOccupancy
				SocketDataPacket[indexA].AvgHeadTime = SocketDataPacket[indexB - 1].AvgHeadTime;					//д��AvgHeadTime
				SocketDataPacket[indexA].AvgSpeed = SocketDataPacket[indexB - 1].AvgSpeed;							//д��AvgSpeed
				SocketDataPacket[indexA].AvgLength = SocketDataPacket[indexB - 1].AvgLength;						//д��AvgLength
				SocketDataPacket[indexA].Saturation = SocketDataPacket[indexB - 1].Saturation;						//д��Saturation
				SocketDataPacket[indexA].Density = SocketDataPacket[indexB - 1].Density;							//д��Density
				SocketDataPacket[indexA].Pcu = SocketDataPacket[indexB - 1].Pcu;									//д��Pcu
				SocketDataPacket[indexA].AvgQueueLength = SocketDataPacket[indexB - 1].AvgQueueLength;				//д��AvgQueueLength
				break;
			}
		}
		if (indexB == 0) {																				//�����ݰ���ͬID���ݰ�,��ȡ����
			SocketDataPacket[indexA].DeviceType = socket_dev.GetDeviceType(SocketDataPacket[indexA].OutputID);			//д��DeviceType
			SocketDataPacket[indexA].Interval = socket_dev.GetInterval(SocketDataPacket[indexA].OutputID);				//д��Interval
			SocketDataPacket[indexA].LaneNo = indexA + 1;													//д��LaneNo
			SocketDataPacket[indexA].DateTime = socket_dev.GetDateTime(SocketDataPacket[indexA].OutputID);				//д��DateTime
			SocketDataPacket[indexA].Volume = socket_dev.GetVolume(SocketDataPacket[indexA].OutputID);				//д��Volume
			SocketDataPacket[indexA].Volume1 = socket_dev.GetVolume1(SocketDataPacket[indexA].OutputID);				//д��Volume1
			SocketDataPacket[indexA].Volume2 = socket_dev.GetVolume2(SocketDataPacket[indexA].OutputID);				//д��Volume2
			SocketDataPacket[indexA].Volume3 = socket_dev.GetVolume3(SocketDataPacket[indexA].OutputID);				//д��Volume3
			SocketDataPacket[indexA].Volume4 = socket_dev.GetVolume4(SocketDataPacket[indexA].OutputID);				//д��Volume4
			SocketDataPacket[indexA].Volume5 = socket_dev.GetVolume5(SocketDataPacket[indexA].OutputID);				//д��Volume5
			SocketDataPacket[indexA].AvgOccupancy = socket_dev.GetAvgOccupancy(SocketDataPacket[indexA].OutputID);		//д��AvgOccupancy
			SocketDataPacket[indexA].AvgHeadTime = socket_dev.GetAvgHeadTime(SocketDataPacket[indexA].OutputID);		//д��AvgHeadTime
			if (CarInorOut == 1) {																		//������
				SocketDataPacket[indexA].AvgSpeed = 0;														//δ�뿪
			}
			else {																					//�����
				SocketDataPacket[indexA].AvgSpeed = 3600.0 * AVGLENGTHSINGLE / SocketDataPacket[indexA].AvgOccupancy;
			}
			SocketDataPacket[indexA].AvgLength = socket_dev.GetAvgLength(SocketDataPacket[indexA].OutputID);			//д��AvgLength
			SocketDataPacket[indexA].Saturation = socket_dev.GetSaturation(SocketDataPacket[indexA].OutputID);			//д��Saturation
			SocketDataPacket[indexA].Density = socket_dev.GetDensity(SocketDataPacket[indexA].OutputID);				//д��Density
			SocketDataPacket[indexA].Pcu = socket_dev.GetPcu(SocketDataPacket[indexA].OutputID);						//д��Pcu
			SocketDataPacket[indexA].AvgQueueLength = socket_dev.GetAvgQueueLength(SocketDataPacket[indexA].OutputID);	//д��AvgQueueLength
		}
	}
	
	/* д������ */
	SocketParkHeadPacket.CrossID = socket_dev.GetCrossID();												//д��CrossID
//	SocketParkHeadPacket.PacketType = PACKETTYPE_FLOWMESSAGE;												//��������
	SocketParkHeadPacket.PacketType = PACKETTYPE_PARKINGLOTDATA;											//ͣ��������
	SocketParkHeadPacket.PacketInfo = 1;																//���ݰ���
	
	SocketParkDataPacket.DeviceType = DEVICETYPE_DICI;													//д��DeviceType
	SocketParkDataPacket.Interval = INTERVALTIME;														//д��Interval
	SocketParkDataPacket.LaneNo = laneNo + 1;															//д��LaneNo
	SocketParkDataPacket.DateTime = RTC_GetCounter();														//д��DateTime
	SocketParkDataPacket.Volume = CarNum;																//д��Volume
	SocketParkDataPacket.Volume1 = 0;																	//д��Volume1
	SocketParkDataPacket.Volume2 = 0;																	//д��Volume2
	SocketParkDataPacket.Volume3 = 0;																	//д��Volume3
	SocketParkDataPacket.Volume4 = 0;																	//д��Volume4
	SocketParkDataPacket.Volume5 = 0;																	//д��Volume5
	SocketParkDataPacket.AvgOccupancy = SocketDataPacket[laneNo].AvgOccupancy;									//д��AvgOccupancy
	SocketParkDataPacket.AvgHeadTime = SocketDataPacket[laneNo].AvgHeadTime;									//д��AvgHeadTime
	SocketParkDataPacket.AvgSpeed = SocketDataPacket[laneNo].AvgSpeed;										//д��AvgSpeed
	SocketParkDataPacket.AvgLength = SocketDataPacket[laneNo].AvgLength;										//д��AvgLength
	SocketParkDataPacket.Saturation = SocketDataPacket[laneNo].Saturation;									//д��Saturation
	SocketParkDataPacket.Density = SocketDataPacket[laneNo].Density;											//д��Density
	SocketParkDataPacket.Pcu = SocketDataPacket[laneNo].Pcu;												//д��Pcu
	SocketParkDataPacket.AvgQueueLength = SocketDataPacket[laneNo].AvgQueueLength;								//д��AvgQueueLength
	
	headlength = SOCKET_ParkObtainPacketHead((u8 *)SocketSendBuf);											//��ȡSocketPark��ͷ���ݲ����뻺��
	datalength = SOCKET_ParkObtainPacketData((u8 *)SocketSendBuf);											//��ȡSocketParkͣ�������ݰ������뻺��
	headlength = headlength + datalength;
	datalength = socket_dev.ObtainPacketManuCheck((u8 *)SocketSendBuf, headlength);
	headlength = headlength + datalength;
	
	SOCKET_USARTSend(SOCKET_USART, (u8 *)SocketSendBuf, headlength);											//��������
}

/**********************************************************************************************************
 @Function			void SOCKET_ParkImplementHeartbeatSend(u8 laneNo, u16 CarNum)
 @Description			Socket���������ϴ�
 @Input				laneNo : ������
					CarNum : ����ֵ
 @Return				void
**********************************************************************************************************/
void SOCKET_ParkImplementHeartbeatSend(u8 laneNo, u16 CarNum)
{
	u16 headlength = 0;
	u16 datalength = 0;
	
	/* д������ */
	SocketParkHeadPacket.CrossID = socket_dev.GetCrossID();					//д��CrossID
	SocketParkHeadPacket.PacketType = PACKETTYPE_PARKINGLOTDATA;				//ͣ��������
	SocketParkHeadPacket.PacketInfo = 1;									//���ݰ���
	
	SocketParkDataPacket.DeviceType = DEVICETYPE_DICI;						//д��DeviceType
	SocketParkDataPacket.Interval = INTERVALTIME;							//д��Interval
	SocketParkDataPacket.LaneNo = laneNo + 1;								//д��LaneNo
	SocketParkDataPacket.DateTime = RTC_GetCounter();							//д��DateTime
	SocketParkDataPacket.Volume = CarNum;									//д��Volume
	SocketParkDataPacket.Volume1 = 0;										//д��Volume1
	SocketParkDataPacket.Volume2 = 0;										//д��Volume2
	SocketParkDataPacket.Volume3 = 0;										//д��Volume3
	SocketParkDataPacket.Volume4 = 0;										//д��Volume4
	SocketParkDataPacket.Volume5 = 0;										//д��Volume5
	SocketParkDataPacket.AvgOccupancy = 0;									//д��AvgOccupancy
	SocketParkDataPacket.AvgHeadTime = 0;									//д��AvgHeadTime
	SocketParkDataPacket.AvgLength = 0;									//д��AvgLength
	SocketParkDataPacket.AvgSpeed = 0;										//д��AvgSpeed
	SocketParkDataPacket.Saturation = 0;									//д��Saturation
	SocketParkDataPacket.Density = 0;										//д��Density
	SocketParkDataPacket.Pcu = 0;											//д��Pcu
	SocketParkDataPacket.AvgQueueLength = 0;								//д��AvgQueueLength
	
	headlength = SOCKET_ParkObtainPacketHead((u8 *)SocketSendBuf);				//��ȡSocketPark��ͷ���ݲ����뻺��
	datalength = SOCKET_ParkObtainPacketData((u8 *)SocketSendBuf);				//��ȡSocketParkͣ�������ݰ������뻺��
	headlength = headlength + datalength;
	datalength = socket_dev.ObtainPacketManuCheck((u8 *)SocketSendBuf, headlength);
	headlength = headlength + datalength;
	
	SOCKET_USARTSend(SOCKET_USART, (u8 *)SocketSendBuf, headlength);				//��������
}

/**********************************************************************************************************
 @Function			void SOCKET_ParkImplementHeartbeat(u8 laneNo, u16 CarNum)
 @Description			Socket����ʵʱ�ϴ�(�������ϴ�����)
 @Input				*buf				: ���ؽ��յ��شŷ��͵����ݰ�
 @Return				void
**********************************************************************************************************/
void SOCKET_ParkImplementHeartbeat(u8 *buf)
{
	u8 i = 0;
	u16 carnumstate = 0;
	RF_DataHeader_TypeDef *phead = (RF_DataHeader_TypeDef *)buf;
	
	//����յ��ش�������
	if ( (phead->type == DATATYPE_HEARTBEAT_WITHOUT_MAGENV) || (phead->type == DATATYPE_HEARTBEAT_WITH_MAGENV) )
	{
		carnumstate = (((~0xffff) | buf[3])<<8) | buf[4];						//��ȡ������״ֵ̬
		for (i = 0; i < OUTPUT_MAX; i++) {									//����ID
			if ((output_ID[i] == (phead->addr_dev)) || (output_ID[i] == 0xFFFF))	//ƥ��ͬID
			{
				SOCKET_ParkImplementHeartbeatSend(i, carnumstate);			//�������ݰ�����
			}
		}
	}
}

/**********************************************************************************************************
 @Function			u16 SOCKET_ObtainPacketHead(u8 *addr)
 @Description			��ȡSocketPark��ͷ���ݲ����뻺��
 @Input				*addr		: Socket�����ַ
 @Return				���뻺�����ݳ���
					SOCKET_FALSE   : ����
**********************************************************************************************************/
u16 SOCKET_ParkObtainPacketHead(u8 *addr)
{
	u16 headlength = 0;																				//���뻺�����ݳ���
	
	if ((SOCKET_ConvertCrossID((u8 *)&addr[headlength], SocketParkHeadPacket.CrossID)) == SOCKET_FALSE) {			//����CrossID
		return SOCKET_FALSE;
	}
	else {
		headlength += 8;																			//��ַƫ��8byte
	}
	
	addr[headlength] = SocketParkHeadPacket.PacketType;													//������ˮ������
	headlength += 1;																				//��ַƫ��1byte
	
	if ((SOCKET_ConvertPacketInfo((u8 *)&addr[headlength], SocketParkHeadPacket.PacketInfo)) == SOCKET_FALSE) {		//��������������ݰ���
		return SOCKET_FALSE;
	}
	else {
		headlength += 4;																			//��ַƫ��4byte
	}
	
	return headlength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_ObtainPacketData(u8 *addr)
 @Description			��ȡSocketParkͣ�������ݰ������뻺��
 @Input				*addr		: Socket�����ַ
 @Return				���뻺�����ݳ���
					SOCKET_FALSE   : ����
**********************************************************************************************************/
u16 SOCKET_ParkObtainPacketData(u8 *addr)
{
	u16 datalength = 0;																				//���뻺�����ݳ���
	
	datalength += 13;																				//��ַƫ�Ƴ����ݰ�ͷ
	
	addr[datalength] = SocketParkDataPacket.DeviceType;													//�����豸���
	datalength += 1;																			//��ַƫ��1byte
	
	if ((SOCKET_ConvertInterval((u8 *)&addr[datalength], SocketParkDataPacket.Interval)) == SOCKET_FALSE) {			//����ͳ��ʱ��
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_ConvertLaneNo((u8 *)&addr[datalength], SocketParkDataPacket.LaneNo)) == SOCKET_FALSE) {				//���복����
		return SOCKET_FALSE;
	}
	else {
		datalength += 3;																		//��ַƫ��3byte
	}
	
	if ((SOCKET_ConvertDateTime((u8 *)&addr[datalength], SocketParkDataPacket.DateTime)) == SOCKET_FALSE) {			//������ˮ����ʱ��
		return SOCKET_FALSE;
	}
	else {
		datalength += 20;																		//��ַƫ��20byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume)) == SOCKET_FALSE) {				//����һ��ͨ������
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume1)) == SOCKET_FALSE) {				//����΢С��������
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume2)) == SOCKET_FALSE) {				//����С������
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume3)) == SOCKET_FALSE) {				//�����г�����
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume4)) == SOCKET_FALSE) {				//���������
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume5)) == SOCKET_FALSE) {				//���볬������
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgOccupancy)) == SOCKET_FALSE) {			//����ƽ��ռ��ʱ��
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgHeadTime)) == SOCKET_FALSE) {			//����ƽ����ͷʱ��
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgLength)) == SOCKET_FALSE) {			//����ƽ������
		return SOCKET_FALSE;
	}
	else {
		datalength += 4;																		//��ַƫ��4byte
	}
	
	if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgSpeed)) == SOCKET_FALSE) {			//����ƽ���ٶ�
		return SOCKET_FALSE;
	}
	else {
		datalength += 4;																		//��ַƫ��4byte
	}
	
	addr[datalength] = SocketParkDataPacket.Saturation;													//���뱥�Ͷ�
	datalength += 1;																			//��ַƫ��1byte
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Density)) == SOCKET_FALSE) {				//�����ܶ�
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Pcu)) == SOCKET_FALSE) {				//���뵱��С����
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//��ַƫ��2byte
	}
	
	if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgQueueLength)) == SOCKET_FALSE) {		//�����Ŷӳ���
		return SOCKET_FALSE;
	}
	else {
		datalength += 4;																		//��ַƫ��4byte
	}
	
	datalength -= 13;																			//��ȥ���ݰ�ͷ13byte
	
	return datalength;
}

/********************************************** END OF FLEE **********************************************/
