/**
  *********************************************************************************************************
  * @file    gatconfig.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   
  *********************************************************************************************************
  * @attention
  *			���� : 
  *			1.  ����GAT�����洢Flash��
  *			2.  ��ȡGAT�����洢Flash����ַ
  *			3.  д��GAT�����洢Flash������
  *			4.  ����GAT�����������Flash
  *			5.  ��ȡGAT�����������Ram
  *			6.  ��ʼ��GAT���������
  *			7.  ��ȡoutput_ID����˿ڵĲ�����GAT�����ϴ�ͨ���Ŷ�Ӧ�شű��
  *			8.  GAT��ʼ���¼�
  *			9.  GAT׼�����¼�
  *			10. GAT��������ǰ�ڴ����¼�
  *			11. GAT�������ݷ��������¼�
  *			12. GAT�������ݺ��ڴ����¼�
  *			13. GAT�����ϴ��¼�
  *			14. ����GATЭ������¼�
  *********************************************************************************************************
  */

#include "gatconfig.h"
#include "gatfunc.h"
#include "gatconnect.h"
#include "gatserial.h"
#include "gatupload.h"
#include "string.h"
#include "stm32_config.h"


#define GAT_RO_FLashAddr  	((u32)0x080BE800)								//SN�洢��ַ
#define GAT_RW_StartAddr		((u32)0x080BF800)
#define GAT_RW_FLashAddr_p1	((u32)0x080BF800)								//GAT���������������
#define GAT_RW_FLashAddr_p2	((u32)0x080BF900)								//GAT���������ϴ����ò���������
#define GAT_RW_FLashAddr_p3	((u32)0x080BFA00)								//GAT�豸���ò���������
#define GAT_RW_FLashAddr_p4	((u32)0x080BFB00)
#define GAT_RW_FLashAddr_p5	((u32)0x080BFC00)
#define GAT_RW_FLashAddr_p6	((u32)0x080BFD00)
#define GAT_RW_FLashAddr_p7	((u32)0x080BFE00)
#define GAT_RW_FLashAddr_p8	((u32)0x080BFF00)
#define GAT_EndAddr			((u32)0x080C0000)
#define GAT_PageSize		((u16)0x800)									//RCT6 PageSize = 2048Byte

#define GATPARAMDETECTORLENGTH	(sizeof(gatParamDetector) % 4) != 0 ? (sizeof(gatParamDetector) / 4 + 1) : (sizeof(gatParamDetector) / 4)
#define GATPARAMPULSELENGTH		(sizeof(gatParamPulse) % 4) != 0 ? (sizeof(gatParamPulse) / 4 + 1) : (sizeof(gatParamPulse) / 4)
#define GATPARAMEQUIPMENTLENGTH	(sizeof(gatParamEquipment) % 4) != 0 ? (sizeof(gatParamEquipment) / 4 + 1) : (sizeof(gatParamEquipment) / 4)

Gat_Param_Detector			gatParamDetector;								//GAT���������
Gat_PulseUploadConfig_Data	gatParamPulse;									//GAT���������ϴ����ò���
Gat_Param_Equipment_Config	gatParamEquipment;								//GAT�豸���ò���

GATEventType				GATEvent = GAT_EVENT_INITIALIZED;					//GAT״̬��״ֵ̬
GATConnectType				GATConnect = GAT_OFFLINE;						//GAT����״̬
GATUPLOADAckType			GATUPLOADAck = GAT_ReceivedAck;					//GAT�ϴ����ݽ���Ӧ��״̬
GATActiveUpLoadModeType		GATActiveUpLoadMode = GAT_ActiveUpLoadMode_NONE;		//GAT�����ϴ�ģʽ

u8 GAT_USART_RXIT = 0;
u8 GAT_USART_TXIT = 0;

/**********************************************************************************************************
 @Function			void GAT_FlashErase(u32 StartAddr)
 @Description			����GAT�����洢Flash��
 @Input				StartAddr : ��ʼ��ַ
 @Return				void
**********************************************************************************************************/
void GAT_FlashErase(u32 StartAddr)
{
	volatile FLASH_Status FLASHStatus;
	volatile u32 NbrOfPage = 0;
	u32 EraseCounter = 0x0;
	
	FLASHStatus = FLASH_COMPLETE;
	NbrOfPage = (GAT_EndAddr - StartAddr) >> 11;
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++) {
		FLASHStatus = FLASH_ErasePage(StartAddr + (GAT_PageSize * EraseCounter));
	}
}

