/**
  *********************************************************************************************************
  * @file    main.c
  * @author  MoveBroad
  * @version V1.0
  * @date    
  * @brief   
  *********************************************************************************************************
  * @attention
  *		   ���ߵشŽ�������Դ��
  *		   ʹ��Ӳ�� : WVD-4V2
  *		   ��Ȩ����  ��ð�ؾ�
  *********************************************************************************************************
  */

#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "math.h"
#include "NRF24L01.h"
#include "stdarg.h"
#include "string.h"
#include "stm32_config.h"
#include "a7108.h"
#include "a7108reg.h"
#include "mbcrc.h"
#include "mb.h"
#include "mbport.h"
#include "mbconfig.h"
#include "mbrtu.h"
#include "rtc.h"
#include "bsp_usart.h"
#include "socketconfig.h"
#include "socketextendconfig.h"
#include "socketmodulationconfig.h"
#include "socketpark.h"
#include "calculationconfig.h"
#include "iooutputconfig.h"
#include "gatconfig.h"
#include "gatfunc.h"
#include "gatconnect.h"
#include "gatserial.h"
#include "gatupload.h"
#include "usrconfig.h"
#include "lestcconfig.h"
#include "lestcfunc.h"


extern u8 ack_geted;
extern int SysTick_count;
extern u8 u8params[RECV_MAX][ACK_LENGTH], u8numx, u8numy, u8numx_now;
extern volatile u8  SOCKET_RTC_CHECK;

u8 a7108_initialized;													//��ʼ��A7108
mvb_param_recv 			param_recv;  									//���������Ĳ���
mvb_param_wvd_config 		param_wvd_cfg; 								//�ش���������
mvb_param_net_config		param_net_cfg;									//�������ò���
mvb_pkg_wvd_cfg			pkg_wvd_cfg;									//����λ�����͵��������ݰ�
u8 						rssi_value[RECV_MAX];							//�洢��rssiֵ
USHORT 					output_ID[OUTPUT_MAX];							//����˿ڵĲ���
u8 OUTPUT_NUM;          													//����ʹ�õ�����˿�
u32 Crossid;															//·�ڴ���

GPIO_TypeDef* OUTPUT_TYPE[16] =											//IO�������
{
	GPIOC,GPIOC,GPIOC,GPIOC,
	GPIOA,GPIOA,GPIOA,GPIOA,
	GPIOB,GPIOB,GPIOB,GPIOB,
	GPIOB,GPIOB,GPIOB,GPIOB
};

u16 OUTPUT_PIN[16] =
{
	GPIO_Pin_5, GPIO_Pin_6,  GPIO_Pin_7,  GPIO_Pin_9,
	GPIO_Pin_8, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_15,
	GPIO_Pin_0, GPIO_Pin_2,  GPIO_Pin_3,  GPIO_Pin_4,
	GPIO_Pin_6, GPIO_Pin_7,  GPIO_Pin_8,  GPIO_Pin_9
};

/* Private function prototypes --------------------------------------------------------------------------*/
USHORT hand_simple_data(void);
USHORT hand_complete_data(void);
void hand_IOOutput(u8* buf);

void init_param_recv_default(u32 sn, u32 crossid);
void hand_config(u8* buf);
void param_save_to_flash(void);

extern void SYSCLKConfig_STOP(void);
extern void SetupClock(void);
extern void SetNVIC(void);


/* Private define ---------------------------------------------------------------------------------------*/
#define RO_FLashAddr  	((u32)0x080BE800)									//SN�洢��ַ
#define RO_CROSSIDAddr	((u32)0x080BE804)									//CROSSID�洢��ַ
#define RW_StartAddr     ((u32)0x080BF000)
#define RW_FLashAddr_p1  ((u32)0x080BF000)									//����������������
#define RW_FLashAddr_p2  ((u32)0x080BF100)									//�ش����ò���������
#define RW_FLashAddr_p3  ((u32)0x080BF200)									//����˿�ӳ�䱣����
#define RW_FLashAddr_p4	((u32)0x080BF300)									//�������ò���������
#define EndAddr		((u32)0x080BF800)
#define PageSize		((u16)0x800)										//RFT6 PageSize = 2048Byte

u32 EraseCounter = 0x0;
vu32 NbrOfPage = 0;
volatile FLASH_Status FLASHStatus;

void flash_erase(u32 StartAddr)
{
  	FLASHStatus = FLASH_COMPLETE;
	// Define the number of page to be erased
  	NbrOfPage = (EndAddr - StartAddr) >> 11;
	// Clear All pending flags 
  	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	// Erase the FLASH pages
  	for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  	{
   		FLASHStatus = FLASH_ErasePage(StartAddr + (PageSize * EraseCounter));
  	}
}
u32 flash_read(u32 addr){
	// Clear All pending flags 
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	return (*(vu32*) addr);
}
u32 flash_write(u32 addr,u32* data,u16 num){
	u16 i = 0;
	// FLASH Word program of data at addr
  	while ((FLASHStatus == FLASH_COMPLETE) && (i<num))
  	{	
    		FLASHStatus = FLASH_ProgramWord((addr+i*4), data[i] );
		i++;
  	}
	return FLASHStatus;
}

void param_save_to_flash(void)
{
	//������д��Flash
	FLASH_Unlock();
	flash_erase(RW_StartAddr);
	flash_write(RW_FLashAddr_p1, (u32*) &param_recv, MVB_PARAM_RECV_BYTE / 4);			//�������ò���
	flash_write(RW_FLashAddr_p2, (u32*) &param_wvd_cfg, MVB_PARAM_WVD_CFG_BYTE / 4);	//�������ò���
	flash_write(RW_FLashAddr_p3, (u32*) &output_ID, OUTPUT_MAX / 2);					//�������ò���
	flash_write(RW_FLashAddr_p4, (u32*) &param_net_cfg, MVB_PARAM_NET_CFG_BYTE / 4);	//�������ò���
	FLASH_Lock();
}


/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 rtc_system_time = 0;													//modbus����ϵͳʱ�䣬����16λ�͵�16λ����Ϊ0���޸�ϵͳʱ��
u16 rtc_minute_cnt = 0;													//ʱ�Ӷ�ʱ����һ��������ʾһ����
u16 rtc_second_cnt = 0;													//ʱ�Ӷ�ʱ����һ��������ʾһ����
u16 rtc_reset_time = 0;													//����ʱ��ֵ�����趨ʱ����û�յ������������豸
u32 recv_sn;

