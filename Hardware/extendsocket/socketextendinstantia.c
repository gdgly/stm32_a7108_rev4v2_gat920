/**
  *********************************************************************************************************
  * @file    socketextendinstantia.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socketExtend各个数据例化接口
  *********************************************************************************************************
  * @attention
  *			功能 : 
  *			1.  例化获取StartX起始字值,直接填值(0x88)					(外部调用)
  *			2.  例化获取ExitX结束字值,直接填值(0x99)
  *			3.  例化获取ServerType业务类型值
  *			4.  例化获取CrossID路口代码
  *			5.  例化获取DataLength数据长度
  *			6.  例化获取Channel传送组号值
  *			7.  例化获取CheckCode校验码值
  *			8.  例化获取DataCount数据条数
  *
  *			9.  例化获取RecordNo流水号
  *			10. 例化获取SendTime上报时间
  *			11. 例化获取VehicleCount车流量
  *			12. 例化获取LargerVehicleCount大车流量
  *			13. 例化获取MidVehicleCount中车流量
  *			14. 例化获取SmallVehilceCount小车流量
  *			15. 例化获取MiniVehicleCount微型车流量
  *			16. 例化获取MotoVehilceCount摩托车流量
  *			17. 例化获取ReserverType1预留车型1
  *			18. 例化获取ReserverType2预留车型2
  *			19. 例化获取ReserverType3预留车型3
  *			20. 例化获取ReserverType4预留车型4
  *			21. 例化获取ReserverType5预留车型5
  *			22. 例化获取ReserverType6预留车型6
  *			23. 例化获取MaxHeadInterval最大车头时距(MS)
  *			24. 例化获取MinHeadInterval最小车头时距(MS)
  *			25. 例化获取AddUpHeadInterval累计车头时距(MS)
  *			26. 例化获取MaxSpeed最大车速(Km/h)
  *			27. 例化获取MinSpeed最小车速(Km/h)
  *			28. 例化获取AddUpSpeed累计车速(Km/h)
  *			29. 例化获取MaxOccupancy最大占有时间(MS)
  *			30. 例化获取MinOccupancy最小占有时间(MS)
  *			31. 例化获取AddUpOccupancy累计占有时间(MS)
  *			32. 例化获取MaxInterval最大间隔时间(MS)
  *			33. 例化获取MinInterval最小间隔时间(MS)
  *			34. 例化获取AddUpInterval累计间隔时间(MS)
  *			35. 例化获取MaxVehicleLength最大车长(米)
  *			36. 例化获取MinVehicleLength最小车长(米)
  *			37. 例化获取AddUpVehilcleLength累计车长(米)
  *			38. 例化获取MaxQueueLength最大排队长度(米)
  *			39. 例化获取MinQueueLength最小排队长度(米)
  *			40. 例化获取AddUpQueueLength累计排队长度(米)
  *			41. 例化获取AddUpQueueTime累计排队时间(秒)
  *			42. 例化获取RedLightCount闯红灯触发次数
  *			43. 例化获取PrecedingWayFullTime前方车道满累计时间
  *			44. 例化获取DirverWayCode车道代码
  *			45. 例化获取Status状态(0未启用1启用)
  *			46. 例化获取Reserver1预留
  *			47. 例化获取Reserver2预留
  *
  *			48. 将数据填入各个数据包
  *			49. 将对时数据填入数据包
  *
  *********************************************************************************************************
  */

#include "socketextendinstantia.h"
#include "socketextendfunc.h"
#include "socketextendinitialization.h"
#include "calculationconfig.h"
#include "calculationavgspeed.h"

extern u32 Crossid;

