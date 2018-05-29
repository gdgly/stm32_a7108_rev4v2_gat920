#ifndef __SOCKET_MODULATION_CONFIG_H
#define   __SOCKET_MODULATION_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

#define	SOCKET_MODULATION_CACHE_SIZE					1024						//1K协议缓存, 32路车道需786Byte
#define	SOCKET_MODULATION_RECVIVE_SIZE				30						//接收缓存,数据包头13Byte

#define	SOCKET_MODULATION_AGAINSENDTIME				20						//20秒后没收到回包重发数据

#define	SOCKET_MODULATION_TRUE						0
#define	SOCKET_MODULATION_FALSE						1

#define	SOCKET_MODULATION_HEAD_FRAME					0xAAAA					//帧头(0xAAAA)
#define	SOCKET_MODULATION_TAIL_FRAME					0xEEEE					//帧尾(0xEEEE)

#define	SOCKET_MODULATION_PACKET_TYPE_FLOWMESSAGE		0x01						//交通流量信息
#define	SOCKET_MODULATION_PACKET_TYPE_FLOWMESSAGE_ACK	0x0A						//交通流量信息应答

#define	SOCKET_MODULATION_DEVICE_ID					"0171280100000001"			//设备身份识别码
#define	SOCKET_MODULATION_STATION_ID					"S229CJXXX321204"			//站点编码

#define	SOCKET_MODULATION_DEVICE_ERR_CDOE				0x00						//设备硬件错误码
#define	SOCKET_MODULATION_SURVEY_CONTENT				0x01						//调查内容

#define	SOCKET_MODULATION_PROCESS_CYCLE				0x05						//交通数据处理周期(分)

#define	SOCKET_MODULATION_FOLLOW_CAR					51						//跟车百分比
#define	SOCKET_MODULATION_AVG_HEADDISTANCE				0x0019					//平均车头间距

/**********************************************************************************************************
 @attention : 
 * Socket Modulation 帧头		  ( 0xAA 0xAA )
 * Socket Modulation 包长		  ( PacketLength )
 * Socket Modulation 数据包内容	  ( PacketData )
 * Socket Modulation CRC校验		  ( CRCCheckCode )
 * Socket Modulation 帧尾		  ( 0xEE 0xEE )
 *
 * Socket Modulation 数据包内容	: ( PacketType,DeviceID,StationID,DeviceErrCode,SurveyContent,DateYear,DateMon,DateDay,
							    ProcessCycle,TimeSerial,LaneNum,DataLane[OUTPUT_MAX] )
 * Socket Modulation 车道数据		: ( LaneNo,FollowCar,AvgHeadDistance,TimeOccupancy,
							    SmallVehilceCount,SmallVehilceAvgSpeed,
							    MidVehicleCount,MidVehicleAvgSpeed,
							    LargerVehicleCount,LargerVehicleAvgSpeed,
							    SupeLargerVehicleCount,SupeLargerVehicleAvgSpeed,
							    TractorCount,TractorAvgSpeed,
							    MotoCount,MotoAvgSpeed )
**********************************************************************************************************/
typedef struct
{
	u16							OutputID;									//输出端口的参数
	u8							LaneNo;									//车道号
	u8							FollowCar;								//跟车百分比
	u16							AvgHeadDistance;							//平均车头间距
	u8							TimeOccupancy;								//时间占有率
	u32							Avgoccupancyval;							//占有时间
	
	u16							SmallVehilceCount;							//小型车交通流量
	u8							SmallVehilceAvgSpeed;						//小型车平均地点车速
	u16							MidVehicleCount;							//中型车交通流量
	u8							MidVehicleAvgSpeed;							//中型车平均地点车速
	u16							LargerVehicleCount;							//大型车交通流量
	u8							LargerVehicleAvgSpeed;						//大型车平均地点车速
	u16							SupeLargerVehicleCount;						//特大型车交通流量
	u8							SupeLargerVehicleAvgSpeed;					//特大型车平均地点车速
	u16							TractorCount;								//拖拉机交通流量
	u8							TractorAvgSpeed;							//拖拉机平均地点车速
	u16							MotoCount;								//摩托车交通流量
	u8							MotoAvgSpeed;								//摩托车平均地点车速
}Socket_Modulation_Data_Lane;													//车道数据包

