#ifndef __GAT_UPLOAD_H
#define   __GAT_UPLOAD_H

#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "gatconfig.h"


#define		UPLOADQUEUELENGTH		50									//�������ݴ洢���г���
#define		STATISTICALLENGTH		20									//ͳ�����ݴ洢���г���

#define		GATOVERTIME			2									//��ʱʱ��/��
#define		GATREPEAT				3									//�ط�����/��

typedef struct
{
	u16 id;															//�ش�id��
	u8  direction;														//�õشų������� 0 ����� 1 ������
}Gat_UploadData;

typedef struct
{
	Gat_UploadData upload[UPLOADQUEUELENGTH];								//��������
	u8 front;															//��ͷ
	u8 rear;															//��β
}Gat_UploadQueue;														//�������ݴ洢����


typedef struct
{
	u32 datetime;														//ͳ���������ɵı���ʱ��
	u8  detection_channels;												//���ͨ����    1 ~ 48
	Gat_DetectionChannel_Data detector_statistical_data[DetectionChannels];		//��·���ͨ��ͳ������
}Gat_StatisticalData;

typedef struct
{
	Gat_StatisticalData statistical[STATISTICALLENGTH];						//��������
	u8 front;															//��ͷ
	u8 rear;															//��β
}Gat_StatisticalQueue;													//ͳ�����ݴ洢����


extern Gat_UploadQueue			gatUploadQueus;							//�������ݴ洢����
extern Gat_StatisticalQueue		gatStatisticalQueue;						//ͳ�����ݴ洢����
extern Gat_StatisticalData		gatStatisticalData;							//�����е���ͳ������
extern volatile u8 gatOverTime;											//��ʱʱ��
extern volatile u8 gatRepeat;												//��ʱ����


void GAT_InitUploadQueue(void);											//��ʼ��Upload����
u8   GAT_UploadQueueisFull(void);											//�������Ƿ�����
u8   GAT_UploadQueueisEmpty(void);											//�������Ƿ�Ϊ��
void GAT_UploadEnqueue(u16 id, u8 direction);								//��������д�����
u8   GAT_UploadDequeue(u16 *id, u8 *direction);								//�������ݶ�������(����ͷƫ��1)
u8   GAT_UploadDequeueNomove(u16 *id, u8 *direction);							//�������ݶ�������(����ͷ����)
u8   GAT_UploadDequeueMove(void);											//�������ݶ���(����ͷƫ��1)

void GAT_UploadOvertime(void);											//�����ϴ�����Ӧ��ʱ����(2����û�յ�Ӧ���ٴη���,3�η�����Ӧ��Ͽ�����)(������RTC���ж�)

void GAT_InitStatisticalQueue(void);										//��ʼ��Statistical����
u8   GAT_StatisticalQueueisFull(void);										//���Statistical�����Ƿ�����
u8   GAT_StatisticalQueueisEmpty(void);										//���Statistical�����Ƿ�Ϊ��
void GAT_StatisticalEnqueue(Gat_StatisticalData *buf);							//ͳ������д�����
u8   GAT_StatisticalDequeue(Gat_StatisticalData *buf);							//ͳ�����ݶ�������(����ͷƫ��1)
u8   GAT_StatisticalDequeueNomove(Gat_StatisticalData *buf);					//ͳ�����ݶ�������(����ͷ����)
u8   GAT_StatisticalDequeueMove(void);										//ͳ�����ݶ���(����ͷƫ��1)

void GAT_ImplementEnqueue(u16 statisticaltime);								//����ͳ��ʱ�佫ͳ�����ݴ������

#endif