/**********************************************************************************************************
 @Function			u32 GAT_FlashRead(u32 addr)
 @Description			��ȡGAT�����洢Flash����ַ
 @Input				addr : ��ȡ��ַ
 @Return				�õ�ֵַ
**********************************************************************************************************/
u32 GAT_FlashRead(u32 addr)
{
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	return (*(volatile u32*) addr);
}

/**********************************************************************************************************
 @Function			u32 GAT_FlashWrite(u32 addr, u32 *data, u16 num)
 @Description			д��GAT�����洢Flash������
 @Input				addr  : д���ַ
					*data : ����
					num   : ����/4
 @Return				FLASHStatus
**********************************************************************************************************/
u32 GAT_FlashWrite(u32 addr, u32 *data, u16 num)
{
	u16 i = 0;
	volatile FLASH_Status FLASHStatus;
	
	while ((FLASHStatus == FLASH_COMPLETE) && (i < num)) {
		FLASHStatus = FLASH_ProgramWord((addr + i * 4), data[i]);
		i++;
	}
	
	return FLASHStatus;
}

/**********************************************************************************************************
 @Function			void GAT_ParamSaveToFlash(void)
 @Description			����GAT�����������Flash
 @Input				void
 @Return				void
**********************************************************************************************************/
void GAT_ParamSaveToFlash(void)
{
	FLASH_Unlock();
	GAT_FlashErase(GAT_RW_StartAddr);												//������Ӧ�洢FlashPage
	GAT_FlashWrite(GAT_RW_FLashAddr_p1, (u32*) &gatParamDetector, GATPARAMDETECTORLENGTH);	//д��GAT���������
	GAT_FlashWrite(GAT_RW_FLashAddr_p2, (u32*) &gatParamPulse, GATPARAMPULSELENGTH);		//д��GAT���������ϴ����ò���
	GAT_FlashWrite(GAT_RW_FLashAddr_p3, (u32*) &gatParamEquipment, GATPARAMEQUIPMENTLENGTH);	//д��GAT�豸���ò���
	FLASH_Lock();
}

/**********************************************************************************************************
 @Function			void GAT_ParamReadToFlash(void)
 @Description			��ȡGAT�����������Ram
 @Input				void
 @Return				void
**********************************************************************************************************/
void GAT_ParamReadToFlash(void)
{
	u32 *params;
	u16 i = 0, length = 0;
	
	length = GATPARAMDETECTORLENGTH;												//��ȡGAT���������
	params = (u32 *)&gatParamDetector;
	for (i = 0; i < length; i++) {
		delay_5us(240);
		params[i] = GAT_FlashRead(GAT_RW_FLashAddr_p1 + i * 4);
	}
	
	length = GATPARAMPULSELENGTH;													//��ȡGAT���������ϴ����ò���
	params = (u32 *)&gatParamPulse;
	for (i = 0; i < length; i++) {
		delay_5us(240);
		params[i] = GAT_FlashRead(GAT_RW_FLashAddr_p2 + i * 4);
	}
	
	length = GATPARAMEQUIPMENTLENGTH;												//��ȡGAT�豸���ò���
	params = (u32 *)&gatParamEquipment;
	for (i = 0; i < length; i++) {
		delay_5us(240);
		params[i] = GAT_FlashRead(GAT_RW_FLashAddr_p3 + i * 4);
	}
}

/**********************************************************************************************************
 @Function			void GAT_InitParamDetectorDefault(u32 sn)
 @Description			��ʼ��GAT���������
 @Input				sn
 @Return				void
**********************************************************************************************************/
void GAT_InitParamDetectorDefault(u32 sn)
{
	gatParamDetector.manufacturers_length 				= sizeof(EquipmentManufacturers);	//����������������ֽ���
	strcpy((char *)gatParamDetector.manufacturers_name, EquipmentManufacturers);			//���������������
	gatParamDetector.model_length						= sizeof(EquipmentModel);		//������ͺŵ��ֽ���
	strcpy((char *)gatParamDetector.model_name, EquipmentModel);						//��������ͺ�
	gatParamDetector.detection_channels				= DetectionChannels;			//�����ͨ����
	gatParamDetector.statistical_mode					= STATISTICALMODE_NONEMODE;		//ͳ�Ʒ�ʽ
	gatParamDetector.avgshare						= AVGSHARE_ENABLE;				//����ƽ��ռ����
	gatParamDetector.avgspeed						= AVGSPEED_DISABLE;				//����ƽ����ʻ�ٶ�
	gatParamDetector.avglength						= AVGLENGTH_DISABLE;			//����ƽ������
	gatParamDetector.avgheadtime						= AVGHEADTIME_ENABLE;			//����ƽ����ͷʱ��
	gatParamDetector.queuing_length					= QUEUINGLENGTH_DISABLE;			//�����Ŷӳ���
	gatParamDetector.reserved						= 0;							//�����ֶ�
	gatParamDetector.detection_mode					= DetectionMode;				//����ֶ�
	gatParamDetector.signal_output_delay				= SignalOutputDelay;			//�ź������ʱ
	gatParamDetector.detector_config.statistical_period	= 60;						//ͳ�����ݵļ�������
	gatParamDetector.detector_config.Acar_langth			= 200;						//A�೵����
	gatParamDetector.detector_config.Bcar_length			= 100;						//B�೵����
	gatParamDetector.detector_config.Ccar_length			= 50;						//C�೵����
	
	gatParamPulse.pulseUploadChannel					= 1;							//�����ϴ����ͨ����
	memset(gatParamPulse.pulseUploadBit, 0x0, sizeof(gatParamPulse.pulseUploadBit));		//���������ϴ�ͨ��ʹ��λ
	
	gatParamEquipment.SN							= sn;						//SN
	gatParamEquipment.baudRate						= GAT920_BAUDRATE;				//Ĭ��ͨ�Ų�����
	memset((u8 *)gatParamEquipment.output_ID, 0x0, sizeof(gatParamEquipment.output_ID));		//�������ϴ�ͨ���Ŷ�Ӧ�شű��
}

