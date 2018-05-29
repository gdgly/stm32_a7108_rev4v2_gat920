#ifndef __SOCKET_EXTEND_TINSTANTIA_H
#define   __SOCKET_EXTEND_TINSTANTIA_H

#include "stm32f10x_lib.h"
#include "socketextendconfig.h"

#define	SOCKET_EXTEND_AVGLENGTHMIN		3								//平均车长最小值
#define	SOCKET_EXTEND_AVGLENGTHSINGLE		5								//单地磁测速默认车长

u8    SOCKET_Extend_GetStartX(void);										//例化获取StartX起始字值,直接填值(0x88)
u8    SOCKET_Extend_GetExitX(void);										//例化获取ExitX结束字值,直接填值(0x99)
u8    SOCKET_Extend_GetServerType(void);									//例化获取ServerType业务类型值
u32   SOCKET_Extend_GetCrossID(void);										//例化获取CrossID路口代码
u16   SOCKET_Extend_GetDataLength(Socket_Extend_Packet_Data *addr);				//例化获取DataLength数据长度
u16   SOCKET_Extend_GetChannel(void);										//例化获取Channel传送组号值
u8    SOCKET_Extend_GetCheckCode(void);										//例化获取CheckCode校验码值
u8    SOCKET_Extend_GetDataCount(Socket_Extend_Packet_Data *addr);				//例化获取DataCount数据条数

u32   SOCKET_Extend_GetRecordNo(u16 outputid);								//例化获取RecordNo流水号
u32   SOCKET_Extend_GetSendTime(u16 outputid);								//例化获取SendTime上报时间
u16   SOCKET_Extend_GetVehicleCount(u16 outputid);							//例化获取VehicleCount车流量
u16   SOCKET_Extend_GetLargerVehicleCount(u16 outputid);						//例化获取LargerVehicleCount大车流量
u16   SOCKET_Extend_GetMidVehicleCount(u16 outputid);							//例化获取MidVehicleCount中车流量
u16   SOCKET_Extend_GetSmallVehilceCount(u16 outputid);						//例化获取SmallVehilceCount小车流量
u16   SOCKET_Extend_GetMiniVehicleCount(u16 outputid);							//例化获取MiniVehicleCount微型车流量
u16   SOCKET_Extend_GetMotoVehilceCount(u16 outputid);							//例化获取MotoVehilceCount摩托车流量
u16   SOCKET_Extend_GetReserverType1(u16 outputid);							//例化获取ReserverType1预留车型1
u16   SOCKET_Extend_GetReserverType2(u16 outputid);							//例化获取ReserverType2预留车型2
u16   SOCKET_Extend_GetReserverType3(u16 outputid);							//例化获取ReserverType3预留车型3
u16   SOCKET_Extend_GetReserverType4(u16 outputid);							//例化获取ReserverType4预留车型4
u16   SOCKET_Extend_GetReserverType5(u16 outputid);							//例化获取ReserverType5预留车型5
u16   SOCKET_Extend_GetReserverType6(u16 outputid);							//例化获取ReserverType6预留车型6
u32   SOCKET_Extend_GetMaxHeadInterval(u16 outputid);							//例化获取MaxHeadInterval最大车头时距(MS)
u32   SOCKET_Extend_GetMinHeadInterval(u16 outputid);							//例化获取MinHeadInterval最小车头时距(MS)
u32   SOCKET_Extend_GetAddUpHeadInterval(u16 outputid);						//例化获取AddUpHeadInterval累计车头时距(MS)
float SOCKET_Extend_GetMaxSpeed(u16 outputid);								//例化获取MaxSpeed最大车速(Km/h)
float SOCKET_Extend_GetMinSpeed(u16 outputid);								//例化获取MinSpeed最小车速(Km/h)
float SOCKET_Extend_GetAddUpSpeed(u16 outputid);								//例化获取AddUpSpeed累计车速(Km/h)
u32   SOCKET_Extend_GetMaxOccupancy(u16 outputid);							//例化获取MaxOccupancy最大占有时间(MS)
u32   SOCKET_Extend_GetMinOccupancy(u16 outputid);							//例化获取MinOccupancy最小占有时间(MS)
u32   SOCKET_Extend_GetAddUpOccupancy(u16 outputid);							//例化获取AddUpOccupancy累计占有时间(MS)
u32   SOCKET_Extend_GetMaxInterval(u16 outputid);								//例化获取MaxInterval最大间隔时间(MS)
u32   SOCKET_Extend_GetMinInterval(u16 outputid);								//例化获取MinInterval最小间隔时间(MS)
u32   SOCKET_Extend_GetAddUpInterval(u16 outputid);							//例化获取AddUpInterval累计间隔时间(MS)
float SOCKET_Extend_GetMaxVehicleLength(u16 outputid);							//例化获取MaxVehicleLength最大车长(米)
float SOCKET_Extend_GetMinVehicleLength(u16 outputid);							//例化获取MinVehicleLength最小车长(米)
float SOCKET_Extend_GetAddUpVehilcleLength(u16 outputid);						//例化获取AddUpVehilcleLength累计车长(米)
u16   SOCKET_Extend_GetMaxQueueLength(u16 outputid);							//例化获取MaxQueueLength最大排队长度(米)
u16   SOCKET_Extend_GetMinQueueLength(u16 outputid);							//例化获取MinQueueLength最小排队长度(米)
u32   SOCKET_Extend_GetAddUpQueueLength(u16 outputid);							//例化获取AddUpQueueLength累计排队长度(米)
u16   SOCKET_Extend_GetAddUpQueueTime(u16 outputid);							//例化获取AddUpQueueTime累计排队时间(秒)
u16   SOCKET_Extend_GetRedLightCount(u16 outputid);							//例化获取RedLightCount闯红灯触发次数
u16   SOCKET_Extend_GetPrecedingWayFullTime(u16 outputid);						//例化获取PrecedingWayFullTime前方车道满累计时间
u32   SOCKET_Extend_GetDirverWayCode(u16 outputid);							//例化获取DirverWayCode车道代码
u8    SOCKET_Extend_GetStatus(u16 outputid);									//例化获取Status状态(0未启用1启用)
u8    SOCKET_Extend_GetReserver1(u16 outputid);								//例化获取Reserver1预留
u8    SOCKET_Extend_GetReserver2(u16 outputid);								//例化获取Reserver2预留

void  SOCKET_Extend_FillData(void);										//将数据填入各个数据包
void  SOCKET_Extend_RtcCheckFillData(void);									//将对时数据填入数据包

#endif
