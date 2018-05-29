/**
  *********************************************************************************************************
  * @file    socketmodulationconfig.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   例化各个外部函数接口
  *********************************************************************************************************
  * @attention
  *		
  *			
  *********************************************************************************************************
  */

#include "socketmodulationconfig.h"
#include "socketmodulationfunc.h"
#include "socketmodulationinstantia.h"
#include "socketmodulationinitialization.h"

/* SocketModulation外部接口数据读取例化各函数 */
struct _m_socket_modulation_dev socket_modulation_dev = 
{
	SOCKET_Modulation_GetHeadFrame,										//例化获取HeadFrame帧头,直接填值(0xAAAA)
	SOCKET_Modulation_GetTailFrame,										//例化获取TailFrame帧尾,直接填值(0xEEEE)
	SOCKET_Modulation_GetCRCCheckCode,										//例化获取CRCCheckCode校验码
	SOCKET_Modulation_GetPacketLength,										//例化获取PacketLength包长
	
	SOCKET_Modulation_GetPacketType,										//例化获取PacketType数据包类型
	SOCKET_Modulation_GetDeviceID,										//例化获取DeviceID设备身份识别码
	SOCKET_Modulation_GetStationID,										//例化获取StationID站点编码
	SOCKET_Modulation_GetDeviceErrCode,									//例化获取DeviceErrCode设备硬件错误码
	SOCKET_Modulation_GetSurveyContent,									//例化获取SurveyContent调查内容
	SOCKET_Modulation_GetDateYear,										//例化获取DateYear年
	SOCKET_Modulation_GetDateMon,											//例化获取DateMon月
	SOCKET_Modulation_GetDateDay,											//例化获取DateDay日
	SOCKET_Modulation_GetProcessCycle,										//例化获取ProcessCycle交通数据处理周期
	SOCKET_Modulation_GetTimeSerial,										//例化获取TimeSerial时间序号
	SOCKET_Modulation_GetLaneNum,											//例化获取LaneNum车道数
	
	SOCKET_Modulation_GetLaneNo,											//例化获取LaneNo车道号
	SOCKET_Modulation_GetFollowCar,										//例化获取FollowCar跟车百分比
	SOCKET_Modulation_GetAvgHeadDistance,									//例化获取AvgHeadDistance平均车头间距
	SOCKET_Modulation_GetTimeOccupancy,									//例化获取TimeOccupancy时间占有率
	
	SOCKET_Modulation_GetSmallVehilceCount,									//例化获取SmallVehilceCount小型车交通流量
	SOCKET_Modulation_GetSmallVehilceAvgSpeed,								//例化获取SmallVehilceAvgSpeed小型车平均地点车速
	
	SOCKET_Modulation_FillData,											//将数据填入各个数据包
	SOCKET_Modulation_ObtainDataFrame,										//读取SocketModulation数据帧并填入缓存
	SOCKET_Modulation_ObtainDataFrameCheckCode,								//计算校验码并填入校验码
	
	SOCKET_Modulation_GetOutputID,										//读取output_ID输出端口的参数到SocketModulation车道数据包
	SOCKET_Modulation_PackagedStorage,										//SocketModulation将数据打包存入缓存区
	
	SOCKET_Modulation_PortSerialInit,										//初始化SocketModulation串口
	SOCKET_Modulation_UARTx_IRQ,											//SocketModulation协议串口中断处理函数
	SOCKET_Modulation_Init,												//初始化SocketModulation协议
	SOCKET_Modulation_Implement,											//SocketModulation协议处理
};

/********************************************** END OF FLEE **********************************************/
