#ifndef __SOCKET_EXTEND_CONFIG_H
#define   __SOCKET_EXTEND_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

#define	SOCKET_EXTEND_CACHE_SIZE				4096					//4K协议缓存, 32路车道需3632Byte
#define	SOCKET_EXTEND_RECVIVE_SIZE			30					//接收缓存,数据包头17Byte

#define	SOCKET_EXTEND_AGAINSENDTIME			5					//5秒后没收到回包重发数据

#define	SOCKET_EXTEND_TRUE					0
#define	SOCKET_EXTEND_FALSE					1

#define	SOCKET_EXTEND_STARTX				0x88					//起始字(0x88)
#define	SOCKET_EXTEND_EXITX					0x99					//结束字(0x99)

#define	SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE	0x01					//RTC不对时
#define	SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT	0x04					//RTC对时

#define	SOCKET_EXTEND_SERVERTYPE_CLIENT		0xC0					//业务类型 客户端
#define	SOCKET_EXTEND_SERVERTYPE_CROSS		0x40					//业务类型 路口路段
#define	SOCKET_EXTEND_SERVERTYPE_SIGNAL		0x80					//业务类型 信号控制器
#define	SOCKET_EXTEND_SERVERTYPE_SERVER		0x00					//业务类型 服务器

#define	SOCKET_EXTEND_CHANNEL				0x0000				//传送组号

#define	SOCKET_EXTEND_STATUS_DISABLE			0x00					//状态(0未启用)
#define	SOCKET_EXTEND_STATUS_ENABLE			0x01					//状态(1启用)

/**********************************************************************************************************
 @attention : 
 * Socket Extend 包头   ( STX, TYPE, HSC, DL, CN, ETX, CRC, COUNT ) 											共 17 Byte
 * Socket Extend 流量包 ( RecordNo, SendTime, VehicleCount, LargerVehicleCount, MidVehicleCount, SmallVehilceCount,
 * 					 MiniVehicleCount, Moto, ReserverType1, ReserverType2, ReserverType3, ReserverType4, 
 *					 ReserverType5, ReserverType6, MaxHeadInterval, MinHeadInterval, AddUpHeadInterval, 
 *					 MaxSpeed, MinSpeed, AddUpSpeed, MaxOccupancy, MinOccupancy, AddUpOccupancy, 
 *					 MaxInterval, MinInterval, AddUpInterval, MaxVehicleLength, MinVehicleLength, 
 *					 AddUpVehilcleLength, MaxQueueLength, MinQueueLength, AddUpQueueLength, AddUpQueueTime, 
 *					 RedLightCount, PrecedingWayFullTime, DirverWayCode, Status, Reserver1, Reserver2)			共113Byte
 *
**********************************************************************************************************/
typedef struct
{
	u8		StartX;											//起始字(0x88)
	u8		ServerType;										//业务类型(0x41, 低4位0x01, 高4位:客户端0xc0;路口路段0x40;信号控制器0x80;服务器0x00)
	u32		CrossID;											//路口代码
	u16		DataLength;										//数据长度
	u16		Channel;											//传送组号
	u8		ExitX;											//结束字(0x99)
	u8		CheckCode;										//校验码
	u8		DataCount;										//数据条数
}Socket_Extend_Packet_Head;										//数据包头