/**********************************************************************************************************
 @Function			void SOCKET_Extend_FillData(void)
 @Description			将数据填入各个数据包
 @Input				void
 @Return				void
**********************************************************************************************************/
void SOCKET_Extend_FillData(void)
{
	u8 indexA = 0;
	u8 indexB = 0;
	
	SocketExtendHeadPacket.StartX			= socket_extend_dev.GetStartX();
	SocketExtendHeadPacket.ServerType		= socket_extend_dev.GetServerType();
	SocketExtendHeadPacket.CrossID		= socket_extend_dev.GetCrossID();
	SocketExtendHeadPacket.DataLength		= socket_extend_dev.GetDataLength(SocketExtendDataPacket);
	SocketExtendHeadPacket.Channel		= socket_extend_dev.GetChannel();
	SocketExtendHeadPacket.ExitX			= socket_extend_dev.GetExitX();
	SocketExtendHeadPacket.CheckCode		= socket_extend_dev.GetCheckCode();
	SocketExtendHeadPacket.DataCount		= socket_extend_dev.GetDataCount(SocketExtendDataPacket);
	
	//以下获取数据顺序不可改变
	for (indexA = 0; indexA < OUTPUT_MAX; indexA++) {														//遍历找寻配置过的车道号
		if (SocketExtendDataPacket[indexA].OutputID != 0) {
			if (indexA < SPEEDLANNUMMAX) {															//主地磁需获取数据
				for (indexB = indexA; indexB > 0; indexB--) {											//寻找本包与上一包同ID数据
					if (SocketExtendDataPacket[indexA].OutputID == SocketExtendDataPacket[indexB - 1].OutputID) {
						SocketExtendDataPacket[indexA].RecordNo				= SocketExtendDataPacket[indexB - 1].RecordNo;								//写入RecordNo
						SocketExtendDataPacket[indexA].SendTime				= SocketExtendDataPacket[indexB - 1].SendTime;								//写入SendTime
						SocketExtendDataPacket[indexA].VehicleCount			= SocketExtendDataPacket[indexB - 1].VehicleCount;							//写入VehicleCount
						SocketExtendDataPacket[indexA].LargerVehicleCount		= SocketExtendDataPacket[indexB - 1].LargerVehicleCount;						//写入LargerVehicleCount
						SocketExtendDataPacket[indexA].MidVehicleCount		= SocketExtendDataPacket[indexB - 1].MidVehicleCount;							//写入MidVehicleCount
						SocketExtendDataPacket[indexA].SmallVehilceCount		= SocketExtendDataPacket[indexB - 1].SmallVehilceCount;						//写入SmallVehilceCount
						SocketExtendDataPacket[indexA].MiniVehicleCount		= SocketExtendDataPacket[indexB - 1].MiniVehicleCount;							//写入MiniVehicleCount
						SocketExtendDataPacket[indexA].Moto				= SocketExtendDataPacket[indexB - 1].Moto;									//写入Moto
						SocketExtendDataPacket[indexA].ReserverType1			= SocketExtendDataPacket[indexB - 1].ReserverType1;							//写入ReserverType1
						SocketExtendDataPacket[indexA].ReserverType2			= SocketExtendDataPacket[indexB - 1].ReserverType2;							//写入ReserverType2
						SocketExtendDataPacket[indexA].ReserverType3			= SocketExtendDataPacket[indexB - 1].ReserverType3;							//写入ReserverType3
						SocketExtendDataPacket[indexA].ReserverType4			= SocketExtendDataPacket[indexB - 1].ReserverType4;							//写入ReserverType4
						SocketExtendDataPacket[indexA].ReserverType5			= SocketExtendDataPacket[indexB - 1].ReserverType5;							//写入ReserverType5
						SocketExtendDataPacket[indexA].ReserverType6			= SocketExtendDataPacket[indexB - 1].ReserverType6;							//写入ReserverType6
						SocketExtendDataPacket[indexA].MaxOccupancy			= SocketExtendDataPacket[indexB - 1].MaxOccupancy;							//写入MaxOccupancy
						SocketExtendDataPacket[indexA].MinOccupancy			= SocketExtendDataPacket[indexB - 1].MinOccupancy;							//写入MinOccupancy
						SocketExtendDataPacket[indexA].AddUpOccupancy		= SocketExtendDataPacket[indexB - 1].AddUpOccupancy;							//写入AddUpOccupancy
						SocketExtendDataPacket[indexA].MaxHeadInterval		= SocketExtendDataPacket[indexB - 1].MaxHeadInterval;							//写入MaxHeadInterval
						SocketExtendDataPacket[indexA].MinHeadInterval		= SocketExtendDataPacket[indexB - 1].MinHeadInterval;							//写入MinHeadInterval
						SocketExtendDataPacket[indexA].AddUpHeadInterval		= SocketExtendDataPacket[indexB - 1].AddUpHeadInterval;						//写入AddUpHeadInterval
						SocketExtendDataPacket[indexA].MaxSpeed				= SocketExtendDataPacket[indexB - 1].MaxSpeed;								//写入MaxSpeed
						SocketExtendDataPacket[indexA].MinSpeed				= SocketExtendDataPacket[indexB - 1].MinSpeed;								//写入MinSpeed
						SocketExtendDataPacket[indexA].AddUpSpeed			= SocketExtendDataPacket[indexB - 1].AddUpSpeed;								//写入AddUpSpeed
						SocketExtendDataPacket[indexA].MaxVehicleLength		= SocketExtendDataPacket[indexB - 1].MaxVehicleLength;							//写入MaxVehicleLength
						SocketExtendDataPacket[indexA].MinVehicleLength		= SocketExtendDataPacket[indexB - 1].MinVehicleLength;							//写入MinVehicleLength
						SocketExtendDataPacket[indexA].AddUpVehilcleLength	= SocketExtendDataPacket[indexB - 1].AddUpVehilcleLength;						//写入AddUpVehilcleLength
						SocketExtendDataPacket[indexA].MaxInterval			= SocketExtendDataPacket[indexB - 1].MaxInterval;								//写入MaxInterval
						SocketExtendDataPacket[indexA].MinInterval			= SocketExtendDataPacket[indexB - 1].MinInterval;								//写入MinInterval
						SocketExtendDataPacket[indexA].AddUpInterval			= SocketExtendDataPacket[indexB - 1].AddUpInterval;							//写入AddUpInterval
						SocketExtendDataPacket[indexA].MaxQueueLength		= SocketExtendDataPacket[indexB - 1].MaxQueueLength;							//写入MaxQueueLength
						SocketExtendDataPacket[indexA].MinQueueLength		= SocketExtendDataPacket[indexB - 1].MinQueueLength;							//写入MinQueueLength
						SocketExtendDataPacket[indexA].AddUpQueueLength		= SocketExtendDataPacket[indexB - 1].AddUpQueueLength;							//写入AddUpQueueLength
						SocketExtendDataPacket[indexA].AddUpQueueTime		= SocketExtendDataPacket[indexB - 1].AddUpQueueTime;							//写入AddUpQueueTime
						SocketExtendDataPacket[indexA].RedLightCount			= SocketExtendDataPacket[indexB - 1].RedLightCount;							//写入RedLightCount
						SocketExtendDataPacket[indexA].PrecedingWayFullTime	= SocketExtendDataPacket[indexB - 1].PrecedingWayFullTime;						//写入PrecedingWayFullTime
						SocketExtendDataPacket[indexA].DirverWayCode			= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);		//写入DirverWayCode
						SocketExtendDataPacket[indexA].Status				= SocketExtendDataPacket[indexB - 1].Status;									//写入Status
						SocketExtendDataPacket[indexA].Reserver1			= SocketExtendDataPacket[indexB - 1].Reserver1;								//写入Reserver1
						SocketExtendDataPacket[indexA].Reserver2			= SocketExtendDataPacket[indexB - 1].Reserver2;								//写入Reserver2
						break;
					}
				}
				if (indexB == 0) {																	//本数据包无同ID数据包,获取数据
					SocketExtendDataPacket[indexA].RecordNo					= socket_extend_dev.GetRecordNo(SocketExtendDataPacket[indexA].OutputID);			//写入RecordNo
					SocketExtendDataPacket[indexA].SendTime					= socket_extend_dev.GetSendTime(SocketExtendDataPacket[indexA].OutputID);			//写入SendTime
					SocketExtendDataPacket[indexA].VehicleCount				= socket_extend_dev.GetVehicleCount(SocketExtendDataPacket[indexA].OutputID);		//写入VehicleCount
					SocketExtendDataPacket[indexA].LargerVehicleCount			= socket_extend_dev.GetLargerVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//写入LargerVehicleCount
					SocketExtendDataPacket[indexA].MidVehicleCount			= socket_extend_dev.GetMidVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//写入MidVehicleCount
					SocketExtendDataPacket[indexA].SmallVehilceCount			= socket_extend_dev.GetSmallVehilceCount(SocketExtendDataPacket[indexA].OutputID);	//写入SmallVehilceCount
					SocketExtendDataPacket[indexA].MiniVehicleCount			= socket_extend_dev.GetMiniVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//写入MiniVehicleCount
					SocketExtendDataPacket[indexA].Moto					= socket_extend_dev.GetMotoVehilceCount(SocketExtendDataPacket[indexA].OutputID);	//写入Moto
					SocketExtendDataPacket[indexA].ReserverType1				= socket_extend_dev.GetReserverType1(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType1
					SocketExtendDataPacket[indexA].ReserverType2				= socket_extend_dev.GetReserverType2(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType2
					SocketExtendDataPacket[indexA].ReserverType3				= socket_extend_dev.GetReserverType3(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType3
					SocketExtendDataPacket[indexA].ReserverType4				= socket_extend_dev.GetReserverType4(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType4
					SocketExtendDataPacket[indexA].ReserverType5				= socket_extend_dev.GetReserverType5(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType5
					SocketExtendDataPacket[indexA].ReserverType6				= socket_extend_dev.GetReserverType6(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType6
					SocketExtendDataPacket[indexA].MaxOccupancy				= socket_extend_dev.GetMaxOccupancy(SocketExtendDataPacket[indexA].OutputID);		//写入MaxOccupancy
					SocketExtendDataPacket[indexA].MinOccupancy				= socket_extend_dev.GetMinOccupancy(SocketExtendDataPacket[indexA].OutputID);		//写入MinOccupancy
					SocketExtendDataPacket[indexA].AddUpOccupancy			= socket_extend_dev.GetAddUpOccupancy(SocketExtendDataPacket[indexA].OutputID);		//写入AddUpOccupancy
					SocketExtendDataPacket[indexA].MaxHeadInterval			= socket_extend_dev.GetMaxHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//写入MaxHeadInterval
					SocketExtendDataPacket[indexA].MinHeadInterval			= socket_extend_dev.GetMinHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//写入MinHeadInterval
					SocketExtendDataPacket[indexA].AddUpHeadInterval			= socket_extend_dev.GetAddUpHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//写入AddUpHeadInterval
					SocketExtendDataPacket[indexA].MaxSpeed					= socket_extend_dev.GetMaxSpeed(SocketExtendDataPacket[indexA].OutputID);			//写入MaxSpeed
					SocketExtendDataPacket[indexA].MinSpeed					= socket_extend_dev.GetMinSpeed(SocketExtendDataPacket[indexA].OutputID);			//写入MinSpeed
					SocketExtendDataPacket[indexA].AddUpSpeed				= socket_extend_dev.GetAddUpSpeed(SocketExtendDataPacket[indexA].OutputID);		//写入AddUpSpeed
					SocketExtendDataPacket[indexA].MaxVehicleLength			= socket_extend_dev.GetMaxVehicleLength(SocketExtendDataPacket[indexA].OutputID);	//写入MaxVehicleLength
					SocketExtendDataPacket[indexA].MinVehicleLength			= socket_extend_dev.GetMinVehicleLength(SocketExtendDataPacket[indexA].OutputID);	//写入MinVehicleLength
					SocketExtendDataPacket[indexA].AddUpVehilcleLength		= socket_extend_dev.GetAddUpVehilcleLength(SocketExtendDataPacket[indexA].OutputID);	//写入AddUpVehilcleLength
					SocketExtendDataPacket[indexA].MaxInterval				= socket_extend_dev.GetMaxInterval(SocketExtendDataPacket[indexA].OutputID);		//写入MaxInterval
					SocketExtendDataPacket[indexA].MinInterval				= socket_extend_dev.GetMinInterval(SocketExtendDataPacket[indexA].OutputID);		//写入MinInterval
					SocketExtendDataPacket[indexA].AddUpInterval				= socket_extend_dev.GetAddUpInterval(SocketExtendDataPacket[indexA].OutputID);		//写入AddUpInterval
					SocketExtendDataPacket[indexA].MaxQueueLength			= socket_extend_dev.GetMaxQueueLength(SocketExtendDataPacket[indexA].OutputID);		//写入MaxQueueLength
					SocketExtendDataPacket[indexA].MinQueueLength			= socket_extend_dev.GetMinQueueLength(SocketExtendDataPacket[indexA].OutputID);		//写入MinQueueLength
					SocketExtendDataPacket[indexA].AddUpQueueLength			= socket_extend_dev.GetAddUpQueueLength(SocketExtendDataPacket[indexA].OutputID);	//写入AddUpQueueLength
					SocketExtendDataPacket[indexA].AddUpQueueTime			= socket_extend_dev.GetAddUpQueueTime(SocketExtendDataPacket[indexA].OutputID);		//写入AddUpQueueTime
					SocketExtendDataPacket[indexA].RedLightCount				= socket_extend_dev.GetRedLightCount(SocketExtendDataPacket[indexA].OutputID);		//写入RedLightCount
					SocketExtendDataPacket[indexA].PrecedingWayFullTime		= socket_extend_dev.GetPrecedingWayFullTime(SocketExtendDataPacket[indexA].OutputID);//写入PrecedingWayFullTime
					SocketExtendDataPacket[indexA].DirverWayCode				= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);		//写入DirverWayCode
					SocketExtendDataPacket[indexA].Status					= socket_extend_dev.GetStatus(SocketExtendDataPacket[indexA].OutputID);			//写入Status
					SocketExtendDataPacket[indexA].Reserver1				= socket_extend_dev.GetReserver1(SocketExtendDataPacket[indexA].OutputID);			//写入Reserver1
					SocketExtendDataPacket[indexA].Reserver2				= socket_extend_dev.GetReserver2(SocketExtendDataPacket[indexA].OutputID);			//写入Reserver2
				}
			}
			else {																				//辅地磁获取数据
				if (SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].OutputID != 0) {							//辅地磁对应主地磁有配置
					SocketExtendDataPacket[indexA].RecordNo					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].RecordNo;						//写入RecordNo
					SocketExtendDataPacket[indexA].SendTime					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].SendTime;						//写入SendTime
					SocketExtendDataPacket[indexA].VehicleCount				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].VehicleCount;					//写入VehicleCount
					SocketExtendDataPacket[indexA].LargerVehicleCount			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].LargerVehicleCount;				//写入LargerVehicleCount
					SocketExtendDataPacket[indexA].MidVehicleCount			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MidVehicleCount;				//写入MidVehicleCount
					SocketExtendDataPacket[indexA].SmallVehilceCount			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].SmallVehilceCount;				//写入SmallVehilceCount
					SocketExtendDataPacket[indexA].MiniVehicleCount			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MiniVehicleCount;				//写入MiniVehicleCount
					SocketExtendDataPacket[indexA].Moto					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].Moto;						//写入Moto
					SocketExtendDataPacket[indexA].ReserverType1				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType1;					//写入ReserverType1
					SocketExtendDataPacket[indexA].ReserverType2				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType2;					//写入ReserverType2
					SocketExtendDataPacket[indexA].ReserverType3				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType3;					//写入ReserverType3
					SocketExtendDataPacket[indexA].ReserverType4				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType4;					//写入ReserverType4
					SocketExtendDataPacket[indexA].ReserverType5				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType5;					//写入ReserverType5
					SocketExtendDataPacket[indexA].ReserverType6				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType6;					//写入ReserverType6
					SocketExtendDataPacket[indexA].MaxOccupancy				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxOccupancy;					//写入MaxOccupancy
					SocketExtendDataPacket[indexA].MinOccupancy				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinOccupancy;					//写入MinOccupancy
					SocketExtendDataPacket[indexA].AddUpOccupancy			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpOccupancy;				//写入AddUpOccupancy
					SocketExtendDataPacket[indexA].MaxHeadInterval			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxHeadInterval;				//写入MaxHeadInterval
					SocketExtendDataPacket[indexA].MinHeadInterval			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinHeadInterval;				//写入MinHeadInterval
					SocketExtendDataPacket[indexA].AddUpHeadInterval			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpHeadInterval;				//写入AddUpHeadInterval
					SocketExtendDataPacket[indexA].MaxSpeed					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxSpeed;						//写入MaxSpeed
					SocketExtendDataPacket[indexA].MinSpeed					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinSpeed;						//写入MinSpeed
					SocketExtendDataPacket[indexA].AddUpSpeed				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpSpeed;					//写入AddUpSpeed
					SocketExtendDataPacket[indexA].MaxVehicleLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxVehicleLength;				//写入MaxVehicleLength
					SocketExtendDataPacket[indexA].MinVehicleLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinVehicleLength;				//写入MinVehicleLength
					SocketExtendDataPacket[indexA].AddUpVehilcleLength		= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpVehilcleLength;			//写入AddUpVehilcleLength
					SocketExtendDataPacket[indexA].MaxInterval				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxInterval;					//写入MaxInterval
					SocketExtendDataPacket[indexA].MinInterval				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinInterval;					//写入MinInterval
					SocketExtendDataPacket[indexA].AddUpInterval				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpInterval;					//写入AddUpInterval
					SocketExtendDataPacket[indexA].MaxQueueLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxQueueLength;				//写入MaxQueueLength
					SocketExtendDataPacket[indexA].MinQueueLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinQueueLength;				//写入MinQueueLength
					SocketExtendDataPacket[indexA].AddUpQueueLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpQueueLength;				//写入AddUpQueueLength
					SocketExtendDataPacket[indexA].AddUpQueueTime			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpQueueTime;				//写入AddUpQueueTime
					SocketExtendDataPacket[indexA].RedLightCount				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].RedLightCount;					//写入RedLightCount
					SocketExtendDataPacket[indexA].PrecedingWayFullTime		= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].PrecedingWayFullTime;			//写入PrecedingWayFullTime
					SocketExtendDataPacket[indexA].DirverWayCode				= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);		//写入DirverWayCode
					SocketExtendDataPacket[indexA].Status					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].Status;						//写入Status
					SocketExtendDataPacket[indexA].Reserver1				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].Reserver1;					//写入Reserver1
					SocketExtendDataPacket[indexA].Reserver2				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].Reserver2;					//写入Reserver2
				}
				else {																			//辅地磁对应主地磁无配置
					for (indexB = indexA; indexB > SPEEDLANNUMMAX; indexB--) {								//寻找本包与上一包同ID数据
						if (SocketExtendDataPacket[indexA].OutputID == SocketExtendDataPacket[indexB - 1].OutputID) {
							SocketExtendDataPacket[indexA].RecordNo				= SocketExtendDataPacket[indexB - 1].RecordNo;							//写入RecordNo
							SocketExtendDataPacket[indexA].SendTime				= SocketExtendDataPacket[indexB - 1].SendTime;							//写入SendTime
							SocketExtendDataPacket[indexA].VehicleCount			= SocketExtendDataPacket[indexB - 1].VehicleCount;						//写入VehicleCount
							SocketExtendDataPacket[indexA].LargerVehicleCount		= SocketExtendDataPacket[indexB - 1].LargerVehicleCount;					//写入LargerVehicleCount
							SocketExtendDataPacket[indexA].MidVehicleCount		= SocketExtendDataPacket[indexB - 1].MidVehicleCount;						//写入MidVehicleCount
							SocketExtendDataPacket[indexA].SmallVehilceCount		= SocketExtendDataPacket[indexB - 1].SmallVehilceCount;					//写入SmallVehilceCount
							SocketExtendDataPacket[indexA].MiniVehicleCount		= SocketExtendDataPacket[indexB - 1].MiniVehicleCount;						//写入MiniVehicleCount
							SocketExtendDataPacket[indexA].Moto				= SocketExtendDataPacket[indexB - 1].Moto;								//写入Moto
							SocketExtendDataPacket[indexA].ReserverType1			= SocketExtendDataPacket[indexB - 1].ReserverType1;						//写入ReserverType1
							SocketExtendDataPacket[indexA].ReserverType2			= SocketExtendDataPacket[indexB - 1].ReserverType2;						//写入ReserverType2
							SocketExtendDataPacket[indexA].ReserverType3			= SocketExtendDataPacket[indexB - 1].ReserverType3;						//写入ReserverType3
							SocketExtendDataPacket[indexA].ReserverType4			= SocketExtendDataPacket[indexB - 1].ReserverType4;						//写入ReserverType4
							SocketExtendDataPacket[indexA].ReserverType5			= SocketExtendDataPacket[indexB - 1].ReserverType5;						//写入ReserverType5
							SocketExtendDataPacket[indexA].ReserverType6			= SocketExtendDataPacket[indexB - 1].ReserverType6;						//写入ReserverType6
							SocketExtendDataPacket[indexA].MaxOccupancy			= SocketExtendDataPacket[indexB - 1].MaxOccupancy;						//写入MaxOccupancy
							SocketExtendDataPacket[indexA].MinOccupancy			= SocketExtendDataPacket[indexB - 1].MinOccupancy;						//写入MinOccupancy
							SocketExtendDataPacket[indexA].AddUpOccupancy		= SocketExtendDataPacket[indexB - 1].AddUpOccupancy;						//写入AddUpOccupancy
							SocketExtendDataPacket[indexA].MaxHeadInterval		= SocketExtendDataPacket[indexB - 1].MaxHeadInterval;						//写入MaxHeadInterval
							SocketExtendDataPacket[indexA].MinHeadInterval		= SocketExtendDataPacket[indexB - 1].MinHeadInterval;						//写入MinHeadInterval
							SocketExtendDataPacket[indexA].AddUpHeadInterval		= SocketExtendDataPacket[indexB - 1].AddUpHeadInterval;					//写入AddUpHeadInterval
							SocketExtendDataPacket[indexA].MaxSpeed				= SocketExtendDataPacket[indexB - 1].MaxSpeed;							//写入MaxSpeed
							SocketExtendDataPacket[indexA].MinSpeed				= SocketExtendDataPacket[indexB - 1].MinSpeed;							//写入MinSpeed
							SocketExtendDataPacket[indexA].AddUpSpeed			= SocketExtendDataPacket[indexB - 1].AddUpSpeed;							//写入AddUpSpeed
							SocketExtendDataPacket[indexA].MaxVehicleLength		= SocketExtendDataPacket[indexB - 1].MaxVehicleLength;						//写入MaxVehicleLength
							SocketExtendDataPacket[indexA].MinVehicleLength		= SocketExtendDataPacket[indexB - 1].MinVehicleLength;						//写入MinVehicleLength
							SocketExtendDataPacket[indexA].AddUpVehilcleLength	= SocketExtendDataPacket[indexB - 1].AddUpVehilcleLength;					//写入AddUpVehilcleLength
							SocketExtendDataPacket[indexA].MaxInterval			= SocketExtendDataPacket[indexB - 1].MaxInterval;							//写入MaxInterval
							SocketExtendDataPacket[indexA].MinInterval			= SocketExtendDataPacket[indexB - 1].MinInterval;							//写入MinInterval
							SocketExtendDataPacket[indexA].AddUpInterval			= SocketExtendDataPacket[indexB - 1].AddUpInterval;						//写入AddUpInterval
							SocketExtendDataPacket[indexA].MaxQueueLength		= SocketExtendDataPacket[indexB - 1].MaxQueueLength;						//写入MaxQueueLength
							SocketExtendDataPacket[indexA].MinQueueLength		= SocketExtendDataPacket[indexB - 1].MinQueueLength;						//写入MinQueueLength
							SocketExtendDataPacket[indexA].AddUpQueueLength		= SocketExtendDataPacket[indexB - 1].AddUpQueueLength;						//写入AddUpQueueLength
							SocketExtendDataPacket[indexA].AddUpQueueTime		= SocketExtendDataPacket[indexB - 1].AddUpQueueTime;						//写入AddUpQueueTime
							SocketExtendDataPacket[indexA].RedLightCount			= SocketExtendDataPacket[indexB - 1].RedLightCount;						//写入RedLightCount
							SocketExtendDataPacket[indexA].PrecedingWayFullTime	= SocketExtendDataPacket[indexB - 1].PrecedingWayFullTime;					//写入PrecedingWayFullTime
							SocketExtendDataPacket[indexA].DirverWayCode			= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);	//写入DirverWayCode
							SocketExtendDataPacket[indexA].Status				= SocketExtendDataPacket[indexB - 1].Status;								//写入Status
							SocketExtendDataPacket[indexA].Reserver1			= SocketExtendDataPacket[indexB - 1].Reserver1;							//写入Reserver1
							SocketExtendDataPacket[indexA].Reserver2			= SocketExtendDataPacket[indexB - 1].Reserver2;							//写入Reserver2
							break;
						}
					}
					if (indexB == SPEEDLANNUMMAX) {													//本数据包无同ID数据包,获取数据
						SocketExtendDataPacket[indexA].RecordNo				= socket_extend_dev.GetRecordNo(SocketExtendDataPacket[indexA].OutputID);			//写入RecordNo
						SocketExtendDataPacket[indexA].SendTime				= socket_extend_dev.GetSendTime(SocketExtendDataPacket[indexA].OutputID);			//写入SendTime
						SocketExtendDataPacket[indexA].VehicleCount			= socket_extend_dev.GetVehicleCount(SocketExtendDataPacket[indexA].OutputID);		//写入VehicleCount
						SocketExtendDataPacket[indexA].LargerVehicleCount		= socket_extend_dev.GetLargerVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//写入LargerVehicleCount
						SocketExtendDataPacket[indexA].MidVehicleCount		= socket_extend_dev.GetMidVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//写入MidVehicleCount
						SocketExtendDataPacket[indexA].SmallVehilceCount		= socket_extend_dev.GetSmallVehilceCount(SocketExtendDataPacket[indexA].OutputID);	//写入SmallVehilceCount
						SocketExtendDataPacket[indexA].MiniVehicleCount		= socket_extend_dev.GetMiniVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//写入MiniVehicleCount
						SocketExtendDataPacket[indexA].Moto				= socket_extend_dev.GetMotoVehilceCount(SocketExtendDataPacket[indexA].OutputID);	//写入Moto
						SocketExtendDataPacket[indexA].ReserverType1			= socket_extend_dev.GetReserverType1(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType1
						SocketExtendDataPacket[indexA].ReserverType2			= socket_extend_dev.GetReserverType2(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType2
						SocketExtendDataPacket[indexA].ReserverType3			= socket_extend_dev.GetReserverType3(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType3
						SocketExtendDataPacket[indexA].ReserverType4			= socket_extend_dev.GetReserverType4(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType4
						SocketExtendDataPacket[indexA].ReserverType5			= socket_extend_dev.GetReserverType5(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType5
						SocketExtendDataPacket[indexA].ReserverType6			= socket_extend_dev.GetReserverType6(SocketExtendDataPacket[indexA].OutputID);		//写入ReserverType6
						SocketExtendDataPacket[indexA].MaxOccupancy			= socket_extend_dev.GetMaxOccupancy(SocketExtendDataPacket[indexA].OutputID);		//写入MaxOccupancy
						SocketExtendDataPacket[indexA].MinOccupancy			= socket_extend_dev.GetMinOccupancy(SocketExtendDataPacket[indexA].OutputID);		//写入MinOccupancy
						SocketExtendDataPacket[indexA].AddUpOccupancy		= socket_extend_dev.GetAddUpOccupancy(SocketExtendDataPacket[indexA].OutputID);		//写入AddUpOccupancy
						SocketExtendDataPacket[indexA].MaxHeadInterval		= socket_extend_dev.GetMaxHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//写入MaxHeadInterval
						SocketExtendDataPacket[indexA].MinHeadInterval		= socket_extend_dev.GetMinHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//写入MinHeadInterval
						SocketExtendDataPacket[indexA].AddUpHeadInterval		= socket_extend_dev.GetAddUpHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//写入AddUpHeadInterval
						SocketExtendDataPacket[indexA].MaxSpeed				= socket_extend_dev.GetMaxSpeed(SocketExtendDataPacket[indexA].OutputID);			//写入MaxSpeed
						SocketExtendDataPacket[indexA].MinSpeed				= socket_extend_dev.GetMinSpeed(SocketExtendDataPacket[indexA].OutputID);			//写入MinSpeed
						SocketExtendDataPacket[indexA].AddUpSpeed			= socket_extend_dev.GetAddUpSpeed(SocketExtendDataPacket[indexA].OutputID);		//写入AddUpSpeed
						SocketExtendDataPacket[indexA].MaxVehicleLength		= socket_extend_dev.GetMaxVehicleLength(SocketExtendDataPacket[indexA].OutputID);	//写入MaxVehicleLength
						SocketExtendDataPacket[indexA].MinVehicleLength		= socket_extend_dev.GetMinVehicleLength(SocketExtendDataPacket[indexA].OutputID);	//写入MinVehicleLength
						SocketExtendDataPacket[indexA].AddUpVehilcleLength	= socket_extend_dev.GetAddUpVehilcleLength(SocketExtendDataPacket[indexA].OutputID);	//写入AddUpVehilcleLength
						SocketExtendDataPacket[indexA].MaxInterval			= socket_extend_dev.GetMaxInterval(SocketExtendDataPacket[indexA].OutputID);		//写入MaxInterval
						SocketExtendDataPacket[indexA].MinInterval			= socket_extend_dev.GetMinInterval(SocketExtendDataPacket[indexA].OutputID);		//写入MinInterval
						SocketExtendDataPacket[indexA].AddUpInterval			= socket_extend_dev.GetAddUpInterval(SocketExtendDataPacket[indexA].OutputID);		//写入AddUpInterval
						SocketExtendDataPacket[indexA].MaxQueueLength		= socket_extend_dev.GetMaxQueueLength(SocketExtendDataPacket[indexA].OutputID);		//写入MaxQueueLength
						SocketExtendDataPacket[indexA].MinQueueLength		= socket_extend_dev.GetMinQueueLength(SocketExtendDataPacket[indexA].OutputID);		//写入MinQueueLength
						SocketExtendDataPacket[indexA].AddUpQueueLength		= socket_extend_dev.GetAddUpQueueLength(SocketExtendDataPacket[indexA].OutputID);	//写入AddUpQueueLength
						SocketExtendDataPacket[indexA].AddUpQueueTime		= socket_extend_dev.GetAddUpQueueTime(SocketExtendDataPacket[indexA].OutputID);		//写入AddUpQueueTime
						SocketExtendDataPacket[indexA].RedLightCount			= socket_extend_dev.GetRedLightCount(SocketExtendDataPacket[indexA].OutputID);		//写入RedLightCount
						SocketExtendDataPacket[indexA].PrecedingWayFullTime	= socket_extend_dev.GetPrecedingWayFullTime(SocketExtendDataPacket[indexA].OutputID);//写入PrecedingWayFullTime
						SocketExtendDataPacket[indexA].DirverWayCode			= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);		//写入DirverWayCode
						SocketExtendDataPacket[indexA].Status				= socket_extend_dev.GetStatus(SocketExtendDataPacket[indexA].OutputID);			//写入Status
						SocketExtendDataPacket[indexA].Reserver1			= socket_extend_dev.GetReserver1(SocketExtendDataPacket[indexA].OutputID);			//写入Reserver1
						SocketExtendDataPacket[indexA].Reserver2			= socket_extend_dev.GetReserver2(SocketExtendDataPacket[indexA].OutputID);			//写入Reserver2
					}
				}
			}
		}
	}
}