/**********************************************************************************************************
 @Function			void GAT_GetOutputID(u16 *outputid)
 @Description			��ȡoutput_ID����˿ڵĲ�����GAT�����ϴ�ͨ���Ŷ�Ӧ�شű�Ų����浽FLASH
 @Input				Output_ID��ŵ�ַ
 @Return				void
**********************************************************************************************************/
void GAT_GetOutputID(u16 *outputid)
{
	u8 i = 0;
	
	for (i = 0; i < DetectionChannels; i++) {
		gatParamEquipment.output_ID[i] = outputid[i];
	}
	
	GAT_ParamSaveToFlash();
}

/**********************************************************************************************************
 @Function			u8 GAT_EventInitialized(void)
 @Description			GAT��ʼ���¼�
 @Input				void
 @Return				GAT_TRUE  : ��ȷ
					GAT_FALSE : ����
**********************************************************************************************************/
u8 GAT_EventInitialized(void)
{
	u8 Initialized = GAT_TRUE;
	u32 romSN = 0;
	USART_TypeDef* GAT920_USART = USART1;
	
	if (PlatformGat920Usart == Gat920_USART1) {											//����SNѡ��Gat920���Ӵ���1
		GAT920_USART = USART1;
	}
	else {																		//����SNѡ��Gat920���Ӵ���2
		GAT920_USART = USART2;
	}
	
	romSN = GAT_FlashRead(GAT_RO_FLashAddr);											//��ȡ�豸SN
	GAT_ParamReadToFlash();															//��ȡFlash��GAT������Ϣ
	if (romSN == 0xFFFFFFFF) {														//SNΪ��
		GAT_InitParamDetectorDefault(0x12345678);
		GAT_ParamSaveToFlash();
	}
	else if (gatParamEquipment.SN != romSN) {											//SN��ΪGAT������Ϣ��sn
		GAT_InitParamDetectorDefault(romSN);
		GAT_ParamSaveToFlash();
	}

	GAT_EnterCriticalSection();
	
	Initialized = GAT_PortSerialInit(GAT920_USART, GAT920_BAUDRATE, 8, GAT_PAR_NONE);			//����Ӳ����ʼ��Ĭ�ϲ�����
	
	memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));								//��ս������ݻ�����
	memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));									//��շ������ݻ�����
	GATReceiveLength = 0;															//�������ݳ�����0
	GATSendLength = 0;																//�������ݳ�����0
	
	GAT_ExitCriticalSection();
	
	if (Initialized != GAT_FALSE) {
		GATEvent = GAT_EVENT_READY;													//����READY�¼�
		GAT_USART_RXIT = 1;
		GAT_USART_TXIT = 0;
		GAT_PortSerialEnable(1, 0);													//���������ж�
	}
	else {
		GATEvent = GAT_EVENT_INITIALIZED;												//����INITIALIZED�¼�
		GAT_USART_RXIT = 0;
		GAT_USART_TXIT = 0;
		GAT_PortSerialEnable(0, 0);													//�رմ����ж�
	}
	
	return Initialized;
}