int main(void)
{
#ifdef DEBUG
	debug();
#endif
	
	int i;
	u32 *u32params;
	u32 temp;
	UCHAR *pFrame ;
	
	SetupClock();														// Configure the system clocks ͬʱʹ������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);		// Enable PWR and BKP clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/* STM32��PA.13,PA.14,PA.15,PB.03,PB.04��Ĭ�ϵ�JTAG����ӳ�� */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);					//�ͷ�PB3��PB4��PA15����ͨI/O��

#ifdef HARDWAREIWDG
	MAX823_IWDGInitEnable();												//Ӳ�����Ź���ʼ��ι��ʱ��1��
	MAX823_IWDGReloadCounter();											//Ӳ�����Ź�ι��
#endif
	
	USART_Config(USART1, 9600);											//��ʼ�����ڷ�ֹ���������
	USART_Config(USART2, 9600);
	USART_Config(USART3, 9600);
	GPIO_Configuration();												// Configure GPIO
	EXTI_Configuration();												// Configure EXTI
	TIM_Configuration();												// Configure TIMs
	RTC_Time_Init(&systemtime);											// Configure RTC clock source and prescaler
	SetNVIC();														// NVIC configuration

#ifdef HARDWAREIWDG
	MAX823_IWDGReloadCounter();											//Ӳ�����Ź�ι��
#endif

#ifdef SOFTWAREIWDG
	/* Check if the system has resumed from IWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
		RCC_ClearFlag();												// Clear reset flags
	} else {
		
	}

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);							//Enable write access to IWDG_PR and IWDG_RLR registers 
	IWDG_SetPrescaler(IWDG_Prescaler_128);									//IWDG counter clock: 40KHz(LSI) / 128 = 312.5 Hz 
	IWDG_SetReload(2500);												//2500 = 8s, debug 80s
#endif
	
	Flag_MASTER = 0;													//��ʼ������
	initRF_a7108();
	
#ifdef SOFTWAREIWDG
	IWDG_Enable();														//������ȥ��IWDG
#endif
#ifdef HARDWAREIWDG
	MAX823_IWDGReloadCounter();											//Ӳ�����Ź�ι��
#endif
	
#if 1
	GPIO_SetBits(GPIOC, GPIO_Pin_13); 										//������
	delay_1ms(300);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	
#ifdef HARDWAREIWDG
	MAX823_IWDGReloadCounter();											//Ӳ�����Ź�ι��
#endif
#endif

	u8numx = u8numx_now;
	rf_mode = SLEEP_MODE;

	/* �� flash ������ */
	u32params = (u32*) &param_recv;
	FLASH_Unlock();

#ifdef WRITE_SN_CROSSID
	recv_sn = (u32)MAC_SN;
	Crossid = (u32)CROSSID;
	flash_erase(RO_FLashAddr);
	flash_write(RO_FLashAddr, (u32*) &recv_sn, 1);
	flash_write(RO_CROSSIDAddr, (u32*) &Crossid, 1);
#endif

	recv_sn = flash_read(RO_FLashAddr);									//��ȡFlash��SN
	Crossid = flash_read(RO_CROSSIDAddr);									//��ȡFlash��Crossid

	for (i = 0; i < MVB_PARAM_RECV_BYTE / 4; i++) {
		delay_5us(240);	
		u32params[i] = flash_read(RW_FLashAddr_p1 + i * 4);
	}
	u32params  = (u32*) &param_wvd_cfg;
	for (i = 0; i < MVB_PARAM_WVD_CFG_BYTE / 4; i++) {
		delay_5us(240);
		u32params[i] = flash_read(RW_FLashAddr_p2 + i * 4);
	}
	u32params  = (u32*) &output_ID;										//��ȡFlash�д洢��outputID
	for (i = 0; i < OUTPUT_MAX / 2; i++) {
		delay_5us(240);	
		u32params[i] = flash_read(RW_FLashAddr_p3 + i * 4);
	}
	u32params  = (u32*) &param_net_cfg;
	for (i = 0; i < MVB_PARAM_NET_CFG_BYTE / 4; i++) {
		delay_5us(240);
		u32params[i] = flash_read(RW_FLashAddr_p4 + i * 4);
	}
	FLASH_Lock();
	
	param_recv.software_version		= MVB_SOFTWARE_VERSION;					//����汾��
	param_recv.hardware_version		= MVB_HARDWARE_VERSION;					//Ӳ���汾��
	
	socket_dev.ReadOutputID(output_ID);									//�ϵ��ȡoutput_ID����˿ڵĲ�����Socket�������ݰ�
	socket_extend_dev.ReadOutputID(output_ID);								//�ϵ��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�
	socket_modulation_dev.ReadOutputID(output_ID);							//�ϵ��ȡoutput_ID����˿ڵĲ�����SocketModulation�������ݰ�
	calculation_dev.ReadOutputID(output_ID);								//�ϵ��ȡoutput_ID����˿ڵĲ�����Calculation�������ݰ�
	iooutput_dev.ReadOutputID(output_ID);									//�ϵ��ȡoutput_ID����˿ڵĲ�����IOOutput�������ݰ�

	OUTPUT_NUM = ((recv_sn / 1000) % 10) * 2;
	if (OUTPUT_NUM > 16) {
	   OUTPUT_NUM = 16;
	}

	for (i = 0; i < IOOutputMAX; i++) {									//ȫ���ص�LED
		GPIO_ResetBits(OUTPUT_TYPE[i], OUTPUT_PIN[i]);
	}
	
#ifdef HARDWAREIWDG
	MAX823_IWDGReloadCounter();											//Ӳ�����Ź�ι��
#endif

#if LED_GPIO
	for (i = 0; i < OUTPUT_NUM; i++) {										//����GPIO,ȫ������һ��
		GPIO_SetBits(OUTPUT_TYPE[i], OUTPUT_PIN[i]); 
		delay_1ms(500);
		GPIO_ResetBits(OUTPUT_TYPE[i], OUTPUT_PIN[i]);
#ifdef SOFTWAREIWDG
		IWDG_ReloadCounter();											//������Ź�ι��
#endif
#ifdef HARDWAREIWDG
		MAX823_IWDGReloadCounter();										//Ӳ�����Ź�ι��
#endif
	}
#endif

	if ((recv_sn == 0xFFFFFFFF) && (Crossid == 0xFFFFFFFF)) {
		init_param_recv_default(0x12345678, 12345678);
		param_save_to_flash();
	}
	else if ((param_recv.head != recv_sn) || (param_recv.crossid != Crossid)) {
		init_param_recv_default(recv_sn, Crossid);
		param_save_to_flash();
	}
	
	PlatForm_SnToFunction(recv_sn);										//����SN��ѡ������Ӧ������
	
	temp = recv_sn;													//recv_sn ��ת
	recv_sn = (temp>>24) | ((temp&0xff0000)>>8) | ((temp&0xff00)<<8) | ((temp&0xff)<<24);
	
#ifdef USR_ENABLE
	if (PlatformGPRS == GPRS_ENABLE) {										//��ȡĬ�ϲ���GPRS/RJ45
		USR_GRPS_Dev.ReadModbusPrarm();
	}
	if (PlatformRJ45 == RJ45_ENABLE) {
		USR_K2_Dev.ReadModbusPrarm();
	}
#endif
	
#ifdef USR_ENABLE
	if (PlatformGPRS == GPRS_ENABLE) {										//�ϵ��ʼ��GPRS/RJ45
		GPIO_SetBits(OUTPUT_TYPE[1], OUTPUT_PIN[1]);
#if LED_GPIO == 0
		delay_1ms(3000);												//�ϵ�ȴ�GPRS����
#endif
		if (USR_GRPS_Dev.ModuleInit() != USR_ENOERR) {
			GPIO_SetBits(OUTPUT_TYPE[1], OUTPUT_PIN[1]);
		}
		else {
			GPIO_ResetBits(OUTPUT_TYPE[1], OUTPUT_PIN[1]);
		}
	}
	if (PlatformRJ45 == RJ45_ENABLE) {
		GPIO_SetBits(OUTPUT_TYPE[1], OUTPUT_PIN[1]);
#if LED_GPIO == 0
		delay_1ms(3000);												//�ϵ�ȴ�K2����
#endif
		if (USR_K2_Dev.ModuleInit() != USR_ENOERR) {
			GPIO_SetBits(OUTPUT_TYPE[1], OUTPUT_PIN[1]);
		}
		else {
			GPIO_ResetBits(OUTPUT_TYPE[1], OUTPUT_PIN[1]);
		}
	}
#endif
	
#ifdef SOCKET_ENABLE													//ʹ��Socket
	if (PlatformSocket == Socket_ENABLE) {									//����SNѡ���Ƿ�ʹ��Socket
		socket_dev.Init();
	}
#endif
	
#ifdef SOCKET_EXTEND_ENABLE												//ʹ��SocketExtend
	if (PlatformSocketExtend == SocketExtend_ENABLE) {						//����SNѡ���Ƿ�ʹ��SocketExtend
		socket_extend_dev.Init();
	}
#endif
	
#ifdef SOCKET_MODULATION_ENABLE
	if (PlatformSocketModulation == SocketModulation_ENABLE) {					//����SNѡ���Ƿ�ʹ��SocketModulation
		socket_modulation_dev.Init();
	}
#endif
	
	if (PlatformLESTC == LESTC_ENABLE) {									//ʹ��Lestc
		LestcInitPackData();											//��ʼ��Lestc
	}

#ifdef MODBUS_ENABLE													//ʹ��ModBus
	/* modbus init */
	eMBInit( MB_RTU, param_recv.addr, 0, param_recv.baud, MB_PAR_NONE );
	/* Enable the Modbus Protocol Stack. */
	eMBEnable(  );
	/* �Զ����͵�ַ */
	pFrame =  GetRTUBuf() + 1;
	/* First byte contains the function code. */
	*pFrame++ = MB_FUNC_READ_HOLDING_REGISTER;
	/* Second byte in the response contain the number of bytes. */
	*pFrame++ = 4;
	/* third byte in the response contain the mac. */ 
	*pFrame++ = param_recv.mac_H >> 8;
	*pFrame++ = param_recv.mac_H;
	*pFrame++ = param_recv.mac_L >> 8;
	*pFrame++ = param_recv.mac_L;
	eMBRTUSend(param_recv.addr, GetRTUBuf() + 1, 6);
#endif

	mvb_RX_Mode(ACK_LENGTH);
	
	while (1)
	{
#ifdef MODBUS_ENABLE
		eMBPoll(  );													//��ѯ����ModBus�¼�
#endif

#ifdef GAT920_ENABLE
		if (PlatformGat920 == Gat920_ENABLE) {								//����SNѡ���Ƿ�ʹ��GAT920
			gat920_dev.PollExecution();									//��ʼ����ѯ����GAT920�¼�
		}
#endif
		
#ifdef USR_ENABLE
	if (PlatformGPRS == GPRS_ENABLE) {
		USR_GRPS_Dev.MonitorChange();										//����GPRS�����Ƿ�ı�
	}
	if (PlatformRJ45 == RJ45_ENABLE) {
		USR_K2_Dev.MonitorChange();										//����RJ45�����Ƿ�ı�
	}
#endif
	
	if (iooutput_dev.EventIRQnFlag == 1) {									//IO��������¼�
		iooutput_dev.EventIRQnFlag = 0;									//IO����жϱ�־λ���
		iooutput_dev.EventIRQn();
	}
	
	if (PlatformLESTC == LESTC_ENABLE) {
		if (LestcSendTimeTick >= LestcSendTime) {
			LestcSendTimeTick = 0;
			LestcPacketData.CheckCode = LestcGetCheckCode(&LestcPacketData);
			USART_Send(USART1, (u8 *)&LestcPacketData, sizeof(LestcPacketData));
		}
	}
	
#ifdef SOFTWAREIWDG
		IWDG_ReloadCounter();											//������Ź�ι��
#endif
#ifdef HARDWAREIWDG
		MAX823_IWDGReloadCounter();										//Ӳ�����Ź�ι��
#endif
	}
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
		
	}
}
#endif