/**********************************************************************************************************
 @Function			void SOCKET_Extend_RtcCheckFillData(void)
 @Description			将对时数据填入数据包
 @Input				void
 @Return				void
**********************************************************************************************************/
void SOCKET_Extend_RtcCheckFillData(void)
{
	SocketExtendHeadPacket.StartX			= socket_extend_dev.GetStartX();
	SocketExtendHeadPacket.ServerType		= socket_extend_dev.GetServerType();
	SocketExtendHeadPacket.CrossID		= socket_extend_dev.GetCrossID();
	SocketExtendHeadPacket.DataLength		= 0x01;
	SocketExtendHeadPacket.Channel		= socket_extend_dev.GetChannel();
	SocketExtendHeadPacket.ExitX			= socket_extend_dev.GetExitX();
	SocketExtendHeadPacket.CheckCode		= socket_extend_dev.GetCheckCode();
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetStartX(void)
 @Description			例化获取StartX起始字值,直接填值(0x88)
 @Input				void
 @Return				u8  StartX : 		起始字0x88
**********************************************************************************************************/
u8 SOCKET_Extend_GetStartX(void)
{
	return SOCKET_EXTEND_STARTX;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetExitX(void)
 @Description			例化获取ExitX结束字值,直接填值(0x99)
 @Input				void
 @Return				u8  ExitX : 		结束字0x99
**********************************************************************************************************/
u8 SOCKET_Extend_GetExitX(void)
{
	return SOCKET_EXTEND_EXITX;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetServerType(void)
 @Description			例化获取ServerType业务类型值
 @Input				void
 @Return				u8  ServerType
**********************************************************************************************************/
u8 SOCKET_Extend_GetServerType(void)
{
	u8 ServerType = SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE;
	
	if (SOCKET_EXTEND_RTC_CHECK == SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT) {		//判断是否需要RTC对时校验
		if (PlatformSocketExtendtime == SocketExtendTime_DISABLE) {				//根据SN选择是否对时
			ServerType = SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE;
		}
		else {
			ServerType = SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT;
		}
	}
	else {
		ServerType = SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE;
	}
	
	ServerType |= SOCKET_EXTEND_SERVERTYPE_CROSS;
	
	return ServerType;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetCrossID(void)
 @Description			例化获取CrossID路口代码
 @Input				
 @Return				u32 CrossID 值
**********************************************************************************************************/
u32 SOCKET_Extend_GetCrossID(void)
{
	if (Crossid > 99999999) {
		Crossid = 99999999;
	}
	
	return Crossid;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetDataLength(Socket_Extend_Packet_Data *addr)
 @Description			例化获取DataLength数据长度 (发送时:数据包长度)
 @Input				Socket_Extend_Packet_Data *addr 	: SocketExtend流量数据包集地址
 @Return				u16 DataLength 值
**********************************************************************************************************/
u16 SOCKET_Extend_GetDataLength(Socket_Extend_Packet_Data *addr)
{
	u8 i = 0;
	u16 datalength = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (addr[i].OutputID != 0) {
			datalength += 1;
		}
	}
	
	datalength *= 113;
	
	return datalength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetChannel(void)
 @Description			例化获取Channel传送组号值
 @Input				void
 @Return				u8  Channel
**********************************************************************************************************/
u16 SOCKET_Extend_GetChannel(void)
{
	return SOCKET_EXTEND_CHANNEL;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetCheckCode(void)
 @Description			例化获取CheckCode校验码值
 @Input				void
 @Return				u8  CheckCode
**********************************************************************************************************/
u8 SOCKET_Extend_GetCheckCode(void)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetDataCount(Socket_Extend_Packet_Data *addr)
 @Description			例化获取DataCount数据条数 (发送时:数据条数)
 @Input				Socket_Extend_Packet_Data *addr 	: SocketExtend流量数据包集地址
 @Return				u8 DataCount 值
**********************************************************************************************************/
u8 SOCKET_Extend_GetDataCount(Socket_Extend_Packet_Data *addr)
{
	u8 i = 0;
	u8 datacount = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (addr[i].OutputID != 0) {
			datacount += 1;
		}
	}
	
	return datacount;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetRecordNo(u16 outputid)
 @Description			例化获取RecordNo流水号
 @Input				u16 outputid   	: 		车道ID号
 @Return				u32 RecordNo		:		流水号
**********************************************************************************************************/
u32 SOCKET_Extend_GetRecordNo(u16 outputid)
{
	static u32 SocketExtendRecordNo = 0;
	
	return SocketExtendRecordNo++;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetSendTime(u16 outputid)
 @Description			例化获取SendTime上报时间
 @Input				u16 outputid   	: 		车道ID号
 @Return				u32 SendTime		:		上报时间
**********************************************************************************************************/
u32 SOCKET_Extend_GetSendTime(u16 outputid)
{
	u32 datetime = 0;
	
	datetime = RTC_GetCounter();
	
	return datetime;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetVehicleCount(u16 outputid)
 @Description			例化获取VehicleCount车流量
 @Input				u16 outputid   	: 		车道ID号
 @Return				u16 VehicleCount	:		车流量
**********************************************************************************************************/
u16 SOCKET_Extend_GetVehicleCount(u16 outputid)
{
	u16 vehicleCount = 0;
	
	vehicleCount = calculation_dev.ReadVolume(outputid);
	
	return vehicleCount;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetLargerVehicleCount(u16 outputid)
 @Description			例化获取LargerVehicleCount大车流量
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 LargerVehicleCount	:		大车流量
**********************************************************************************************************/
u16 SOCKET_Extend_GetLargerVehicleCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMidVehicleCount(u16 outputid)
 @Description			例化获取MidVehicleCount中车流量
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 MidVehicleCount		:		中车流量
**********************************************************************************************************/
u16 SOCKET_Extend_GetMidVehicleCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetSmallVehilceCount(u16 outputid)
 @Description			例化获取SmallVehilceCount小车流量
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 SmallVehilceCount	:		小车流量
**********************************************************************************************************/
u16 SOCKET_Extend_GetSmallVehilceCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMiniVehicleCount(u16 outputid)
 @Description			例化获取MiniVehicleCount微型车流量
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 MiniVehicleCount	:		微型车流量
**********************************************************************************************************/
u16 SOCKET_Extend_GetMiniVehicleCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMotoVehilceCount(u16 outputid)
 @Description			例化获取MotoVehilceCount摩托车流量
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 MotoVehilceCount	:		摩托车流量
**********************************************************************************************************/
u16 SOCKET_Extend_GetMotoVehilceCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType1(u16 outputid)
 @Description			例化获取ReserverType1预留车型1
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 ReserverType1		:		预留车型1
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType1(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType2(u16 outputid)
 @Description			例化获取ReserverType2预留车型2
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 ReserverType2		:		预留车型2
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType2(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType3(u16 outputid)
 @Description			例化获取ReserverType3预留车型3
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 ReserverType3		:		预留车型3
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType3(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType4(u16 outputid)
 @Description			例化获取ReserverType4预留车型4
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 ReserverType4		:		预留车型4
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType4(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType5(u16 outputid)
 @Description			例化获取ReserverType5预留车型5
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 ReserverType5		:		预留车型5
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType5(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType6(u16 outputid)
 @Description			例化获取ReserverType6预留车型6
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 ReserverType6		:		预留车型6
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType6(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMaxHeadInterval(u16 outputid)
 @Description			例化获取MaxHeadInterval最大车头时距(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 MaxHeadInterval		:		最大车头时距(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMaxHeadInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMinHeadInterval(u16 outputid)
 @Description			例化获取MinHeadInterval最小车头时距(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 MinHeadInterval		:		最小车头时距(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMinHeadInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetAddUpHeadInterval(u16 outputid)
 @Description			例化获取AddUpHeadInterval累计车头时距(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 AddUpHeadInterval	:		累计车头时距(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetAddUpHeadInterval(u16 outputid)
{
	u32 avgheadtimeval = 0;
	
	avgheadtimeval = calculation_dev.ReadAvgHeadTime(outputid);
	
	return avgheadtimeval;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetMaxSpeed(u16 outputid)
 @Description			例化获取MaxSpeed最大车速(Km/h)
 @Input				u16 outputid   		: 		车道ID号
 @Return				float MaxSpeed			:		最大车速(Km/h)
**********************************************************************************************************/
float SOCKET_Extend_GetMaxSpeed(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetMinSpeed(u16 outputid)
 @Description			例化获取MinSpeed最小车速(Km/h)
 @Input				u16 outputid   		: 		车道ID号
 @Return				float MinSpeed			:		最小车速(Km/h)
**********************************************************************************************************/
float SOCKET_Extend_GetMinSpeed(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetAddUpSpeed(u16 outputid)
 @Description			例化获取AddUpSpeed累计车速(Km/h)
					如为单地磁测速,测速值由默认车长与占有时间决定
					如为双地磁测速,测速值由双地磁数据获取
 @Input				u16 outputid   		: 		车道ID号
 @Return				float AddUpSpeed		:		累计车速(Km/h)
**********************************************************************************************************/
float SOCKET_Extend_GetAddUpSpeed(u16 outputid)
{
	u8 i = 0;
	u8 outputnum = 0;
	u32 avgoccupancyval = 0;
	float avgspeedval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {										//获取已配置ID号最大值
		if (SocketExtendDataPacket[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) 										//判断单地磁测速还是双地磁测速
	{
		avgspeedval = calculation_dev.ReadAvgSpeed(outputid);
	}
	else {
		for (i = 0; i < OUTPUT_MAX; i++) {
			if (SocketExtendDataPacket[i].OutputID == outputid) {
				avgoccupancyval = SocketExtendDataPacket[i].AddUpOccupancy;
			}
		}
		if (avgoccupancyval != 0) {
			avgspeedval = 3600.0 * SOCKET_EXTEND_AVGLENGTHSINGLE / avgoccupancyval;
		}
	}
	
	return avgspeedval;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMaxOccupancy(u16 outputid)
 @Description			例化获取MaxOccupancy最大占有时间(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 MaxOccupancy		:		最大占有时间(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMaxOccupancy(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMinOccupancy(u16 outputid)
 @Description			例化获取MinOccupancy最小占有时间(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 MinOccupancy		:		最小占有时间(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMinOccupancy(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetAddUpOccupancy(u16 outputid)
 @Description			例化获取AddUpOccupancy累计占有时间(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 AddUpOccupancy		:		累计占有时间(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetAddUpOccupancy(u16 outputid)
{
	u32 avgoccupancyval = 0;
	
	avgoccupancyval = calculation_dev.ReadAvgOccupancy(outputid);
	
	return avgoccupancyval;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMaxInterval(u16 outputid)
 @Description			例化获取MaxInterval最大间隔时间(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 MaxInterval		:		最大间隔时间(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMaxInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMinInterval(u16 outputid)
 @Description			例化获取MinInterval最小间隔时间(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 MinInterval		:		最小间隔时间(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMinInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetAddUpInterval(u16 outputid)
 @Description			例化获取AddUpInterval累计间隔时间(MS)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 AddUpInterval		:		累计间隔时间(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetAddUpInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetMaxVehicleLength(u16 outputid)
 @Description			例化获取MaxVehicleLength最大车长(米)
 @Input				u16 outputid   		: 		车道ID号
 @Return				float MaxVehicleLength	:		最大车长(米)
**********************************************************************************************************/
float SOCKET_Extend_GetMaxVehicleLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetMinVehicleLength(u16 outputid)
 @Description			例化获取MinVehicleLength最小车长(米)
 @Input				u16 outputid   		: 		车道ID号
 @Return				float MinVehicleLength	:		最小车长(米)
**********************************************************************************************************/
float SOCKET_Extend_GetMinVehicleLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetAddUpVehilcleLength(u16 outputid)
 @Description			例化获取AddUpVehilcleLength累计车长(米)
					如为单地磁测速模式 车长由宏定义配置,不可改变
					如为双地磁测速模式 车长由速度与占有时间决定
 @Input				u16 outputid   		: 		车道ID号
 @Return				float AddUpVehilcleLength:		累计车长(米)
**********************************************************************************************************/
float SOCKET_Extend_GetAddUpVehilcleLength(u16 outputid)
{
	u8 i = 0;
	u8 outputnum = 0;
	float avglengthval = 0;
	float avgspeedval = 0;
	u16 avgoccupancyval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {										//获取已配置ID号最大值
		if (SocketExtendDataPacket[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) 										//判断单地磁测速还是双地磁测速
	{
		for (i = 0; i < OUTPUT_MAX; i++) {
			if (SocketExtendDataPacket[i].OutputID == outputid) {
				if (i < SPEEDLANNUMMAX) {
					avgspeedval = SocketExtendDataPacket[i].AddUpSpeed;
					avgoccupancyval = SocketExtendDataPacket[i].AddUpOccupancy;
				}
				else {
					avgspeedval = SocketExtendDataPacket[i - SPEEDLANNUMMAX].AddUpSpeed;
					avgoccupancyval = SocketExtendDataPacket[i - SPEEDLANNUMMAX].AddUpOccupancy;
				}
			}
		}
		
		avgspeedval /= 3.6;
		avglengthval = avgspeedval * avgoccupancyval;
		avglengthval /= 1000;
		
		if ((avglengthval < SOCKET_EXTEND_AVGLENGTHMIN) && (avglengthval > 0.1)) {
			avglengthval = SOCKET_EXTEND_AVGLENGTHMIN;
		}
	}
	else {
		avglengthval = SOCKET_EXTEND_AVGLENGTHSINGLE;
	}
	
	return avglengthval;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMaxQueueLength(u16 outputid)
 @Description			例化获取MaxQueueLength最大排队长度(米)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 MaxQueueLength		:		最大排队长度(米)
**********************************************************************************************************/
u16 SOCKET_Extend_GetMaxQueueLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMinQueueLength(u16 outputid)
 @Description			例化获取MinQueueLength最小排队长度(米)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 MinQueueLength		:		最小排队长度(米)
**********************************************************************************************************/
u16 SOCKET_Extend_GetMinQueueLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetAddUpQueueLength(u16 outputid)
 @Description			例化获取AddUpQueueLength累计排队长度(米)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 AddUpQueueLength	:		累计排队长度(米)
**********************************************************************************************************/
u32 SOCKET_Extend_GetAddUpQueueLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetAddUpQueueTime(u16 outputid)
 @Description			例化获取AddUpQueueTime累计排队时间(秒)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 AddUpQueueTime		:		累计排队时间(秒)
**********************************************************************************************************/
u16 SOCKET_Extend_GetAddUpQueueTime(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetRedLightCount(u16 outputid)
 @Description			例化获取RedLightCount闯红灯触发次数
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 RedLightCount		:		闯红灯触发次数
**********************************************************************************************************/
u16 SOCKET_Extend_GetRedLightCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetPrecedingWayFullTime(u16 outputid)
 @Description			例化获取PrecedingWayFullTime前方车道满累计时间
 @Input				u16 outputid   		: 		车道ID号
 @Return				u16 PrecedingWayFullTime	:		前方车道满累计时间
**********************************************************************************************************/
u16 SOCKET_Extend_GetPrecedingWayFullTime(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetDirverWayCode(u16 outputid, u16 inlen)
 @Description			例化获取DirverWayCode车道代码
 @Input				u16 outputid   		: 		车道ID号
 @Return				u32 DirverWayCode		:		车道代码
**********************************************************************************************************/
u32 SOCKET_Extend_GetDirverWayCode(u16 outputid, u16 inlen)
{
	u8 i = 0;
	u16 laneno = 101;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (SocketExtendDataPacket[i].OutputID == outputid) {
			i = inlen;
			if (i <= 3) {
				laneno = i + 1 + 100;
				break;
			}
			else if ((i >= 4) && (i <= 7)) {
				laneno = i + 1 + 200 - 4;
				break;
			}
			else if ((i >= 8) && (i <= 11)) {
				laneno = i + 1 + 300 - 8;
				break;
			}
			else if ((i >= 12) && (i <= 15)) {
				laneno = i + 1 + 400 - 12;
				break;
			}
			else if ((i >= 16) && (i <= 19)) {
				laneno = i + 1 + 100 - 12;
				break;
			}
			else if ((i >= 20) && (i <= 23)) {
				laneno = i + 1 + 200 - 16;
				break;
			}
			else if ((i >= 24) && (i <= 27)) {
				laneno = i + 1 + 300 - 20;
				break;
			}
			else if ((i >= 28) && (i <= 31)) {
				laneno = i + 1 + 400 - 24;
				break;
			}
		}
	}
	
	return laneno;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetStatus(u16 outputid)
 @Description			例化获取Status状态(0未启用1启用)
 @Input				u16 outputid   		: 		车道ID号
 @Return				u8  Status			:		状态(0未启用1启用)
**********************************************************************************************************/
u8 SOCKET_Extend_GetStatus(u16 outputid)
{
	return SOCKET_EXTEND_STATUS_ENABLE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetReserver1(u16 outputid)
 @Description			例化获取Reserver1预留
 @Input				u16 outputid   		: 		车道ID号
 @Return				u8  Reserver1			:		预留
**********************************************************************************************************/
u8 SOCKET_Extend_GetReserver1(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetReserver2(u16 outputid)
 @Description			例化获取Reserver2预留
 @Input				u16 outputid   		: 		车道ID号
 @Return				u8  Reserver2			:		预留
**********************************************************************************************************/
u8 SOCKET_Extend_GetReserver2(u16 outputid)
{
	return 0;
}

/********************************************** END OF FLEE **********************************************/
