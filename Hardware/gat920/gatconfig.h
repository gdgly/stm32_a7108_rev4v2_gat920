#ifndef __GAT_CONFIG_H
#define   __GAT_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"


/* GAT״̬��״ֵ̬ */
typedef enum
{
	GAT_EVENT_READY,													//Ready
	GAT_EVENT_FRAME_RECEIVED,											//Received Frame
	GAT_EVENT_FRAME_SENT,												//Sent Frame
	GAT_EVENT_EXECUTE,													//Execute function
	GAT_EVENT_INITIALIZED,												//Initialized
	GAT_EVENT_ACTIVEUPLOAD												//ACTIVEUPLOAD
}GATEventType;
extern GATEventType	GATEvent;

/* GAT����״̬ */
typedef enum
{
	GAT_ONLINE,														//������
	GAT_OFFLINE														//δ����
}GATConnectType;
extern GATConnectType GATConnect;

/* GAT�ϴ����ݽ���Ӧ��״̬ */
typedef enum
{
	GAT_NotReceivedAck,													//δ�յ������ϴ�Ӧ��
	GAT_ReceivedAck													//�յ������ϴ�Ӧ��
}GATUPLOADAckType;
extern GATUPLOADAckType GATUPLOADAck;

/* GAT�����ϴ�ģʽ */
typedef enum
{
	GAT_ActiveUpLoadMode_NONE,											//���������ϴ�
	GAT_ActiveUpLoadMode_STATISTICS,										//ͳ�����������ϴ�
	GAT_ActiveUpLoadMode_PULSE,											//�������������ϴ�
	GAT_ActiveUpLoadMode_FAULT											//��������������ϴ�
}GATActiveUpLoadModeType;
extern GATActiveUpLoadModeType GATActiveUpLoadMode;

#define	GAT920_BAUDRATE						9600						//Ĭ�ϲ�����

#define	GAT920_UPLOAD_SEND_DELAY_MS				100

#define	GAT_TRUE								0
#define	GAT_FALSE								1
#define	GAT_DATAFALSE							2
#define	GAT_RECEIVEEVENT						3

#define	GAT_OUTDETECTIONAREA					0						//�����뿪�����
#define	GAT_INDETECTIONAREA						1						//������������

#define	GAT_CACHE_SIZE							1024						//Э�黺���С MAX 32768

#define	EquipmentManufacturers					"MoveBroad"				//�豸������
#define	EquipmentModel							"wvd"					//�豸�ͺ�
#define	DetectionChannels						32						//�����ͨ���� MAX 48 (���� <= OUTPUT_MAX 32)
#define	DetectionMode							0x04						//����ֶ� 1:��Ȧ 2:���� 3:�״� 4:����
#define	SignalOutputDelay						0						//�ź������ʱ
//#define	LINKADDRESS							1234						//��·��ַ(��Χ 0 ~ 8191)
#define	LINKADDRESS							param_recv.addr			//��·��ַ(��Χ 0 ~ 8191)
#define	PROTOCOL_VER							0x10						//Э��汾��

/* �������� */
#define	OPERATIONTYPE_QUERY_REQUEST				0x80						//��ѯ����
#define	OPERATIONTYPE_SET_REQUEST				0x81						//��������
#define	OPERATIONTYPE_ACTIVE_UPLOAD				0x82						//�����ϴ�
#define	OPERATIONTYPE_QUERY_ACK					0x83						//��ѯӦ��
#define	OPERATIONTYPE_SET_ACK					0x84						//����Ӧ��
#define	OPERATIONTYPE_ACTIVE_UPLOAD_ACK			0x85						//�����ϴ�Ӧ��
#define	OPERATIONTYPE_ERROR_ACK					0x86						//����Ӧ��

