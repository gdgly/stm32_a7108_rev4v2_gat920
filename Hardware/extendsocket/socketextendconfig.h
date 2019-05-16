#ifndef __SOCKET_EXTEND_CONFIG_H
#define   __SOCKET_EXTEND_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

#define	SOCKET_EXTEND_CACHE_SIZE				4096					//4KЭ�黺��, 32·������3632Byte
#define	SOCKET_EXTEND_RECVIVE_SIZE			30					//���ջ���,���ݰ�ͷ17Byte

#define	SOCKET_EXTEND_AGAINSENDTIME			5					//5���û�յ��ذ��ط�����

#define	SOCKET_EXTEND_TRUE					0
#define	SOCKET_EXTEND_FALSE					1

#define	SOCKET_EXTEND_STARTX				0x88					//��ʼ��(0x88)
#define	SOCKET_EXTEND_EXITX					0x99					//������(0x99)

#define	SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE	0x01					//RTC����ʱ
#define	SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT	0x04					//RTC��ʱ

#define	SOCKET_EXTEND_SERVERTYPE_CLIENT		0xC0					//ҵ������ �ͻ���
#define	SOCKET_EXTEND_SERVERTYPE_CROSS		0x40					//ҵ������ ·��·��
#define	SOCKET_EXTEND_SERVERTYPE_SIGNAL		0x80					//ҵ������ �źſ�����
#define	SOCKET_EXTEND_SERVERTYPE_SERVER		0x00					//ҵ������ ������

#define	SOCKET_EXTEND_CHANNEL				0x0000				//�������

#define	SOCKET_EXTEND_STATUS_DISABLE			0x00					//״̬(0δ����)
#define	SOCKET_EXTEND_STATUS_ENABLE			0x01					//״̬(1����)

/**********************************************************************************************************
 @attention : 
 * Socket Extend ��ͷ   ( STX, TYPE, HSC, DL, CN, ETX, CRC, COUNT ) 											�� 17 Byte
 * Socket Extend ������ ( RecordNo, SendTime, VehicleCount, LargerVehicleCount, MidVehicleCount, SmallVehilceCount,
 * 					 MiniVehicleCount, Moto, ReserverType1, ReserverType2, ReserverType3, ReserverType4, 
 *					 ReserverType5, ReserverType6, MaxHeadInterval, MinHeadInterval, AddUpHeadInterval, 
 *					 MaxSpeed, MinSpeed, AddUpSpeed, MaxOccupancy, MinOccupancy, AddUpOccupancy, 
 *					 MaxInterval, MinInterval, AddUpInterval, MaxVehicleLength, MinVehicleLength, 
 *					 AddUpVehilcleLength, MaxQueueLength, MinQueueLength, AddUpQueueLength, AddUpQueueTime, 
 *					 RedLightCount, PrecedingWayFullTime, DirverWayCode, Status, Reserver1, Reserver2)			��113Byte
 *
**********************************************************************************************************/
typedef struct
{
	u8		StartX;											//��ʼ��(0x88)
	u8		ServerType;										//ҵ������(0x41, ��4λ0x01, ��4λ:�ͻ���0xc0;·��·��0x40;�źſ�����0x80;������0x00)
	u32		CrossID;											//·�ڴ���
	u16		DataLength;										//���ݳ���
	u16		Channel;											//�������
	u8		ExitX;											//������(0x99)
	u8		CheckCode;										//У����
	u8		DataCount;										//��������
}Socket_Extend_Packet_Head;										//���ݰ�ͷ