//�����ù���IDת��Ϊ1~32
USHORT ID_Convert(USHORT id)
{
	USHORT i;
	for (i = 0; i < OUTPUT_MAX; i++)
	{
		if(output_ID[i] == id)
		{
			return (i+1);
		}
	}
	
	//�����û������
	if(i == OUTPUT_MAX)
		return id;
	return id;
}

/*******************************************************************************
* Function Name  : hand_RTdata
* Description    : ʵʱ�����յ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hand_RTdata(void)
{
	USHORT usLen;

	if (param_recv.simple_mode)
		usLen = hand_simple_data();
	else
		usLen = hand_complete_data();
	
	if (usLen > 6)														//����֡���㹻
		eMBRTUSend(param_recv.addr, GetRTUBuf()+1, usLen);					//�����ý����������滻��ַ
}

//����RFEND���������������ݣ���Ҫ�����򷵻�
void hand_config(u8* buf)
{
	RF_DataHeader_TypeDef* phead = (RF_DataHeader_TypeDef*)buf;
	if(phead->type == DATATYPE_CONFIG_H)
	{
		//�����������ݸ�WVD
		if ((param_wvd_cfg.config_item != 0)&&((param_wvd_cfg.addr_dev == phead->addr_dev)||(param_wvd_cfg.addr_dev == 0xffff))) //�������Ҫ���õĲ���
		{
			mvb_TX_Mode((u8*)&param_wvd_cfg, sizeof(param_wvd_cfg), ACK_LENGTH);
		}
	}
}

//����RFEND�����������ݣ��򵥷���
USHORT hand_simple_data(void)
{
	RF_DataHeader_TypeDef* phead ;
	UCHAR *  pFrame =  GetRTUBuf()+1;
	UCHAR*   pLen =  GetRTUBuf()+2;										//Ӧ���ֽ���
	USHORT usLen = 6;
	/* First byte contains the function code. */
	*pFrame++ = MB_FUNC_READ_HOLDING_REGISTER;
	*pLen  = 4;
	pFrame++;
	//���ƽ�����ID
	memcpy(pFrame, &recv_sn, 4);
	pFrame += 4;
	while(u8numx_now != u8numx)
	{
		phead = (RF_DataHeader_TypeDef*)u8params[u8numx_now];
		if(phead->type != DATATYPE_CONFIG_L)								//ȥ�����ð���һ�η���
		{
			//IDת��
			USHORT id = ID_Convert(phead->addr_dev);
			*pFrame++ = id >> 8;
			*pFrame++ = id;

			//������
			*pFrame++ = u8params[u8numx_now][2];
	
			//������
			*pFrame++ = u8params[u8numx_now][4]; 

			*pLen += 4;
			usLen += 4;
		}
		u8numx_now ++ ;
		u8numx_now %= RECV_MAX;
	}
	return usLen;
}