/* �����ʶ */
#define	OBJECTID_ONLINE						0x01						//�������
#define	OBJECTID_TIME							0x02						//��ʱ
#define	OBJECTID_BAUD							0x03						//ͨ�Ų�����
#define	OBJECTID_CONFIG_PARAM					0x04						//���ò���
#define	OBJECTID_STATISTICAL					0x05						//ͳ������
#define	OBJECTID_HISTORICAL						0x06						//��ʷ����
#define	OBJECTID_PULSE_UPLOADMODE				0x07						//���������ϴ�ģʽ
#define	OBJECTID_PULSE_DATA						0x08						//��������
#define	OBJECTID_ERROR_MESSAGE					0x09						//������Ϣ

/* �������� */
#define	CHECKCODEERROR							0x01						//У�����
#define	CHECKVERSIONERROR						0x02						//Э��汾������
#define	CHECKMESSAGETYPEERROR					0x03						//��Ϣ���ʹ�
#define	CHECKMESSAGECONTENTERROR					0x04						//��Ϣ��������������Ч

/* ����������Ŀ */
#define	STATISTICALMODE_NONEMODE					0x00						//�����ֳ��ͽ�������ͳ��
#define	STATISTICALMODE_A_C						0x01						//��A��C���೵�ͽ�������ͳ��
#define	STATISTICALMODE_A_B_C					0x02						//��A��B��C���೵�ͽ�������ͳ��
#define	STATISTICALMODE_NONE					0x03						//���ṩ������ͳ������
#define	AVGSHARE_ENABLE						0x00						//�ṩ����ƽ��ռ����
#define	AVGSHARE_DISABLE						0x01						//���ṩ����ƽ��ռ����
#define	AVGSPEED_ENABLE						0x00						//�ṩ����ƽ����ʻ�ٶ�
#define	AVGSPEED_DISABLE						0x01						//���ṩ����ƽ����ʻ�ٶ�
#define	AVGLENGTH_ENABLE						0x00						//�ṩƽ������
#define	AVGLENGTH_DISABLE						0x01						//���ṩƽ������
#define	AVGHEADTIME_ENABLE						0x00						//�ṩƽ����ͷʱ��
#define	AVGHEADTIME_DISABLE						0x01						//���ṩƽ����ͷʱ��
#define	QUEUINGLENGTH_ENABLE					0x00						//�ṩ�����Ŷӳ���
#define	QUEUINGLENGTH_DISABLE					0x01						//���ṩ�����Ŷӳ���

/* ��������ò��� */
typedef __packed struct
{
	u16 statistical_period;												//0 ͳ�����ݵļ�������,��λs	0 ~ 1000
	u8  Acar_langth;													//2 A�೵����				0 ~ 255
	u8  Bcar_length;													//3 B�೵����				0 ~ 150
	u8  Ccar_length;													//4 C�೵����				0 ~ 50
	u32 reserved;														//5 �����ֶ�
}Gat_Param_Detector_Config;												//Gat��������ò��� ��9���ֽ�

/* ��������� */
typedef __packed struct
{
	u8  manufacturers_length;											//����������������ֽ���		0 ~ 100
	u8  manufacturers_name[sizeof(EquipmentManufacturers)];					//���������������
	u8  model_length;													//������ͺŵ��ֽ���		0 ~ 100
	u8  model_name[sizeof(EquipmentModel)];									//��������ͺ�
	u8  detection_channels;												//�����ͨ����			1 ~ 128
	u16 statistical_mode:2;												//ͳ�Ʒ�ʽ
	u16 avgshare:1;													//����ƽ��ռ����			0.�ṩ 1.���ṩ
	u16 avgspeed:1;													//����ƽ����ʻ�ٶ�			0.�ṩ 1.���ṩ
	u16 avglength:1;													//����ƽ������				0.�ṩ 1.���ṩ
	u16 avgheadtime:1;													//����ƽ����ͷʱ��			0.�ṩ 1.���ṩ
	u16 queuing_length:1;												//�����Ŷӳ���				0.�ṩ 1.���ṩ
	u16 reserved:9;													//�����ֶ�
	u8  detection_mode;													//����ֶ�
	u8  signal_output_delay;												//�ź������ʱ��λ10ms		0 ~ 255
	Gat_Param_Detector_Config detector_config;								//��������ò���
}Gat_Param_Detector;													//��������� ���216�ֽ�

/* ��·���ͨ��ͳ������ */
typedef __packed struct
{
	u8  detectionChannelNo;												//���ͨ�����				1 ~ 255
	u8  AcarVolume;													//A�೵����				0 ~ 255
	u8  BcarVolume;													//B�೵����				0 ~ 255
	u8  CcarVolume;													//C�೵����				0 ~ 255
	u8  avgShare;														//ƽ��ռ����				0 ~ 200 ��λ0.5%
	u8  avgSpeed;														//ƽ������				0 ~ 255 ��λkm/h
	u8  avgLength;														//ƽ������				0 ~ 255 ��λ0.1m
	u8  avgHeadTime;													//ƽ����ͷʱ��				0 ~ 255 ��λs
	u8  queuingLength;													//�����Ŷӳ���				0 ~ 255 ��λm
	u32 reserved;														//�����ֶ�
}Gat_DetectionChannel_Data;												//��·���ͨ��ͳ������ ��13�ֽ�

/* ͳ������ */
typedef __packed struct
{
	u32 DateTime;														//ͳ���������ɵı���ʱ��
	Gat_Param_Detector_Config detector_config;								//��������ò���
	u8  detection_channels;												//���ͨ����				1 ~ 48
	Gat_DetectionChannel_Data detector_statistical_data[DetectionChannels];		//��·���ͨ��ͳ������
}Gat_Statistical_Data;													//ͳ������ ���638�ֽ�

/* ��ʷͳ������ */
typedef __packed struct
{
	u8  historyDataNo;													//��ʷ������ˮ��			0 ~ 255
	Gat_Statistical_Data statistical_data;									//����ˮ��ͳ������
}Gat_History_Statistical_Data;											//��ʷͳ������

/* ���������ź�Դ */
typedef __packed struct
{
	u8  pulseUploadChannelNo;											//�����ϴ����ͨ�����
	u8  pulseUploadCarInOut;												//�����ϴ��������� 0:�����뿪 1:��������
}Gat_PulseUpload_Data;													//���������ź�Դ ��2�ֽ�

/* ���������ϴ����ò��� */
typedef __packed struct
{
	u8  pulseUploadChannel;												//�����ϴ����ͨ����		1 ~ 128
	u8  pulseUploadBit[16];												//���������ϴ�ͨ��ʹ��λ
}Gat_PulseUploadConfig_Data;

/* �������ò��� */
typedef __packed struct
{
	u32 SN;															//�豸SN��
	u32 baudRate;														//����������ֵ
	u16 output_ID[DetectionChannels];										//�����ϴ�ͨ���Ŷ�Ӧ�شű��
}Gat_Param_Equipment_Config;


/* GAT920�ⲿ�ӿڿ��ƹ����� */
struct _m_gat920_dev
{
	void		(*PollExecution)(void);										//����GATЭ������¼�
	u8		(*EventInitialized)(void);									//GAT��ʼ���¼�
	u8		(*EventReady)(void);										//GAT׼�����¼�
	u8		(*EventFrameReceived)(void);									//GAT��������ǰ�ڴ����¼�
	u8		(*EventExecute)(void);										//GAT�������ݷ��������¼�
	u8		(*EventFrameSent)(void);										//GAT�������ݺ��ڴ����¼�
	u8		(*EventActiveUpload)(void);									//GAT�����ϴ��¼�
	
	void		(*ParamSaveToFlash)(void);									//����GAT�����������Flash
	void		(*ParamReadToFlash)(void);									//��ȡGAT�����������Ram
	void		(*InitParamDetectorDefault)(u32 sn);							//��ʼ��GAT���������
	void		(*ReadOutputID)(u16 *outputid);								//��ȡoutput_ID����˿ڵĲ�����GAT�����ϴ�ͨ���Ŷ�Ӧ�شű��
	