typedef struct
{
	u16		OutputID;											//����˿ڵĲ���
	u32		RecordNo;											//��ˮ��
	u32		SendTime;											//�ϱ�ʱ��
	
	u16		VehicleCount;										//������(һ����ͨ���ĳ�����)
	u16		LargerVehicleCount;									//������(һ����ͨ���Ĵ�����)
	u16		MidVehicleCount;									//�г�����(һ����ͨ�����г�����)
	u16		SmallVehilceCount;									//С������(һ����ͨ����С������)
	u16		MiniVehicleCount;									//΢�ͳ�����(һ����ͨ����΢�ͳ�����)
	u16		Moto;											//Ħ�г�����(һ����ͨ����Ħ�г�����)
	
	u16		ReserverType1;										//Ԥ������1
	u16		ReserverType2;										//Ԥ������2
	u16		ReserverType3;										//Ԥ������3
	u16		ReserverType4;										//Ԥ������4
	u16		ReserverType5;										//Ԥ������5
	u16		ReserverType6;										//Ԥ������6
	
	u32		MaxHeadInterval;									//���ͷʱ��(MS)
	u32		MinHeadInterval;									//��С��ͷʱ��(MS)
	u32		AddUpHeadInterval;									//�ۼƳ�ͷʱ��(MS)
	
	float	MaxSpeed;											//�����(Km/h)
	float	MinSpeed;											//��С����(Km/h)
	float	AddUpSpeed;										//�ۼƳ���(Km/h)
	
	u32		MaxOccupancy;										//���ռ��ʱ��(MS)
	u32		MinOccupancy;										//��Сռ��ʱ��(MS)
	u32		AddUpOccupancy;									//�ۼ�ռ��ʱ��(MS)
	
	u32		MaxInterval;										//�����ʱ��(MS)
	u32		MinInterval;										//��С���ʱ��(MS)
	u32		AddUpInterval;										//�ۼƼ��ʱ��(MS)
	
	float	MaxVehicleLength;									//��󳵳�(��)
	float	MinVehicleLength;									//��С����(��)
	float	AddUpVehilcleLength;								//�ۼƳ���(��)
	
	u16		MaxQueueLength;									//����Ŷӳ���(��)
	u16		MinQueueLength;									//��С�Ŷӳ���(��)
	u32		AddUpQueueLength;									//�ۼ��Ŷӳ���(��)
	
	u16		AddUpQueueTime;									//�ۼ��Ŷ�ʱ��(��)
	u16		RedLightCount;										//����ƴ�������
	u16		PrecedingWayFullTime;								//ǰ���������ۼ�ʱ��
	u32		DirverWayCode;										//��������
	u8		Status;											//״̬(0δ����1����)
	u8		Reserver1;										//Ԥ��
	u8		Reserver2;										//Ԥ��
}Socket_Extend_Packet_Data;										//�������ݰ�

/* SocketExtend�ⲿ�ӿ����ݶ�ȡ���ƹ����� */
struct _m_socket_extend_dev
{
	u8		(*GetStartX)(void);									//��ȡStartX��ʼ��
	u8		(*GetExitX)(void);									//��ȡExitX������
	u8		(*GetServerType)(void);								//��ȡServerTypeҵ������
	u32		(*GetCrossID)(void);								//��ȡCrossID·�ڴ���
	u16		(*GetDataLength)(Socket_Extend_Packet_Data *addr);		//��ȡDataLength���ݳ���
	u16		(*GetChannel)(void);								//��ȡChannel�������
	u8		(*GetCheckCode)(void);								//��ȡCheckCodeУ����
	u8		(*GetDataCount)(Socket_Extend_Packet_Data *addr);			//��ȡDataCount��������
	
