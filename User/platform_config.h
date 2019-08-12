/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : platform_config.h
* Author             : MoveBroad Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : Evaluation board specific configuration file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define   __PLATFORM_CONFIG_H

//���ò���
#define REV 0x01

//IO
#define OUTPUT_MAX		32

#define CHECK_PKG_MAX	50

/* GPIO LEG �Ƿ�������һ�� */
#define LED_GPIO		1

//�Զ�дSN, CROSSID, jFlash��д����汾��Ҫȥ��
//#define WRITE_SN_CROSSID
#define MAC_SN			1908128001
#define CROSSID		19081201
#define INTERVALTIME	60												//ͳ��ʱ��,Ĭ��60(��λ��),0�ϴ�ʵʱ����
#define SOCKETLANNUMMAX	16												//Socket Lan Num Max 16 or 32

#define SOCKET_TYPE_NORMAL			0
#define SOCKET_TYPE_XUNFEI			1
#define SOCKET_FUNC_TYPE				SOCKET_TYPE_NORMAL						//Select socket function normal or xunfei

#define SDIO_SDCARD_NONE				0
#define SDIO_SDCARD_USER				1
#define SDIO_SDCARD_TYPE				SDIO_SDCARD_NONE						//Sdio SD Card

/*
MAC_SN : ��������λ
0 :  Modbus --> USART2, Gat920 --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, GPRS/RJ45 --> DISABLE
1 :  Modbus --> USART1, Gat920 --> USART2, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, GPRS/RJ45 --> DISABLE
2 :  Modbus --> USART2, Gat920 --> USART1, Socket --> ENABLE,  SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, RJ45 --> ENABLE or SocketXunfei --> ENABLE
3 :  Modbus --> USART2, Gat920 --> USART1, Socket --> ENABLE,  SocketTime --> ENABLE,  Gat920 --> DISABLE, LESTC --> DISABLE, RJ45 --> ENABLE
4 :  Modbus --> USART2, Gat920 --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> ENABLE,  LESTC --> DISABLE, GPRS/RJ45 --> DISABLE
5 :  Modbus --> USART1, Gat920 --> USART2, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> ENABLE,  LESTC --> DISABLE, GPRS/RJ45 --> DISABLE
6 :	Modbus --> USART2, Gat920 --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, RJ45 --> ENABLE, SocketModulation --> ENABLE
7 :  Modbus --> USART2, Gat920 --> USART1, Socket --> ENABLE,  SocketTime --> ENABLE,  Gat920 --> DISABLE, LESTC --> DISABLE, GPRS --> ENABLE
8 :  Modbus --> USART2, LESTC  --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> ENABLE,  GPRS --> DISABLE
9 :  Modbus --> USART2, Gat920 --> USART1, Socket --> DISABLE, SocketTime --> DISABLE, Gat920 --> DISABLE, LESTC --> DISABLE, RJ45 --> ENABLE, SocketExtend --> ENABLE, SocketExtendTime --> ENABLE
*/

/* USRʹ��, ���ע�͸ú궨��������USRģ�� */
#define	USR_ENABLE
#define	LOCALPORTMODE		1											//���ض˿� 0:��� 1:�̶�
#define	LOCALPORT			"4008"										//���ض˿�ֵ0~65535
#define	DHCP				0											//0:��̬IP 1:DHCP

#define	MVB_SOFTWARE_VERSION		0x0113
#define	MVB_HARDWARE_VERSION		0x0402

#define	SOCKET_CARLENGTH_V1			0
#define	SOCKET_CARLENGTH_V2			1
#define	SOCKET_CARLENGTH_TYPE		SOCKET_CARLENGTH_V2

/* ModBusʹ��, ��ע�͸ú궨����ʹ��ModBusЭ�� */
#define	MODBUS_ENABLE
/* ModBus��ʹ�ô���, ����ʹ���ĸ���ͨ��SNѡ�� */
#define	MB_SERIALPORT_USART1
#define	MB_SERIALPORT_USART2
//#define	MB_SERIALPORT_USART3

/* Socketʹ��, ��ע�͸ú궨����ʹ��SocketЭ��  */
#define	SOCKET_ENABLE
/* Socketʹ�ô���ѡ�� */
//#define	SOCKET_SERIALPORT_USART1
//#define	SOCKET_SERIALPORT_USART2
#define	SOCKET_SERIALPORT_USART3

