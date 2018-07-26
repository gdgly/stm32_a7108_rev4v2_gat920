/**
  *********************************************************************************************************
  * @file    socketmodulationinstantia.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   SocketModulation�������������ӿ�
  *********************************************************************************************************
  * @attention
  *			���� : 
  *
  *
  *********************************************************************************************************
  */

#include "socketmodulationinstantia.h"
#include "socketmodulationfunc.h"
#include "socketmodulationinitialization.h"
#include "calculationconfig.h"
#include "calculationavgspeed.h"
#include "rtc.h"
#include "string.h"

extern u32 Crossid;

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_FillData(void)
 @Description			����������������ݰ�
 @Input				void
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_FillData(void)
{
	u8 indexA = 0;
	u8 indexB = 0;
	
	SocketModulationDataFrame.HeadFrame				= socket_modulation_dev.GetHeadFrame();
	SocketModulationDataFrame.TailFrame				= socket_modulation_dev.GetTailFrame();
	SocketModulationDataFrame.PacketLength				= socket_modulation_dev.GetPacketLength(&SocketModulationDataFrame);
	SocketModulationDataFrame.CRCCheckCode				= socket_modulation_dev.GetCRCCheckCode();
	
	SocketModulationDataFrame.PacketData.PacketType		= socket_modulation_dev.GetPacketType();
	socket_modulation_dev.GetDeviceID(SocketModulationDataFrame.PacketData.DeviceID);
	socket_modulation_dev.GetStationID(SocketModulationDataFrame.PacketData.StationID);
	SocketModulationDataFrame.PacketData.DeviceErrCode	= socket_modulation_dev.GetDeviceErrCode();
	SocketModulationDataFrame.PacketData.SurveyContent	= socket_modulation_dev.GetSurveyContent();
	SocketModulationDataFrame.PacketData.DateYear		= socket_modulation_dev.GetDateYear();
	SocketModulationDataFrame.PacketData.DateMon			= socket_modulation_dev.GetDateMon();
	SocketModulationDataFrame.PacketData.DateDay			= socket_modulation_dev.GetDateDay();
	SocketModulationDataFrame.PacketData.ProcessCycle		= socket_modulation_dev.GetProcessCycle();
	SocketModulationDataFrame.PacketData.TimeSerial		= socket_modulation_dev.GetTimeSerial();
	SocketModulationDataFrame.PacketData.LaneNum			= socket_modulation_dev.GetLaneNum(&SocketModulationDataFrame);
	
	//���»�ȡ����˳�򲻿ɸı�
	for (indexA = 0; indexA < OUTPUT_MAX; indexA++) {														//������Ѱ���ù��ĳ�����
		if (SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID != 0) {
			if (indexA < SPEEDLANNUMMAX) {															//���ش����ȡ����
				for (indexB = indexA; indexB > 0; indexB--) {											//Ѱ�ұ�������һ��ͬID����
					if (SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID == SocketModulationDataFrame.PacketData.DataLane[indexB - 1].OutputID) {
						SocketModulationDataFrame.PacketData.DataLane[indexA].LaneNo			= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].LaneNo;
						SocketModulationDataFrame.PacketData.DataLane[indexA].FollowCar			= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].FollowCar;
						SocketModulationDataFrame.PacketData.DataLane[indexA].AvgHeadDistance		= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].AvgHeadDistance;
						SocketModulationDataFrame.PacketData.DataLane[indexA].TimeOccupancy		= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].TimeOccupancy;
						SocketModulationDataFrame.PacketData.DataLane[indexA].Avgoccupancyval		= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].Avgoccupancyval;
						SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceCount	= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].SmallVehilceCount;
						SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceAvgSpeed	= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].SmallVehilceAvgSpeed;
						break;
					}
				}
				if (indexB == 0) {																	//�����ݰ���ͬID���ݰ�,��ȡ����
					SocketModulationDataFrame.PacketData.DataLane[indexA].LaneNo = 
					socket_modulation_dev.GetLaneNo(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
					SocketModulationDataFrame.PacketData.DataLane[indexA].FollowCar = 
					socket_modulation_dev.GetFollowCar(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
					SocketModulationDataFrame.PacketData.DataLane[indexA].AvgHeadDistance = 
					socket_modulation_dev.GetAvgHeadDistance(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
					SocketModulationDataFrame.PacketData.DataLane[indexA].TimeOccupancy = 
					socket_modulation_dev.GetTimeOccupancy(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
					SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceCount = 
					socket_modulation_dev.GetSmallVehilceCount(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
					SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceAvgSpeed = 
					socket_modulation_dev.GetSmallVehilceAvgSpeed(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
				}
			}
			else {																				//���شŻ�ȡ����
				if (SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].OutputID != 0) {		//���شŶ�Ӧ���ش�������
					SocketModulationDataFrame.PacketData.DataLane[indexA].LaneNo				= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].LaneNo;
					SocketModulationDataFrame.PacketData.DataLane[indexA].FollowCar			= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].FollowCar;
					SocketModulationDataFrame.PacketData.DataLane[indexA].AvgHeadDistance		= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].AvgHeadDistance;
					SocketModulationDataFrame.PacketData.DataLane[indexA].TimeOccupancy		= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].TimeOccupancy;
					SocketModulationDataFrame.PacketData.DataLane[indexA].Avgoccupancyval		= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].Avgoccupancyval;
					SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceCount	= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].SmallVehilceCount;
					SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceAvgSpeed	= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].SmallVehilceAvgSpeed;
				}
				else {																			//���شŶ�Ӧ���ش�������
					for (indexB = indexA; indexB > SPEEDLANNUMMAX; indexB--) {								//Ѱ�ұ�������һ��ͬID����
						if (SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID == SocketModulationDataFrame.PacketData.DataLane[indexB - 1].OutputID) {
							SocketModulationDataFrame.PacketData.DataLane[indexA].LaneNo
							= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].LaneNo;
							SocketModulationDataFrame.PacketData.DataLane[indexA].FollowCar
							= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].FollowCar;
							SocketModulationDataFrame.PacketData.DataLane[indexA].AvgHeadDistance
							= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].AvgHeadDistance;
							SocketModulationDataFrame.PacketData.DataLane[indexA].TimeOccupancy
							= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].TimeOccupancy;
							SocketModulationDataFrame.PacketData.DataLane[indexA].Avgoccupancyval
							= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].Avgoccupancyval;
							SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceCount
							= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].SmallVehilceCount;
							SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceAvgSpeed
							= SocketModulationDataFrame.PacketData.DataLane[indexB - 1].SmallVehilceAvgSpeed;
							break;
						}
					}
					if (indexB == SPEEDLANNUMMAX) {													//�����ݰ���ͬID���ݰ�,��ȡ����
						SocketModulationDataFrame.PacketData.DataLane[indexA].LaneNo = 
						socket_modulation_dev.GetLaneNo(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
						SocketModulationDataFrame.PacketData.DataLane[indexA].FollowCar = 
						socket_modulation_dev.GetFollowCar(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
						SocketModulationDataFrame.PacketData.DataLane[indexA].AvgHeadDistance = 
						socket_modulation_dev.GetAvgHeadDistance(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
						SocketModulationDataFrame.PacketData.DataLane[indexA].TimeOccupancy = 
						socket_modulation_dev.GetTimeOccupancy(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
						SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceCount = 
						socket_modulation_dev.GetSmallVehilceCount(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
						SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceAvgSpeed = 
						socket_modulation_dev.GetSmallVehilceAvgSpeed(&SocketModulationDataFrame, SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID);
					}
				}
			}
		}
	}
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetHeadFrame(void)
 @Description			������ȡHeadFrame֡ͷ,ֱ����ֵ(0xAAAA)
 @Input				void
 @Return				u16  HeadFrame : 		֡ͷ0xAAAA
**********************************************************************************************************/
u16 SOCKET_Modulation_GetHeadFrame(void)
{
	return SOCKET_MODULATION_HEAD_FRAME;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetTailFrame(void)
 @Description			������ȡTailFrame֡β,ֱ����ֵ(0xEEEE)
 @Input				void
 @Return				u16  TailFrame : 		֡ͷ0xEEEE
**********************************************************************************************************/
u16 SOCKET_Modulation_GetTailFrame(void)
{
	return SOCKET_MODULATION_TAIL_FRAME;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetPacketLength(Socket_Modulation_Data_Frame *addr)
 @Description			������ȡPacketLength����
 @Input				void
 @Return				u16  PacketLength : 		����
**********************************************************************************************************/
u16 SOCKET_Modulation_GetPacketLength(Socket_Modulation_Data_Frame *addr)
{
	u8 i = 0;
	u16 packetlength = 0;
	
	for (i = 0; i < (OUTPUT_MAX / 2); i++) {
		if (addr->PacketData.DataLane[i].OutputID != 0) {
			packetlength += 1;
		}
	}
	
	packetlength *= 23;
	
	packetlength += 42;
	
	return packetlength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetCRCCheckCode(void)
 @Description			������ȡCRCCheckCodeУ����
 @Input				void
 @Return				u16  CRCCheckCode	 : 	У����
**********************************************************************************************************/
u16 SOCKET_Modulation_GetCRCCheckCode(void)
{
	return 0x0000;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetPacketType(void)
 @Description			������ȡPacketType���ݰ�����
 @Input				void
 @Return				u8  PacketType	 : 	���ݰ�����
**********************************************************************************************************/
u8 SOCKET_Modulation_GetPacketType(void)
{
	return SOCKET_MODULATION_PACKET_TYPE_FLOWMESSAGE;
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_GetDeviceID(u8 *deviceID)
 @Description			������ȡDeviceID�豸���ʶ����
 @Input				*deviceID
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_GetDeviceID(u8 *deviceID)
{
	memcpy(deviceID, SOCKET_MODULATION_DEVICE_ID, 16);
	
	if (Crossid > 99999999) {
		Crossid = 99999999;
	}
	
	deviceID[8]  = (Crossid / 10000000) + '0';
	deviceID[9]  = ((Crossid % 10000000) / 1000000) + '0';
	deviceID[10] = ((Crossid % 1000000) / 100000) + '0';
	deviceID[11] = ((Crossid % 100000) / 10000) + '0';
	deviceID[12] = ((Crossid % 10000) / 1000) + '0';
	deviceID[13] = ((Crossid % 1000) / 100) + '0';
	deviceID[14] = ((Crossid % 100) / 10) + '0';
	deviceID[15] = (Crossid % 10) + '0';
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_GetStationID(u8 *stationID)
 @Description			������ȡStationIDվ�����
 @Input				*stationID
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_GetStationID(u8 *stationID)
{
	memcpy(stationID, SOCKET_MODULATION_STATION_ID, 15);
	
	if (Crossid > 99999999) {
		Crossid = 99999999;
	}
	
	stationID[6] = ((Crossid % 1000) / 100) + '0';
	stationID[7] = ((Crossid % 100) / 10) + '0';
	stationID[8] = (Crossid % 10) + '0';
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetDeviceErrCode(void)
 @Description			������ȡDeviceErrCode�豸Ӳ��������
 @Input				void
 @Return				u8  DeviceErrCode	 : 	�豸Ӳ��������
**********************************************************************************************************/
u8 SOCKET_Modulation_GetDeviceErrCode(void)
{
	return SOCKET_MODULATION_DEVICE_ERR_CDOE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetSurveyContent(void)
 @Description			������ȡSurveyContent��������
 @Input				void
 @Return				u8  SurveyContent	 : 	��������
**********************************************************************************************************/
u8 SOCKET_Modulation_GetSurveyContent(void)
{
	return SOCKET_MODULATION_SURVEY_CONTENT;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetDateYear(void)
 @Description			������ȡDateYear��
 @Input				void
 @Return				u16  DateYear	 : 	��
**********************************************************************************************************/
u16 SOCKET_Modulation_GetDateYear(void)
{
	return RTC_Time_GetDateYear();
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetDateMon(void)
 @Description			������ȡDateMon��
 @Input				void
 @Return				u8  DateMon	 : 	��
**********************************************************************************************************/
u8 SOCKET_Modulation_GetDateMon(void)
{
	return RTC_Time_GetDateMon();
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetDateMon(void)
 @Description			������ȡDateDay��
 @Input				void
 @Return				u8  DateDay	 : 	��
**********************************************************************************************************/
u8 SOCKET_Modulation_GetDateDay(void)
{
	return RTC_Time_GetDateDay();
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetProcessCycle(void)
 @Description			������ȡProcessCycle��ͨ���ݴ�������
 @Input				void
 @Return				u8  ProcessCycle	 : 	��ͨ���ݴ�������
**********************************************************************************************************/
u8 SOCKET_Modulation_GetProcessCycle(void)
{
	return SOCKET_MODULATION_PROCESS_CYCLE;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetTimeSerial(void)
 @Description			������ȡTimeSerialʱ�����
 @Input				void
 @Return				u16  TimeSerial	 : 	ʱ�����
**********************************************************************************************************/
u16 SOCKET_Modulation_GetTimeSerial(void)
{
	u16 timeSerial = 0;
	
	if ((RTC_Time_GetTimeHour() * 60 + RTC_Time_GetTimeMin()) == 0) {
		timeSerial = 1440 / SOCKET_MODULATION_PROCESS_CYCLE;
	}
	else if (((RTC_Time_GetTimeHour() * 60 + RTC_Time_GetTimeMin()) % SOCKET_MODULATION_PROCESS_CYCLE) != 0) {
		timeSerial = ((RTC_Time_GetTimeHour() * 60 + RTC_Time_GetTimeMin()) / SOCKET_MODULATION_PROCESS_CYCLE) + 1;
	}
	else {
		timeSerial = ((RTC_Time_GetTimeHour() * 60 + RTC_Time_GetTimeMin()) / SOCKET_MODULATION_PROCESS_CYCLE);
	}
	
	return timeSerial;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetLaneNum(void)
 @Description			������ȡLaneNum������
 @Input				void
 @Return				u8  LaneNum		 : 	������
**********************************************************************************************************/
u8 SOCKET_Modulation_GetLaneNum(Socket_Modulation_Data_Frame *addr)
{
	u8 i = 0;
	u8 lanecount = 0;
	
	for (i = 0; i < (OUTPUT_MAX / 2); i++) {
		if (addr->PacketData.DataLane[i].OutputID != 0) {
			lanecount += 1;
		}
	}
	
	return lanecount;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetLaneNo(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			������ȡLaneNo������
 @Input				void
 @Return				u8  LaneNo		 : 	������
**********************************************************************************************************/
u8 SOCKET_Modulation_GetLaneNo(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	u8 i = 0;
	u8 lanecount = 0;
	u8 index = 0;
	
	for (i = 0; i < (OUTPUT_MAX / 2); i++) {
		if (addr->PacketData.DataLane[i].OutputID != 0) {
			lanecount += 1;
		}
	}
	
	if (lanecount == 1) {
		return 0x01;
	}
	
	for (index = 0; index < (OUTPUT_MAX / 2); index++) {
		if (addr->PacketData.DataLane[index].OutputID == outputid) {
			break;
		}
	}
	
	index += 1;
	
	if (index <= (lanecount / 2)) {
		index += 10;
	}
	else {
		index = index - (lanecount / 2) + 30;
	}
	
	return index;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetFollowCar(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			������ȡFollowCar�����ٷֱ�
 @Input				void
 @Return				u8  FollowCar		 : 	�����ٷֱ�
**********************************************************************************************************/
u8 SOCKET_Modulation_GetFollowCar(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	return SOCKET_MODULATION_FOLLOW_CAR;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetAvgHeadDistance(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			������ȡAvgHeadDistanceƽ����ͷ���
 @Input				void
 @Return				u16  AvgHeadDistance		 : 	ƽ����ͷ���
**********************************************************************************************************/
u16 SOCKET_Modulation_GetAvgHeadDistance(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	return SOCKET_MODULATION_AVG_HEADDISTANCE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetTimeOccupancy(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			������ȡTimeOccupancyʱ��ռ����
 @Input				void
 @Return				u8  TimeOccupancy		 : 	ʱ��ռ����
**********************************************************************************************************/
u8 SOCKET_Modulation_GetTimeOccupancy(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	u32 avgoccupancyval = 0;
	u8 TimeOccupancy = 0;
	u8 i = 0;
	
	avgoccupancyval = calculation_dev.ReadAvgOccupancy(outputid);
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (addr->PacketData.DataLane[i].OutputID == outputid) {
			addr->PacketData.DataLane[i].Avgoccupancyval = avgoccupancyval;
		}
	}
	TimeOccupancy = avgoccupancyval / (SOCKET_MODULATION_PROCESS_CYCLE * 60 * 1000);
	
	return TimeOccupancy;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetSmallVehilceCount(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			������ȡSmallVehilceCountС�ͳ���ͨ����
 @Input				void
 @Return				u16  SmallVehilceCount		 : 	С�ͳ���ͨ����
**********************************************************************************************************/
u16 SOCKET_Modulation_GetSmallVehilceCount(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	u16 vehicleCount = 0;
	
	vehicleCount = calculation_dev.ReadVolume(outputid);
	
	return vehicleCount;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetSmallVehilceAvgSpeed(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			������ȡSmallVehilceAvgSpeedС�ͳ�ƽ���ص㳵��
 @Input				void
 @Return				u8  SmallVehilceAvgSpeed		 : 	С�ͳ�ƽ���ص㳵��
**********************************************************************************************************/
u8 SOCKET_Modulation_GetSmallVehilceAvgSpeed(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	u8 i = 0;
	u8 outputnum = 0;
	u32 avgoccupancyval = 0;
	u8 avgspeedval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {										//��ȡ������ID�����ֵ
		if (addr->PacketData.DataLane[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) 										//�жϵ��شŲ��ٻ���˫�شŲ���
	{
		avgspeedval = calculation_dev.ReadAvgSpeed(outputid);
	}
	else {
		for (i = 0; i < OUTPUT_MAX; i++) {
			if (addr->PacketData.DataLane[i].OutputID == outputid) {
				avgoccupancyval = addr->PacketData.DataLane[i].Avgoccupancyval;
			}
		}
		if (avgoccupancyval != 0) {
			avgspeedval = 3600.0 * SOCKET_MODULATION_AVGLENGTHSINGLE / avgoccupancyval;
		}
	}
	
	return avgspeedval;
}

/********************************************** END OF FLEE **********************************************/
