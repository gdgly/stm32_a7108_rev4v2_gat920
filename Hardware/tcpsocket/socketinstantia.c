/**
  *********************************************************************************************************
  * @file    socketinstantia.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socket�������������ӿ�
  *********************************************************************************************************
  * @attention
  *			���� : 
  *			1.  ������ȡCrossID·�ڴ���								(�ⲿ����)
  *			2.  ������ȡPacketType��ˮ������							(�ⲿ����)
  *			3.  ������ȡPacketInfo����Ϣ (����ʱ:���ݰ��ڵļ�¼��)			(�ⲿ����)
  *			4.  ������ȡDeviceType�豸����ֵ,�شż����,ֱ����ֵB(0x42)		(�ⲿ����)
  *			5.  ������ȡIntervalͳ��ʱ��								(�ⲿ����)
  *			6.  ������ȡLaneNo������									(�ⲿ����)
  *			7.  ������ȡDateTime��ˮ������ʱ��							(�ⲿ����)
  *			8.  ������ȡVolumeһ������ͨ���ĳ���						(�ⲿ����)
  *			9.  ������ȡVolume1΢С��������							(�ⲿ����)
  *			10. ������ȡVolume2С������								(�ⲿ����)
  *			11. ������ȡVolume3�г�����								(�ⲿ����)
  *			12. ������ȡVolume4������								(�ⲿ����)
  *			13. ������ȡVolume5��������								(�ⲿ����)
  *			14. ������ȡAvgOccupancyƽ��ռ��ʱ��						(�ⲿ����)
  *			15. ������ȡAvgHeadTimeƽ����ͷʱ��						(�ⲿ����)
  *			16. ������ȡAvgLengthƽ������								(�ⲿ����)
  *			17. ������ȡAvgSpeedƽ���ٶ�								(�ⲿ����)
  *			18. ������ȡSaturation���Ͷ�								(�ⲿ����)
  *			19. ������ȡDensity�ܶ�									(�ⲿ����)
  *			20. ������ȡPcu����С����								(�ⲿ����)
  *			21. ������ȡAvgQueueLength�Ŷӳ���							(�ⲿ����)
  *			
  *			22. ����������������ݰ�									(�ⲿ����)
  *
  *********************************************************************************************************
  */

#include "socketinstantia.h"
#include "socketfunc.h"
#include "socketinitialization.h"
#include "calculationconfig.h"
#include "calculationavgspeed.h"


extern u32 Crossid;

