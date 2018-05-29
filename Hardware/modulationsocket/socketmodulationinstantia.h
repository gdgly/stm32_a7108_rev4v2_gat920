#ifndef __SOCKET_MODULATION_TINSTANTIA_H
#define   __SOCKET_MODULATION_TINSTANTIA_H

#include "stm32f10x_lib.h"
#include "socketmodulationconfig.h"

#define	SOCKET_MODULATION_AVGLENGTHSINGLE		5											//单地磁测速默认车长

u16   SOCKET_Modulation_GetHeadFrame(void);													//例化获取HeadFrame帧头,直接填值(0xAAAA)
u16   SOCKET_Modulation_GetTailFrame(void);													//例化获取TailFrame帧尾,直接填值(0xEEEE)
u16   SOCKET_Modulation_GetCRCCheckCode(void);												//例化获取CRCCheckCode校验码
u16   SOCKET_Modulation_GetPacketLength(Socket_Modulation_Data_Frame *addr);						//例化获取PacketLength包长

u8    SOCKET_Modulation_GetPacketType(void);													//例化获取PacketType数据包类型
void  SOCKET_Modulation_GetDeviceID(u8 *deviceID);											//例化获取DeviceID设备身份识别码
void  SOCKET_Modulation_GetStationID(u8 *stationID);											//例化获取StationID站点编码
u8    SOCKET_Modulation_GetDeviceErrCode(void);												//例化获取DeviceErrCode设备硬件错误码
u8    SOCKET_Modulation_GetSurveyContent(void);												//例化获取SurveyContent调查内容
u16   SOCKET_Modulation_GetDateYear(void);													//例化获取DateYear年
u8    SOCKET_Modulation_GetDateMon(void);													//例化获取DateMon月
u8    SOCKET_Modulation_GetDateDay(void);													//例化获取DateDay日
u8    SOCKET_Modulation_GetProcessCycle(void);												//例化获取ProcessCycle交通数据处理周期
u16   SOCKET_Modulation_GetTimeSerial(void);													//例化获取TimeSerial时间序号
u8    SOCKET_Modulation_GetLaneNum(Socket_Modulation_Data_Frame *addr);							//例化获取LaneNum车道数

u8    SOCKET_Modulation_GetLaneNo(Socket_Modulation_Data_Frame *addr, u16 outputid);					//例化获取LaneNo车道号
u8    SOCKET_Modulation_GetFollowCar(Socket_Modulation_Data_Frame *addr, u16 outputid);				//例化获取FollowCar跟车百分比
u16   SOCKET_Modulation_GetAvgHeadDistance(Socket_Modulation_Data_Frame *addr, u16 outputid);			//例化获取AvgHeadDistance平均车头间距
u8    SOCKET_Modulation_GetTimeOccupancy(Socket_Modulation_Data_Frame *addr, u16 outputid);			//例化获取TimeOccupancy时间占有率

u16   SOCKET_Modulation_GetSmallVehilceCount(Socket_Modulation_Data_Frame *addr, u16 outputid);		//例化获取SmallVehilceCount小型车交通流量
u8    SOCKET_Modulation_GetSmallVehilceAvgSpeed(Socket_Modulation_Data_Frame *addr, u16 outputid);		//例化获取SmallVehilceAvgSpeed小型车平均地点车速

void  SOCKET_Modulation_FillData(void);														//将数据填入各个数据包

#endif