	u16		(*ReceiveBufToDataFrame)(u8 *receive_buf, u16 receive_length);		//�ӽ��յ�����������ȡ����֡
	u16		(*DataSheetToDataFrame)(u8 *data_sheet, u16 data_length);			//�����ݱ�ת��Ϊ����֡
	u16		(*DataFrameToDataSheet)(u8 *data_frame, u16 data_length);			//������֡ת��Ϊ���ݱ�
	u16		(*DataSheetToMessage)(u8 *data_sheet, u16 data_length);			//�����ݱ�����ȡ��Ϣ����
	
	u8		(*CheckCode)(u8 *data_frame, u16 data_length);					//���У�����Ƿ���ȷ-----------�������� 1
	u8		(*CheckVersion)(u8 *data_sheet);								//���Э��汾�Ƿ����---------�������� 2
	u8		(*CheckMessageType)(u8 *data_sheet);							//�����Ϣ�����Ƿ���ȷ---------�������� 3
	u8		(*CheckMessageContent)(u8 *data_sheet, u16 data_length);			//�����Ϣ���������Ƿ���ȷ-----�������� 4		
	
	u8		(*LinkAddress_Send)(u8 *linkaddress);							//�������·��ַ����
	u16		(*LinkAddress_Receive)(u8 *data_sheet);							//�����������·��ַ��ȡ
	
	u8		(*GatOperationType)(u8 *data_sheet);							//��ȡ��������ֵ
	u8		(*GatObjectId)(u8 *data_sheet);								//��ȡ�����ʶֵ
	
	void		(*UARTx_IRQ)(USART_TypeDef* USARTx);							//GATЭ�鴮���жϴ�����
	void		(*EnterCriticalSection)(void);								//����ؼ�����,�ر��ж�,�ȴ��жϽ�������
	void		(*ExitCriticalSection)(void);									//�˳��ؼ�����,�����ж�
	
	void		(*InitUploadQueue)(void);									//��ʼ��Upload����
	void		(*UploadEnqueue)(u16 id, u8 direction);							//��������д�����
	u8		(*UploadDequeue)(u16 *id, u8 *direction);						//�������ݶ�������
	
	void		(*UploadOvertime)(void);										//�����ϴ�����Ӧ��ʱ����
	
	void		(*ImplementEnqueue)(u16 statisticaltime);						//����ͳ��ʱ�佫ͳ�����ݴ������
};
extern struct _m_gat920_dev gat920_dev;


extern Gat_Param_Detector			gatParamDetector;						//GAT���������
extern Gat_PulseUploadConfig_Data		gatParamPulse;							//GAT���������ϴ����ò���
extern Gat_Param_Equipment_Config		gatParamEquipment;						//GAT�豸���ò���


void GAT_FlashErase(u32 StartAddr);										//����GAT�����洢Flash��
u32  GAT_FlashRead(u32 addr);												//��ȡGAT�����洢Flash����ַ
u32  GAT_FlashWrite(u32 addr, u32 *data, u16 num);							//д��GAT�����洢Flash������
void GAT_ParamSaveToFlash(void);											//����GAT�����������Flash
void GAT_ParamReadToFlash(void);											//��ȡGAT�����������Ram
void GAT_InitParamDetectorDefault(u32 sn);									//��ʼ��GAT���������
void GAT_GetOutputID(u16 *outputid);										//��ȡoutput_ID����˿ڵĲ�����GAT�����ϴ�ͨ���Ŷ�Ӧ�شű�Ų����浽FLASH

u8   GAT_EventInitialized(void);											//GAT��ʼ���¼�
u8   GAT_EventReady(void);												//GAT׼�����¼�
u8   GAT_EventFrameReceived(void);											//GAT��������ǰ�ڴ����¼�
u8   GAT_EventExecute(void);												//GAT�������ݷ��������¼�
u8   GAT_EventFrameSent(void);											//GAT�������ݺ��ڴ����¼�
u8   GAT_EventActiveUpload(void);											//GAT�����ϴ��¼�
void GAT_PollExecution(void);												//����GATЭ������¼�

void GAT_CarInUploadEnqueue(unsigned char dici_index);							//��ȡ����ʻ����Ϣ
void GAT_CarOutUploadEnqueue(unsigned char dici_index);						//��ȡ����ʻ����Ϣ

#endif