/**********************************************************************************************************
 @Function			u8 GAT_EventReady(void)
 @Description			GAT׼�����¼�
 @Input				void
 @Return				GAT_TRUE  : ��ȷ
					GAT_FALSE : ����
**********************************************************************************************************/
u8 GAT_EventReady(void)
{
	u8 error = GAT_TRUE;
	
	if ((GAT_USART_RXIT != 1) && (GAT_USART_TXIT != 1)) {
		GAT_USART_RXIT = 1;
		GAT_USART_TXIT = 0;
		GAT_PortSerialEnable(1, 0);													//���������ж�
	}
	
	/* ���߲�ѯ/�����¼� */
	if (GATReceiveLength & 0X8000) {													//���յ�һ֡��������
		GAT_USART_RXIT = 0;
		GAT_USART_TXIT = 0;
		GAT_PortSerialEnable(0, 0);													//�رմ����ж�
		GATEvent = GAT_EVENT_FRAME_RECEIVED;											//����FRAME_RECEIVED�¼�
		goto exit;
	}
	
	if (GATConnect != GAT_OFFLINE) {
		/* ֧���������������ϴ� */
		if ((GAT_UploadQueueisEmpty() == 0) && (GATUPLOADAck != GAT_NotReceivedAck)) {				//���������ϴ������д���������������ȴ������ϴ�Ӧ��
			//delay_1ms(GAT920_UPLOAD_SEND_DELAY_MS);
			if (systime_time_meter1 != 1) {
				systime_time_meter1 = 1;
				systime_runtime_ms1 = 0;
			}
			if (systime_runtime_ms1 >= GAT920_UPLOAD_SEND_DELAY_MS) {
				systime_time_meter1 = 0;
				systime_runtime_ms1 = 0;
				GAT_USART_RXIT = 0;
				GAT_USART_TXIT = 0;
				GAT_PortSerialEnable(0, 0);													//�رմ����ж�
				GATActiveUpLoadMode = GAT_ActiveUpLoadMode_PULSE;									//�������������ϴ�
				GATEvent = GAT_EVENT_ACTIVEUPLOAD;												//����ACTIVEUPLOAD�¼�
			}
		}
		/* ֧��ͳ�����������ϴ� */
		else if ((GAT_StatisticalQueueisEmpty() == 0) && (GATUPLOADAck != GAT_NotReceivedAck)) {		//ͳ�������ϴ������д���������������ȴ������ϴ�Ӧ��
			//delay_1ms(GAT920_UPLOAD_SEND_DELAY_MS);
			if (systime_time_meter2 != 1) {
				systime_time_meter2 = 1;
				systime_runtime_ms2 = 0;
			}
			if (systime_runtime_ms2 >= GAT920_UPLOAD_SEND_DELAY_MS) {
				systime_time_meter2 = 0;
				systime_runtime_ms2 = 0;
				GAT_USART_RXIT = 0;
				GAT_USART_TXIT = 0;
				GAT_PortSerialEnable(0, 0);													//�رմ����ж�
				GATActiveUpLoadMode = GAT_ActiveUpLoadMode_STATISTICS;								//ͳ�����������ϴ�
				GATEvent = GAT_EVENT_ACTIVEUPLOAD;												//����ACTIVEUPLOAD�¼�
			}
		}
	}
	/* �����¼� */
	else {
		GATEvent = GAT_EVENT_READY;													//����READY�¼�
	}
	
exit:
	return error;
}

/**********************************************************************************************************
 @Function			u8 GAT_EventFrameReceived(void)
 @Description			GAT��������ǰ�ڴ����¼�
 @Input				void
 @Return				GAT_TRUE  : ��ȷ
					GAT_FALSE : ����
**********************************************************************************************************/
u8 GAT_EventFrameReceived(void)
{
	u8 error = GAT_TRUE;
	u16 frameLength = 0;															//�������ݳ���
	u16 changeLength = 0;															//ת�������ݳ���
	
	frameLength = GATReceiveLength & (~(1<<15));											//��ȡ�������ݳ���
	if (frameLength == 0) {															//û�н��ܵ����ݶ�����������ݴ����¼�
		memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));							//��ս������ݻ�����
		GATReceiveLength = 0;														//�������ݳ�����0
		GATEvent = GAT_EVENT_READY;													//����READY�¼�
		GAT_USART_RXIT = 1;
		GAT_USART_TXIT = 0;
		GAT_PortSerialEnable(1, 0);													//���������ж�
		return GAT_FALSE;
	}
	
	changeLength = GAT_ReceiveBufToDataFrame((u8 *)GATReceiveBuf, frameLength);				//��ȡGAT����֡
	if (changeLength == 0) {															//����ЧGAT����֡
		memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));							//��ս������ݻ�����
		GATReceiveLength = 0;														//�������ݳ�����0
		GATEvent = GAT_EVENT_READY;													//����READY�¼�
		GAT_USART_RXIT = 1;
		GAT_USART_TXIT = 0;
		GAT_PortSerialEnable(1, 0);													//���������ж�
		return GAT_FALSE;
	}
	else {																		//��ЧGAT����֡
		GATReceiveLength = changeLength;												//֡������
		GATEvent = GAT_EVENT_EXECUTE;													//����EXECUTE�¼�
	}
	
	return error;
}

