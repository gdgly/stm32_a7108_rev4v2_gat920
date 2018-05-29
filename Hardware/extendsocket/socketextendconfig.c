/**
  *********************************************************************************************************
  * @file    socketextendconfig.c
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

#include "socketextendconfig.h"
#include "socketextendfunc.h"
#include "socketextendinstantia.h"
#include "socketextendinitialization.h"

/* SocketExtend外部接口数据读取例化各函数 */
struct _m_socket_extend_dev socket_extend_dev = 
{
	SOCKET_Extend_GetStartX,												//获取StartX起始字
	SOCKET_Extend_GetExitX,												//获取ExitX结束字
	SOCKET_Extend_GetServerType,											//获取ServerType业务类型
	SOCKET_Extend_GetCrossID,											//获取CrossID路口代码
	SOCKET_Extend_GetDataLength,											//获取DataLength数据长度
	SOCKET_Extend_GetChannel,											//获取Channel传送组号
	SOCKET_Extend_GetCheckCode,											//获取CheckCode校验码
	SOCKET_Extend_GetDataCount,											//获取DataCount数据条数
	
	SOCKET_Extend_GetRecordNo,											//获取RecordNo流水号
	SOCKET_Extend_GetSendTime,											//获取SendTime上报时间
	SOCKET_Extend_GetVehicleCount,										//例化获取VehicleCount车流量
	SOCKET_Extend_GetLargerVehicleCount,									//例化获取LargerVehicleCount大车流量
	SOCKET_Extend_GetMidVehicleCount,										//例化获取MidVehicleCount中车流量
	SOCKET_Extend_GetSmallVehilceCount,									//例化获取SmallVehilceCount小车流量
	SOCKET_Extend_GetMiniVehicleCount,										//例化获取MiniVehicleCount微型车流量
	SOCKET_Extend_GetMotoVehilceCount,										//例化获取MotoVehilceCount摩托车流量
	SOCKET_Extend_GetReserverType1,										//例化获取ReserverType1预留车型1
	SOCKET_Extend_GetReserverType2,										//例化获取ReserverType2预留车型2
	SOCKET_Extend_GetReserverType3,										//例化获取ReserverType3预留车型3
	SOCKET_Extend_GetReserverType4,										//例化获取ReserverType4预留车型4
	SOCKET_Extend_GetReserverType5,										//例化获取ReserverType5预留车型5
	SOCKET_Extend_GetReserverType6,										//例化获取ReserverType6预留车型6
	SOCKET_Extend_GetMaxHeadInterval,										//例化获取MaxHeadInterval最大车头时距(MS)
	SOCKET_Extend_GetMinHeadInterval,										//例化获取MinHeadInterval最小车头时距(MS)
	SOCKET_Extend_GetAddUpHeadInterval,									//例化获取AddUpHeadInterval累计车头时距(MS)
	SOCKET_Extend_GetMaxSpeed,											//例化获取MaxSpeed最大车速(Km/h)
	SOCKET_Extend_GetMinSpeed,											//例化获取MinSpeed最小车速(Km/h)
	SOCKET_Extend_GetAddUpSpeed,											//例化获取AddUpSpeed累计车速(Km/h)
	SOCKET_Extend_GetMaxOccupancy,										//例化获取MaxOccupancy最大占有时间(MS)
	SOCKET_Extend_GetMinOccupancy,										//例化获取MinOccupancy最小占有时间(MS)
	SOCKET_Extend_GetAddUpOccupancy,										//例化获取AddUpOccupancy累计占有时间(MS)
	SOCKET_Extend_GetMaxInterval,											//例化获取MaxInterval最大间隔时间(MS)
	SOCKET_Extend_GetMinInterval,											//例化获取MinInterval最小间隔时间(MS)
	SOCKET_Extend_GetAddUpInterval,										//例化获取AddUpInterval累计间隔时间(MS)
	SOCKET_Extend_GetMaxVehicleLength,										//例化获取MaxVehicleLength最大车长(米)
	SOCKET_Extend_GetMinVehicleLength,										//例化获取MinVehicleLength最小车长(米)
	SOCKET_Extend_GetAddUpVehilcleLength,									//例化获取AddUpVehilcleLength累计车长(米)
	SOCKET_Extend_GetMaxQueueLength,										//例化获取MaxQueueLength最大排队长度(米)
	SOCKET_Extend_GetMinQueueLength,										//例化获取MinQueueLength最小排队长度(米)
	SOCKET_Extend_GetAddUpQueueLength,										//例化获取AddUpQueueLength累计排队长度(米)
	SOCKET_Extend_GetAddUpQueueTime,										//例化获取AddUpQueueTime累计排队时间(秒)
	SOCKET_Extend_GetRedLightCount,										//例化获取RedLightCount闯红灯触发次数
	SOCKET_Extend_GetPrecedingWayFullTime,									//例化获取PrecedingWayFullTime前方车道满累计时间
	SOCKET_Extend_GetDirverWayCode,										//例化获取DirverWayCode车道代码
	SOCKET_Extend_GetStatus,												//例化获取Status状态(0未启用1启用)
	SOCKET_Extend_GetReserver1,											//例化获取Reserver1预留
	SOCKET_Extend_GetReserver2,											//例化获取Reserver2预留
	
	SOCKET_Extend_FillData,												//将数据填入各个数据包
	SOCKET_Extend_RtcCheckFillData,										//将对时数据填入数据包
	SOCKET_Extend_ObtainPacketHead,										//读取SocketExtend包头数据并填入缓存
	SOCKET_Extend_ObtainPacketData,										//读取SocketExtend流量数据包并填入缓存
	SOCKET_Extend_ObtainPacketCheckCode,									//计算校验码并填入校验码
	
	SOCKET_Extend_GetOutputID,											//读取output_ID输出端口的参数到SocketExtend流量数据包
	SOCKET_Extend_PackagedStorage,										//SocketExtend将数据打包存入缓存区
	SOCKET_Extend_RtcCheckPackagedStorage,									//SocketExtend将对时数据打包存入缓存区
	
	SOCKET_Extend_PortSerialInit,											//初始化SocketExtend串口
	SOCKET_Extend_UARTx_IRQ,												//SocketExtend协议串口中断处理函数
	SOCKET_Extend_Init,													//初始化SocketExtend协议
	SOCKET_Extend_Implement,												//SocketExtend协议处理
};

/********************************************** END OF FLEE **********************************************/