/* SocketExtendʹ��, ��ע�͸ú궨����ʹ��SocketExtendЭ��  */
#define	SOCKET_EXTEND_ENABLE
/* SocketExtendʹ�ô���ѡ�� */
//#define	SOCKET_EXTEND_SERIALPORT_USART1
//#define	SOCKET_EXTEND_SERIALPORT_USART2
#define	SOCKET_EXTEND_SERIALPORT_USART3

/* SocketModulationʹ��, ��ע�͸ú궨����ʹ��SocketModulationЭ�� */
#define	SOCKET_MODULATION_ENABLE
/* SocketModulationʹ�ô���ѡ�� */
//#define	SOCKET_MODULATION_SERIALPORT_USART1
//#define	SOCKET_MODULATION_SERIALPORT_USART2
#define	SOCKET_MODULATION_SERIALPORT_USART3

/* SocketXunfeiʹ��, ��ע�͸ú궨����ʹ��SocketModulationЭ�� */
#define	SOCKET_XUNFEI_ENABLE
/* SocketXunfeiʹ�ô���ѡ�� */
//#define	SOCKET_XUNFEI_SERIALPORT_USART1
//#define	SOCKET_XUNFEI_SERIALPORT_USART2
#define	SOCKET_XUNFEI_SERIALPORT_USART3

/* Gat920ʹ��, ��ע�͸ú궨����ʹ��Gat920Э��  */
#define	GAT920_ENABLE
/* Gat920��ʹ�ô���, ����ʹ���ĸ���ͨ��SNѡ�� */
#define	GAT920_SERIALPORT_USART1
#define	GAT920_SERIALPORT_USART2
//#define	GAT920_SERIALPORT_USART3

#define	HARDWAREIWDG													//Ӳ�����Ź�ʹ��
#define	SOFTWAREIWDG													//������Ź�ʹ��


/* ������ʱ��û�յ��شŷ����������������ջ�0~65535(��) */
#define	RESETTIME				650


#define	GPIO_RS485_CTL			GPIOA
#define	GPIO_RS485_CTL_PIN		GPIO_Pin_1

#define	USART485				USART1
#define	GPIO485				GPIOA
#define	RS485_RxPin			GPIO_Pin_10
#define	RS485_TxPin			GPIO_Pin_9

#define	RECV_MAX				24 										// ���յ�������ݰ���
#define	ACK_LENGTH			7

#define	MVB_SYSCLOCK			8000000
//#define	MVB_SYSCLOCK			72000000


//���º궨���������ֲ�ͬ�����ݰ�
#define	DATATYPE_CARIN_WITHOUT_MAGVALUE				0x00
#define	DATATYPE_CARIN_WITH_MAGVALUE					0x01
#define	DATATYPE_CONFIG							0x02
#define	DATATYPE_CONFIG_L							0x02
#define	DATATYPE_CONFIG_H							0x03
#define	DATATYPE_HEARTBEAT_WITHOUT_MAGENV				0x04
#define	DATATYPE_HEARTBEAT_WITH_MAGENV				0x05
#define	DATATYPE_CAROUT_WITHOUT_MAGVALUE				0x06
#define	DATATYPE_CAROUT_WITH_MAGVALUE					0x07

#define	MVB_PARAM_RECV_BYTE							44
#define	MVB_PARAM_WVD_CFG_BYTE						16
#define	MVB_PARAM_NET_CFG_BYTE						24


//�شŷ��������ݵ�ͷ������3Byte
typedef struct
{
	u16 addr_dev;				// 2�ֽڵĳ���ID 
	u8  sequence_num:5;
	u8  type:3;				// �����, ������, ���ð�. 
}RF_DataHeader_TypeDef;


//��ϸ�������ݰ�20Byte
typedef struct
{
	u16 addr_dev;				// 2�ֽڵ����к�
	//u8 sequence_num:5;
	//u8 type:3;  
	u8  sequence_type;
	u8  rssi;

	//u8 word_mode;
	//u8 config_time;
	//u16 heart_time;
	u32 p1;

	//u8 carin_threshhold;
	//u8 carout_threshhold;
	//u8 measure_value;
	//u8 recalibration_value;
	u32 p2;

	u8  recalibration_time;
	u8  reserved1;
	u16 reserved2;

	u16 reserved3;
	u16 crc;
}mvb_pkg_wvd_cfg;