/**********************************************************************************************************
 @Function			u8 GAT_EventExecute(void)
 @Description			GAT�������ݷ��������¼�
 @Input				void
 @Return				GAT_TRUE  : ��ȷ
					GAT_FALSE : ����
**********************************************************************************************************/
u8 GAT_EventExecute(void)
{
	u8 error = GAT_TRUE;
	u16 sheetlength = 0;															//���ݱ���
	
	error = GAT_CheckCode((u8 *)GATReceiveBuf, GATReceiveLength);							//У������
	if (error != GAT_TRUE) {															//У�������
		GAT_ErrorDataWrite(CHECKCODEERROR);											//����Ӧ��д�뻺����
		GATEvent = GAT_EVENT_FRAME_SENT;												//����SENT�¼�
		return GAT_TRUE;
	}
	
	sheetlength = GAT_DataFrameToDataSheet((u8 *)GATReceiveBuf, GATReceiveLength);				//������֡ת��Ϊ���ݱ�
	if (sheetlength != 0) {															//ת���ɹ�
		GATReceiveLength = sheetlength;												//�������ݱ���
		
		error = GAT_CheckLinkAddress((u8 *)GATReceiveBuf, LINKADDRESS);						//��·��ַ���
		if (error != GAT_TRUE) {														//��·��ַ��Ϊ�����豸��·��ַ
			error = GAT_FALSE;
			memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));						//��ս������ݻ�����
			memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));							//��շ������ݻ�����
			GATReceiveLength = 0;													//�������ݳ�����0
			GATSendLength = 0;														//�������ݳ�����0
			GATEvent = GAT_EVENT_READY;												//����READY�¼�
			GAT_USART_RXIT = 1;
			GAT_USART_TXIT = 0;
			GAT_PortSerialEnable(1, 0);												//���������ж�
			return GAT_FALSE;
		}
		
		error = GAT_CheckVersion((u8 *)GATReceiveBuf);									//Э��汾���
		if (error != GAT_TRUE) {														//Э��汾������
			GAT_ErrorDataWrite(CHECKVERSIONERROR);										//����Ӧ��д�뻺����
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
			return GAT_TRUE;
		}
		
		error = GAT_CheckMessageType((u8 *)GATReceiveBuf);								//��Ϣ���ͼ��
		if (error != GAT_TRUE) {														//��Ϣ���ʹ���
			GAT_ErrorDataWrite(CHECKMESSAGETYPEERROR);									//����Ӧ��д�뻺����
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
			return GAT_TRUE;
		}
		
		error = GAT_CheckMessageContent((u8 *)GATReceiveBuf, GATReceiveLength);				//��Ϣ���ݼ��
		if (error != GAT_TRUE) {														//��Ϣ���ݴ���
			GAT_ErrorDataWrite(CHECKMESSAGECONTENTERROR);								//����Ӧ��д�뻺����
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
			return GAT_TRUE;
		}
		
		/* ����֡ת���ɹ���У������ */
		error = GAT_SelectExecuteCmd((u8 *)GATReceiveBuf, GATReceiveLength);					//�жϴ���ͬ���ݱ�
		if (error == GAT_TRUE) {														//ִ����ȷ
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
			return GAT_TRUE;
		}
		else if (error == GAT_DATAFALSE) {												//��Ϣ���ݴ���
			memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));						//��ս������ݻ�����
			memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));							//��շ������ݻ�����
			GATReceiveLength = 0;													//�������ݳ�����0
			GATSendLength = 0;														//�������ݳ�����0
			GAT_ErrorDataWrite(CHECKMESSAGECONTENTERROR);								//����Ӧ��д�뻺����
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
			return GAT_TRUE;
		}
		else if (error == GAT_RECEIVEEVENT) {											//���յ������ϴ�Ӧ���
			memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));						//��ս������ݻ�����
			memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));							//��շ������ݻ�����
			GATReceiveLength = 0;													//�������ݳ�����0
			GATSendLength = 0;														//�������ݳ�����0
			GATEvent = GAT_EVENT_READY;												//����READY�¼�
			GAT_USART_RXIT = 1;
			GAT_USART_TXIT = 0;
			GAT_PortSerialEnable(1, 0);												//���������ж�
			return GAT_TRUE;
		}
		else {																	//ִ�д���
			error = GAT_FALSE;
			memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));						//��ս������ݻ�����
			memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));							//��շ������ݻ�����
			GATReceiveLength = 0;													//�������ݳ�����0
			GATSendLength = 0;														//�������ݳ�����0
			GATEvent = GAT_EVENT_READY;												//����READY�¼�
			GAT_USART_RXIT = 1;
			GAT_USART_TXIT = 0;
			GAT_PortSerialEnable(1, 0);												//���������ж�
			return GAT_FALSE;
		}
	}
	else {																		//ת��ʧ��
		error = GAT_FALSE;
		memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));							//��ս������ݻ�����
		GATReceiveLength = 0;														//�������ݳ�����0
		GATEvent = GAT_EVENT_READY;													//����READY�¼�
		GAT_USART_RXIT = 1;
		GAT_USART_TXIT = 0;
		GAT_PortSerialEnable(1, 0);													//���������ж�
		return GAT_FALSE;
	}
}

