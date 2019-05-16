/**
  *********************************************************************************************************
  * @file    socketextendinstantia.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socketExtend�������������ӿ�
  *********************************************************************************************************
  * @attention
  *			���� : 
  *			1.  ������ȡStartX��ʼ��ֵ,ֱ����ֵ(0x88)					(�ⲿ����)
  *			2.  ������ȡExitX������ֵ,ֱ����ֵ(0x99)
  *			3.  ������ȡServerTypeҵ������ֵ
  *			4.  ������ȡCrossID·�ڴ���
  *			5.  ������ȡDataLength���ݳ���
  *			6.  ������ȡChannel�������ֵ
  *			7.  ������ȡCheckCodeУ����ֵ
  *			8.  ������ȡDataCount��������
  *
  *			9.  ������ȡRecordNo��ˮ��
  *			10. ������ȡSendTime�ϱ�ʱ��
  *			11. ������ȡVehicleCount������
  *			12. ������ȡLargerVehicleCount������
  *			13. ������ȡMidVehicleCount�г�����
  *			14. ������ȡSmallVehilceCountС������
  *			15. ������ȡMiniVehicleCount΢�ͳ�����
  *			16. ������ȡMotoVehilceCountĦ�г�����
  *			17. ������ȡReserverType1Ԥ������1
  *			18. ������ȡReserverType2Ԥ������2
  *			19. ������ȡReserverType3Ԥ������3
  *			20. ������ȡReserverType4Ԥ������4
  *			21. ������ȡReserverType5Ԥ������5
  *			22. ������ȡReserverType6Ԥ������6
  *			23. ������ȡMaxHeadInterval���ͷʱ��(MS)
  *			24. ������ȡMinHeadInterval��С��ͷʱ��(MS)
  *			25. ������ȡAddUpHeadInterval�ۼƳ�ͷʱ��(MS)
  *			26. ������ȡMaxSpeed�����(Km/h)
  *			27. ������ȡMinSpeed��С����(Km/h)
  *			28. ������ȡAddUpSpeed�ۼƳ���(Km/h)
  *			29. ������ȡMaxOccupancy���ռ��ʱ��(MS)
  *			30. ������ȡMinOccupancy��Сռ��ʱ��(MS)
  *			31. ������ȡAddUpOccupancy�ۼ�ռ��ʱ��(MS)
  *			32. ������ȡMaxInterval�����ʱ��(MS)
  *			33. ������ȡMinInterval��С���ʱ��(MS)
  *			34. ������ȡAddUpInterval�ۼƼ��ʱ��(MS)
  *			35. ������ȡMaxVehicleLength��󳵳�(��)
  *			36. ������ȡMinVehicleLength��С����(��)
  *			37. ������ȡAddUpVehilcleLength�ۼƳ���(��)
  *			38. ������ȡMaxQueueLength����Ŷӳ���(��)
  *			39. ������ȡMinQueueLength��С�Ŷӳ���(��)
  *			40. ������ȡAddUpQueueLength�ۼ��Ŷӳ���(��)
  *			41. ������ȡAddUpQueueTime�ۼ��Ŷ�ʱ��(��)
  *			42. ������ȡRedLightCount����ƴ�������
  *			43. ������ȡPrecedingWayFullTimeǰ���������ۼ�ʱ��
  *			44. ������ȡDirverWayCode��������
  *			45. ������ȡStatus״̬(0δ����1����)
  *			46. ������ȡReserver1Ԥ��
  *			47. ������ȡReserver2Ԥ��
  *
  *			48. ����������������ݰ�
  *			49. ����ʱ�����������ݰ�
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
 @Description			����������������ݰ�
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
	
	//���»�ȡ����˳�򲻿ɸı�
	for (indexA = 0; indexA < OUTPUT_MAX; indexA++) {														//������Ѱ���ù��ĳ�����
		if (SocketExtendDataPacket[indexA].OutputID != 0) {
			if (indexA < SPEEDLANNUMMAX) {															//���ش����ȡ����
				for (indexB = indexA; indexB > 0; indexB--) {											//Ѱ�ұ�������һ��ͬID����
					if (SocketExtendDataPacket[indexA].OutputID == SocketExtendDataPacket[indexB - 1].OutputID) {
						SocketExtendDataPacket[indexA].RecordNo				= SocketExtendDataPacket[indexB - 1].RecordNo;								//д��RecordNo
						SocketExtendDataPacket[indexA].SendTime				= SocketExtendDataPacket[indexB - 1].SendTime;								//д��SendTime
						SocketExtendDataPacket[indexA].VehicleCount			= SocketExtendDataPacket[indexB - 1].VehicleCount;							//д��VehicleCount
						SocketExtendDataPacket[indexA].LargerVehicleCount		= SocketExtendDataPacket[indexB - 1].LargerVehicleCount;						//д��LargerVehicleCount
						SocketExtendDataPacket[indexA].MidVehicleCount		= SocketExtendDataPacket[indexB - 1].MidVehicleCount;							//д��MidVehicleCount
						SocketExtendDataPacket[indexA].SmallVehilceCount		= SocketExtendDataPacket[indexB - 1].SmallVehilceCount;						//д��SmallVehilceCount
						SocketExtendDataPacket[indexA].MiniVehicleCount		= SocketExtendDataPacket[indexB - 1].MiniVehicleCount;							//д��MiniVehicleCount
						SocketExtendDataPacket[indexA].Moto				= SocketExtendDataPacket[indexB - 1].Moto;									//д��Moto
						SocketExtendDataPacket[indexA].ReserverType1			= SocketExtendDataPacket[indexB - 1].ReserverType1;							//д��ReserverType1
						SocketExtendDataPacket[indexA].ReserverType2			= SocketExtendDataPacket[indexB - 1].ReserverType2;							//д��ReserverType2
						SocketExtendDataPacket[indexA].ReserverType3			= SocketExtendDataPacket[indexB - 1].ReserverType3;							//д��ReserverType3
						SocketExtendDataPacket[indexA].ReserverType4			= SocketExtendDataPacket[indexB - 1].ReserverType4;							//д��ReserverType4
						SocketExtendDataPacket[indexA].ReserverType5			= SocketExtendDataPacket[indexB - 1].ReserverType5;							//д��ReserverType5
						SocketExtendDataPacket[indexA].ReserverType6			= SocketExtendDataPacket[indexB - 1].ReserverType6;							//д��ReserverType6
						SocketExtendDataPacket[indexA].MaxOccupancy			= SocketExtendDataPacket[indexB - 1].MaxOccupancy;							//д��MaxOccupancy
						SocketExtendDataPacket[indexA].MinOccupancy			= SocketExtendDataPacket[indexB - 1].MinOccupancy;							//д��MinOccupancy
						SocketExtendDataPacket[indexA].AddUpOccupancy		= SocketExtendDataPacket[indexB - 1].AddUpOccupancy;							//д��AddUpOccupancy
						SocketExtendDataPacket[indexA].MaxHeadInterval		= SocketExtendDataPacket[indexB - 1].MaxHeadInterval;							//д��MaxHeadInterval
						SocketExtendDataPacket[indexA].MinHeadInterval		= SocketExtendDataPacket[indexB - 1].MinHeadInterval;							//д��MinHeadInterval
						SocketExtendDataPacket[indexA].AddUpHeadInterval		= SocketExtendDataPacket[indexB - 1].AddUpHeadInterval;						//д��AddUpHeadInterval
						SocketExtendDataPacket[indexA].MaxSpeed				= SocketExtendDataPacket[indexB - 1].MaxSpeed;								//д��MaxSpeed
						SocketExtendDataPacket[indexA].MinSpeed				= SocketExtendDataPacket[indexB - 1].MinSpeed;								//д��MinSpeed
						SocketExtendDataPacket[indexA].AddUpSpeed			= SocketExtendDataPacket[indexB - 1].AddUpSpeed;								//д��AddUpSpeed
						SocketExtendDataPacket[indexA].MaxVehicleLength		= SocketExtendDataPacket[indexB - 1].MaxVehicleLength;							//д��MaxVehicleLength
						SocketExtendDataPacket[indexA].MinVehicleLength		= SocketExtendDataPacket[indexB - 1].MinVehicleLength;							//д��MinVehicleLength
						SocketExtendDataPacket[indexA].AddUpVehilcleLength	= SocketExtendDataPacket[indexB - 1].AddUpVehilcleLength;						//д��AddUpVehilcleLength
						SocketExtendDataPacket[indexA].MaxInterval			= SocketExtendDataPacket[indexB - 1].MaxInterval;								//д��MaxInterval
						SocketExtendDataPacket[indexA].MinInterval			= SocketExtendDataPacket[indexB - 1].MinInterval;								//д��MinInterval
						SocketExtendDataPacket[indexA].AddUpInterval			= SocketExtendDataPacket[indexB - 1].AddUpInterval;							//д��AddUpInterval
						SocketExtendDataPacket[indexA].MaxQueueLength		= SocketExtendDataPacket[indexB - 1].MaxQueueLength;							//д��MaxQueueLength
						SocketExtendDataPacket[indexA].MinQueueLength		= SocketExtendDataPacket[indexB - 1].MinQueueLength;							//д��MinQueueLength
						SocketExtendDataPacket[indexA].AddUpQueueLength		= SocketExtendDataPacket[indexB - 1].AddUpQueueLength;							//д��AddUpQueueLength
						SocketExtendDataPacket[indexA].AddUpQueueTime		= SocketExtendDataPacket[indexB - 1].AddUpQueueTime;							//д��AddUpQueueTime
						SocketExtendDataPacket[indexA].RedLightCount			= SocketExtendDataPacket[indexB - 1].RedLightCount;							//д��RedLightCount
						SocketExtendDataPacket[indexA].PrecedingWayFullTime	= SocketExtendDataPacket[indexB - 1].PrecedingWayFullTime;						//д��PrecedingWayFullTime
						SocketExtendDataPacket[indexA].DirverWayCode			= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);		//д��DirverWayCode
						SocketExtendDataPacket[indexA].Status				= SocketExtendDataPacket[indexB - 1].Status;									//д��Status
						SocketExtendDataPacket[indexA].Reserver1			= SocketExtendDataPacket[indexB - 1].Reserver1;								//д��Reserver1
						SocketExtendDataPacket[indexA].Reserver2			= SocketExtendDataPacket[indexB - 1].Reserver2;								//д��Reserver2
						break;
					}
				}
				if (indexB == 0) {																	//�����ݰ���ͬID���ݰ�,��ȡ����
					SocketExtendDataPacket[indexA].RecordNo					= socket_extend_dev.GetRecordNo(SocketExtendDataPacket[indexA].OutputID);			//д��RecordNo
					SocketExtendDataPacket[indexA].SendTime					= socket_extend_dev.GetSendTime(SocketExtendDataPacket[indexA].OutputID);			//д��SendTime
					SocketExtendDataPacket[indexA].VehicleCount				= socket_extend_dev.GetVehicleCount(SocketExtendDataPacket[indexA].OutputID);		//д��VehicleCount
					SocketExtendDataPacket[indexA].LargerVehicleCount			= socket_extend_dev.GetLargerVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//д��LargerVehicleCount
					SocketExtendDataPacket[indexA].MidVehicleCount			= socket_extend_dev.GetMidVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//д��MidVehicleCount
					SocketExtendDataPacket[indexA].SmallVehilceCount			= socket_extend_dev.GetSmallVehilceCount(SocketExtendDataPacket[indexA].OutputID);	//д��SmallVehilceCount
					SocketExtendDataPacket[indexA].MiniVehicleCount			= socket_extend_dev.GetMiniVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//д��MiniVehicleCount
					SocketExtendDataPacket[indexA].Moto					= socket_extend_dev.GetMotoVehilceCount(SocketExtendDataPacket[indexA].OutputID);	//д��Moto
					SocketExtendDataPacket[indexA].ReserverType1				= socket_extend_dev.GetReserverType1(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType1
					SocketExtendDataPacket[indexA].ReserverType2				= socket_extend_dev.GetReserverType2(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType2
					SocketExtendDataPacket[indexA].ReserverType3				= socket_extend_dev.GetReserverType3(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType3
					SocketExtendDataPacket[indexA].ReserverType4				= socket_extend_dev.GetReserverType4(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType4
					SocketExtendDataPacket[indexA].ReserverType5				= socket_extend_dev.GetReserverType5(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType5
					SocketExtendDataPacket[indexA].ReserverType6				= socket_extend_dev.GetReserverType6(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType6
					SocketExtendDataPacket[indexA].MaxOccupancy				= socket_extend_dev.GetMaxOccupancy(SocketExtendDataPacket[indexA].OutputID);		//д��MaxOccupancy
					SocketExtendDataPacket[indexA].MinOccupancy				= socket_extend_dev.GetMinOccupancy(SocketExtendDataPacket[indexA].OutputID);		//д��MinOccupancy
					SocketExtendDataPacket[indexA].AddUpOccupancy			= socket_extend_dev.GetAddUpOccupancy(SocketExtendDataPacket[indexA].OutputID);		//д��AddUpOccupancy
					SocketExtendDataPacket[indexA].MaxHeadInterval			= socket_extend_dev.GetMaxHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//д��MaxHeadInterval
					SocketExtendDataPacket[indexA].MinHeadInterval			= socket_extend_dev.GetMinHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//д��MinHeadInterval
					SocketExtendDataPacket[indexA].AddUpHeadInterval			= socket_extend_dev.GetAddUpHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//д��AddUpHeadInterval
					SocketExtendDataPacket[indexA].MaxSpeed					= socket_extend_dev.GetMaxSpeed(SocketExtendDataPacket[indexA].OutputID);			//д��MaxSpeed
					SocketExtendDataPacket[indexA].MinSpeed					= socket_extend_dev.GetMinSpeed(SocketExtendDataPacket[indexA].OutputID);			//д��MinSpeed
					SocketExtendDataPacket[indexA].AddUpSpeed				= socket_extend_dev.GetAddUpSpeed(SocketExtendDataPacket[indexA].OutputID);		//д��AddUpSpeed
					SocketExtendDataPacket[indexA].MaxVehicleLength			= socket_extend_dev.GetMaxVehicleLength(SocketExtendDataPacket[indexA].OutputID);	//д��MaxVehicleLength
					SocketExtendDataPacket[indexA].MinVehicleLength			= socket_extend_dev.GetMinVehicleLength(SocketExtendDataPacket[indexA].OutputID);	//д��MinVehicleLength
					SocketExtendDataPacket[indexA].AddUpVehilcleLength		= socket_extend_dev.GetAddUpVehilcleLength(SocketExtendDataPacket[indexA].OutputID);	//д��AddUpVehilcleLength
					SocketExtendDataPacket[indexA].MaxInterval				= socket_extend_dev.GetMaxInterval(SocketExtendDataPacket[indexA].OutputID);		//д��MaxInterval
					SocketExtendDataPacket[indexA].MinInterval				= socket_extend_dev.GetMinInterval(SocketExtendDataPacket[indexA].OutputID);		//д��MinInterval
					SocketExtendDataPacket[indexA].AddUpInterval				= socket_extend_dev.GetAddUpInterval(SocketExtendDataPacket[indexA].OutputID);		//д��AddUpInterval
					SocketExtendDataPacket[indexA].MaxQueueLength			= socket_extend_dev.GetMaxQueueLength(SocketExtendDataPacket[indexA].OutputID);		//д��MaxQueueLength
					SocketExtendDataPacket[indexA].MinQueueLength			= socket_extend_dev.GetMinQueueLength(SocketExtendDataPacket[indexA].OutputID);		//д��MinQueueLength
					SocketExtendDataPacket[indexA].AddUpQueueLength			= socket_extend_dev.GetAddUpQueueLength(SocketExtendDataPacket[indexA].OutputID);	//д��AddUpQueueLength
					SocketExtendDataPacket[indexA].AddUpQueueTime			= socket_extend_dev.GetAddUpQueueTime(SocketExtendDataPacket[indexA].OutputID);		//д��AddUpQueueTime
					SocketExtendDataPacket[indexA].RedLightCount				= socket_extend_dev.GetRedLightCount(SocketExtendDataPacket[indexA].OutputID);		//д��RedLightCount
					SocketExtendDataPacket[indexA].PrecedingWayFullTime		= socket_extend_dev.GetPrecedingWayFullTime(SocketExtendDataPacket[indexA].OutputID);//д��PrecedingWayFullTime
					SocketExtendDataPacket[indexA].DirverWayCode				= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);		//д��DirverWayCode
					SocketExtendDataPacket[indexA].Status					= socket_extend_dev.GetStatus(SocketExtendDataPacket[indexA].OutputID);			//д��Status
					SocketExtendDataPacket[indexA].Reserver1				= socket_extend_dev.GetReserver1(SocketExtendDataPacket[indexA].OutputID);			//д��Reserver1
					SocketExtendDataPacket[indexA].Reserver2				= socket_extend_dev.GetReserver2(SocketExtendDataPacket[indexA].OutputID);			//д��Reserver2
				}
			}
			else {																				//���شŻ�ȡ����
				if (SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].OutputID != 0) {							//���شŶ�Ӧ���ش�������
					SocketExtendDataPacket[indexA].RecordNo					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].RecordNo;						//д��RecordNo
					SocketExtendDataPacket[indexA].SendTime					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].SendTime;						//д��SendTime
					SocketExtendDataPacket[indexA].VehicleCount				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].VehicleCount;					//д��VehicleCount
					SocketExtendDataPacket[indexA].LargerVehicleCount			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].LargerVehicleCount;				//д��LargerVehicleCount
					SocketExtendDataPacket[indexA].MidVehicleCount			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MidVehicleCount;				//д��MidVehicleCount
					SocketExtendDataPacket[indexA].SmallVehilceCount			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].SmallVehilceCount;				//д��SmallVehilceCount
					SocketExtendDataPacket[indexA].MiniVehicleCount			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MiniVehicleCount;				//д��MiniVehicleCount
					SocketExtendDataPacket[indexA].Moto					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].Moto;						//д��Moto
					SocketExtendDataPacket[indexA].ReserverType1				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType1;					//д��ReserverType1
					SocketExtendDataPacket[indexA].ReserverType2				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType2;					//д��ReserverType2
					SocketExtendDataPacket[indexA].ReserverType3				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType3;					//д��ReserverType3
					SocketExtendDataPacket[indexA].ReserverType4				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType4;					//д��ReserverType4
					SocketExtendDataPacket[indexA].ReserverType5				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType5;					//д��ReserverType5
					SocketExtendDataPacket[indexA].ReserverType6				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].ReserverType6;					//д��ReserverType6
					SocketExtendDataPacket[indexA].MaxOccupancy				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxOccupancy;					//д��MaxOccupancy
					SocketExtendDataPacket[indexA].MinOccupancy				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinOccupancy;					//д��MinOccupancy
					SocketExtendDataPacket[indexA].AddUpOccupancy			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpOccupancy;				//д��AddUpOccupancy
					SocketExtendDataPacket[indexA].MaxHeadInterval			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxHeadInterval;				//д��MaxHeadInterval
					SocketExtendDataPacket[indexA].MinHeadInterval			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinHeadInterval;				//д��MinHeadInterval
					SocketExtendDataPacket[indexA].AddUpHeadInterval			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpHeadInterval;				//д��AddUpHeadInterval
					SocketExtendDataPacket[indexA].MaxSpeed					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxSpeed;						//д��MaxSpeed
					SocketExtendDataPacket[indexA].MinSpeed					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinSpeed;						//д��MinSpeed
					SocketExtendDataPacket[indexA].AddUpSpeed				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpSpeed;					//д��AddUpSpeed
					SocketExtendDataPacket[indexA].MaxVehicleLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxVehicleLength;				//д��MaxVehicleLength
					SocketExtendDataPacket[indexA].MinVehicleLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinVehicleLength;				//д��MinVehicleLength
					SocketExtendDataPacket[indexA].AddUpVehilcleLength		= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpVehilcleLength;			//д��AddUpVehilcleLength
					SocketExtendDataPacket[indexA].MaxInterval				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxInterval;					//д��MaxInterval
					SocketExtendDataPacket[indexA].MinInterval				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinInterval;					//д��MinInterval
					SocketExtendDataPacket[indexA].AddUpInterval				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpInterval;					//д��AddUpInterval
					SocketExtendDataPacket[indexA].MaxQueueLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MaxQueueLength;				//д��MaxQueueLength
					SocketExtendDataPacket[indexA].MinQueueLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].MinQueueLength;				//д��MinQueueLength
					SocketExtendDataPacket[indexA].AddUpQueueLength			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpQueueLength;				//д��AddUpQueueLength
					SocketExtendDataPacket[indexA].AddUpQueueTime			= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].AddUpQueueTime;				//д��AddUpQueueTime
					SocketExtendDataPacket[indexA].RedLightCount				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].RedLightCount;					//д��RedLightCount
					SocketExtendDataPacket[indexA].PrecedingWayFullTime		= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].PrecedingWayFullTime;			//д��PrecedingWayFullTime
					SocketExtendDataPacket[indexA].DirverWayCode				= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);		//д��DirverWayCode
					SocketExtendDataPacket[indexA].Status					= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].Status;						//д��Status
					SocketExtendDataPacket[indexA].Reserver1				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].Reserver1;					//д��Reserver1
					SocketExtendDataPacket[indexA].Reserver2				= SocketExtendDataPacket[indexA - SPEEDLANNUMMAX].Reserver2;					//д��Reserver2
				}
				else {																			//���شŶ�Ӧ���ش�������
					for (indexB = indexA; indexB > SPEEDLANNUMMAX; indexB--) {								//Ѱ�ұ�������һ��ͬID����
						if (SocketExtendDataPacket[indexA].OutputID == SocketExtendDataPacket[indexB - 1].OutputID) {
							SocketExtendDataPacket[indexA].RecordNo				= SocketExtendDataPacket[indexB - 1].RecordNo;							//д��RecordNo
							SocketExtendDataPacket[indexA].SendTime				= SocketExtendDataPacket[indexB - 1].SendTime;							//д��SendTime
							SocketExtendDataPacket[indexA].VehicleCount			= SocketExtendDataPacket[indexB - 1].VehicleCount;						//д��VehicleCount
							SocketExtendDataPacket[indexA].LargerVehicleCount		= SocketExtendDataPacket[indexB - 1].LargerVehicleCount;					//д��LargerVehicleCount
							SocketExtendDataPacket[indexA].MidVehicleCount		= SocketExtendDataPacket[indexB - 1].MidVehicleCount;						//д��MidVehicleCount
							SocketExtendDataPacket[indexA].SmallVehilceCount		= SocketExtendDataPacket[indexB - 1].SmallVehilceCount;					//д��SmallVehilceCount
							SocketExtendDataPacket[indexA].MiniVehicleCount		= SocketExtendDataPacket[indexB - 1].MiniVehicleCount;						//д��MiniVehicleCount
							SocketExtendDataPacket[indexA].Moto				= SocketExtendDataPacket[indexB - 1].Moto;								//д��Moto
							SocketExtendDataPacket[indexA].ReserverType1			= SocketExtendDataPacket[indexB - 1].ReserverType1;						//д��ReserverType1
							SocketExtendDataPacket[indexA].ReserverType2			= SocketExtendDataPacket[indexB - 1].ReserverType2;						//д��ReserverType2
							SocketExtendDataPacket[indexA].ReserverType3			= SocketExtendDataPacket[indexB - 1].ReserverType3;						//д��ReserverType3
							SocketExtendDataPacket[indexA].ReserverType4			= SocketExtendDataPacket[indexB - 1].ReserverType4;						//д��ReserverType4
							SocketExtendDataPacket[indexA].ReserverType5			= SocketExtendDataPacket[indexB - 1].ReserverType5;						//д��ReserverType5
							SocketExtendDataPacket[indexA].ReserverType6			= SocketExtendDataPacket[indexB - 1].ReserverType6;						//д��ReserverType6
							SocketExtendDataPacket[indexA].MaxOccupancy			= SocketExtendDataPacket[indexB - 1].MaxOccupancy;						//д��MaxOccupancy
							SocketExtendDataPacket[indexA].MinOccupancy			= SocketExtendDataPacket[indexB - 1].MinOccupancy;						//д��MinOccupancy
							SocketExtendDataPacket[indexA].AddUpOccupancy		= SocketExtendDataPacket[indexB - 1].AddUpOccupancy;						//д��AddUpOccupancy
							SocketExtendDataPacket[indexA].MaxHeadInterval		= SocketExtendDataPacket[indexB - 1].MaxHeadInterval;						//д��MaxHeadInterval
							SocketExtendDataPacket[indexA].MinHeadInterval		= SocketExtendDataPacket[indexB - 1].MinHeadInterval;						//д��MinHeadInterval
							SocketExtendDataPacket[indexA].AddUpHeadInterval		= SocketExtendDataPacket[indexB - 1].AddUpHeadInterval;					//д��AddUpHeadInterval
							SocketExtendDataPacket[indexA].MaxSpeed				= SocketExtendDataPacket[indexB - 1].MaxSpeed;							//д��MaxSpeed
							SocketExtendDataPacket[indexA].MinSpeed				= SocketExtendDataPacket[indexB - 1].MinSpeed;							//д��MinSpeed
							SocketExtendDataPacket[indexA].AddUpSpeed			= SocketExtendDataPacket[indexB - 1].AddUpSpeed;							//д��AddUpSpeed
							SocketExtendDataPacket[indexA].MaxVehicleLength		= SocketExtendDataPacket[indexB - 1].MaxVehicleLength;						//д��MaxVehicleLength
							SocketExtendDataPacket[indexA].MinVehicleLength		= SocketExtendDataPacket[indexB - 1].MinVehicleLength;						//д��MinVehicleLength
							SocketExtendDataPacket[indexA].AddUpVehilcleLength	= SocketExtendDataPacket[indexB - 1].AddUpVehilcleLength;					//д��AddUpVehilcleLength
							SocketExtendDataPacket[indexA].MaxInterval			= SocketExtendDataPacket[indexB - 1].MaxInterval;							//д��MaxInterval
							SocketExtendDataPacket[indexA].MinInterval			= SocketExtendDataPacket[indexB - 1].MinInterval;							//д��MinInterval
							SocketExtendDataPacket[indexA].AddUpInterval			= SocketExtendDataPacket[indexB - 1].AddUpInterval;						//д��AddUpInterval
							SocketExtendDataPacket[indexA].MaxQueueLength		= SocketExtendDataPacket[indexB - 1].MaxQueueLength;						//д��MaxQueueLength
							SocketExtendDataPacket[indexA].MinQueueLength		= SocketExtendDataPacket[indexB - 1].MinQueueLength;						//д��MinQueueLength
							SocketExtendDataPacket[indexA].AddUpQueueLength		= SocketExtendDataPacket[indexB - 1].AddUpQueueLength;						//д��AddUpQueueLength
							SocketExtendDataPacket[indexA].AddUpQueueTime		= SocketExtendDataPacket[indexB - 1].AddUpQueueTime;						//д��AddUpQueueTime
							SocketExtendDataPacket[indexA].RedLightCount			= SocketExtendDataPacket[indexB - 1].RedLightCount;						//д��RedLightCount
							SocketExtendDataPacket[indexA].PrecedingWayFullTime	= SocketExtendDataPacket[indexB - 1].PrecedingWayFullTime;					//д��PrecedingWayFullTime
							SocketExtendDataPacket[indexA].DirverWayCode			= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);	//д��DirverWayCode
							SocketExtendDataPacket[indexA].Status				= SocketExtendDataPacket[indexB - 1].Status;								//д��Status
							SocketExtendDataPacket[indexA].Reserver1			= SocketExtendDataPacket[indexB - 1].Reserver1;							//д��Reserver1
							SocketExtendDataPacket[indexA].Reserver2			= SocketExtendDataPacket[indexB - 1].Reserver2;							//д��Reserver2
							break;
						}
					}
					if (indexB == SPEEDLANNUMMAX) {													//�����ݰ���ͬID���ݰ�,��ȡ����
						SocketExtendDataPacket[indexA].RecordNo				= socket_extend_dev.GetRecordNo(SocketExtendDataPacket[indexA].OutputID);			//д��RecordNo
						SocketExtendDataPacket[indexA].SendTime				= socket_extend_dev.GetSendTime(SocketExtendDataPacket[indexA].OutputID);			//д��SendTime
						SocketExtendDataPacket[indexA].VehicleCount			= socket_extend_dev.GetVehicleCount(SocketExtendDataPacket[indexA].OutputID);		//д��VehicleCount
						SocketExtendDataPacket[indexA].LargerVehicleCount		= socket_extend_dev.GetLargerVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//д��LargerVehicleCount
						SocketExtendDataPacket[indexA].MidVehicleCount		= socket_extend_dev.GetMidVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//д��MidVehicleCount
						SocketExtendDataPacket[indexA].SmallVehilceCount		= socket_extend_dev.GetSmallVehilceCount(SocketExtendDataPacket[indexA].OutputID);	//д��SmallVehilceCount
						SocketExtendDataPacket[indexA].MiniVehicleCount		= socket_extend_dev.GetMiniVehicleCount(SocketExtendDataPacket[indexA].OutputID);	//д��MiniVehicleCount
						SocketExtendDataPacket[indexA].Moto				= socket_extend_dev.GetMotoVehilceCount(SocketExtendDataPacket[indexA].OutputID);	//д��Moto
						SocketExtendDataPacket[indexA].ReserverType1			= socket_extend_dev.GetReserverType1(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType1
						SocketExtendDataPacket[indexA].ReserverType2			= socket_extend_dev.GetReserverType2(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType2
						SocketExtendDataPacket[indexA].ReserverType3			= socket_extend_dev.GetReserverType3(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType3
						SocketExtendDataPacket[indexA].ReserverType4			= socket_extend_dev.GetReserverType4(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType4
						SocketExtendDataPacket[indexA].ReserverType5			= socket_extend_dev.GetReserverType5(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType5
						SocketExtendDataPacket[indexA].ReserverType6			= socket_extend_dev.GetReserverType6(SocketExtendDataPacket[indexA].OutputID);		//д��ReserverType6
						SocketExtendDataPacket[indexA].MaxOccupancy			= socket_extend_dev.GetMaxOccupancy(SocketExtendDataPacket[indexA].OutputID);		//д��MaxOccupancy
						SocketExtendDataPacket[indexA].MinOccupancy			= socket_extend_dev.GetMinOccupancy(SocketExtendDataPacket[indexA].OutputID);		//д��MinOccupancy
						SocketExtendDataPacket[indexA].AddUpOccupancy		= socket_extend_dev.GetAddUpOccupancy(SocketExtendDataPacket[indexA].OutputID);		//д��AddUpOccupancy
						SocketExtendDataPacket[indexA].MaxHeadInterval		= socket_extend_dev.GetMaxHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//д��MaxHeadInterval
						SocketExtendDataPacket[indexA].MinHeadInterval		= socket_extend_dev.GetMinHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//д��MinHeadInterval
						SocketExtendDataPacket[indexA].AddUpHeadInterval		= socket_extend_dev.GetAddUpHeadInterval(SocketExtendDataPacket[indexA].OutputID);	//д��AddUpHeadInterval
						SocketExtendDataPacket[indexA].MaxSpeed				= socket_extend_dev.GetMaxSpeed(SocketExtendDataPacket[indexA].OutputID);			//д��MaxSpeed
						SocketExtendDataPacket[indexA].MinSpeed				= socket_extend_dev.GetMinSpeed(SocketExtendDataPacket[indexA].OutputID);			//д��MinSpeed
						SocketExtendDataPacket[indexA].AddUpSpeed			= socket_extend_dev.GetAddUpSpeed(SocketExtendDataPacket[indexA].OutputID);		//д��AddUpSpeed
						SocketExtendDataPacket[indexA].MaxVehicleLength		= socket_extend_dev.GetMaxVehicleLength(SocketExtendDataPacket[indexA].OutputID);	//д��MaxVehicleLength
						SocketExtendDataPacket[indexA].MinVehicleLength		= socket_extend_dev.GetMinVehicleLength(SocketExtendDataPacket[indexA].OutputID);	//д��MinVehicleLength
						SocketExtendDataPacket[indexA].AddUpVehilcleLength	= socket_extend_dev.GetAddUpVehilcleLength(SocketExtendDataPacket[indexA].OutputID);	//д��AddUpVehilcleLength
						SocketExtendDataPacket[indexA].MaxInterval			= socket_extend_dev.GetMaxInterval(SocketExtendDataPacket[indexA].OutputID);		//д��MaxInterval
						SocketExtendDataPacket[indexA].MinInterval			= socket_extend_dev.GetMinInterval(SocketExtendDataPacket[indexA].OutputID);		//д��MinInterval
						SocketExtendDataPacket[indexA].AddUpInterval			= socket_extend_dev.GetAddUpInterval(SocketExtendDataPacket[indexA].OutputID);		//д��AddUpInterval
						SocketExtendDataPacket[indexA].MaxQueueLength		= socket_extend_dev.GetMaxQueueLength(SocketExtendDataPacket[indexA].OutputID);		//д��MaxQueueLength
						SocketExtendDataPacket[indexA].MinQueueLength		= socket_extend_dev.GetMinQueueLength(SocketExtendDataPacket[indexA].OutputID);		//д��MinQueueLength
						SocketExtendDataPacket[indexA].AddUpQueueLength		= socket_extend_dev.GetAddUpQueueLength(SocketExtendDataPacket[indexA].OutputID);	//д��AddUpQueueLength
						SocketExtendDataPacket[indexA].AddUpQueueTime		= socket_extend_dev.GetAddUpQueueTime(SocketExtendDataPacket[indexA].OutputID);		//д��AddUpQueueTime
						SocketExtendDataPacket[indexA].RedLightCount			= socket_extend_dev.GetRedLightCount(SocketExtendDataPacket[indexA].OutputID);		//д��RedLightCount
						SocketExtendDataPacket[indexA].PrecedingWayFullTime	= socket_extend_dev.GetPrecedingWayFullTime(SocketExtendDataPacket[indexA].OutputID);//д��PrecedingWayFullTime
						SocketExtendDataPacket[indexA].DirverWayCode			= socket_extend_dev.GetDirverWayCode(SocketExtendDataPacket[indexA].OutputID, indexA);		//д��DirverWayCode
						SocketExtendDataPacket[indexA].Status				= socket_extend_dev.GetStatus(SocketExtendDataPacket[indexA].OutputID);			//д��Status
						SocketExtendDataPacket[indexA].Reserver1			= socket_extend_dev.GetReserver1(SocketExtendDataPacket[indexA].OutputID);			//д��Reserver1
						SocketExtendDataPacket[indexA].Reserver2			= socket_extend_dev.GetReserver2(SocketExtendDataPacket[indexA].OutputID);			//д��Reserver2
					}
				}
			}
		}
	}
}

/**********************************************************************************************************
 @Function			void SOCKET_Extend_RtcCheckFillData(void)
 @Description			����ʱ�����������ݰ�
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
 @Description			������ȡStartX��ʼ��ֵ,ֱ����ֵ(0x88)
 @Input				void
 @Return				u8  StartX : 		��ʼ��0x88
**********************************************************************************************************/
u8 SOCKET_Extend_GetStartX(void)
{
	return SOCKET_EXTEND_STARTX;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetExitX(void)
 @Description			������ȡExitX������ֵ,ֱ����ֵ(0x99)
 @Input				void
 @Return				u8  ExitX : 		������0x99
**********************************************************************************************************/
u8 SOCKET_Extend_GetExitX(void)
{
	return SOCKET_EXTEND_EXITX;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetServerType(void)
 @Description			������ȡServerTypeҵ������ֵ
 @Input				void
 @Return				u8  ServerType
**********************************************************************************************************/
u8 SOCKET_Extend_GetServerType(void)
{
	u8 ServerType = SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE;
	
	if (SOCKET_EXTEND_RTC_CHECK == SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT) {		//�ж��Ƿ���ҪRTC��ʱУ��
		if (PlatformSocketExtendtime == SocketExtendTime_DISABLE) {				//����SNѡ���Ƿ��ʱ
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
 @Description			������ȡCrossID·�ڴ���
 @Input				
 @Return				u32 CrossID ֵ
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
 @Description			������ȡDataLength���ݳ��� (����ʱ:���ݰ�����)
 @Input				Socket_Extend_Packet_Data *addr 	: SocketExtend�������ݰ�����ַ
 @Return				u16 DataLength ֵ
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
 @Description			������ȡChannel�������ֵ
 @Input				void
 @Return				u8  Channel
**********************************************************************************************************/
u16 SOCKET_Extend_GetChannel(void)
{
	return SOCKET_EXTEND_CHANNEL;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetCheckCode(void)
 @Description			������ȡCheckCodeУ����ֵ
 @Input				void
 @Return				u8  CheckCode
**********************************************************************************************************/
u8 SOCKET_Extend_GetCheckCode(void)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetDataCount(Socket_Extend_Packet_Data *addr)
 @Description			������ȡDataCount�������� (����ʱ:��������)
 @Input				Socket_Extend_Packet_Data *addr 	: SocketExtend�������ݰ�����ַ
 @Return				u8 DataCount ֵ
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
 @Description			������ȡRecordNo��ˮ��
 @Input				u16 outputid   	: 		����ID��
 @Return				u32 RecordNo		:		��ˮ��
**********************************************************************************************************/
u32 SOCKET_Extend_GetRecordNo(u16 outputid)
{
	static u32 SocketExtendRecordNo = 0;
	
	return SocketExtendRecordNo++;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetSendTime(u16 outputid)
 @Description			������ȡSendTime�ϱ�ʱ��
 @Input				u16 outputid   	: 		����ID��
 @Return				u32 SendTime		:		�ϱ�ʱ��
**********************************************************************************************************/
u32 SOCKET_Extend_GetSendTime(u16 outputid)
{
	u32 datetime = 0;
	
	datetime = RTC_GetCounter();
	
	return datetime;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetVehicleCount(u16 outputid)
 @Description			������ȡVehicleCount������
 @Input				u16 outputid   	: 		����ID��
 @Return				u16 VehicleCount	:		������
**********************************************************************************************************/
u16 SOCKET_Extend_GetVehicleCount(u16 outputid)
{
	u16 vehicleCount = 0;
	
	vehicleCount = calculation_dev.ReadVolume(outputid);
	
	return vehicleCount;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetLargerVehicleCount(u16 outputid)
 @Description			������ȡLargerVehicleCount������
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 LargerVehicleCount	:		������
**********************************************************************************************************/
u16 SOCKET_Extend_GetLargerVehicleCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMidVehicleCount(u16 outputid)
 @Description			������ȡMidVehicleCount�г�����
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 MidVehicleCount		:		�г�����
**********************************************************************************************************/
u16 SOCKET_Extend_GetMidVehicleCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetSmallVehilceCount(u16 outputid)
 @Description			������ȡSmallVehilceCountС������
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 SmallVehilceCount	:		С������
**********************************************************************************************************/
u16 SOCKET_Extend_GetSmallVehilceCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMiniVehicleCount(u16 outputid)
 @Description			������ȡMiniVehicleCount΢�ͳ�����
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 MiniVehicleCount	:		΢�ͳ�����
**********************************************************************************************************/
u16 SOCKET_Extend_GetMiniVehicleCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMotoVehilceCount(u16 outputid)
 @Description			������ȡMotoVehilceCountĦ�г�����
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 MotoVehilceCount	:		Ħ�г�����
**********************************************************************************************************/
u16 SOCKET_Extend_GetMotoVehilceCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType1(u16 outputid)
 @Description			������ȡReserverType1Ԥ������1
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 ReserverType1		:		Ԥ������1
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType1(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType2(u16 outputid)
 @Description			������ȡReserverType2Ԥ������2
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 ReserverType2		:		Ԥ������2
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType2(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType3(u16 outputid)
 @Description			������ȡReserverType3Ԥ������3
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 ReserverType3		:		Ԥ������3
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType3(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType4(u16 outputid)
 @Description			������ȡReserverType4Ԥ������4
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 ReserverType4		:		Ԥ������4
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType4(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType5(u16 outputid)
 @Description			������ȡReserverType5Ԥ������5
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 ReserverType5		:		Ԥ������5
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType5(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetReserverType6(u16 outputid)
 @Description			������ȡReserverType6Ԥ������6
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 ReserverType6		:		Ԥ������6
**********************************************************************************************************/
u16 SOCKET_Extend_GetReserverType6(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMaxHeadInterval(u16 outputid)
 @Description			������ȡMaxHeadInterval���ͷʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 MaxHeadInterval		:		���ͷʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMaxHeadInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMinHeadInterval(u16 outputid)
 @Description			������ȡMinHeadInterval��С��ͷʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 MinHeadInterval		:		��С��ͷʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMinHeadInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetAddUpHeadInterval(u16 outputid)
 @Description			������ȡAddUpHeadInterval�ۼƳ�ͷʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 AddUpHeadInterval	:		�ۼƳ�ͷʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetAddUpHeadInterval(u16 outputid)
{
	u32 avgheadtimeval = 0;
	
	avgheadtimeval = calculation_dev.ReadAvgHeadTime(outputid);
	
	return avgheadtimeval;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetMaxSpeed(u16 outputid)
 @Description			������ȡMaxSpeed�����(Km/h)
 @Input				u16 outputid   		: 		����ID��
 @Return				float MaxSpeed			:		�����(Km/h)
**********************************************************************************************************/
float SOCKET_Extend_GetMaxSpeed(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetMinSpeed(u16 outputid)
 @Description			������ȡMinSpeed��С����(Km/h)
 @Input				u16 outputid   		: 		����ID��
 @Return				float MinSpeed			:		��С����(Km/h)
**********************************************************************************************************/
float SOCKET_Extend_GetMinSpeed(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetAddUpSpeed(u16 outputid)
 @Description			������ȡAddUpSpeed�ۼƳ���(Km/h)
					��Ϊ���شŲ���,����ֵ��Ĭ�ϳ�����ռ��ʱ�����
					��Ϊ˫�شŲ���,����ֵ��˫�ش����ݻ�ȡ
 @Input				u16 outputid   		: 		����ID��
 @Return				float AddUpSpeed		:		�ۼƳ���(Km/h)
**********************************************************************************************************/
float SOCKET_Extend_GetAddUpSpeed(u16 outputid)
{
	u8 i = 0;
	u8 outputnum = 0;
	u32 avgoccupancyval = 0;
	float avgspeedval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {										//��ȡ������ID�����ֵ
		if (SocketExtendDataPacket[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) 										//�жϵ��شŲ��ٻ���˫�شŲ���
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
 @Description			������ȡMaxOccupancy���ռ��ʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 MaxOccupancy		:		���ռ��ʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMaxOccupancy(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMinOccupancy(u16 outputid)
 @Description			������ȡMinOccupancy��Сռ��ʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 MinOccupancy		:		��Сռ��ʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMinOccupancy(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetAddUpOccupancy(u16 outputid)
 @Description			������ȡAddUpOccupancy�ۼ�ռ��ʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 AddUpOccupancy		:		�ۼ�ռ��ʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetAddUpOccupancy(u16 outputid)
{
	u32 avgoccupancyval = 0;
	
	avgoccupancyval = calculation_dev.ReadAvgOccupancy(outputid);
	
	return avgoccupancyval;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMaxInterval(u16 outputid)
 @Description			������ȡMaxInterval�����ʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 MaxInterval		:		�����ʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMaxInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetMinInterval(u16 outputid)
 @Description			������ȡMinInterval��С���ʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 MinInterval		:		��С���ʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetMinInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetAddUpInterval(u16 outputid)
 @Description			������ȡAddUpInterval�ۼƼ��ʱ��(MS)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 AddUpInterval		:		�ۼƼ��ʱ��(MS)
**********************************************************************************************************/
u32 SOCKET_Extend_GetAddUpInterval(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetMaxVehicleLength(u16 outputid)
 @Description			������ȡMaxVehicleLength��󳵳�(��)
 @Input				u16 outputid   		: 		����ID��
 @Return				float MaxVehicleLength	:		��󳵳�(��)
**********************************************************************************************************/
float SOCKET_Extend_GetMaxVehicleLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetMinVehicleLength(u16 outputid)
 @Description			������ȡMinVehicleLength��С����(��)
 @Input				u16 outputid   		: 		����ID��
 @Return				float MinVehicleLength	:		��С����(��)
**********************************************************************************************************/
float SOCKET_Extend_GetMinVehicleLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			float SOCKET_Extend_GetAddUpVehilcleLength(u16 outputid)
 @Description			������ȡAddUpVehilcleLength�ۼƳ���(��)
					��Ϊ���شŲ���ģʽ �����ɺ궨������,���ɸı�
					��Ϊ˫�شŲ���ģʽ �������ٶ���ռ��ʱ�����
 @Input				u16 outputid   		: 		����ID��
 @Return				float AddUpVehilcleLength:		�ۼƳ���(��)
**********************************************************************************************************/
float SOCKET_Extend_GetAddUpVehilcleLength(u16 outputid)
{
	u8 i = 0;
	u8 outputnum = 0;
	float avglengthval = 0;
	float avgspeedval = 0;
	u16 avgoccupancyval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {										//��ȡ������ID�����ֵ
		if (SocketExtendDataPacket[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) 										//�жϵ��شŲ��ٻ���˫�شŲ���
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
 @Description			������ȡMaxQueueLength����Ŷӳ���(��)
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 MaxQueueLength		:		����Ŷӳ���(��)
**********************************************************************************************************/
u16 SOCKET_Extend_GetMaxQueueLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetMinQueueLength(u16 outputid)
 @Description			������ȡMinQueueLength��С�Ŷӳ���(��)
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 MinQueueLength		:		��С�Ŷӳ���(��)
**********************************************************************************************************/
u16 SOCKET_Extend_GetMinQueueLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetAddUpQueueLength(u16 outputid)
 @Description			������ȡAddUpQueueLength�ۼ��Ŷӳ���(��)
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 AddUpQueueLength	:		�ۼ��Ŷӳ���(��)
**********************************************************************************************************/
u32 SOCKET_Extend_GetAddUpQueueLength(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetAddUpQueueTime(u16 outputid)
 @Description			������ȡAddUpQueueTime�ۼ��Ŷ�ʱ��(��)
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 AddUpQueueTime		:		�ۼ��Ŷ�ʱ��(��)
**********************************************************************************************************/
u16 SOCKET_Extend_GetAddUpQueueTime(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetRedLightCount(u16 outputid)
 @Description			������ȡRedLightCount����ƴ�������
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 RedLightCount		:		����ƴ�������
**********************************************************************************************************/
u16 SOCKET_Extend_GetRedLightCount(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_GetPrecedingWayFullTime(u16 outputid)
 @Description			������ȡPrecedingWayFullTimeǰ���������ۼ�ʱ��
 @Input				u16 outputid   		: 		����ID��
 @Return				u16 PrecedingWayFullTime	:		ǰ���������ۼ�ʱ��
**********************************************************************************************************/
u16 SOCKET_Extend_GetPrecedingWayFullTime(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u32 SOCKET_Extend_GetDirverWayCode(u16 outputid, u16 inlen)
 @Description			������ȡDirverWayCode��������
 @Input				u16 outputid   		: 		����ID��
 @Return				u32 DirverWayCode		:		��������
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
 @Description			������ȡStatus״̬(0δ����1����)
 @Input				u16 outputid   		: 		����ID��
 @Return				u8  Status			:		״̬(0δ����1����)
**********************************************************************************************************/
u8 SOCKET_Extend_GetStatus(u16 outputid)
{
	return SOCKET_EXTEND_STATUS_ENABLE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetReserver1(u16 outputid)
 @Description			������ȡReserver1Ԥ��
 @Input				u16 outputid   		: 		����ID��
 @Return				u8  Reserver1			:		Ԥ��
**********************************************************************************************************/
u8 SOCKET_Extend_GetReserver1(u16 outputid)
{
	return 0;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_GetReserver2(u16 outputid)
 @Description			������ȡReserver2Ԥ��
 @Input				u16 outputid   		: 		����ID��
 @Return				u8  Reserver2			:		Ԥ��
**********************************************************************************************************/
u8 SOCKET_Extend_GetReserver2(u16 outputid)
{
	return 0;
}

/********************************************** END OF FLEE **********************************************/