typedef struct
{
	u32 head;					// SN
	u16 mac_H;				// ������ŵĸ�λ
	u16 mac_L;				// ������ŵĵ�λ
	u32 crossid;				// CROSSID
	u16 crossid_H;				// ����·�ڱ���ĸ�λ
	u16 crossid_L;				// ����·�ڱ���ĵ�λ
	u16 produce_year;			// �������
	u16 produce_day;			// ��������
	u8  addr;					// 0~255��ַ
	u8  baud;					// �����ʼĴ���ֵ	0		1		2		3		4		  5		6		7		8		9
										   //1200		2400		4800		9600		14400	  19200	38400	56000	57600	115200
	u8  rt_channel;			// ʵʱ�ϴ�ѡ��
	u8  init_flag;				// ��ʼ��������ģ���־
	u8  output_mode;			// ����źŷ�ʽ
	u16 vi_output_high_time;		// �������������ĸߵ�ƽʱ��
	u16 vo_output_high_time;		// �����뿪������ĸߵ�ƽʱ��
	u16 output_low_min;			// ��С�͵�ƽʱ��
	u16 output_high_max;		// ���ߵ�ƽʱ��
	u8  handle_lost;			// �Ƿ�Զ������д���
	u8  check_repeat_time;		// ������ݰ��Ƿ��ظ���ʱ��
	u8  simple_mode;			// RT����ģʽ�Ƿ��Ǽ�ģʽ
	u16 software_version;		// ����汾��
	u16 hardware_version;		// Ӳ���汾��
	u16 dirverway_config;		// Dirverway Config Socket
}mvb_param_recv;				// ��������������42���ֽ�(ռ44�ֽ�)

typedef struct
{
	u16 addr_dev;				// 0��������к�
	u16 config_item;			// 2����λ

	//u8  car_bytes:2;			// 4 ��ǳ��������������ֽ���
	//u8  detect_with_magvalue:1;	// ����Я���ش���Ϣ
	//u8  heart_with_magvalue:1;	// ������Я���ش���Ϣ
	//u8  pkg_sent_cnt:2;		// ÿ֡���ݷ��ʹ���
	//u8  work_mode:2;			// 00���ر�ģʽ��01������ͳ��ģʽ��10��ʵʱ�ϱ�ģʽ�������뿪ʱ�����ͣ�11��ʵʱ�ϱ�ģʽ�������뿪ʱҲ����
	u8  work_mode;

	u8  config_interval;		// 5 ���ð�ʱ����������		
	u16 heartbeat_interval;		// 6 ������ʱ������λ��

	u8  vi_threshhold;			// 8 ����������ֵ
	u8  vo_threshhold;			// 9 �����뿪��ֵ

	//u8  measure_mode:2;		// 10 ����ģʽ
	//u8  measure_freq:3;		// ����Ƶ��
	//u8  remeasure_num:2;		// ÿ���������ļ������
	//u8 :1;					// ����
	u8  measure_mode;			// 10 ����ģʽ

	u8  recalibration_value:4;	// 11 ��Ч�Ļ�׼ֵ����仯��Χ
	u8  recalibration_time:4;	// ���¼����׼ֵʱ��

	u8  reserved1;				// 12 ����
	u8  reserved2;				// 13 ����
}mvb_param_wvd_config;			// ���������ò�����14���ֽ�(ռ16�ֽ�)

