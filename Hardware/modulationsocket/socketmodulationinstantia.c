/**
  *********************************************************************************************************
  * @file    socketmodulationinstantia.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   SocketModulation各个数据例化接口
  *********************************************************************************************************
  * @attention
  *			功能 : 
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
 @Description			将数据填入各个数据包
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
	
	//以下获取数据顺序不可改变
	for (indexA = 0; indexA < OUTPUT_MAX; indexA++) {														//遍历找寻配置过的车道号
		if (SocketModulationDataFrame.PacketData.DataLane[indexA].OutputID != 0) {
			if (indexA < SPEEDLANNUMMAX) {															//主地磁需获取数据
				for (indexB = indexA; indexB > 0; indexB--) {											//寻找本包与上一包同ID数据
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
				if (indexB == 0) {																	//本数据包无同ID数据包,获取数据
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
			else {																				//辅地磁获取数据
				if (SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].OutputID != 0) {		//辅地磁对应主地磁有配置
					SocketModulationDataFrame.PacketData.DataLane[indexA].LaneNo				= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].LaneNo;
					SocketModulationDataFrame.PacketData.DataLane[indexA].FollowCar			= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].FollowCar;
					SocketModulationDataFrame.PacketData.DataLane[indexA].AvgHeadDistance		= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].AvgHeadDistance;
					SocketModulationDataFrame.PacketData.DataLane[indexA].TimeOccupancy		= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].TimeOccupancy;
					SocketModulationDataFrame.PacketData.DataLane[indexA].Avgoccupancyval		= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].Avgoccupancyval;
					SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceCount	= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].SmallVehilceCount;
					SocketModulationDataFrame.PacketData.DataLane[indexA].SmallVehilceAvgSpeed	= SocketModulationDataFrame.PacketData.DataLane[indexA - SPEEDLANNUMMAX].SmallVehilceAvgSpeed;
				}
				else {																			//辅地磁对应主地磁无配置
					for (indexB = indexA; indexB > SPEEDLANNUMMAX; indexB--) {								//寻找本包与上一包同ID数据
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
					if (indexB == SPEEDLANNUMMAX) {													//本数据包无同ID数据包,获取数据
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
 @Description			例化获取HeadFrame帧头,直接填值(0xAAAA)
 @Input				void
 @Return				u16  HeadFrame : 		帧头0xAAAA
**********************************************************************************************************/
u16 SOCKET_Modulation_GetHeadFrame(void)
{
	return SOCKET_MODULATION_HEAD_FRAME;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetTailFrame(void)
 @Description			例化获取TailFrame帧尾,直接填值(0xEEEE)
 @Input				void
 @Return				u16  TailFrame : 		帧头0xEEEE
**********************************************************************************************************/
u16 SOCKET_Modulation_GetTailFrame(void)
{
	return SOCKET_MODULATION_TAIL_FRAME;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetPacketLength(Socket_Modulation_Data_Frame *addr)
 @Description			例化获取PacketLength包长
 @Input				void
 @Return				u16  PacketLength : 		包长
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
 @Description			例化获取CRCCheckCode校验码
 @Input				void
 @Return				u16  CRCCheckCode	 : 	校验码
**********************************************************************************************************/
u16 SOCKET_Modulation_GetCRCCheckCode(void)
{
	return 0x0000;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetPacketType(void)
 @Description			例化获取PacketType数据包类型
 @Input				void
 @Return				u8  PacketType	 : 	数据包类型
**********************************************************************************************************/
u8 SOCKET_Modulation_GetPacketType(void)
{
	return SOCKET_MODULATION_PACKET_TYPE_FLOWMESSAGE;
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_GetDeviceID(u8 *deviceID)
 @Description			例化获取DeviceID设备身份识别码
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
 @Description			例化获取StationID站点编码
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
 @Description			例化获取DeviceErrCode设备硬件错误码
 @Input				void
 @Return				u8  DeviceErrCode	 : 	设备硬件错误码
**********************************************************************************************************/
u8 SOCKET_Modulation_GetDeviceErrCode(void)
{
	return SOCKET_MODULATION_DEVICE_ERR_CDOE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetSurveyContent(void)
 @Description			例化获取SurveyContent调查内容
 @Input				void
 @Return				u8  SurveyContent	 : 	调查内容
**********************************************************************************************************/
u8 SOCKET_Modulation_GetSurveyContent(void)
{
	return SOCKET_MODULATION_SURVEY_CONTENT;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetDateYear(void)
 @Description			例化获取DateYear年
 @Input				void
 @Return				u16  DateYear	 : 	年
**********************************************************************************************************/
u16 SOCKET_Modulation_GetDateYear(void)
{
	return RTC_Time_GetDateYear();
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetDateMon(void)
 @Description			例化获取DateMon月
 @Input				void
 @Return				u8  DateMon	 : 	月
**********************************************************************************************************/
u8 SOCKET_Modulation_GetDateMon(void)
{
	return RTC_Time_GetDateMon();
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetDateMon(void)
 @Description			例化获取DateDay日
 @Input				void
 @Return				u8  DateDay	 : 	日
**********************************************************************************************************/
u8 SOCKET_Modulation_GetDateDay(void)
{
	return RTC_Time_GetDateDay();
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetProcessCycle(void)
 @Description			例化获取ProcessCycle交通数据处理周期
 @Input				void
 @Return				u8  ProcessCycle	 : 	交通数据处理周期
**********************************************************************************************************/
u8 SOCKET_Modulation_GetProcessCycle(void)
{
	return SOCKET_MODULATION_PROCESS_CYCLE;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetTimeSerial(void)
 @Description			例化获取TimeSerial时间序号
 @Input				void
 @Return				u16  TimeSerial	 : 	时间序号
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
 @Description			例化获取LaneNum车道数
 @Input				void
 @Return				u8  LaneNum		 : 	车道数
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
 @Description			例化获取LaneNo车道号
 @Input				void
 @Return				u8  LaneNo		 : 	车道号
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
 @Description			例化获取FollowCar跟车百分比
 @Input				void
 @Return				u8  FollowCar		 : 	跟车百分比
**********************************************************************************************************/
u8 SOCKET_Modulation_GetFollowCar(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	return SOCKET_MODULATION_FOLLOW_CAR;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_GetAvgHeadDistance(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			例化获取AvgHeadDistance平均车头间距
 @Input				void
 @Return				u16  AvgHeadDistance		 : 	平均车头间距
**********************************************************************************************************/
u16 SOCKET_Modulation_GetAvgHeadDistance(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	return SOCKET_MODULATION_AVG_HEADDISTANCE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetTimeOccupancy(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			例化获取TimeOccupancy时间占有率
 @Input				void
 @Return				u8  TimeOccupancy		 : 	时间占有率
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
 @Description			例化获取SmallVehilceCount小型车交通流量
 @Input				void
 @Return				u16  SmallVehilceCount		 : 	小型车交通流量
**********************************************************************************************************/
u16 SOCKET_Modulation_GetSmallVehilceCount(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	u16 vehicleCount = 0;
	
	vehicleCount = calculation_dev.ReadVolume(outputid);
	
	return vehicleCount;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_GetSmallVehilceAvgSpeed(Socket_Modulation_Data_Frame *addr, u16 outputid)
 @Description			例化获取SmallVehilceAvgSpeed小型车平均地点车速
 @Input				void
 @Return				u8  SmallVehilceAvgSpeed		 : 	小型车平均地点车速
**********************************************************************************************************/
u8 SOCKET_Modulation_GetSmallVehilceAvgSpeed(Socket_Modulation_Data_Frame *addr, u16 outputid)
{
	u8 i = 0;
	u8 outputnum = 0;
	u32 avgoccupancyval = 0;
	u8 avgspeedval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {										//获取已配置ID号最大值
		if (addr->PacketData.DataLane[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) 										//判断单地磁测速还是双地磁测速
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