/**********************************************************************************************************
 @Function			void SOCKET_FillData(void)
 @Description			����������������ݰ�
 @Input				void
 @Return				void
**********************************************************************************************************/
void SOCKET_FillData(void)
{
	u8 indexA = 0;
	u8 indexB = 0;
	
	SocketHeadPacket.CrossID = socket_dev.GetCrossID();													//д��CrossID
	SocketHeadPacket.PacketType = socket_dev.GetPacketType();												//д��PacketType
	SocketHeadPacket.PacketInfo = socket_dev.GetPacketInfo(SocketDataPacket);									//д��PacketInfo
	
	//���»�ȡ����˳�򲻿ɸı�
	for (indexA = 0; indexA < OUTPUT_MAX; indexA++) {														//������Ѱ���ù��ĳ�����
		if (SocketDataPacket[indexA].OutputID != 0) {
			if (indexA < SPEEDLANNUMMAX) {															//���ش����ȡ����
				for (indexB = indexA; indexB > 0; indexB--) {											//Ѱ�ұ�������һ��ͬID����
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
				if (indexB == 0) {																	//�����ݰ���ͬID���ݰ�,��ȡ����
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
			else {																				//���شŻ�ȡ����
				if (SocketDataPacket[indexA - SPEEDLANNUMMAX].OutputID != 0) {								//���شŶ�Ӧ���ش�������
					SocketDataPacket[indexA].DeviceType = SocketDataPacket[indexA - SPEEDLANNUMMAX].DeviceType;				//д��DeviceType
					SocketDataPacket[indexA].Interval = SocketDataPacket[indexA - SPEEDLANNUMMAX].Interval;					//д��Interval
					SocketDataPacket[indexA].LaneNo = indexA + 1;													//д��LaneNo
					SocketDataPacket[indexA].DateTime = SocketDataPacket[indexA - SPEEDLANNUMMAX].DateTime;					//д��DateTime
					SocketDataPacket[indexA].Volume = SocketDataPacket[indexA - SPEEDLANNUMMAX].Volume;						//д��Volume
					SocketDataPacket[indexA].Volume1 = SocketDataPacket[indexA - SPEEDLANNUMMAX].Volume1;					//д��Volume1
					SocketDataPacket[indexA].Volume2 = SocketDataPacket[indexA - SPEEDLANNUMMAX].Volume2;					//д��Volume2
					SocketDataPacket[indexA].Volume3 = SocketDataPacket[indexA - SPEEDLANNUMMAX].Volume3;					//д��Volume3
					SocketDataPacket[indexA].Volume4 = SocketDataPacket[indexA - SPEEDLANNUMMAX].Volume4;					//д��Volume4
					SocketDataPacket[indexA].Volume5 = SocketDataPacket[indexA - SPEEDLANNUMMAX].Volume5;					//д��Volume5
					SocketDataPacket[indexA].AvgOccupancy = SocketDataPacket[indexA - SPEEDLANNUMMAX].AvgOccupancy;			//д��AvgOccupancy
					SocketDataPacket[indexA].AvgHeadTime = SocketDataPacket[indexA - SPEEDLANNUMMAX].AvgHeadTime;				//д��AvgHeadTime
					SocketDataPacket[indexA].AvgSpeed = SocketDataPacket[indexA - SPEEDLANNUMMAX].AvgSpeed;					//д��AvgSpeed
					SocketDataPacket[indexA].AvgLength = SocketDataPacket[indexA - SPEEDLANNUMMAX].AvgLength;					//д��AvgLength
					SocketDataPacket[indexA].Saturation = SocketDataPacket[indexA - SPEEDLANNUMMAX].Saturation;				//д��Saturation
					SocketDataPacket[indexA].Density = SocketDataPacket[indexA - SPEEDLANNUMMAX].Density;					//д��Density
					SocketDataPacket[indexA].Pcu = SocketDataPacket[indexA - SPEEDLANNUMMAX].Pcu;							//д��Pcu
					SocketDataPacket[indexA].AvgQueueLength = SocketDataPacket[indexA - SPEEDLANNUMMAX].AvgQueueLength;			//д��AvgQueueLength
				}
				else {																			//���شŶ�Ӧ���ش�������
					for (indexB = indexA; indexB > SPEEDLANNUMMAX; indexB--) {								//Ѱ�ұ�������һ��ͬID����
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
					if (indexB == SPEEDLANNUMMAX) {													//�����ݰ���ͬID���ݰ�,��ȡ����
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
	}
}

/**********************************************************************************************************
 @Function			u32 SOCKET_FillCrossID(void)
 @Description			������ȡCrossID·�ڴ���
 @Input				
 @Return				u32 CrossID ֵ
**********************************************************************************************************/
u32 SOCKET_GetCrossID(void)
{
	if (Crossid > 99999999) {
		Crossid = 99999999;
	}
	
	return Crossid;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_GatPacketType(void)
 @Description			������ȡPacketType��ˮ������
 @Input				
 @Return				u8 PacketType ֵ
**********************************************************************************************************/
u8 SOCKET_GetPacketType(void)
{
	if (SOCKET_RTC_CHECK == PACKETTYPE_RTCCHECKINIT) {						//�ж��Ƿ���ҪRTC�ϵ��ʱУ��
		if (PlatformSockettime == SocketTime_DISABLE) {						//����SNѡ���Ƿ��ʱ
			return PACKETTYPE_FLOWMESSAGE;								//������ͨ��ͨ������Ϣ
		}
		else {
			return PACKETTYPE_RTCCHECKINIT;								//����RTC�ϵ��ʱУ������
		}
	}
	else if (SOCKET_RTC_CHECK == PACKETTYPE_RTCCHECK) {						//�ж��Ƿ���ҪRTC���ж�ʱУ��
		if (PlatformSockettime == SocketTime_DISABLE) {						//����SNѡ���Ƿ��ʱ
			return PACKETTYPE_FLOWMESSAGE;								//������ͨ��ͨ������Ϣ
		}
		else {
			return PACKETTYPE_RTCCHECK;									//����RTC���ж�ʱУ������
		}
	}
	else {
		return PACKETTYPE_FLOWMESSAGE;									//������ͨ��ͨ������Ϣ
	}
}

/**********************************************************************************************************
 @Function			u32 SOCKET_GatPacketInfo(Socket_Packet_Data *addr)
 @Description			������ȡPacketInfo����Ϣ (����ʱ:���ݰ��ڵļ�¼��)
 @Input				Socket_Packet_Data *addr 	: Socket�������ݰ�����ַ
 @Return				u32 PacketInfo ֵ
**********************************************************************************************************/
u32 SOCKET_GetPacketInfo(Socket_Packet_Data *addr)
{
	u8 i = 0;
	u32 outputnum = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (addr[i].OutputID != 0) {
			outputnum += 1;
		}
	}
	
	return outputnum;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_GatDeviceType(u16 outputid)
 @Description			������ȡDeviceType�豸����ֵ,�شż����,ֱ����ֵB(0x42)
 @Input				u16 outputid   : 		����ID��
 @Return				u8  DeviceType : 		�ó���DeviceTypeֵ
**********************************************************************************************************/
u8 SOCKET_GetDeviceType(u16 outputid)
{
	return DEVICETYPE_DICI;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatInterval(u16 outputid)
 @Description			������ȡIntervalͳ��ʱ��
 @Input				u16 outputid   : 		����ID��
 @Return				u16 Interval	: 		�ó���Intervalֵ
**********************************************************************************************************/
u16 SOCKET_GetInterval(u16 outputid)
{
	return INTERVALTIME;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatInterval(u16 outputid)
 @Description			������ȡLaneNo������
 @Input				u16 outputid   : 		����ID��
 @Return				u16 LaneNo	: 		�ó���LaneNoֵ
**********************************************************************************************************/
u16 SOCKET_GetLaneNo(u16 outputid)
{
	u8 i = 0;
	u16 laneno = 1;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (SocketDataPacket[i].OutputID == outputid) {
			laneno = i + 1;
			break;
		}
	}
	
	return laneno;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_GatDateTime(u16 outputid)
 @Description			������ȡDateTime��ˮ������ʱ��
 @Input				u16 outputid   : 		����ID��
 @Return				u32 DateTime	: 		�ó���DateTimeֵ
**********************************************************************************************************/
u32 SOCKET_GetDateTime(u16 outputid)
{
	u32 datetime = 0;
	
	datetime = RTC_GetCounter();
	
	return datetime;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatVolume(u16 outputid)
 @Description			������ȡVolumeһ������ͨ���ĳ���
 @Input				u16 outputid   : 		����ID��
 @Return				u16 Volume	: 		�ó���Volumeֵ
**********************************************************************************************************/
u16 SOCKET_GetVolume(u16 outputid)
{
	u16 volumeval = 0;
	
	volumeval = calculation_dev.ReadVolume(outputid);
	
	return volumeval;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatVolume1(u16 outputid)
 @Description			������ȡVolume1΢С��������
 @Input				u16 outputid   : 		����ID��
 @Return				u16 Volume1	: 		�ó���Volume1ֵ
**********************************************************************************************************/
u16 SOCKET_GetVolume1(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatVolume2(u16 outputid)
 @Description			������ȡVolume2С������
 @Input				u16 outputid   : 		����ID��
 @Return				u16 Volume2	: 		�ó���Volume2ֵ
**********************************************************************************************************/
u16 SOCKET_GetVolume2(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatVolume3(u16 outputid)
 @Description			������ȡVolume3�г�����
 @Input				u16 outputid   : 		����ID��
 @Return				u16 Volume3	: 		�ó���Volume3ֵ
**********************************************************************************************************/
u16 SOCKET_GetVolume3(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatVolume4(u16 outputid)
 @Description			������ȡVolume4������
 @Input				u16 outputid   : 		����ID��
 @Return				u16 Volume4	: 		�ó���Volume4ֵ
**********************************************************************************************************/
u16 SOCKET_GetVolume4(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatVolume5(u16 outputid)
 @Description			������ȡVolume5��������
 @Input				u16 outputid   : 		����ID��
 @Return				u16 Volume5	: 		�ó���Volume5ֵ
**********************************************************************************************************/
u16 SOCKET_GetVolume5(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatAvgOccupancy(u16 outputid)
 @Description			������ȡAvgOccupancyƽ��ռ��ʱ��
 @Input				u16 outputid   	: 		����ID��
 @Return				u16 AvgOccupancy	: 		�ó���AvgOccupancyֵ
**********************************************************************************************************/
u16 SOCKET_GetAvgOccupancy(u16 outputid)
{
	u16 avgoccupancyval = 0;
	
	avgoccupancyval = calculation_dev.ReadAvgOccupancy(outputid);
	
	return avgoccupancyval;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatAvgHeadTime(u16 outputid)
 @Description			������ȡAvgHeadTimeƽ����ͷʱ��
 @Input				u16 outputid   	: 		����ID��
 @Return				u16 AvgHeadTime	: 		�ó���AvgHeadTimeֵ
**********************************************************************************************************/
u16 SOCKET_GetAvgHeadTime(u16 outputid)
{
	u16 avgheadtimeval = 0;
	
	avgheadtimeval = calculation_dev.ReadAvgHeadTime(outputid);
	
	return avgheadtimeval;
}

/**********************************************************************************************************
 @Function			float SOCKET_GatAvgLength(u16 outputid)
 @Description			������ȡAvgLengthƽ������
					��Ϊ���شŲ���ģʽ �����ɺ궨������,���ɸı�
					��Ϊ˫�شŲ���ģʽ �������ٶ���ռ��ʱ�����
 @Input				u16 outputid   	: 		����ID��
 @Return				u16 AvgLength		: 		�ó���AvgLengthֵ
**********************************************************************************************************/
float SOCKET_GetAvgLength(u16 outputid)
{
	u8 i = 0;
	u8 outputnum = 0;
	float avglengthval = 0;
	float avgspeedval = 0;
	u16 avgoccupancyval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {										//��ȡ������ID�����ֵ
		if (SocketDataPacket[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) 										//�жϵ��شŲ��ٻ���˫�شŲ���
	{
		for (i = 0; i < OUTPUT_MAX; i++) {
			if (SocketDataPacket[i].OutputID == outputid) {
				if (i < SPEEDLANNUMMAX) {
					avgspeedval = SocketDataPacket[i].AvgSpeed;
					avgoccupancyval = SocketDataPacket[i].AvgOccupancy;
				}
				else {
					avgspeedval = SocketDataPacket[i - SPEEDLANNUMMAX].AvgSpeed;
					avgoccupancyval = SocketDataPacket[i - SPEEDLANNUMMAX].AvgOccupancy;
				}
			}
		}
		
		avgspeedval /= 3.6;
		avglengthval = avgspeedval * avgoccupancyval;
		avglengthval /= 1000;
		
		if ((avglengthval < AVGLENGTHMIN) && (avglengthval > 0.1)) {
			avglengthval = AVGLENGTHMIN;
		}
	}
	else {
		avglengthval = AVGLENGTHSINGLE;
	}
	
	return avglengthval;
}

/**********************************************************************************************************
 @Function			float SOCKET_GatAvgSpeed(u16 outputid)
 @Description			������ȡAvgSpeedƽ���ٶ�
					��Ϊ���شŲ���,����ֵ��Ĭ�ϳ�����ռ��ʱ�����
					��Ϊ˫�شŲ���,����ֵ��˫�ش����ݻ�ȡ
 @Input				u16 outputid   	: 		����ID��
 @Return				u16 AvgSpeed		: 		�ó���AvgSpeedֵ
**********************************************************************************************************/
float SOCKET_GetAvgSpeed(u16 outputid)
{
	u8 i = 0;
	u8 outputnum = 0;
	u32 avgoccupancyval = 0;
	float avgspeedval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {										//��ȡ������ID�����ֵ
		if (SocketDataPacket[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) 										//�жϵ��شŲ��ٻ���˫�شŲ���
	{
		avgspeedval = calculation_dev.ReadAvgSpeed(outputid);
	}
	else {
		for (i = 0; i < OUTPUT_MAX; i++) {
			if (SocketDataPacket[i].OutputID == outputid) {
				avgoccupancyval = SocketDataPacket[i].AvgOccupancy;
			}
		}
		if (avgoccupancyval != 0) {
			avgspeedval = 3600.0 * AVGLENGTHSINGLE / avgoccupancyval;
		}
	}
	
	return avgspeedval;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_GatSaturation(u16 outputid)
 @Description			������ȡSaturation���Ͷ�
 @Input				u16 outputid   	: 		����ID��
 @Return				u16 Saturation		: 		�ó���Saturationֵ
**********************************************************************************************************/
u8 SOCKET_GetSaturation(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatDensity(u16 outputid)
 @Description			������ȡDensity�ܶ�
 @Input				u16 outputid   	: 		����ID��
 @Return				u16 Density		: 		�ó���Densityֵ
**********************************************************************************************************/
u16 SOCKET_GetDensity(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_GatPcu(u16 outputid)
 @Description			������ȡPcu����С����
 @Input				u16 outputid   	: 		����ID��
 @Return				u16 Pcu			: 		�ó���Pcuֵ
**********************************************************************************************************/
u16 SOCKET_GetPcu(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_GatAvgQueueLength(u16 outputid)
 @Description			������ȡAvgQueueLength�Ŷӳ���
 @Input				u16 outputid   			: 		����ID��
 @Return				u16 AvgQueueLength			: 		�ó���AvgQueueLengthֵ
**********************************************************************************************************/
float SOCKET_GetAvgQueueLength(u16 outputid)
{
	return 0;
}

/********************************************** END OF FLEE **********************************************/