typedef struct
{
	u16		OutputID;											//输出端口的参数
	u32		RecordNo;											//流水号
	u32		SendTime;											//上报时间
	
	u16		VehicleCount;										//车流量(一分钟通过的车流量)
	u16		LargerVehicleCount;									//大车流量(一分钟通过的大车流量)
	u16		MidVehicleCount;									//中车流量(一分钟通过的中车流量)
	u16		SmallVehilceCount;									//小车流量(一分钟通过的小车流量)
	u16		MiniVehicleCount;									//微型车流量(一分钟通过的微型车流量)
	u16		Moto;											//摩托车流量(一分钟通过的摩托车流量)
	
	u16		ReserverType1;										//预留车型1
	u16		ReserverType2;										//预留车型2
	u16		ReserverType3;										//预留车型3
	u16		ReserverType4;										//预留车型4
	u16		ReserverType5;										//预留车型5
	u16		ReserverType6;										//预留车型6
	
	u32		MaxHeadInterval;									//最大车头时距(MS)
	u32		MinHeadInterval;									//最小车头时距(MS)
	u32		AddUpHeadInterval;									//累计车头时距(MS)
	
	float	MaxSpeed;											//最大车速(Km/h)
	float	MinSpeed;											//最小车速(Km/h)
	float	AddUpSpeed;										//累计车速(Km/h)
	
	u32		MaxOccupancy;										//最大占有时间(MS)
	u32		MinOccupancy;										//最小占有时间(MS)
	u32		AddUpOccupancy;									//累计占有时间(MS)
	
	u32		MaxInterval;										//最大间隔时间(MS)
	u32		MinInterval;										//最小间隔时间(MS)
	u32		AddUpInterval;										//累计间隔时间(MS)
	
	float	MaxVehicleLength;									//最大车长(米)
	float	MinVehicleLength;									//最小车长(米)
	float	AddUpVehilcleLength;								//累计车长(米)
	
	u16		MaxQueueLength;									//最大排队长度(米)
	u16		MinQueueLength;									//最小排队长度(米)
	u32		AddUpQueueLength;									//累计排队长度(米)
	
	u16		AddUpQueueTime;									//累计排队时间(秒)
	u16		RedLightCount;										//闯红灯触发次数
	u16		PrecedingWayFullTime;								//前方车道满累计时间
	u32		DirverWayCode;										//车道代码
	u8		Status;											//状态(0未启用1启用)
	u8		Reserver1;										//预留
	u8		Reserver2;										//预留
}Socket_Extend_Packet_Data;										//流量数据包

/* SocketExtend外部接口数据读取控制管理器 */
struct _m_socket_extend_dev
{
	u8		(*GetStartX)(void);									//获取StartX起始字
	u8		(*GetExitX)(void);									//获取ExitX结束字
	u8		(*GetServerType)(void);								//获取ServerType业务类型
	u32		(*GetCrossID)(void);								//获取CrossID路口代码
	u16		(*GetDataLength)(Socket_Extend_Packet_Data *addr);		//获取DataLength数据长度
	u16		(*GetChannel)(void);								//获取Channel传送组号
	u8		(*GetCheckCode)(void);								//获取CheckCode校验码
	u8		(*GetDataCount)(Socket_Extend_Packet_Data *addr);			//获取DataCount数据条数
	