/**********************************************************************************************************
 @Function			u8 GAT_EventFrameSent(void)
 @Description			GAT�������ݺ��ڴ����¼�
 @Input				void
 @Return				GAT_TRUE  : ��ȷ
					GAT_FALSE : ����
**********************************************************************************************************/
u8 GAT_EventFrameSent(void)
{
	u8 error = GAT_TRUE;
	u16 framelength = 0;															//����֡����
	static u8 sending = 0;															//��������״̬ 1.������ 0.�޷��� 
	
	if (sending == 0) {																//�����ݷ���
		if (GATSendLength != 0) {													//��������Ҫ����
			framelength = GAT_DataSheetToDataFrame((u8 *)GATSendBuf, GATSendLength);			//�����ݱ�ת��Ϊ����֡
			GATSendLength = framelength;												//��������֡����
			sending = 1;															//��Ƿ���������
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
			GAT_USART_RXIT = 0;
			GAT_USART_TXIT = 1;
			GAT_PortSerialEnable(0, 1);												//���������ж�
			return GAT_TRUE;
		}
		else {																	//��������Ҫ����
			error = GAT_FALSE;
			memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));						//��ս������ݻ�����
			memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));							//��շ������ݻ�����
			GATReceiveLength = 0;													//�������ݳ�����0
			GATSendLength = 0;														//�������ݳ�����0
			GATEvent = GAT_EVENT_READY;												//����READY�¼�
			GAT_USART_RXIT = 1;
			GAT_USART_TXIT = 0;
			GAT_PortSerialEnable(1, 0);												//���������ж�
			return GAT_FALSE;
		}
	}
	else {																		//����������
		if (GATSendLength & 0X8000) {													//������һ֡����
			GAT_USART_RXIT = 0;
			GAT_USART_TXIT = 0;
			GAT_PortSerialEnable(0, 0);												//�رմ����ж�
			sending = 0;															//��������ݷ�����
			memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));						//��ս������ݻ�����
			memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));							//��շ������ݻ�����
			GATReceiveLength = 0;													//�������ݳ�����0
			GATSendLength = 0;														//�������ݳ�����0
			GATEvent = GAT_EVENT_READY;												//����READY�¼�
			GAT_USART_RXIT = 1;
			GAT_USART_TXIT = 0;
			GAT_PortSerialEnable(1, 0);												//���������ж�
			return GAT_TRUE;
		}
		else {																	//���ڷ�����
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
		}
	}
	
	return error;
}

