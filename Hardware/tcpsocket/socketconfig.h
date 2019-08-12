#ifndef __SOCKET_CONFIG_H
#define   __SOCKET_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"


#define	SOCKET_CACHE_SIZE				2048						//2KЭ�黺��, 32·������1903Byte
#define	SOCKET_RECVIVE_SIZE				20						//���ջ���,���ݰ�ͷ13Byte

#if (INTERVALTIME == 0)
#define	SOCKET_AGAINSENDTIME			5*900					//5���û�յ��ذ��ط�����
#define	SOCKET_PARK_AGAINSENDTIME		50						//50ms��û�յ��ذ��ط�����
#else
#define	SOCKET_AGAINSENDTIME			5						//5���û�յ��ذ��ط�����
#define	SOCKET_PARK_AGAINSENDTIME		50						//50ms��û�յ��ذ��ط�����
#endif

#define	SOCKET_TRUE					0
#define	SOCKET_FALSE					1


//��ˮ������
#define	PACKETTYPE_FLOWMESSAGE			0x01						//��ͨ������Ϣ
#define	PACKETTYPE_ERRORMESSAGE			0x02						//������Ϣ
#define	PACKETTYPE_BUTTONEVENT			0x03						//���˰�ť�¼�
#define	PACKETTYPE_BUSPRIORITYEVENT		0x04						//���������¼�
#define	PACKETTYPE_PARKINGLOTDATA		0x05						//ͣ��������
#define	PACKETTYPE_RTCCHECKINIT			0x80						//RTC�ϵ��ʱ
#define	PACKETTYPE_RTCCHECK				0x81						//RTC���ж�ʱ

#define	DEVICETYPE_DICI				0x42						//�ش��豸

#define	MANUFACTURERCODE				0x01						//�����̱���


/**********************************************************************************************************
 @attention : 
 * Socket ��ͷ   ( CrossID, PacketType, PacketInfo ) 														�� 13 Byte
 * Socket ������ ( DeviceType, Interval, LaneNo, DateTime, Volume, Volume1, Volume2, Volume3, Volume4, Volume5, 
 *			    AvgOccupancy, AvgHeadTime, AvgLength, AvgSpeed, Saturation, Density, Pcu, AvgQueueLength ) 		�� 59 Byte
 * Socket У��   ( ManufacturerCode, CheckCode ) 															�� 2 Byte
 *
**********************************************************************************************************/
typedef struct
{
	u32		CrossID;											//·�ڴ���
	u8		PacketType;										//��ˮ������
	u32		PacketInfo;										//����Ϣ(����ʱ:���ݰ��ڵļ�¼��,����ʱ:02/00/00/00�ɹ���ȡ)
}Socket_Packet_Head;											//���ݰ�ͷ

typedef struct
{
	u16		OutputID;											//����˿ڵĲ���
	u8		DeviceType;										//�شż����,ֱ����ֵB(0x42)
	u16		Interval;											//ͳ��ʱ��
	u16		LaneNo;											//������
	u32		DateTime;											//��ˮ������ʱ��
	u16		Volume;											//һ������ͨ���ĳ���
	u16		Volume1;											//΢С��������
	u16		Volume2;											//С������
	u16		Volume3;											//�г�����
	u16		Volume4;											//������
	u16		Volume5;											//��������
	u16		AvgOccupancy;										//ƽ��ռ��ʱ��
	u16		AvgHeadTime;										//ƽ����ͷʱ��
	float	AvgLength;										//ƽ������
	float	AvgSpeed;											//ƽ���ٶ�
	u8		Saturation;										//���Ͷ�
	u16		Density;											//�ܶ�
	u16		Voltage;											//Voltage
	float	AvgQueueLength;									//�Ŷӳ���
}Socket_Packet_Data;											//�������ݰ�


/* socket�ⲿ�ӿ����ݶ�ȡ���ƹ����� */
struct _m_socket_dev
{
	u32		(*GetCrossID)(void);								//��ȡCrossID·�ڴ���
	u8		(*GetPacketType)(void);								//��ȡPacketType��ˮ������
	u32		(*GetPacketInfo)(Socket_Packet_Data *addr);				//��ȡPacketInfo����Ϣ (����ʱ:���ݰ��ڵļ�¼��)
	
	u8		(*GetDeviceType)(u16);								//��ȡDeviceType�豸����ֵ,�شż����,ֱ����ֵB(0x42)
	u16		(*GetInterval)(u16);								//��ȡIntervalͳ��ʱ��
	u16		(*GetLaneNo)(u16);									//��ȡLaneNo������
	u32		(*GetDateTime)(u16);								//��ȡDateTime��ˮ������ʱ��
	u16		(*GetVolume)(u16);									//��ȡVolumeһ������ͨ���ĳ���
	u16		(*GetVolume1)(u16);									//��ȡVolume1΢С��������
	u16		(*GetVolume2)(u16);									//��ȡVolume2С������
	u16		(*GetVolume3)(u16);									//��ȡVolume3�г�����
	u16		(*GetVolume4)(u16);									//��ȡVolume4������
	u16		(*GetVolume5)(u16);									//��ȡVolume5��������
	u16		(*GetAvgOccupancy)(u16);								//��ȡAvgOccupancyƽ��ռ��ʱ��
	u16		(*GetAvgHeadTime)(u16);								//��ȡAvgHeadTimeƽ����ͷʱ��
	float	(*GetAvgLength)(u16);								//��ȡAvgLengthƽ������
	float	(*GetAvgSpeed)(u16);								//��ȡAvgSpeedƽ���ٶ�
	u8		(*GetSaturation)(u16);								//��ȡSaturation���Ͷ�
	u16		(*GetDensity)(u16);									//��ȡDensity�ܶ�
	u16		(*GetVoltage)(u16);									//��ȡVoltage
	float	(*GetAvgQueueLength)(u16);							//��ȡAvgQueueLength�Ŷӳ���
	
	void		(*FillData)(void);									//����������������ݰ�
	u16		(*ObtainPacketHead)(u8 *addr);						//��ȡSocket��ͷ���ݲ����뻺��
	u16		(*ObtainPacketData)(u8 *addr);						//��ȡSocket�������ݰ������뻺��
	u16		(*ObtainPacketManuCheck)(u8 *addr, u16);				//����У���벢���������̱����У����
	
	void		(*ReadOutputID)(u16 *outputid);						//��ȡoutput_ID����˿ڵĲ�����Socket�������ݰ�
	u16		(*PackagedStorage)(void);							//Socket�����ݴ�����뻺����
	
	void		(*PortSerialInit)(USART_TypeDef* USARTx, u32 BaudRate);	//��ʼ��Socket����
	void		(*UARTx_IRQ)(USART_TypeDef* USARTx);					//SocketЭ�鴮���жϴ�����
	void		(*Init)(void);										//��ʼ��SocketЭ��
	void		(*Implement)(u16 sendtime);							//SocketЭ�鴦��
};

extern struct _m_socket_dev socket_dev;
extern u32 Crossid;

#endif