typedef struct
{
	u16 device_addr_h;			// �豸����ӿ�IPv4��ַ���ֽ�		��: 192.168.0.1 	192 * 256 + 168 = 49320
	u16 device_addr_l;			// �豸����ӿ�IPv4��ַ���ֽ�						0   * 256 + 1	 = 1
	
	u16 device_mask_h;			// �豸����ӿ�IPv4����������ֽ� 	��: 255.254.10.1	255 * 256 + 254 = 65534
	u16 device_mask_l;			// �豸����ӿ�IPv4����������ֽ�					10  * 256 + 1	 = 2561
	
	u16 device_bcast_h;			// �豸����ӿ�IPv4���ظ��ֽ�		��: 192.168.0.254	192 * 256 + 168 = 49320
	u16 device_bcast_l;			// �豸����ӿ�IPv4���ص��ֽ�						0   * 256 + 254 = 254
	
	u8  socketA_connect_mode;	// SOCKET�ӿ�A�����ӷ�ʽ			00: �ö˿�û������; 01: UDPͨ��; 02: TCP�ͻ���; 03: TCP��������
	
	u16 socketA_addr_h;			// SOCKET�ӿ�A�ķ�����IP���ֽ�	��: 192.168.0.1 	192 * 256 + 168 = 49320
	u16 socketA_addr_l;			// SOCKET�ӿ�A�ķ�����IP���ֽ�					0   * 256 + 1	 = 1
	
	u16 socketA_port;			// SOCKET�ӿ�A�ķ������˿�
	
	u8  socketA_connect_state;	// SOCKET�ӿ�A������״̬			00: �ӿڶϿ�; 01: �ӿ�������; 02: �ӿ����ڼ���(�����); 03: �ӿ��������ӷ�����(�ͻ���)
}mvb_param_net_config;			// �������ò�����20���ֽ�(ռ24�ֽ�)


/* ��Ŀ����ѡ�� */
typedef enum
{
	SocketTime_DISABLE,			// Socket����ʱ
	SocketTime_ENABLE			// Socket��ʱ
}PlatFormSocketTime;			// Socket��ʱ��
extern PlatFormSocketTime PlatformSockettime;

typedef enum
{
	Socket_DISABLE,			// Socket��ʹ��
	Socket_ENABLE				// Socketʹ��
}PlatFormSocket;				// Socketʹ����
extern PlatFormSocket PlatformSocket;

typedef enum
{
	SocketExtendTime_DISABLE,	// SocketExtend����ʱ
	SocketExtendTime_ENABLE		// SocketExtend��ʱ
}PlatFormSocketExtendTime;		// SocketExtend��ʱ��
extern PlatFormSocketExtendTime PlatformSocketExtendtime;

typedef enum
{
	SocketExtend_DISABLE,		// SocketExtend��ʹ��
	SocketExtend_ENABLE			// SocketExtendʹ��
}PlatFormSocketExtend;			// SocketExtendʹ����
extern PlatFormSocketExtend PlatformSocketExtend;

typedef enum
{
	SocketModulation_DISABLE,	// SocketModulation��ʹ��
	SocketModulation_ENABLE		// SocketModulationʹ��
}PlatFormSocketModulation;		// SocketModulationʹ����
extern PlatFormSocketModulation PlatformSocketModulation;

typedef enum
{
	Gat920_DISABLE,			// GAT920��ʹ��
	Gat920_ENABLE				// GAT920ʹ��
}PlatFormGat920;				// GAT920ʹ����
extern PlatFormGat920 PlatformGat920;

typedef enum
{
	Modbus_USART1,				// Modbus <--> USART1
	Modbus_USART2,				// Modbus <--> USART2
}PlatFormModBusUSART;			// Modbus���Ӵ�����
extern PlatFormModBusUSART PlatformModbusUsart;

typedef enum
{
	Gat920_USART1,				// Gat920 <--> USART1
	Gat920_USART2				// Gat920 <--> USART2
}PlatFormGat920USART;			// Gat920���Ӵ�����
extern PlatFormGat920USART PlatformGat920Usart;

typedef enum
{
	GPRS_DISABLE,				// GPRS��ʹ��
	GPRS_ENABLE				// GPRSʹ��
}PlatFormGPRS;					// GPRSʹ����
extern PlatFormGPRS PlatformGPRS;

typedef enum
{
	RJ45_DISABLE,				// RJ45��ʹ��
	RJ45_ENABLE				// RJ45ʹ��
}PlatFormRJ45;					// RJ45ʹ����
extern PlatFormRJ45 PlatformRJ45;

typedef enum
{
	LESTC_DISABLE,				// LESTC��ʹ��
	LESTC_ENABLE				// LESTCʹ��
}PlatFormLESTC;				// LESTCʹ����
extern PlatFormLESTC PlatformLESTC;

void PlatForm_SnToFunction(u32 SN);										// ����SN��ѡ������Ӧ������

extern mvb_param_recv param_recv;
extern u8 a7108_initialized;
extern mvb_param_wvd_config 	param_wvd_cfg;									// �ش���������
extern u32 systime_runtime_ms1;
extern u16 systime_time_meter1;
extern u32 systime_runtime_ms2;
extern u16 systime_time_meter2;

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *******************/