/**********************************************************************************************************
 @Function			u8 GAT_EventActiveUpload(void)
 @Description			GAT�����ϴ��¼�
 @Input				void
 @Return				GAT_TRUE  : ��ȷ
					GAT_FALSE : ����
**********************************************************************************************************/
u8 GAT_EventActiveUpload(void)
{
	u8 error = GAT_TRUE;
	
	/* ͳ�����������ϴ� */
	if (GATActiveUpLoadMode == GAT_ActiveUpLoadMode_STATISTICS) {
		error = GAT_ActiveStatisticalDataWrite();										//��ͳ������д���������������ϴ������ͻ�����
		if (error == GAT_TRUE) {														//ִ����ȷ
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
			GATUPLOADAck = GAT_NotReceivedAck;											//�ȴ�����Ӧ���
			GATActiveUpLoadMode = GAT_ActiveUpLoadMode_NONE;
			return GAT_TRUE;
		}
		else {																	//ִ�д���
			error = GAT_FALSE;
			GATActiveUpLoadMode = GAT_ActiveUpLoadMode_NONE;
			GAT_StatisticalDequeueMove();
			memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));						//��ս������ݻ�����
			memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));							//��շ������ݻ�����
			GATReceiveLength = 0;													//�������ݳ�����0
			GATSendLength = 0;														//�������ݳ�����0
			GATEvent = GAT_EVENT_READY;												//����READY�¼�
			GAT_USART_RXIT = 1;
			GAT_USART_TXIT = 0;
			GAT_PortSerialEnable(1, 0);												//���������ж�
			return GAT_FALSE;
		}
	}
	/* �������������ϴ� */
	else if (GATActiveUpLoadMode == GAT_ActiveUpLoadMode_PULSE) {
		error = GAT_ActiveUpLoadDataWrite();											//����������д���������������ϴ������ͻ�����
		if (error == GAT_TRUE) {														//ִ����ȷ
			GATEvent = GAT_EVENT_FRAME_SENT;											//����SENT�¼�
			GATUPLOADAck = GAT_NotReceivedAck;											//�ȴ�����Ӧ���
			GATActiveUpLoadMode = GAT_ActiveUpLoadMode_NONE;
			return GAT_TRUE;
		}
		else {																	//ִ�д���
			error = GAT_FALSE;
			GATActiveUpLoadMode = GAT_ActiveUpLoadMode_NONE;
			GAT_UploadDequeueMove();
			memset((u8 *)&GATReceiveBuf, 0x0, sizeof(GATReceiveBuf));						//��ս������ݻ�����
			memset((u8 *)&GATSendBuf, 0x0, sizeof(GATSendBuf));							//��շ������ݻ�����
			GATReceiveLength = 0;													//�������ݳ�����0
			GATSendLength = 0;														//�������ݳ�����0
			GATEvent = GAT_EVENT_READY;												//����READY�¼�
			GAT_USART_RXIT = 1;
			GAT_USART_TXIT = 0;
			GAT_PortSerialEnable(1, 0);												//���������ж�
			return GAT_FALSE;
		}
	}
	/* ��������������ϴ� */
	else if (GATActiveUpLoadMode == GAT_ActiveUpLoadMode_FAULT) {
		
	}
	else {
		return GAT_FALSE;
	}
	
	return error;
}

/**********************************************************************************************************
 @Function			void GAT_PollExecution(void)
 @Description			����GATЭ������¼�
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
							    <-----  |             <--error--        |
ִ�д���:			SENT ERR MESSAGE   <-----  |  <--error--  |                |
									  |              |                |
����״̬:				INITIALIZED  ---->  READY  ---->  RECEIVED  ---->  EXECUTE  ---->  SENT
									  |                                              |
ִ�гɹ�:								  |  <----------------successd-----------------  |
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ִ�д���:			                           |    <--error--   |
									  |                 |
֧��״̬:				INITIALIZED  ---->  READY  ---->  ACTIVEUPLOAD  ---->  SENT
									  |                                  |
ִ�гɹ�:								  |  <----------successd-----------  |
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 @Input				void
 @Return				void
**********************************************************************************************************/
void GAT_PollExecution(void)
{
	switch (GATEvent)															//GATЭ�鴦��״̬ת����
	{
	case GAT_EVENT_READY:														//READY
		if (GAT_EventReady() != GAT_TRUE) {
			GATEvent = GAT_EVENT_READY;
		}
		break;
	
	case GAT_EVENT_FRAME_RECEIVED:												//RECEIVED
		if (GAT_EventFrameReceived() != GAT_TRUE) {
			GATEvent = GAT_EVENT_READY;
		}
		break;
	
	case GAT_EVENT_FRAME_SENT:													//SENT
		if (GAT_EventFrameSent() != GAT_TRUE) {
			GATEvent = GAT_EVENT_READY;
		}
		break;
	
	case GAT_EVENT_EXECUTE:														//EXECUTE
		if (GAT_EventExecute() != GAT_TRUE) {
			GATEvent = GAT_EVENT_READY;
		}
		break;
	
	case GAT_EVENT_INITIALIZED:													//INITIALIZED
		if (GAT_EventInitialized() != GAT_TRUE) {
			GATEvent = GAT_EVENT_INITIALIZED;
		}
		break;
		
	case GAT_EVENT_ACTIVEUPLOAD:													//ACTIVEUPLOAD
		if (GAT_EventActiveUpload() != GAT_TRUE) {
			GATEvent = GAT_EVENT_READY;
		}
		break;
	}
}