	u32		(*GetRecordNo)(u16);								//��ȡRecordNo��ˮ��
	u32		(*GetSendTime)(u16);								//��ȡSendTime�ϱ�ʱ��
	u16		(*GetVehicleCount)(u16);								//������ȡVehicleCount������
	u16		(*GetLargerVehicleCount)(u16);						//������ȡLargerVehicleCount������
	u16		(*GetMidVehicleCount)(u16);							//������ȡMidVehicleCount�г�����
	u16		(*GetSmallVehilceCount)(u16);							//������ȡSmallVehilceCountС������
	u16		(*GetMiniVehicleCount)(u16);							//������ȡMiniVehicleCount΢�ͳ�����
	u16		(*GetMotoVehilceCount)(u16);							//������ȡMotoVehilceCountĦ�г�����
	u16		(*GetReserverType1)(u16);							//������ȡReserverType1Ԥ������1
	u16		(*GetReserverType2)(u16);							//������ȡReserverType2Ԥ������2
	u16		(*GetReserverType3)(u16);							//������ȡReserverType3Ԥ������3
	u16		(*GetReserverType4)(u16);							//������ȡReserverType4Ԥ������4
	u16		(*GetReserverType5)(u16);							//������ȡReserverType5Ԥ������5
	u16		(*GetReserverType6)(u16);							//������ȡReserverType6Ԥ������6
	u32		(*GetMaxHeadInterval)(u16);							//������ȡMaxHeadInterval���ͷʱ��(MS)
	u32		(*GetMinHeadInterval)(u16);							//������ȡMinHeadInterval��С��ͷʱ��(MS)
	u32		(*GetAddUpHeadInterval)(u16);							//������ȡAddUpHeadInterval�ۼƳ�ͷʱ��(MS)
	float	(*GetMaxSpeed)(u16);								//������ȡMaxSpeed�����(Km/h)
	float	(*GetMinSpeed)(u16);								//������ȡMinSpeed��С����(Km/h)
	float	(*GetAddUpSpeed)(u16);								//������ȡAddUpSpeed�ۼƳ���(Km/h)
	u32		(*GetMaxOccupancy)(u16);								//������ȡMaxOccupancy���ռ��ʱ��(MS)
	u32		(*GetMinOccupancy)(u16);								//������ȡMinOccupancy��Сռ��ʱ��(MS)
	u32		(*GetAddUpOccupancy)(u16);							//������ȡAddUpOccupancy�ۼ�ռ��ʱ��(MS)
	u32		(*GetMaxInterval)(u16);								//������ȡMaxInterval�����ʱ��(MS)
	u32		(*GetMinInterval)(u16);								//������ȡMinInterval��С���ʱ��(MS)
	u32		(*GetAddUpInterval)(u16);							//������ȡAddUpInterval�ۼƼ��ʱ��(MS)
	float	(*GetMaxVehicleLength)(u16);							//������ȡMaxVehicleLength��󳵳�(��)
	float	(*GetMinVehicleLength)(u16);							//������ȡMinVehicleLength��С����(��)
	float	(*GetAddUpVehilcleLength)(u16);						//������ȡAddUpVehilcleLength�ۼƳ���(��)
	u16		(*GetMaxQueueLength)(u16);							//������ȡMaxQueueLength����Ŷӳ���(��)
	u16		(*GetMinQueueLength)(u16);							//������ȡMinQueueLength��С�Ŷӳ���(��)
	u32		(*GetAddUpQueueLength)(u16);							//������ȡAddUpQueueLength�ۼ��Ŷӳ���(��)
	u16		(*GetAddUpQueueTime)(u16);							//������ȡAddUpQueueTime�ۼ��Ŷ�ʱ��(��)
	u16		(*GetRedLightCount)(u16);							//������ȡRedLightCount����ƴ�������
	u16		(*GetPrecedingWayFullTime)(u16);						//������ȡPrecedingWayFullTimeǰ���������ۼ�ʱ��
	u32		(*GetDirverWayCode)(u16,u16);							//������ȡDirverWayCode��������
	u8		(*GetStatus)(u16);									//������ȡStatus״̬(0δ����1����)
	u8		(*GetReserver1)(u16);								//������ȡReserver1Ԥ��
	u8		(*GetReserver2)(u16);								//������ȡReserver2Ԥ��
	
	void		(*FillData)(void);									//����������������ݰ�
	void		(*RtcCheckFillData)(void);							//����ʱ�����������ݰ�
	u16		(*ObtainPacketHead)(u8 *addr);						//��ȡSocketExtend��ͷ���ݲ����뻺��
	u16		(*ObtainPacketData)(u8 *addr);						//��ȡSocketExtend�������ݰ������뻺��
	void		(*ObtainPacketCheckCode)(u8 *addr, u16);				//����У���벢����У����
	
	void		(*ReadOutputID)(u16 *outputid);						//��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�
	u16		(*PackagedStorage)(void);							//SocketExtend�����ݴ�����뻺����
	u16		(*RtcCheckPackagedStorage)(void);						//SocketExtend����ʱ���ݴ�����뻺����
	
	void		(*PortSerialInit)(USART_TypeDef* USARTx, u32 BaudRate);	//��ʼ��SocketExtend����
	void		(*UARTx_IRQ)(USART_TypeDef* USARTx);					//SocketExtendЭ�鴮���жϴ�����
	void		(*Init)(void);										//��ʼ��SocketExtendЭ��
	void		(*Implement)(u16 sendtime);							//SocketExtendЭ�鴦��
};

extern struct _m_socket_extend_dev socket_extend_dev;
extern u32 Crossid;

#endif