	u32		(*GetRecordNo)(u16);								//获取RecordNo流水号
	u32		(*GetSendTime)(u16);								//获取SendTime上报时间
	u16		(*GetVehicleCount)(u16);								//例化获取VehicleCount车流量
	u16		(*GetLargerVehicleCount)(u16);						//例化获取LargerVehicleCount大车流量
	u16		(*GetMidVehicleCount)(u16);							//例化获取MidVehicleCount中车流量
	u16		(*GetSmallVehilceCount)(u16);							//例化获取SmallVehilceCount小车流量
	u16		(*GetMiniVehicleCount)(u16);							//例化获取MiniVehicleCount微型车流量
	u16		(*GetMotoVehilceCount)(u16);							//例化获取MotoVehilceCount摩托车流量
	u16		(*GetReserverType1)(u16);							//例化获取ReserverType1预留车型1
	u16		(*GetReserverType2)(u16);							//例化获取ReserverType2预留车型2
	u16		(*GetReserverType3)(u16);							//例化获取ReserverType3预留车型3
	u16		(*GetReserverType4)(u16);							//例化获取ReserverType4预留车型4
	u16		(*GetReserverType5)(u16);							//例化获取ReserverType5预留车型5
	u16		(*GetReserverType6)(u16);							//例化获取ReserverType6预留车型6
	u32		(*GetMaxHeadInterval)(u16);							//例化获取MaxHeadInterval最大车头时距(MS)
	u32		(*GetMinHeadInterval)(u16);							//例化获取MinHeadInterval最小车头时距(MS)
	u32		(*GetAddUpHeadInterval)(u16);							//例化获取AddUpHeadInterval累计车头时距(MS)
	float	(*GetMaxSpeed)(u16);								//例化获取MaxSpeed最大车速(Km/h)
	float	(*GetMinSpeed)(u16);								//例化获取MinSpeed最小车速(Km/h)
	float	(*GetAddUpSpeed)(u16);								//例化获取AddUpSpeed累计车速(Km/h)
	u32		(*GetMaxOccupancy)(u16);								//例化获取MaxOccupancy最大占有时间(MS)
	u32		(*GetMinOccupancy)(u16);								//例化获取MinOccupancy最小占有时间(MS)
	u32		(*GetAddUpOccupancy)(u16);							//例化获取AddUpOccupancy累计占有时间(MS)
	u32		(*GetMaxInterval)(u16);								//例化获取MaxInterval最大间隔时间(MS)
	u32		(*GetMinInterval)(u16);								//例化获取MinInterval最小间隔时间(MS)
	u32		(*GetAddUpInterval)(u16);							//例化获取AddUpInterval累计间隔时间(MS)
	float	(*GetMaxVehicleLength)(u16);							//例化获取MaxVehicleLength最大车长(米)
	float	(*GetMinVehicleLength)(u16);							//例化获取MinVehicleLength最小车长(米)
	float	(*GetAddUpVehilcleLength)(u16);						//例化获取AddUpVehilcleLength累计车长(米)
	u16		(*GetMaxQueueLength)(u16);							//例化获取MaxQueueLength最大排队长度(米)
	u16		(*GetMinQueueLength)(u16);							//例化获取MinQueueLength最小排队长度(米)
	u32		(*GetAddUpQueueLength)(u16);							//例化获取AddUpQueueLength累计排队长度(米)
	u16		(*GetAddUpQueueTime)(u16);							//例化获取AddUpQueueTime累计排队时间(秒)
	u16		(*GetRedLightCount)(u16);							//例化获取RedLightCount闯红灯触发次数
	u16		(*GetPrecedingWayFullTime)(u16);						//例化获取PrecedingWayFullTime前方车道满累计时间
	u32		(*GetDirverWayCode)(u16,u16);							//例化获取DirverWayCode车道代码
	u8		(*GetStatus)(u16);									//例化获取Status状态(0未启用1启用)
	u8		(*GetReserver1)(u16);								//例化获取Reserver1预留
	u8		(*GetReserver2)(u16);								//例化获取Reserver2预留
	
	void		(*FillData)(void);									//将数据填入各个数据包
	void		(*RtcCheckFillData)(void);							//将对时数据填入数据包
	u16		(*ObtainPacketHead)(u8 *addr);						//读取SocketExtend包头数据并填入缓存
	u16		(*ObtainPacketData)(u8 *addr);						//读取SocketExtend流量数据包并填入缓存
	void		(*ObtainPacketCheckCode)(u8 *addr, u16);				//计算校验码并填入校验码
	
	void		(*ReadOutputID)(u16 *outputid);						//读取output_ID输出端口的参数到SocketExtend流量数据包
	u16		(*PackagedStorage)(void);							//SocketExtend将数据打包存入缓存区
	u16		(*RtcCheckPackagedStorage)(void);						//SocketExtend将对时数据打包存入缓存区
	
	void		(*PortSerialInit)(USART_TypeDef* USARTx, u32 BaudRate);	//初始化SocketExtend串口
	void		(*UARTx_IRQ)(USART_TypeDef* USARTx);					//SocketExtend协议串口中断处理函数
	void		(*Init)(void);										//初始化SocketExtend协议
	void		(*Implement)(u16 sendtime);							//SocketExtend协议处理
};

extern struct _m_socket_extend_dev socket_extend_dev;
extern u32 Crossid;

#endif