//����IO�������
void hand_IOOutput(u8* buf)
{
#ifdef GAT920_ENABLE
	u8 x = 0;
	u8 y = 0;
#endif
	int i;
	u16 carnumstate = 0;
	RF_DataHeader_TypeDef header;
	
	memcpy(&header, buf, 3);
	
	carnumstate = (((~0xffff) | buf[3])<<8) | buf[4];												//��ȡ������״ֵ̬
	
	//�����������ݰ�����
	if ((header.type == DATATYPE_CARIN_WITH_MAGVALUE ) || (header.type  == DATATYPE_CARIN_WITHOUT_MAGVALUE))
	{
		for (i = 0; i < OUTPUT_MAX; i++)
		{
			if ((output_ID[i] == (header.addr_dev)) || (output_ID[i] == 0xFFFF))
			{
#ifdef GAT920_ENABLE
				if (PlatformGat920 == Gat920_ENABLE) {											//����SNѡ���Ƿ�ʹ��GAT920
					if (GATConnect == GAT_ONLINE) {
						x = i / 8;
						y = i % 8;
						if ((gatParamPulse.pulseUploadBit[x] & (0x01 << y)) != 0) {					//������������ϴ�ͨ��ʹ��λ
							gat920_dev.UploadEnqueue(output_ID[i], GAT_INDETECTIONAREA);
						}
					}
				}
#endif
				if (PlatformLESTC == LESTC_DISABLE) {
					if (i < OUTPUT_NUM) {													//�޶���16·�����Χ
						switch (param_recv.output_mode)
						{
						//�����ʽ0 : ���泵�����
						case 0:
							if (param_recv.handle_lost != 1) {									//�ж��Ƿ���Ҫ�Զ�������
								GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							else {
								iooutput_dev.Mode0IOCheck(i, carnumstate, 1);					//���泵�����
							}
							break;
						//�����ʽ1 : ������������̶�ʱ��(����)
						case 1:
							iooutput_dev.Mode1IOCheck(i, carnumstate, 1);						//������������̶�ʱ��(����)
							break;
						//�����ʽ2 : �����뿪����̶�ʱ��(����)
						case 2:
							//NULL
							break;
						//�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����)
						case 3:
							iooutput_dev.Mode3IOCheck(i, carnumstate, 1);						//��������,�뿪ʱ������̶�ʱ��(����)
							break;
						//�����ʽ4 : ������������̶�ʱ��(������)
						case 4:
							iooutput_dev.Mode4IOCheck(i, carnumstate, 1);						//������������̶�ʱ��(������)
							break;
						//�����ʽ5 : �����뿪����̶�ʱ��(������)
						case 5:
							//NULL
							break;
						//�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������)
						case 6:
							iooutput_dev.Mode6IOCheck(i, carnumstate, 1);						//��������,�뿪ʱ������̶�ʱ��(������)
							break;
						//Ĭ�� �����ʽ0 : ���泵�����
						default :
							if (param_recv.handle_lost != 1) {									//�ж��Ƿ���Ҫ�Զ�������
								GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							else {
								iooutput_dev.Mode0IOCheck(i, carnumstate, 1);					//IO���У������ ���泵�����
							}
							break;
						}
					}
				}
				else {
					switch (param_recv.output_mode)
					{
					//�����ʽ0 : ���泵�����
					case 0:
						if (param_recv.handle_lost != 1) {										//�ж��Ƿ���Ҫ�Զ�������
							if (i < OUTPUT_NUM) {
								GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							LestcCarInSetStatus(&LestcPacketData, i);
						}
						else {
							iooutput_dev.Mode0IOCheck(i, carnumstate, 1);						//���泵�����
						}
						break;
					//�����ʽ1 : ������������̶�ʱ��(����)
					case 1:
						iooutput_dev.Mode1IOCheck(i, carnumstate, 1);							//������������̶�ʱ��(����)
						break;
					//�����ʽ2 : �����뿪����̶�ʱ��(����)
					case 2:
						//NULL
						break;
					//�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����)
					case 3:
						iooutput_dev.Mode3IOCheck(i, carnumstate, 1);							//��������,�뿪ʱ������̶�ʱ��(����)
						break;
					//�����ʽ4 : ������������̶�ʱ��(������)
					case 4:
						iooutput_dev.Mode4IOCheck(i, carnumstate, 1);							//������������̶�ʱ��(������)
						break;
					//�����ʽ5 : �����뿪����̶�ʱ��(������)
					case 5:
						//NULL
						break;
					//�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������)
					case 6:
						iooutput_dev.Mode6IOCheck(i, carnumstate, 1);							//��������,�뿪ʱ������̶�ʱ��(������)
						break;
					//Ĭ�� �����ʽ0 : ���泵�����
					default :
						if (param_recv.handle_lost != 1) {										//�ж��Ƿ���Ҫ�Զ�������
							if (i < OUTPUT_NUM) {
								GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							LestcCarInSetStatus(&LestcPacketData, i);
						}
						else {
							iooutput_dev.Mode0IOCheck(i, carnumstate, 1);						//IO���У������ ���泵�����
						}
						break;
					}
				}
				
#ifdef SOCKET_ENABLE
				if (PlatformSocket == Socket_ENABLE) {											//����SNѡ���Ƿ�ʹ��Socket
					if (INTERVALTIME == 0) {
						if (PlatformSockettime == SocketTime_ENABLE) {							//�ж��Ƿ�����ʱ��
							if (SOCKET_RTC_CHECK == 0) {										//�Ժ�ʱ��
								SOCKET_ParkImplement(i, carnumstate, 1);
							}
						}
						else if (PlatformSockettime == SocketTime_DISABLE) {
							SOCKET_ParkImplement(i, carnumstate, 1);
						}
					}
				}
#endif
			}
		}
	}
	//�����뿪���ݰ�����
	else if ((header.type  == DATATYPE_CAROUT_WITH_MAGVALUE ) || (header.type  == DATATYPE_CAROUT_WITHOUT_MAGVALUE))
	{
		for (i = 0; i < OUTPUT_MAX; i++)
		{
			if ((output_ID[i] == (header.addr_dev)) || (output_ID[i] == 0xFFFF))
			{
#ifdef GAT920_ENABLE
				if (PlatformGat920 == Gat920_ENABLE) {											//����SNѡ���Ƿ�ʹ��GAT920
					if (GATConnect == GAT_ONLINE) {
						x = i / 8;
						y = i % 8;
						if ((gatParamPulse.pulseUploadBit[x] & (0x01 << y)) != 0) {					//������������ϴ�ͨ��ʹ��λ
							gat920_dev.UploadEnqueue(output_ID[i], GAT_OUTDETECTIONAREA);
						}
					}
				}
#endif
				if (PlatformLESTC == LESTC_DISABLE) {
					if (i < OUTPUT_NUM) {													//�޶���16·�����Χ
						switch (param_recv.output_mode)
						{
						//�����ʽ0 : ���泵�����
						case 0:
							if (param_recv.handle_lost != 1) {									//�ж��Ƿ���Ҫ�Զ�������
								GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							else {
								iooutput_dev.Mode0IOCheck(i, carnumstate, 0);					//IO���У������ ���泵�����
							}
							break;
						//�����ʽ1 : ������������̶�ʱ��(����)
						case 1:
							//NULL
							break;
						//�����ʽ2 : �����뿪����̶�ʱ��(����)
						case 2:
							iooutput_dev.Mode2IOCheck(i, carnumstate, 0);						//�����뿪����̶�ʱ��(����)
							break;
						//�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����)
						case 3:
							iooutput_dev.Mode3IOCheck(i, carnumstate, 0);						//��������,�뿪ʱ������̶�ʱ��(����)
							break;
						//�����ʽ4 : ������������̶�ʱ��(������)
						case 4:
							//NULL
							break;
						//�����ʽ5 : �����뿪����̶�ʱ��(������)
						case 5:
							iooutput_dev.Mode5IOCheck(i, carnumstate, 0);						//�����뿪����̶�ʱ��(������)
							break;
						//�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������)
						case 6:
							iooutput_dev.Mode6IOCheck(i, carnumstate, 0);						//��������,�뿪ʱ������̶�ʱ��(������)
							break;
						//Ĭ�� �����ʽ0 : ���泵�����
						default :
							if (param_recv.handle_lost != 1) {									//�ж��Ƿ���Ҫ�Զ�������
								GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							else {
								iooutput_dev.Mode0IOCheck(i, carnumstate, 0);					//IO���У������ ���泵�����
							}
							break;
						}
					}
				}
				else {
					switch (param_recv.output_mode)
					{
					//�����ʽ0 : ���泵�����
					case 0:
						if (param_recv.handle_lost != 1) {										//�ж��Ƿ���Ҫ�Զ�������
							if (i < OUTPUT_NUM) {
								GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							LestcCarOutSetStatus(&LestcPacketData, i);
						}
						else {
							iooutput_dev.Mode0IOCheck(i, carnumstate, 0);						//IO���У������ ���泵�����
						}
						break;
					//�����ʽ1 : ������������̶�ʱ��(����)
					case 1:
						//NULL
						break;
					//�����ʽ2 : �����뿪����̶�ʱ��(����)
					case 2:
						iooutput_dev.Mode2IOCheck(i, carnumstate, 0);							//�����뿪����̶�ʱ��(����)
						break;
					//�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����)
					case 3:
						iooutput_dev.Mode3IOCheck(i, carnumstate, 0);							//��������,�뿪ʱ������̶�ʱ��(����)
						break;
					//�����ʽ4 : ������������̶�ʱ��(������)
					case 4:
						//NULL
						break;
					//�����ʽ5 : �����뿪����̶�ʱ��(������)
					case 5:
						iooutput_dev.Mode5IOCheck(i, carnumstate, 0);							//�����뿪����̶�ʱ��(������)
						break;
					//�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������)
					case 6:
						iooutput_dev.Mode6IOCheck(i, carnumstate, 0);							//��������,�뿪ʱ������̶�ʱ��(������)
						break;
					//Ĭ�� �����ʽ0 : ���泵�����
					default :
						if (param_recv.handle_lost != 1) {										//�ж��Ƿ���Ҫ�Զ�������
							if (i < OUTPUT_NUM) {
								GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							LestcCarOutSetStatus(&LestcPacketData, i);
						}
						else {
							iooutput_dev.Mode0IOCheck(i, carnumstate, 0);						//IO���У������ ���泵�����
						}
						break;
					}
				}
				
#ifdef SOCKET_ENABLE
				if (PlatformSocket == Socket_ENABLE) {											//����SNѡ���Ƿ�ʹ��Socket
					if (INTERVALTIME == 0) {
						if (PlatformSockettime == SocketTime_ENABLE) {							//�ж��Ƿ�����ʱ��
							if (SOCKET_RTC_CHECK == 0) {										//�Ժ�ʱ��
								SOCKET_ParkImplement(i, carnumstate, 0);
							}
						}
						else if (PlatformSockettime == SocketTime_DISABLE) {
							SOCKET_ParkImplement(i, carnumstate, 0);
						}
					}
				}
#endif
			}
		}
	}
}

//����RFEND������������,��������
USHORT hand_complete_data(void)
{
	RF_DataHeader_TypeDef* phead ;
	UCHAR*  buf;
	UCHAR*  pFrame = GetRTUBuf() + 1;
	UCHAR*  pLen =  GetRTUBuf() + 2;										//Ӧ���ֽ���
	USHORT usLen = 6;
	   
	/* First byte contains the function code. */
	*pFrame++ = MB_FUNC_READ_HOLDING_REGISTER;
	*pLen  = 4;
	pFrame++;
	//���ƽ�����ID
	memcpy(pFrame, &recv_sn, 4);
	pFrame += 4;
	while(u8numx_now != u8numx)
	{
		phead = (RF_DataHeader_TypeDef*)u8params[u8numx_now];	
		buf = u8params[u8numx_now];	
		if(phead->type == DATATYPE_CONFIG_L)
		{															//�����λ���ð�
			memcpy(&pkg_wvd_cfg, phead, 3);
			pkg_wvd_cfg.rssi = rssi_value[u8numx_now];
			pkg_wvd_cfg.p1=  *(u32*)(buf+3);
		}
		else if(phead->type == DATATYPE_CONFIG_H)
		{
			//ƴ�����ݰ�
			if(pkg_wvd_cfg.addr_dev == phead->addr_dev) {
				//IDת��
				pkg_wvd_cfg.addr_dev = ID_Convert(pkg_wvd_cfg.addr_dev);
				*pFrame++ = (pkg_wvd_cfg.addr_dev)  >> 8;
				*pFrame++ = pkg_wvd_cfg.addr_dev ;
				
				pkg_wvd_cfg.rssi =( pkg_wvd_cfg.rssi + rssi_value[u8numx_now]) / 2;		//ȡ2�ε�ƽ��ֵ
				pkg_wvd_cfg.p2= *(u32*)(buf+3);

				memcpy(pFrame, &(pkg_wvd_cfg.sequence_type), 16);				//ֱ�Ӹ���
				pFrame += 14;

				//ԭʼID
				*pFrame++ = (phead->addr_dev)>>8;
				*pFrame++ =  phead->addr_dev;

				*pLen += 18;
				usLen += 18;
			}
		}
		else
		{
			//IDת��
			USHORT id = ID_Convert(phead->addr_dev);
			*pFrame++ = id >> 8;
			*pFrame++ = id;

			//������
			*pFrame++ = u8params[u8numx_now][2];

			//RSSI
			*pFrame++ = rssi_value[u8numx_now];

			//Input Stauts
			memset(pFrame,0,14);										//��0
			//�شű仯��ֵ
			memcpy(pFrame,&u8params[u8numx_now][5],2);
			pFrame += 8;
			//������
			memcpy(pFrame,&u8params[u8numx_now][3],2); 
			pFrame += 4;

			//ԭʼID
			*pFrame++ = (phead->addr_dev)>>8;
			*pFrame++ =  phead->addr_dev;

			*pLen += 18;
			usLen += 18;
		
		}
		u8numx_now ++ ;
		u8numx_now %= RECV_MAX;
	}
	return usLen;
}

void init_param_recv_default(u32 sn, u32 crossid)
{
	u32 temp;
	memset(&param_recv, 0, MVB_PARAM_RECV_BYTE);
	memset(&param_wvd_cfg, 0, MVB_PARAM_WVD_CFG_BYTE);
	memset(&param_net_cfg, 0, MVB_PARAM_NET_CFG_BYTE);

  	param_recv.head            		= sn;								//SN
  	param_recv.mac_H           		= sn >> 16;							//������ŵĸ�λ
  	param_recv.mac_L           		= sn;								//������ŵĵ�λ
	temp = sn / 10000;
	param_recv.produce_day = temp % 10;
	temp /= 10;
	param_recv.produce_day += (temp % 10) << 4;
	temp /= 10;
	param_recv.produce_day += (temp % 10) << 8;
	temp /= 10;
	param_recv.produce_day += (temp % 10) << 12;
	temp /= 10;
	param_recv.produce_year   = 0x2000 + ((temp / 10) << 4) + (temp % 10);
	
	param_recv.crossid				= crossid;							//CROSSID
	param_recv.crossid_H			= crossid >> 16;						//����·�ڱ���ĸ�λ
	param_recv.crossid_L			= crossid;							//����·�ڱ���ĵ�λ

	param_recv.addr		  		= 1;									//0~255��ַ
  	param_recv.baud      	  		= 3;									//�����ʼĴ���ֵ
  	param_recv.rt_channel      		= 0;									//ʵʱ�ϴ�ѡ��
  	param_recv.init_flag       		= 0;									//��ʼ��������ģ���־
  	param_recv.output_mode    		= 0;									//����źŷ�ʽ
  	param_recv.vi_output_high_time 	= 10;								//�������������ĸߵ�ƽʱ��
  	param_recv.vo_output_high_time 	= 10;								//�����뿪������ĸߵ�ƽʱ��
  	param_recv.output_low_min  		= 10;								//��С�͵�ƽʱ��
  	param_recv.output_high_max 		= 120;								//���ߵ�ƽʱ��
  	param_recv.handle_lost 			= 1;									//�Ƿ�Զ������д���
  	param_recv.check_repeat_time 		= 0;									//������ݰ��Ƿ��ظ���ʱ��
	param_recv.simple_mode 			= 1;									//RT����ģʽ�Ƿ��Ǽ�ģʽ
	param_recv.software_version		= MVB_SOFTWARE_VERSION;					//����汾��
	param_recv.hardware_version		= MVB_HARDWARE_VERSION;					//Ӳ���汾��

	param_wvd_cfg.addr_dev 			= 0xffff;								//��������к�
	param_wvd_cfg.config_item 		= 0x0;								//����λ
	param_wvd_cfg.heartbeat_interval 	= 300;								//���������ʱ�� ��
	param_wvd_cfg.config_interval 	= 5;									//���ñ����ʱ�� ��
	param_wvd_cfg.work_mode 			= 0xcd;								//����ģʽ
	param_wvd_cfg.vi_threshhold 		= 80;								//��������������
	param_wvd_cfg.vo_threshhold 		= 13;								//�����뿪����
	param_wvd_cfg.measure_mode 		= 0x10;								//����ģʽ
	param_wvd_cfg.recalibration_time 	= 1;									//ѧϰʱ�� ��
	param_wvd_cfg.recalibration_value 	= 10;								//��Ч�Ļ�׼ֵ����仯��Χ
	
	param_net_cfg.device_addr_h		= 0xC0A8;								//�豸����ӿ�IPv4��ַ���ֽ�
	param_net_cfg.device_addr_l		= 0x0132;								//�豸����ӿ�IPv4��ַ���ֽ�
	param_net_cfg.device_mask_h		= 0xFFFF;								//�豸����ӿ�IPv4����������ֽ�
	param_net_cfg.device_mask_l		= 0xFF00;								//�豸����ӿ�IPv4����������ֽ�
	param_net_cfg.device_bcast_h		= 0xC0A8;								//�豸����ӿ�IPv4���ظ��ֽ�
	param_net_cfg.device_bcast_l		= 0x0101;								//�豸����ӿ�IPv4���ص��ֽ�
	param_net_cfg.socketA_connect_mode = 0x02;								//SOCKET�ӿ�A�����ӷ�ʽ
	param_net_cfg.socketA_addr_h		= 0x6A0E;								//SOCKET�ӿ�A�ķ�����IP���ֽ� //0xC0A8 = 192.168
	param_net_cfg.socketA_addr_l		= 0x8EA9;								//SOCKET�ӿ�A�ķ�����IP���ֽ� //0x016E = 1.110
	param_net_cfg.socketA_port		= 4001;								//SOCKET�ӿ�A�ķ������˿�
	param_net_cfg.socketA_connect_state= 0;									//SOCKET�ӿ�A������״̬

	memset(output_ID, 0x0, 2 * OUTPUT_MAX);
	socket_dev.ReadOutputID(output_ID);									//��ȡoutput_ID����˿ڵĲ�����Socket�������ݰ�
	socket_extend_dev.ReadOutputID(output_ID);								//��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�
	socket_modulation_dev.ReadOutputID(output_ID);							//��ȡoutput_ID����˿ڵĲ�����SocketModulation�������ݰ�
	calculation_dev.ReadOutputID(output_ID);								//��ȡoutput_ID����˿ڵĲ�����Calculation�������ݰ�
	iooutput_dev.ReadOutputID(output_ID);									//��ȡoutput_ID����˿ڵĲ�����IOOutput�������ݰ�
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;
	
	if ((( usAddress >= REG_RECV_START ) && ( usAddress + usNRegs <= REG_RECV_START + REG_RECV_NREGS )) ||
	    (( usAddress >= REG_NET_START ) && ( usAddress + usNRegs <= REG_NET_START + REG_NET_NREGS ))    ||
	    (( usAddress >= REG_WVD_START ) && ( usAddress + usNRegs <= REG_WVD_START + REG_WVD_NREGS )) )
	{
		switch ( eMode )
		{
		/* Pass current register values to the protocol stack. */
		case MB_REG_READ:
		while( usNRegs > 0 )
		{
			switch ( usAddress)
        		{
				case REG_RW_ADDR:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_recv.addr;
					break;
				case  REG_RW_BAUD:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char ) param_recv.baud;
					break;
				case  REG_RW_RT_CHANNEL:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_recv.rt_channel;
					break;
				case REG_RO_MAC_H:
					*pucRegBuffer++ = ( unsigned char )( param_recv.mac_H>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.mac_H & 0xff);
					break;
				case  REG_RO_MAC_L:
					*pucRegBuffer++ = ( unsigned char )( param_recv.mac_L>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.mac_L & 0xff);
					break;
				case  REG_RO_PRODUCE_YEAR:
					*pucRegBuffer++ = ( unsigned char )( param_recv.produce_year>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.produce_year & 0xff);
					break;		
				case  REG_RO_PRODUCE_DAY:
					*pucRegBuffer++ = ( unsigned char )( param_recv.produce_day>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.produce_day& 0xff);
					break;		
				case  REG_RW_INIT_FLAG:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char ) param_recv.init_flag;
					break;		
				case  REG_RO_PORT_NUM:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = OUTPUT_NUM;
					break;		
				case  REG_RW_OUTPUT_MODE:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_recv.output_mode;
					break;
				case  REG_RW_VI_OUTPUT_HIGH_TIME:
					*pucRegBuffer++ = ( unsigned char )( param_recv.vi_output_high_time>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.vi_output_high_time & 0xff);
					break;
				case  REG_RW_VO_OUTPUT_HIGH_TIME:
					*pucRegBuffer++ = ( unsigned char )( param_recv.vo_output_high_time>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.vo_output_high_time & 0xff);
					break;
				case  REG_RW_OUTPUT_LOW_MIN:
					*pucRegBuffer++ = ( unsigned char )( param_recv.output_low_min>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.output_low_min & 0xff);
					break;
				case  REG_RW_OUTPUT_HIGH_MAX:
					*pucRegBuffer++ = ( unsigned char )( param_recv.output_high_max>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.output_high_max & 0xff);
					break;
				case REG_RW_HANDLE_LOST:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_recv.handle_lost;
					break;
				case  REG_RW_CHECK_REPEAT_TIME:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_recv.check_repeat_time;
					break;
				case REG_RO_SOFTWARE_VERSION:
					*pucRegBuffer++ = ( unsigned char )( param_recv.software_version>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.software_version & 0xff);
					break;
				case REG_RO_HARDWARE_VERSION:
					*pucRegBuffer++ = ( unsigned char )( param_recv.hardware_version>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.hardware_version & 0xff);
					break;
				case REG_RW_RTC_TIME_H:
					*pucRegBuffer++ = ( unsigned char )( (RTC_GetCounter() - (8 * 60 * 60))>> 24);
	                		*pucRegBuffer++ = ( unsigned char )( (RTC_GetCounter() - (8 * 60 * 60))>> 16);
					break;
				case REG_RW_RTC_TIME_L:
					*pucRegBuffer++ = ( unsigned char )( (RTC_GetCounter() - (8 * 60 * 60))>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( (RTC_GetCounter() - (8 * 60 * 60)) & 0xff);
					break;
				case REG_RO_CROSSID_H:
					*pucRegBuffer++ = ( unsigned char )( param_recv.crossid_H>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.crossid_H & 0xff);
					break;
				case REG_RO_CROSSID_L:
					*pucRegBuffer++ = ( unsigned char )( param_recv.crossid_L>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_recv.crossid_L & 0xff);
					break;
				case  REG_RW_WVD_ID:
					*pucRegBuffer++ = ( unsigned char )( param_wvd_cfg.addr_dev>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_wvd_cfg.addr_dev & 0xff);
					break;
				case  REG_RW_WVD_CONFIG_FLAG:
					*pucRegBuffer++ = ( unsigned char )( param_wvd_cfg.config_item>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_wvd_cfg.config_item & 0xff);
					break;
				case  REG_RW_WVD_WORKMODE:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_wvd_cfg.work_mode;
					break;
				case  REG_RW_WVD_HEARTBEAT_INTERVAL:
					*pucRegBuffer++ = ( unsigned char )( param_wvd_cfg.heartbeat_interval>> 8);
	                		*pucRegBuffer++ = ( unsigned char )( param_wvd_cfg.heartbeat_interval & 0xff);
					break;
				case  REG_RW_WVD_CONFIG_INTERVAL:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_wvd_cfg.config_interval;
					break;
				case  REG_RW_WVD_VI_THRESHHOLD:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_wvd_cfg.vi_threshhold;
					break;
				case  REG_RW_WVD_VO_THRESHHOLD:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_wvd_cfg.vo_threshhold;
					break;
				case  REG_RW_WVD_MEASURE_MODE:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_wvd_cfg.measure_mode;
					break;
				case  REG_RW_WVD_REBASE_VALUE:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char ) param_wvd_cfg.recalibration_value ;
					break;
				case  REG_RW_WVD_REBASE_TIME:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_wvd_cfg.recalibration_time;
					break;
				case  REG_RW_NET_DEVICE_ADDR_H:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_addr_h>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_addr_h & 0xff);
					break;
				case  REG_RW_NET_DEVICE_ADDR_L:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_addr_l>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_addr_l & 0xff);
					break;
				case  REG_RW_NET_DEVICE_MASK_H:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_mask_h>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_mask_h & 0xff);
					break;
				case  REG_RW_NET_DEVICE_MASK_L:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_mask_l>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_mask_l & 0xff);
					break;
				case  REG_RW_NET_DEVICE_BCAST_H:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_bcast_h>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_bcast_h & 0xff);
					break;
				case  REG_RW_NET_DEVICE_BCAST_L:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_bcast_l>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.device_bcast_l & 0xff);
					break;
				case  REG_RW_NET_SOCKETA_CONNECT_MODE:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_net_cfg.socketA_connect_mode;
					break;
				case  REG_RW_NET_SOCKETA_ADDR_H:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.socketA_addr_h>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.socketA_addr_h & 0xff);
					break;
				case  REG_RW_NET_SOCKETA_ADDR_L:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.socketA_addr_l>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.socketA_addr_l & 0xff);
					break;
				case  REG_RW_NET_SOCKETA_PORT:
					*pucRegBuffer++ = ( unsigned char )( param_net_cfg.socketA_port>> 8);
						*pucRegBuffer++ = ( unsigned char )( param_net_cfg.socketA_port & 0xff);
					break;
				case  REG_RO_NET_SOCKETA_CONNECT_STATE:
					*pucRegBuffer++ = 0;
	                		*pucRegBuffer++ = ( unsigned char )param_net_cfg.socketA_connect_state;
					break;
				default:
					*pucRegBuffer++ = 0;
					*pucRegBuffer++ = 0;
			}
			usAddress++;
			usNRegs--;
		}
		break;

            /* Update current register values with new values from the
             * protocol stack. */
		case MB_REG_WRITE:
		while( usNRegs > 0 )
		{
			switch ( usAddress)
			{
				case REG_RW_ADDR:
					pucRegBuffer++;
	                		param_recv.addr = *pucRegBuffer++;							
					eMBDisable(  );
  					eMBInit( MB_RTU, param_recv.addr, 0, param_recv.baud, MB_PAR_NONE );
					eMBEnable(  );	
					break;
				case  REG_RW_BAUD:
					pucRegBuffer++;
	                		param_recv.baud = *pucRegBuffer++;
					eMBDisable(  );
  					eMBInit( MB_RTU, param_recv.addr, 0, param_recv.baud, MB_PAR_NONE );
					eMBEnable(  );
					break;
				case  REG_RW_RT_CHANNEL:
					pucRegBuffer++;
	                		param_recv.rt_channel = *pucRegBuffer++;
					break;			
				case  REG_RW_INIT_FLAG:
					pucRegBuffer++;
	                		param_recv.init_flag = *pucRegBuffer++;
					break;			
				case  REG_RW_OUTPUT_MODE:
					pucRegBuffer++;
	                		param_recv.output_mode = *pucRegBuffer++;
					break;
				case  REG_RW_VI_OUTPUT_HIGH_TIME:
					param_recv.vi_output_high_time = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_recv.vi_output_high_time |= *pucRegBuffer++;
					break;
				case  REG_RW_VO_OUTPUT_HIGH_TIME:
					param_recv.vo_output_high_time = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_recv.vo_output_high_time |= *pucRegBuffer++;
					break;
				case  REG_RW_OUTPUT_LOW_MIN:
					param_recv.output_low_min = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_recv.output_low_min |= *pucRegBuffer++;
					break;
				case  REG_RW_OUTPUT_HIGH_MAX:
					param_recv.output_high_max = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_recv.output_high_max |= *pucRegBuffer++;
					break;
				case REG_RW_HANDLE_LOST:
					pucRegBuffer++;
	                		param_recv.handle_lost = *pucRegBuffer++;
					break;
				case  REG_RW_CHECK_REPEAT_TIME:
					pucRegBuffer++;
	                		param_recv.check_repeat_time = *pucRegBuffer++;
					break;
				case  REG_RW_RTC_TIME_H:
					rtc_system_time = *pucRegBuffer << 24;
					pucRegBuffer++;
	                		rtc_system_time |= *pucRegBuffer << 16;
						pucRegBuffer++;
					break;
				case  REG_RW_RTC_TIME_L:
					rtc_system_time |= *pucRegBuffer << 8;
					pucRegBuffer++;
	                		rtc_system_time |= *pucRegBuffer++;
					break;
				case  REG_RW_WVD_ID:
					param_wvd_cfg.addr_dev = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_wvd_cfg.addr_dev |= *pucRegBuffer++;
					break;
				case  REG_RW_WVD_CONFIG_FLAG:
					param_wvd_cfg.config_item = *pucRegBuffer << 8;
					pucRegBuffer++;
						param_wvd_cfg.config_item|= *pucRegBuffer++;
					break;
				case  REG_RW_WVD_WORKMODE:
					pucRegBuffer++;
	                		param_wvd_cfg.work_mode = *pucRegBuffer++;
					break;
				case  REG_RW_WVD_HEARTBEAT_INTERVAL:
					param_wvd_cfg.heartbeat_interval = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_wvd_cfg.heartbeat_interval |= *pucRegBuffer++;
					break;
				case  REG_RW_WVD_CONFIG_INTERVAL:
					pucRegBuffer++;
	                		param_wvd_cfg.config_interval = *pucRegBuffer++;
					break;
				case  REG_RW_WVD_VI_THRESHHOLD:
					pucRegBuffer++;
	                		param_wvd_cfg.vi_threshhold = *pucRegBuffer++;
					break;
				case  REG_RW_WVD_VO_THRESHHOLD:
					pucRegBuffer++;
	                		param_wvd_cfg.vo_threshhold = *pucRegBuffer++;
					break;
				case  REG_RW_WVD_MEASURE_MODE:
					pucRegBuffer++;
	                		param_wvd_cfg.measure_mode = *pucRegBuffer++;
					break;
				case  REG_RW_WVD_REBASE_VALUE:
					pucRegBuffer++;
	                		param_wvd_cfg.recalibration_value = *pucRegBuffer++;
					break;
				case  REG_RW_WVD_REBASE_TIME:
					pucRegBuffer++;
	                		param_wvd_cfg.recalibration_time = *pucRegBuffer++;
					break;
				case  REG_RW_NET_DEVICE_ADDR_H:
					param_net_cfg.device_addr_h = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.device_addr_h |= *pucRegBuffer++;
					break;
				case  REG_RW_NET_DEVICE_ADDR_L:
					param_net_cfg.device_addr_l = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.device_addr_l |= *pucRegBuffer++;
					break;
				case  REG_RW_NET_DEVICE_MASK_H:
					param_net_cfg.device_mask_h = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.device_mask_h |= *pucRegBuffer++;
					break;
				case  REG_RW_NET_DEVICE_MASK_L:
					param_net_cfg.device_mask_l = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.device_mask_l |= *pucRegBuffer++;
					break;
				case  REG_RW_NET_DEVICE_BCAST_H:
					param_net_cfg.device_bcast_h = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.device_bcast_h |= *pucRegBuffer++;
					break;
				case  REG_RW_NET_DEVICE_BCAST_L:
					param_net_cfg.device_bcast_l = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.device_bcast_l |= *pucRegBuffer++;
					break;
				case  REG_RW_NET_SOCKETA_CONNECT_MODE:
					pucRegBuffer++;
	                		param_net_cfg.socketA_connect_mode = *pucRegBuffer++;
					break;
				case  REG_RW_NET_SOCKETA_ADDR_H:
					param_net_cfg.socketA_addr_h = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.socketA_addr_h |= *pucRegBuffer++;
					break;
				case  REG_RW_NET_SOCKETA_ADDR_L:
					param_net_cfg.socketA_addr_l = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.socketA_addr_l |= *pucRegBuffer++;
					break;
				case  REG_RW_NET_SOCKETA_PORT:
					param_net_cfg.socketA_port = *pucRegBuffer << 8;
					pucRegBuffer++;
	                		param_net_cfg.socketA_port |= *pucRegBuffer++;
					break;
				default:pucRegBuffer +=2;

				//���������λ����24Сʱ���Զ����
				if (param_wvd_cfg.config_item != 0)
					rtc_minute_cnt = 0;				
				}
				
				//���ModBus�޸���ϵͳʱ�䣬������ϵͳʱ��
				if (((rtc_system_time & 0xFFFF0000) != 0) && ((rtc_system_time & 0xFFFF) != 0)) {
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
					PWR_BackupAccessCmd(ENABLE);
					RTC_WaitForLastTask();
					RTC_SetCounter(rtc_system_time + (8 * 60 * 60));			//д��RTC�Ĵ���ֵ
					RTC_WaitForLastTask();
					rtc_system_time = 0;								//ʱ�����ý�����0
				}
				
				usAddress++;
				usNRegs--;
			}
			param_save_to_flash();
			socket_dev.ReadOutputID(output_ID);							//��ȡoutput_ID����˿ڵĲ�����Socket�������ݰ�
			socket_extend_dev.ReadOutputID(output_ID);						//��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�
			socket_modulation_dev.ReadOutputID(output_ID);					//��ȡoutput_ID����˿ڵĲ�����SocketModulation�������ݰ�
			calculation_dev.ReadOutputID(output_ID);						//��ȡoutput_ID����˿ڵĲ�����Calculation�������ݰ�
			iooutput_dev.ReadOutputID(output_ID);							//��ȡoutput_ID����˿ڵĲ�����IOOutput�������ݰ�
		}
	}
	else if (( usAddress >= REG_OUTPUT_START ) && ( usAddress + usNRegs <= REG_OUTPUT_START + OUTPUT_MAX))
	{
    	    iRegIndex = ( int )( usAddress - REG_OUTPUT_START);
          switch ( eMode )
		{
		/* Pass current register values to the protocol stack. */
		case MB_REG_READ:
			while( usNRegs > 0 )
            		{
                		*pucRegBuffer++ = ( unsigned char )( output_ID[iRegIndex] >> 8);
                		*pucRegBuffer++ = ( unsigned char )( output_ID[iRegIndex] & 0xFF);
               		iRegIndex++;
                		usNRegs--;
            		}
            break;
		/* Update current register values with new values from the protocol stack. */
		case MB_REG_WRITE:
			while ( usNRegs > 0 )
			{
				output_ID[iRegIndex] = *pucRegBuffer++<<8;
				output_ID[iRegIndex] |= *pucRegBuffer++;
				iRegIndex++;
				usNRegs--;
			}
			param_save_to_flash();
			socket_dev.ReadOutputID(output_ID);							//��ȡoutput_ID����˿ڵĲ�����Socket�������ݰ�
			socket_extend_dev.ReadOutputID(output_ID);						//��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�
			socket_modulation_dev.ReadOutputID(output_ID);					//��ȡoutput_ID����˿ڵĲ�����SocketModulation�������ݰ�
			calculation_dev.ReadOutputID(output_ID);						//��ȡoutput_ID����˿ڵĲ�����Calculation�������ݰ�
			iooutput_dev.ReadOutputID(output_ID);							//��ȡoutput_ID����˿ڵĲ�����IOOutput�������ݰ�
#ifdef GAT920_ENABLE
			if (PlatformGat920 == Gat920_ENABLE) {							//����SNѡ���Ƿ�ʹ��GAT920
				gat920_dev.ReadOutputID(output_ID);						//��ȡoutput_ID����˿ڵĲ�����GAT�����ϴ�ͨ���Ŷ�Ӧ�شű��
			}
#endif
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	return MB_ENOREG;
}

/********************************************** END OF FLEE **********************************************/
