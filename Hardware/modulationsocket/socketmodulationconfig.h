#ifndef __SOCKET_MODULATION_CONFIG_H
#define   __SOCKET_MODULATION_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

#define	SOCKET_MODULATION_CACHE_SIZE					1024						//1KЭ�黺��, 32·������786Byte
#define	SOCKET_MODULATION_RECVIVE_SIZE				30						//���ջ���,���ݰ�ͷ13Byte

#define	SOCKET_MODULATION_AGAINSENDTIME				20						//20���û�յ��ذ��ط�����

#define	SOCKET_MODULATION_TRUE						0
#define	SOCKET_MODULATION_FALSE						1

#define	SOCKET_MODULATION_HEAD_FRAME					0xAAAA					//֡ͷ(0xAAAA)
#define	SOCKET_MODULATION_TAIL_FRAME					0xEEEE					//֡β(0xEEEE)

#define	SOCKET_MODULATION_PACKET_TYPE_FLOWMESSAGE		0x01						//��ͨ������Ϣ
#define	SOCKET_MODULATION_PACKET_TYPE_FLOWMESSAGE_ACK	0x0A						//��ͨ������ϢӦ��

#define	SOCKET_MODULATION_DEVICE_ID					"0171280100000001"			//�豸���ʶ����
#define	SOCKET_MODULATION_STATION_ID					"S229CJXXX321204"			//վ�����

#define	SOCKET_MODULATION_DEVICE_ERR_CDOE				0x00						//�豸Ӳ��������
#define	SOCKET_MODULATION_SURVEY_CONTENT				0x01						//��������

#define	SOCKET_MODULATION_PROCESS_CYCLE				0x05						//��ͨ���ݴ�������(��)

#define	SOCKET_MODULATION_FOLLOW_CAR					51						//�����ٷֱ�
#define	SOCKET_MODULATION_AVG_HEADDISTANCE				0x0019					//ƽ����ͷ���

/**********************************************************************************************************
 @attention : 
 * Socket Modulation ֡ͷ		  ( 0xAA 0xAA )
 * Socket Modulation ����		  ( PacketLength )
 * Socket Modulation ���ݰ�����	  ( PacketData )
 * Socket Modulation CRCУ��		  ( CRCCheckCode )
 * Socket Modulation ֡β		  ( 0xEE 0xEE )
 *
 * Socket Modulation ���ݰ�����	: ( PacketType,DeviceID,StationID,DeviceErrCode,SurveyContent,DateYear,DateMon,DateDay,
							    ProcessCycle,TimeSerial,LaneNum,DataLane[OUTPUT_MAX] )
 * Socket Modulation ��������		: ( LaneNo,FollowCar,AvgHeadDistance,TimeOccupancy,
							    SmallVehilceCount,SmallVehilceAvgSpeed,
							    MidVehicleCount,MidVehicleAvgSpeed,
							    LargerVehicleCount,LargerVehicleAvgSpeed,
							    SupeLargerVehicleCount,SupeLargerVehicleAvgSpeed,
							    TractorCount,TractorAvgSpeed,
							    MotoCount,MotoAvgSpeed )
**********************************************************************************************************/
typedef struct
{
	u16							OutputID;									//����˿ڵĲ���
	u8							LaneNo;									//������
	u8							FollowCar;								//�����ٷֱ�
	u16							AvgHeadDistance;							//ƽ����ͷ���
	u8							TimeOccupancy;								//ʱ��ռ����
	u32							Avgoccupancyval;							//ռ��ʱ��
	
	u16							SmallVehilceCount;							//С�ͳ���ͨ����
	u8							SmallVehilceAvgSpeed;						//С�ͳ�ƽ���ص㳵��
	u16							MidVehicleCount;							//���ͳ���ͨ����
	u8							MidVehicleAvgSpeed;							//���ͳ�ƽ���ص㳵��
	u16							LargerVehicleCount;							//���ͳ���ͨ����
	u8							LargerVehicleAvgSpeed;						//���ͳ�ƽ���ص㳵��
	u16							SupeLargerVehicleCount;						//�ش��ͳ���ͨ����
	u8							SupeLargerVehicleAvgSpeed;					//�ش��ͳ�ƽ���ص㳵��
	u16							TractorCount;								//��������ͨ����
	u8							TractorAvgSpeed;							//������ƽ���ص㳵��
	u16							MotoCount;								//Ħ�г���ͨ����
	u8							MotoAvgSpeed;								//Ħ�г�ƽ���ص㳵��
}Socket_Modulation_Data_Lane;													//�������ݰ�

typedef struct
{
	u8							PacketType;								//���ݰ�����
	u8							DeviceID[16];								//�豸���ʶ����
	u8							StationID[15];								//վ�����
	u8							DeviceErrCode;								//�豸Ӳ��������
	u8							SurveyContent;								//��������
	u16							DateYear;									//��
	u8							DateMon;									//��
	u8							DateDay;									//��
	u8							ProcessCycle;								//��ͨ���ݴ�������
	u16							TimeSerial;								//ʱ�����
	u8							LaneNum;									//������
	Socket_Modulation_Data_Lane		DataLane[OUTPUT_MAX];						//�������ݰ�
}Socket_Modulation_Data_Packet;												//���ݰ�

typedef struct
{
	u16							HeadFrame;								//֡ͷ(0xAA 0xAA)
	u16							PacketLength;								//����
	Socket_Modulation_Data_Packet		PacketData;								//���ݰ�
	u16							CRCCheckCode;								//CRCУ��
	u16							TailFrame;								//֡β(0xEE 0xEE)
}Socket_Modulation_Data_Frame;												//����֡

/* SocketModulation�ⲿ�ӿ����ݶ�ȡ���ƹ����� */
struct _m_socket_modulation_dev
{
	u16		(*GetHeadFrame)(void);												//������ȡHeadFrame֡ͷ,ֱ����ֵ(0xAAAA)
	u16		(*GetTailFrame)(void);												//������ȡTailFrame֡β,ֱ����ֵ(0xEEEE)
	u16		(*GetCRCCheckCode)(void);											//������ȡCRCCheckCodeУ����
	u16		(*GetPacketLength)(Socket_Modulation_Data_Frame *addr);					//������ȡPacketLength����
	
	u8		(*GetPacketType)(void);												//������ȡPacketType���ݰ�����
	void		(*GetDeviceID)(u8 *deviceID);											//������ȡDeviceID�豸���ʶ����
	void		(*GetStationID)(u8 *stationID);										//������ȡStationIDվ�����
	u8		(*GetDeviceErrCode)(void);											//������ȡDeviceErrCode�豸Ӳ��������
	u8		(*GetSurveyContent)(void);											//������ȡSurveyContent��������
	u16		(*GetDateYear)(void);												//������ȡDateYear��
	u8		(*GetDateMon)(void);												//������ȡDateMon��
	u8		(*GetDateDay)(void);												//������ȡDateDay��
	u8		(*GetProcessCycle)(void);											//������ȡProcessCycle��ͨ���ݴ�������
	u16		(*GetTimeSerial)(void);												//������ȡTimeSerialʱ�����
	u8		(*GetLaneNum)(Socket_Modulation_Data_Frame *addr);						//������ȡLaneNum������
	
	u8		(*GetLaneNo)(Socket_Modulation_Data_Frame *addr, u16 outputid);				//������ȡLaneNo������
	u8		(*GetFollowCar)(Socket_Modulation_Data_Frame *addr, u16 outputid);			//������ȡFollowCar�����ٷֱ�
	u16		(*GetAvgHeadDistance)(Socket_Modulation_Data_Frame *addr, u16 outputid);		//������ȡAvgHeadDistanceƽ����ͷ���
	u8		(*GetTimeOccupancy)(Socket_Modulation_Data_Frame *addr, u16 outputid);		//������ȡTimeOccupancyʱ��ռ����
	
	u16		(*GetSmallVehilceCount)(Socket_Modulation_Data_Frame *addr, u16 outputid);		//������ȡSmallVehilceCountС�ͳ���ͨ����
	u8		(*GetSmallVehilceAvgSpeed)(Socket_Modulation_Data_Frame *addr, u16 outputid);	//������ȡSmallVehilceAvgSpeedС�ͳ�ƽ���ص㳵��
	
	void		(*FillData)(void);													//����������������ݰ�
	u16		(*ObtainDataFrame)(u8 *addr);											//��ȡSocketModulation����֡�����뻺��
	void		(*ObtainDataFrameCheckCode)(u8 *addr, u16 buflength);						//����У���벢����У����
	
	void		(*ReadOutputID)(u16 *outputid);										//��ȡoutput_ID����˿ڵĲ�����SocketModulation�������ݰ�
	u16		(*PackagedStorage)(void);											//SocketModulation�����ݴ�����뻺����
	
	void		(*PortSerialInit)(USART_TypeDef* USARTx, u32 BaudRate);					//��ʼ��SocketModulation����
	void		(*UARTx_IRQ)(USART_TypeDef* USARTx);									//SocketModulationЭ�鴮���жϴ�����
	void		(*Init)(void);														//��ʼ��SocketModulationЭ��
	void		(*Implement)(u16 sendtime);											//SocketModulationЭ�鴦��
};

extern struct _m_socket_modulation_dev socket_modulation_dev;
extern u32 Crossid;

#endif