/* gat920�ⲿ�ӿ����������� */
struct _m_gat920_dev gat920_dev = 
{
	GAT_PollExecution,															//����GATЭ������¼�
	GAT_EventInitialized,														//GAT��ʼ���¼�
	GAT_EventReady,															//GAT׼�����¼�
	GAT_EventFrameReceived,														//GAT��������ǰ�ڴ����¼�
	GAT_EventExecute,															//GAT�������ݷ��������¼�
	GAT_EventFrameSent,															//GAT�������ݺ��ڴ����¼�
	GAT_EventActiveUpload,														//GAT�����ϴ��¼�
	
	GAT_ParamSaveToFlash,														//����GAT�����������Flash
	GAT_ParamReadToFlash,														//��ȡGAT�����������Ram
	GAT_InitParamDetectorDefault,													//��ʼ��GAT���������
	GAT_GetOutputID,															//��ȡoutput_ID����˿ڵĲ�����GAT�����ϴ�ͨ���Ŷ�Ӧ�شű��
	
	GAT_ReceiveBufToDataFrame,													//�ӽ��յ�����������ȡ����֡
	GAT_DataSheetToDataFrame,													//�����ݱ�ת��Ϊ����֡
	GAT_DataFrameToDataSheet,													//������֡ת��Ϊ���ݱ�
	GAT_DataSheetToMessage,														//�����ݱ�����ȡ��Ϣ����
	
	GAT_CheckCode,																//���У�����Ƿ���ȷ-----------�������� 1
	GAT_CheckVersion,															//���Э��汾�Ƿ����---------�������� 2
	GAT_CheckMessageType,														//�����Ϣ�����Ƿ���ȷ---------�������� 3
	GAT_CheckMessageContent,														//�����Ϣ���������Ƿ���ȷ-----�������� 4
	
	GAT_LinkAddress_Send,														//�������·��ַ����
	GAT_LinkAddress_Receive,														//�����������·��ַ��ȡ
	
	GAT_GatOperationType,														//��ȡ��������ֵ
	GAT_GatObjectId,															//��ȡ�����ʶֵ
	
	GAT_UARTx_IRQ,																//GATЭ�鴮���жϴ�����
	GAT_EnterCriticalSection,													//����ؼ�����,�ر��ж�,�ȴ��жϽ�������
	GAT_ExitCriticalSection,														//�˳��ؼ�����,�����ж�
	
	GAT_InitUploadQueue,														//��ʼ��Upload����
	GAT_UploadEnqueue,															//��������д�����
	GAT_UploadDequeue,															//�������ݶ�������
	
	GAT_UploadOvertime,															//�����ϴ�����Ӧ��ʱ����
	
	GAT_ImplementEnqueue,														//����ͳ��ʱ�佫ͳ�����ݴ������
};

extern unsigned short output_ID[OUTPUT_MAX];

/**********************************************************************************************************
 @Function			void GAT_CarInUploadEnqueue(unsigned char dici_index)
 @Description			GAT_CarInUploadEnqueue	: ��ȡ����ʻ����Ϣ
 @Input				dici_index			: 0 ~ 31������
 @Return				void
**********************************************************************************************************/
void GAT_CarInUploadEnqueue(unsigned char dici_index)
{
#ifdef GAT920_ENABLE
	u8 xx = 0;
	u8 yy = 0;
	
	if (PlatformGat920 == Gat920_ENABLE) {
		if (GATConnect == GAT_ONLINE) {
			xx = dici_index / 8;
			yy = dici_index % 8;
			if ((gatParamPulse.pulseUploadBit[xx] & (0x01 << yy)) != 0) {
				gat920_dev.UploadEnqueue(output_ID[dici_index], GAT_INDETECTIONAREA);
			}
		}
	}
#endif
}

/**********************************************************************************************************
 @Function			void GAT_CarOutUploadEnqueue(unsigned char dici_index)
 @Description			GAT_CarOutUploadEnqueue	: ��ȡ����ʻ����Ϣ
 @Input				dici_index			: 0 ~ 31������
 @Return				void
**********************************************************************************************************/
void GAT_CarOutUploadEnqueue(unsigned char dici_index)
{
#ifdef GAT920_ENABLE
	u8 xx = 0;
	u8 yy = 0;
	
	if (PlatformGat920 == Gat920_ENABLE) {
		if (GATConnect == GAT_ONLINE) {
			xx = dici_index / 8;
			yy = dici_index % 8;
			if ((gatParamPulse.pulseUploadBit[xx] & (0x01 << yy)) != 0) {
				gat920_dev.UploadEnqueue(output_ID[dici_index], GAT_OUTDETECTIONAREA);
			}
		}
	}
#endif
}

/********************************************** END OF FLEE **********************************************/