typedef struct
{
	u8							PacketType;								//数据包类型
	u8							DeviceID[16];								//设备身份识别码
	u8							StationID[15];								//站点编码
	u8							DeviceErrCode;								//设备硬件错误码
	u8							SurveyContent;								//调查内容
	u16							DateYear;									//年
	u8							DateMon;									//月
	u8							DateDay;									//日
	u8							ProcessCycle;								//交通数据处理周期
	u16							TimeSerial;								//时间序号
	u8							LaneNum;									//车道数
	Socket_Modulation_Data_Lane		DataLane[OUTPUT_MAX];						//车道数据包
}Socket_Modulation_Data_Packet;												//数据包

typedef struct
{
	u16							HeadFrame;								//帧头(0xAA 0xAA)
	u16							PacketLength;								//包长
	Socket_Modulation_Data_Packet		PacketData;								//数据包
	u16							CRCCheckCode;								//CRC校验
	u16							TailFrame;								//帧尾(0xEE 0xEE)
}Socket_Modulation_Data_Frame;												//数据帧

/* SocketModulation外部接口数据读取控制管理器 */
struct _m_socket_modulation_dev
{
	u16		(*GetHeadFrame)(void);												//例化获取HeadFrame帧头,直接填值(0xAAAA)
	u16		(*GetTailFrame)(void);												//例化获取TailFrame帧尾,直接填值(0xEEEE)
	u16		(*GetCRCCheckCode)(void);											//例化获取CRCCheckCode校验码
	u16		(*GetPacketLength)(Socket_Modulation_Data_Frame *addr);					//例化获取PacketLength包长
	
	u8		(*GetPacketType)(void);												//例化获取PacketType数据包类型
	void		(*GetDeviceID)(u8 *deviceID);											//例化获取DeviceID设备身份识别码
	void		(*GetStationID)(u8 *stationID);										//例化获取StationID站点编码
	u8		(*GetDeviceErrCode)(void);											//例化获取DeviceErrCode设备硬件错误码
	u8		(*GetSurveyContent)(void);											//例化获取SurveyContent调查内容
	u16		(*GetDateYear)(void);												//例化获取DateYear年
	u8		(*GetDateMon)(void);												//例化获取DateMon月
	u8		(*GetDateDay)(void);												//例化获取DateDay日
	u8		(*GetProcessCycle)(void);											//例化获取ProcessCycle交通数据处理周期
	u16		(*GetTimeSerial)(void);												//例化获取TimeSerial时间序号
	u8		(*GetLaneNum)(Socket_Modulation_Data_Frame *addr);						//例化获取LaneNum车道数
	
	u8		(*GetLaneNo)(Socket_Modulation_Data_Frame *addr, u16 outputid);				//例化获取LaneNo车道号
	u8		(*GetFollowCar)(Socket_Modulation_Data_Frame *addr, u16 outputid);			//例化获取FollowCar跟车百分比
	u16		(*GetAvgHeadDistance)(Socket_Modulation_Data_Frame *addr, u16 outputid);		//例化获取AvgHeadDistance平均车头间距
	u8		(*GetTimeOccupancy)(Socket_Modulation_Data_Frame *addr, u16 outputid);		//例化获取TimeOccupancy时间占有率
	
	u16		(*GetSmallVehilceCount)(Socket_Modulation_Data_Frame *addr, u16 outputid);		//例化获取SmallVehilceCount小型车交通流量
	u8		(*GetSmallVehilceAvgSpeed)(Socket_Modulation_Data_Frame *addr, u16 outputid);	//例化获取SmallVehilceAvgSpeed小型车平均地点车速
	
	void		(*FillData)(void);													//将数据填入各个数据包
	u16		(*ObtainDataFrame)(u8 *addr);											//读取SocketModulation数据帧并填入缓存
	void		(*ObtainDataFrameCheckCode)(u8 *addr, u16 buflength);						//计算校验码并填入校验码
	
	void		(*ReadOutputID)(u16 *outputid);										//读取output_ID输出端口的参数到SocketModulation车道数据包
	u16		(*PackagedStorage)(void);											//SocketModulation将数据打包存入缓存区
	
	void		(*PortSerialInit)(USART_TypeDef* USARTx, u32 BaudRate);					//初始化SocketModulation串口
	void		(*UARTx_IRQ)(USART_TypeDef* USARTx);									//SocketModulation协议串口中断处理函数
	void		(*Init)(void);														//初始化SocketModulation协议
	void		(*Implement)(u16 sendtime);											//SocketModulation协议处理
};

extern struct _m_socket_modulation_dev socket_modulation_dev;
extern u32 Crossid;

#endif
