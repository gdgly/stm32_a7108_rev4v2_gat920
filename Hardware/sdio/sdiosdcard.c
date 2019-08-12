/**
  *********************************************************************************************************
  * @file    sdiosdcard.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   SD卡
  *********************************************************************************************************
  * @attention
  *		This file provides a set of functions needed to manage the SDIO SD Card memory mounted on STM32xx-EVAL board (refer to stm32_eval.h to know about the boards supporting this memory). 
  *
  *
  *
  *********************************************************************************************************
  */

#include "sdiosdcard.h"
#include "stm32_config.h"
#include "bsp_usart.h"

/* Private macro -------------------------------------------------------------*/
/** 
  * @brief  SDIO Static flags, TimeOut, FIFO Address  
  */
#define NULL 0
#define SDIO_STATIC_FLAGS               ((uint32_t)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((uint32_t)0x00010000)

/** 
  * @brief  Mask for errors Card Status R1 (OCR Register) 
  */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define SD_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((uint32_t)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((uint32_t)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)

/** 
  * @brief  Masks for R6 Response 
  */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define SD_STD_CAPACITY                 ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)
#define SD_ALLZERO                      ((uint32_t)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)
#define SD_CARD_LOCKED                  ((uint32_t)0x02000000)

#define SD_DATATIMEOUT                  ((uint32_t)0xFFFFFFFF)
#define SD_0TO7BITS                     ((uint32_t)0x000000FF)
#define SD_8TO15BITS                    ((uint32_t)0x0000FF00)
#define SD_16TO23BITS                   ((uint32_t)0x00FF0000)
#define SD_24TO31BITS                   ((uint32_t)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFF)

#define SD_HALFFIFO                     ((uint32_t)0x00000008)
#define SD_HALFFIFOBYTES                ((uint32_t)0x00000020)

/** 
  * @brief  Command Class Supported 
  */
#define SD_CCCC_LOCK_UNLOCK             ((uint32_t)0x00000080)
#define SD_CCCC_WRITE_PROT              ((uint32_t)0x00000040)
#define SD_CCCC_ERASE                   ((uint32_t)0x00000020)

/** 
  * @brief  Following commands are SD Card Specific commands.
  *         SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SDIO_SEND_IF_COND               ((uint32_t)0x00000008)


/* Private variables ---------------------------------------------------------*/
static uint32_t CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1;						//存储卡类型
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 0;								//存储CSD, CID, 卡相对地址
static uint8_t  SDSTATUS_Tab[16];											//存储卡状态
__IO uint32_t StopCondition = 0;											//用于停止卡操作的标志
__IO uint32_t TransferEnd = 0;											//用于标志传输是否结束
__IO SD_Error TransferError = SD_OK;										//用于存储卡错误

SD_CardInfo			SDCardInfo;										//存储卡的信息

SDIO_InitTypeDef		SDIO_struct;
SDIO_CmdInitTypeDef		SDIO_Cmdstruct;
SDIO_DataInitTypeDef	SDIO_Datastruct;

/* Private function prototypes -----------------------------------------------*/
static SD_Error CmdError(void);
static SD_Error CmdResp7Error(void);
static SD_Error CmdResp1Error(uint8_t cmd);
static SD_Error CmdResp3Error(void);
static SD_Error CmdResp2Error(void);
static SD_Error CmdResp6Error(uint8_t cmd, uint16_t *prca);

static SD_Error SDEnWideBus(FunctionalState NewState);
static SD_Error IsCardProgramming(uint8_t *pstatus);
static SD_Error FindSCR(uint16_t rca, uint32_t *pscr);

static void NVIC_SDIO_Configuration(void);
static void GPIO_SDIO_Configuration(void);

static uint32_t SD_DMAEndOfTransferStatus(void);
static void SD_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize);
static void SD_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);

/* Private functions ---------------------------------------------------------*/

/**********************************************************************************************************
 @Function			uint32_t SD_DMAEndOfTransferStatus(void)
 @Description			SD_DMAEndOfTransferStatus							: Returns the DMA End Of Transfer Status
 @Input				void
 @Return				DMA SDIO Channel Status
**********************************************************************************************************/
uint32_t SD_DMAEndOfTransferStatus(void)
{
	return (uint32_t)DMA_GetFlagStatus(DMA2_FLAG_TC4);
}

/**********************************************************************************************************
 @Function			void SD_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
 @Description			SD_DMA_RxConfig									: 为SDIO接收数据配置DMA2的通道4的请求
 @Input				BufferDST											: 用于装载数据的变量指针
					BufferSize										: 缓冲区大小
 @Return				void
**********************************************************************************************************/
void SD_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* 清除DMA标志位 */
	DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);
	
	/*!< DMA2 Channel4 disable */
	DMA_Cmd(DMA2_Channel4, DISABLE);														//SDIO为DMA2第四通道
	
	/*!< DMA2 Channel4 Config */
	DMA_InitStructure.DMA_PeripheralBaseAddr		= (uint32_t)SDIO_FIFO_ADDRESS;				//外设地址FiFo
	DMA_InitStructure.DMA_MemoryBaseAddr			= (uint32_t)BufferDST;						//目标地址
	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralSRC;						//外设为原地址
	DMA_InitStructure.DMA_BufferSize				= BufferSize / 4;							//1/4缓存大小
	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;					//使能外设地址不自增
	DMA_InitStructure.DMA_MemoryInc				= DMA_MemoryInc_Enable;						//使能存储目标地址自增
	DMA_InitStructure.DMA_PeripheralDataSize		= DMA_PeripheralDataSize_Word;				//外设数据大小为字32位
	DMA_InitStructure.DMA_MemoryDataSize			= DMA_MemoryDataSize_Word;					//存储目标数据大小为字32位
	DMA_InitStructure.DMA_Mode					= DMA_Mode_Normal;							//不循环
	DMA_InitStructure.DMA_Priority				= DMA_Priority_High;						//通道优先级高
	DMA_InitStructure.DMA_M2M					= DMA_M2M_Disable;							//非存储器至存储器模式
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	
	/*!< DMA2 Channel4 enable */
	DMA_Cmd(DMA2_Channel4, ENABLE);
}

/**********************************************************************************************************
 @Function			void SD_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
 @Description			SD_DMA_TxConfig									: 为SDIO发送数据配置DMA2的通道4的请求
 @Input				BufferDST											: 用于装载数据的变量指针
					BufferSize										: 缓冲区大小
 @Return				void
**********************************************************************************************************/
void SD_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* 清除DMA标志位 */
	DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);
	
	/*!< DMA2 Channel4 disable */
	DMA_Cmd(DMA2_Channel4, DISABLE);
	
	/*!< DMA2 Channel4 Config */
	DMA_InitStructure.DMA_PeripheralBaseAddr		= (uint32_t)SDIO_FIFO_ADDRESS;				//外设地址FiFo
	DMA_InitStructure.DMA_MemoryBaseAddr			= (uint32_t)BufferSRC;						//目标地址
	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralDST;						//外设为写入目标
	DMA_InitStructure.DMA_BufferSize				= BufferSize / 4;							//1/4缓存大小
	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;					//使能外设地址不自增
	DMA_InitStructure.DMA_MemoryInc				= DMA_MemoryInc_Enable;						//使能存储目标地址自增
	DMA_InitStructure.DMA_PeripheralDataSize		= DMA_PeripheralDataSize_Word;				//外设数据大小为字32位
	DMA_InitStructure.DMA_MemoryDataSize			= DMA_MemoryDataSize_Word;					//存储目标数据大小为字32位
	DMA_InitStructure.DMA_Mode					= DMA_Mode_Normal;							//不循环
	DMA_InitStructure.DMA_Priority				= DMA_Priority_High;						//通道优先级高
	DMA_InitStructure.DMA_M2M					= DMA_M2M_Disable;							//非存储器至存储器模式
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	
	/*!< DMA2 Channel4 enable */
	DMA_Cmd(DMA2_Channel4, ENABLE);
}

/**********************************************************************************************************
 @Function			SD_Error SD_DeInit(void)
 @Description			SD_DeInit									: 复位SDIO端口
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_DeInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*!< Disable SDIO Clock */
	SDIO_ClockCmd(DISABLE);
	
	/*!< Set Power State to OFF */
	SDIO_SetPowerState(SDIO_PowerState_OFF);
	
	/*!< DeInitializes the SDIO peripheral */
	SDIO_DeInit();
	
	/*!< Disable the SDIO AHB Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, DISABLE);
	
	/*!< Configure SDIO_SDCARD_D0 pin: D0 pin */
	GPIO_InitStructure.GPIO_Pin = SDIO_SDCARD_D0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SDIO_SDCARD_D0_GPIOx, &GPIO_InitStructure);
	
	/*!< Configure SDIO_SDCARD_CLK pin: CLK pin */
	GPIO_InitStructure.GPIO_Pin = SDIO_SDCARD_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SDIO_SDCARD_CLK_GPIOx, &GPIO_InitStructure);
	
	/*!< Configure SDIO_SDCARD_CMD pin: CMD line */
	GPIO_InitStructure.GPIO_Pin = SDIO_SDCARD_CMD_PIN;
	GPIO_Init(SDIO_SDCARD_CMD_GPIOx, &GPIO_InitStructure);
	
	return SD_OK;
}

/**********************************************************************************************************
 @Function			void NVIC_SDIO_Configuration(void)
 @Description			NVIC_SDIO_Configuration						: SDIO配置中断优先级
 @Input				void
 @Return				void
**********************************************************************************************************/
void NVIC_SDIO_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**********************************************************************************************************
 @Function			void GPIO_SDIO_Configuration(void)
 @Description			GPIO_SDIO_Configuration						: SDIO配置GPIO引脚, 启动时钟
 @Input				void
 @Return				void
**********************************************************************************************************/
void GPIO_SDIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_struct;
	
	/*!< GPIOC and GPIOD Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	
	/*!< Configure SDIO_SDCARD_D0 pin: D0 pin */
	GPIO_struct.GPIO_Pin = SDIO_SDCARD_D0_PIN;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SDIO_SDCARD_D0_GPIOx, &GPIO_struct);
	
	/*!< Configure SDIO_SDCARD_CLK pin: CLK pin */
	GPIO_struct.GPIO_Pin = SDIO_SDCARD_CLK_PIN;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SDIO_SDCARD_CLK_GPIOx, &GPIO_struct);
	
	/*!< Configure SDIO_SDCARD_CMD pin: CMD line */
	GPIO_struct.GPIO_Pin = SDIO_SDCARD_CMD_PIN;
	GPIO_Init(SDIO_SDCARD_CMD_GPIOx, &GPIO_struct);
	
	/*!< Enable the SDIO AHB Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
	
	/*!< Enable the DMA2 Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}

/**********************************************************************************************************
 @Function			SD_Error SD_Init(void)
 @Description			SD_Init									: 初始化SD卡
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_Init(void)
{
	/* 重置SD_Error状态 */
	SD_Error errorstatus = SD_OK;
	
	/* SDIO配置中断优先级 */
	NVIC_SDIO_Configuration();
	
	/* SDIO配置GPIO引脚, 启动时钟 */
	GPIO_SDIO_Configuration();
	
	/* SDIO复位所有寄存器 */
	SDIO_DeInit();
	
	/* 上电并进行卡识别流程, 确认卡的操作电压 */
	errorstatus = SD_PowerON();
	
	/* 如果上电, 识别不成功, 返回"响应超时"错误 */
	if (errorstatus != SD_OK) {
		/* CMD Response TimeOut (wait for CMDSENT flag) */
		return (errorstatus);
	}
	
	/* 卡识别成功, 进行卡初始化 */
	errorstatus = SD_InitializeCards();
	
	/* 失败返回 */
	if (errorstatus != SD_OK) {
		/* CMD Response TimeOut (wait for CMDSENT flag) */
		return(errorstatus);
	}
	
	/** 配置SDIO外设
	  * 上电识别, 卡初始化都完成后, 进入数据传输模式, 提高读写速度
	  * 速度若超过24M要进入bypass模式
	  */
	
	/* SDIOCLK = HCLK, SDIO_CK = HCLK/(2 + SDIO_TRANSFER_CLK_DIV) */
	SDIO_struct.SDIO_ClockDiv			= SDIO_TRANSFER_CLK_DIV;
	
	/* 上升沿采集数据 */
	SDIO_struct.SDIO_ClockEdge			= SDIO_ClockEdge_Rising;
	
	/* 不使用bypass模式, 直接使用HCLK进行分频得到SDIO_CK, 时钟若超过24M, 要开启此模式 */
	SDIO_struct.SDIO_ClockBypass			= SDIO_ClockBypass_Disable;
	
	/* 空闲时不关闭时钟电源 */
	SDIO_struct.SDIO_ClockPowerSave		= SDIO_ClockPowerSave_Disable;
	
	/* 配置数据线为1bit */
	SDIO_struct.SDIO_BusWide				= SDIO_BusWide_1b;
	
	/* 失能硬件流控 */
	SDIO_struct.SDIO_HardwareFlowControl	= SDIO_HardwareFlowControl_Disable;
	
	/* 初始化SDIO控制器 */
	SDIO_Init(&SDIO_struct);
	
	if (errorstatus == SD_OK) {
		/* 读取CSD/CID寄存器 */
		errorstatus = SD_GetCardInfo(&SDCardInfo);
	}
	
	if (errorstatus == SD_OK) {
		/* 通过CMD7, RCA选择要操作的卡 */
		errorstatus = SD_SelectDeselect((uint32_t)(SDCardInfo.RCA << 16));
	}
	
	if (errorstatus == SD_OK) {
		/* 配置1bits/4bits模式 */
		errorstatus = SD_EnableWideBusOperation(SDIO_BusWide_1b);
	}
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_PowerON(void)
 @Description			SD_PowerON								: 确保SD卡的工作电压和配置控制时钟
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_PowerON(void)
{
	SD_Error errorstatus = SD_OK;
	uint32_t SDType = SD_STD_CAPACITY;
	uint32_t response = 0, count = 0, validvoltage = 0;
	
	/** 上电初始化
	  * 配置SDIO外设
	  * SDIOCLK = HCLK, SDIO_CK = HCLK/(2 + SDIO_INIT_CLK_DIV)
	  * 初始化时钟不能大于400KHz
	  */
	
	/* HCLK = 8MHz, SDIOCLK = 8MHz, SDIO_CK = HCLK/(18 + 2) = 400 KHz */
	SDIO_struct.SDIO_ClockDiv			= SDIO_INIT_CLK_DIV;
	
	/* 上升沿采集数据 */
	SDIO_struct.SDIO_ClockEdge			= SDIO_ClockEdge_Rising;
	
	/* 不使用bypass模式, 直接使用HCLK进行分频得到SDIO_CK */
	SDIO_struct.SDIO_ClockBypass			= SDIO_ClockBypass_Disable;
	
	/* 空闲时不关闭时钟电源 */
	SDIO_struct.SDIO_ClockPowerSave		= SDIO_ClockPowerSave_Disable;
	
	/* 初始化时配置数据线为1bit */
	SDIO_struct.SDIO_BusWide				= SDIO_BusWide_1b;
	
	/* 失能硬件流控 */
	SDIO_struct.SDIO_HardwareFlowControl	= SDIO_HardwareFlowControl_Disable;
	
	/* 初始化SDIO控制器 */
	SDIO_Init(&SDIO_struct);
	
	/* 开启SDIO外设电源 */
	SDIO_SetPowerState(SDIO_PowerState_ON);
	
	/* 使能 SDIO 时钟 */
	SDIO_ClockCmd(ENABLE);
	
	/* 下面发送一系列命令, 开始卡识别流程 */
	
	/** CMD0: GO_IDLE_STATE(复位所有SD卡进入空闲状态)
	  * 无响应
	  */
	SDIO_Cmdstruct.SDIO_Argument			= 0x00;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_GO_IDLE_STATE;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_No;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	/* 检测是否正确接收到CMD0 */
	errorstatus = CmdError();
	
	/* 命令发送错误, 返回 */
	if (errorstatus != SD_OK) {
		return (errorstatus);
	}
	
	/** CMD8: SEND_IF_COND
	  * Send CMD8 to verify SD card interface operating condition
	  * 
	  * Argument: - [31:12]: Reserved (shall be set to '0')
	  *           - [11:8] : Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
	  *           - [7:0]  : Check Pattern (recommended 0xAA)
	  * 
	  * CMD Response: R7
	  */
	SDIO_Cmdstruct.SDIO_Argument			= SD_CHECK_PATTERN;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SDIO_SEND_IF_COND;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	/* 检测是否接收到命令 */
	errorstatus = CmdResp7Error();
	
	/* 有响应则Card遵循SD协议2.0版本, 无响应为1.x或mmc卡 */
	if (errorstatus == SD_OK) {
		/* SD Card 2.0, 先把类型定义为SDSC卡 */
		CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0;
		
		/* SDType用作ACMD41参数, 用来询问是SDSC卡还是SDHC卡 */
		SDType = SD_HIGH_CAPACITY;
	}
	else {
		/* 发命令CMD55 */
		SDIO_Cmdstruct.SDIO_Argument		= 0x00;
		SDIO_Cmdstruct.SDIO_CmdIndex		= SD_CMD_APP_CMD;
		SDIO_Cmdstruct.SDIO_Response		= SDIO_Response_Short;
		SDIO_Cmdstruct.SDIO_Wait			= SDIO_Wait_No;
		SDIO_Cmdstruct.SDIO_CPSM			= SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_Cmdstruct);
		errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
	}
	
	/** CMD55: 用于检测是SD卡还是MMC卡, 或是不支持的卡
	  * CMD Response: R1
	  */
	SDIO_Cmdstruct.SDIO_Argument			= 0x00;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_APP_CMD;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	/* 是否响应, 未响应是MMC卡或不支持的卡 */
	errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
	
	/** If errorstatus is Command TimeOut, it is a MMC card
	  * If errorstatus is SD_OK it is a SD card: SD card 2.0 (voltage range mismatch) or SD card 1.x
	  */
	if (errorstatus == SD_OK) {
		/** 循环发送SDIO支持的电压范围
		  * SD CARD
		  * Send ACMD41 SD_APP_OP_COND with Argument 0x80100000
		  */
		while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL)) {
			/** 在发送ACMD命令前先要向卡发送CMD55
			  * SEND CMD55 APP_CMD with RCA as 0
			  */
			SDIO_Cmdstruct.SDIO_Argument	= 0x00;
			SDIO_Cmdstruct.SDIO_CmdIndex	= SD_CMD_APP_CMD;
			SDIO_Cmdstruct.SDIO_Response	= SDIO_Response_Short;
			SDIO_Cmdstruct.SDIO_Wait		= SDIO_Wait_No;
			SDIO_Cmdstruct.SDIO_CPSM		= SDIO_CPSM_Enable;
			SDIO_SendCommand(&SDIO_Cmdstruct);
			
			errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
			if (errorstatus != SD_OK) {
				return (errorstatus);
			}
			
			/** ACMD41
			  * 命令参数由支持的电压范围及HCS位组成, HCS位置一来区分卡是SDSC还是SDHC
			  * 0: SDSC
			  * 1: SDHC
			  * 响应: R3,对应的是OCR寄存器
			  */
			SDIO_Cmdstruct.SDIO_Argument	= SD_VOLTAGE_WINDOW_SD | SDType;
			SDIO_Cmdstruct.SDIO_CmdIndex	= SD_CMD_SD_APP_OP_COND;
			SDIO_Cmdstruct.SDIO_Response	= SDIO_Response_Short;
			SDIO_Cmdstruct.SDIO_Wait		= SDIO_Wait_No;
			SDIO_Cmdstruct.SDIO_CPSM		= SDIO_CPSM_Enable;
			SDIO_SendCommand(&SDIO_Cmdstruct);
			
			errorstatus = CmdResp3Error();
			if (errorstatus != SD_OK) {
				return (errorstatus);
			}
			
			/** 若卡需求电压在SDIO的供电电压范围内, 会自动上电并标志pwr_up位
			  * 读取卡寄存器, 卡状态
			  */
			response = SDIO_GetResponse(SDIO_RESP1);
			
			/* 读取卡的ocr寄存器的pwr_up位, 检查是否已在正常电压 */
			validvoltage = (((response >> 31) == 1) ? 1 : 0);
			count++;
		}
		
		/* 循环检测超过一定次数还没上电 */
		if (count >= SD_MAX_VOLT_TRIAL) {
			errorstatus = SD_INVALID_VOLTRANGE;
			return (errorstatus);
		}
		
		/* 检查卡返回信息中的HCS位, 判断OCR中的CCS位, 如果是SDSC卡则不执行下面的语句 */
		if (response &= SD_HIGH_CAPACITY) {
			CardType = SDIO_HIGH_CAPACITY_SD_CARD;
		}
	}
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_PowerOFF(void)
 @Description			SD_PowerOFF								: 关闭SDIO输出信号
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_PowerOFF(void)
{
	SD_Error errorstatus = SD_OK;
	
	/* Set Power State to OFF */
	SDIO_SetPowerState(SDIO_PowerState_OFF);
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_InitializeCards(void)
 @Description			SD_InitializeCards							: 初始化所有的卡或单卡进入就绪状态
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_InitializeCards(void)
{
	SD_Error errorstatus = SD_OK;
	uint16_t rca = 0x01;
	
	if (SDIO_GetPowerState() == SDIO_PowerState_OFF) {
		errorstatus = SD_REQUEST_NOT_APPLICABLE;
		return (errorstatus);
	}
	
	/* 判断卡的类型 */
	if (SDIO_SECURE_DIGITAL_IO_CARD != CardType) {
		/** Send CMD2 ALL_SEND_CID
		  * CMD Response: R2 CID Register
		  */
		SDIO_Cmdstruct.SDIO_Argument		= 0x00;
		SDIO_Cmdstruct.SDIO_CmdIndex		= SD_CMD_ALL_SEND_CID;
		SDIO_Cmdstruct.SDIO_Response		= SDIO_Response_Long;
		SDIO_Cmdstruct.SDIO_Wait			= SDIO_Wait_No;
		SDIO_Cmdstruct.SDIO_CPSM			= SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_Cmdstruct);
		
		errorstatus = CmdResp2Error();
		if (errorstatus != SD_OK) {
			return (errorstatus);
		}
		
		/* 将返回的CID信息存储起来 */
		CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
		CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
		CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
		CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
	}
	
	if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||
	    (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
		/** Send CMD3 SET_REL_ADDR with argument 0
		  * SD Card publishes its RCA
		  * CMD Response: R6 RCA Register
		  */
		SDIO_Cmdstruct.SDIO_Argument		= 0x00;
		SDIO_Cmdstruct.SDIO_CmdIndex		= SD_CMD_SET_REL_ADDR;
		SDIO_Cmdstruct.SDIO_Response		= SDIO_Response_Short;
		SDIO_Cmdstruct.SDIO_Wait			= SDIO_Wait_No;
		SDIO_Cmdstruct.SDIO_CPSM			= SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_Cmdstruct);
		
		/* 把接收到的卡相对地址存起来 */
		errorstatus = CmdResp6Error(SD_CMD_SET_REL_ADDR, &rca);
		if (errorstatus != SD_OK) {
			return (errorstatus);
		}
	}
	
	if (SDIO_SECURE_DIGITAL_IO_CARD != CardType) {
		RCA = rca;
		
		/** Send CMD9 SEND_CSD with argument as card's RCA
		  * CMD Response: R2 CSD Register
		  */
		SDIO_Cmdstruct.SDIO_Argument		= (uint32_t)(rca << 16);
		SDIO_Cmdstruct.SDIO_CmdIndex		= SD_CMD_SEND_CSD;
		SDIO_Cmdstruct.SDIO_Response		= SDIO_Response_Long;
		SDIO_Cmdstruct.SDIO_Wait			= SDIO_Wait_No;
		SDIO_Cmdstruct.SDIO_CPSM			= SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_Cmdstruct);
		
		errorstatus = CmdResp2Error();
		if (errorstatus != SD_OK) {
			return(errorstatus);
		}
		
		/* 将返回的CSD信息存储起来 */
		CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
		CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
		CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
		CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
	}
	
	/* 全部卡初始化成功 */
	errorstatus = SD_OK;
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			static SD_Error CmdError(void)
 @Description			CmdError									: 对CMD0命令检查
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error CmdError(void)
{
	SD_Error errorstatus = SD_OK;
	uint32_t timeout = SDIO_CMD0TIMEOUT;
	
	/* 检查命令是否已送达 */
	while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET)) {
		timeout--;
	}
	
	if (timeout == 0) {
		errorstatus = SD_CMD_RSP_TIMEOUT;
		return (errorstatus);
	}
	
	/* Clear all the static flags */
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			static SD_Error CmdResp7Error(void)
 @Description			CmdResp7Error								: 对响应类型为R7的命令进行检查
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error CmdResp7Error(void)
{
	SD_Error errorstatus = SD_OK;
	uint32_t status;
	uint32_t timeout = SDIO_CMD0TIMEOUT;
	
	status = SDIO->STA;
	
	/* Command response received (CRC check failed) : Command response received (CRC check passed) : Command response timeout */
	while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) && (timeout > 0)) {
		timeout--;
		status = SDIO->STA;
	}
	
	if ((timeout == 0) || (status & SDIO_FLAG_CTIMEOUT)) {
		/*!< Card is not V2.0 complient or card does not support the set voltage range */
		errorstatus = SD_CMD_RSP_TIMEOUT;
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
		return (errorstatus);
	}
	
	if (status & SDIO_FLAG_CMDREND) {
		/*!< Card is SD V2.0 compliant */
		errorstatus = SD_OK;
		SDIO_ClearFlag(SDIO_FLAG_CMDREND);
		return (errorstatus);
	}
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			static SD_Error CmdResp1Error(uint8_t cmd)
 @Description			CmdResp1Error								: 对响应类型为R1的命令进行检查
 @Input				cmd
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error CmdResp1Error(uint8_t cmd)
{
	/* 不是这些状态就等待 */
	while (!(SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)));
	
	/* 清中断标志 */
	SDIO->ICR = SDIO_STATIC_FLAGS;
	
	/* 判断是否在供电范围 */
	return (SD_Error)(SDIO->RESP1 & SD_OCR_ERRORBITS);
}

/**********************************************************************************************************
 @Function			static SD_Error CmdResp3Error(void)
 @Description			CmdResp3Error								: 对响应类型为R3的命令进行检查
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error CmdResp3Error(void)
{
	SD_Error errorstatus = SD_OK;
	uint32_t status;
	
	status = SDIO->STA;
	
	while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT))) {
		status = SDIO->STA;
	}
	
	if (status & SDIO_FLAG_CTIMEOUT) {
		errorstatus = SD_CMD_RSP_TIMEOUT;
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
		return (errorstatus);
	}
	
	/* Clear all the static flags */
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			static SD_Error CmdResp2Error(void)
 @Description			CmdResp2Error								: 对响应类型为R2的命令进行检查
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error CmdResp2Error(void)
{
	SD_Error errorstatus = SD_OK;
	uint32_t status;
	
	status = SDIO->STA;
	
	while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND))) {
		status = SDIO->STA;
	}
	
	if (status & SDIO_FLAG_CTIMEOUT) {
		errorstatus = SD_CMD_RSP_TIMEOUT;
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
		return (errorstatus);
	}
	else if (status & SDIO_FLAG_CCRCFAIL) {
		errorstatus = SD_CMD_CRC_FAIL;
		SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
		return (errorstatus);
	}
	
	/* Clear all the static flags */
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			static SD_Error CmdResp6Error(uint8_t cmd, uint16_t *prca)
 @Description			CmdResp6Error								: 对响应类型为R6的命令进行检查
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error CmdResp6Error(uint8_t cmd, uint16_t *prca)
{
	SD_Error errorstatus = SD_OK;
	uint32_t status;
	uint32_t response_r1;
	
	status = SDIO->STA;
	
	while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND))) {
		status = SDIO->STA;
	}
	
	if (status & SDIO_FLAG_CTIMEOUT) {
		errorstatus = SD_CMD_RSP_TIMEOUT;
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
		return (errorstatus);
	}
	else if (status & SDIO_FLAG_CCRCFAIL) {
		errorstatus = SD_CMD_CRC_FAIL;
		SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
		return (errorstatus);
	}
	
	/*!< Check response received is of desired command */
	if (SDIO_GetCommandResponse() != cmd) {
		errorstatus = SD_ILLEGAL_CMD;
		return (errorstatus);
	}
	
	/*!< Clear all the static flags */
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
	/*!< We have received response, retrieve it */
	response_r1 = SDIO_GetResponse(SDIO_RESP1);
	
	/* 以下状态全为0表示成功接收到card返回rca */
	if (SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED))) {
		*prca = (uint16_t) (response_r1 >> 16);
		return (errorstatus);
	}
	
	if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR) {
		return (SD_GENERAL_UNKNOWN_ERROR);
	}
	
	if (response_r1 & SD_R6_ILLEGAL_CMD) {
		return (SD_ILLEGAL_CMD);
	}
	
	if (response_r1 & SD_R6_COM_CRC_FAILED) {
		return (SD_COM_CRC_FAILED);
	}
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
 @Description			SD_GetCardInfo								: 获取SD卡的具体信息
 @Input				cardinfo									: 指向SD_CardInfo结构体的指针
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
	SD_Error errorstatus = SD_OK;
	uint8_t tmp = 0;
	
	cardinfo->CardType			= (uint8_t)CardType;
	cardinfo->RCA				= (uint16_t)RCA;
	
	/*!< Byte 0 */
	tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
	cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
	cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
	cardinfo->SD_csd.Reserved1 = tmp & 0x03;
	
	/*!< Byte 1 */
	tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
	cardinfo->SD_csd.TAAC = tmp;
	
	/*!< Byte 2 */
	tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
	cardinfo->SD_csd.NSAC = tmp;
	
	/*!< Byte 3 */
	tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
	cardinfo->SD_csd.MaxBusClkFrec = tmp;
	
	/*!< Byte 4 */
	tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
	cardinfo->SD_csd.CardComdClasses = tmp << 4;
	
	/*!< Byte 5 */
	tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
	cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
	cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;
	
	/*!< Byte 6 */
	tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
	cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
	cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
	cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
	cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
	cardinfo->SD_csd.Reserved2 = 0;
	
	if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0)) {
		
		cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;
		
		/*!< Byte 7 */
		tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
		cardinfo->SD_csd.DeviceSize |= (tmp) << 2;
		
		/*!< Byte 8 */
		tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
		cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;
		
		cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
		cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);
		
		/*!< Byte 9 */
		tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
		cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
		cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
		cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
		
		/*!< Byte 10 */
		tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
		cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
		
		cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1);
		cardinfo->CardCapacity *= (uint64_t)(1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
		cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
		cardinfo->CardCapacity *= (uint64_t)cardinfo->CardBlockSize;
	}
	else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
		
		/*!< Byte 7 */
		tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
		cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;
		
		/*!< Byte 8 */
		tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
		
		cardinfo->SD_csd.DeviceSize |= (tmp << 8);
		
		/*!< Byte 9 */
		tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
		
		cardinfo->SD_csd.DeviceSize |= (tmp);
		
		/*!< Byte 10 */
		tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
		
		cardinfo->CardCapacity = (uint64_t)(cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
		cardinfo->CardBlockSize = 512;
	}
	
	cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
	cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;
	
	/*!< Byte 11 */
	tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
	cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
	cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);
	
	/*!< Byte 12 */
	tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
	cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
	cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
	cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
	cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;
	
	/*!< Byte 13 */
	tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
	cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
	cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
	cardinfo->SD_csd.Reserved3 = 0;
	cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);
	
	/*!< Byte 14 */
	tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
	cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
	cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
	cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
	cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
	cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
	cardinfo->SD_csd.ECC = (tmp & 0x03);
	
	/*!< Byte 15 */
	tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
	cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
	cardinfo->SD_csd.Reserved4 = 1;
	
	
	/*!< Byte 0 */
	tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
	cardinfo->SD_cid.ManufacturerID = tmp;
	
	/*!< Byte 1 */
	tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
	cardinfo->SD_cid.OEM_AppliID = tmp << 8;
	
	/*!< Byte 2 */
	tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
	cardinfo->SD_cid.OEM_AppliID |= tmp;
	
	/*!< Byte 3 */
	tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
	cardinfo->SD_cid.ProdName1 = tmp << 24;
	
	/*!< Byte 4 */
	tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
	cardinfo->SD_cid.ProdName1 |= tmp << 16;
	
	/*!< Byte 5 */
	tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
	cardinfo->SD_cid.ProdName1 |= tmp << 8;
	
	/*!< Byte 6 */
	tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
	cardinfo->SD_cid.ProdName1 |= tmp;
	
	/*!< Byte 7 */
	tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
	cardinfo->SD_cid.ProdName2 = tmp;
	
	/*!< Byte 8 */
	tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
	cardinfo->SD_cid.ProdRev = tmp;
	
	/*!< Byte 9 */
	tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
	cardinfo->SD_cid.ProdSN = tmp << 24;
	
	/*!< Byte 10 */
	tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
	cardinfo->SD_cid.ProdSN |= tmp << 16;
	
	/*!< Byte 11 */
	tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
	cardinfo->SD_cid.ProdSN |= tmp << 8;
	
	/*!< Byte 12 */
	tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
	cardinfo->SD_cid.ProdSN |= tmp;
	
	/*!< Byte 13 */
	tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
	cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
	cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;
	
	/*!< Byte 14 */
	tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
	cardinfo->SD_cid.ManufactDate |= tmp;
	
	/*!< Byte 15 */
	tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
	cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
	cardinfo->SD_cid.Reserved2 = 1;
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_GetCardStatus(SD_CardStatus *cardstatus)
 @Description			SD_GetCardStatus							: 获取SD卡的状态信息
 @Input				cardstatus								: 指向SD_CardStatus结构体的指针
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_GetCardStatus(SD_CardStatus *cardstatus)
{
	SD_Error errorstatus = SD_OK;
	uint8_t tmp = 0;
	
	errorstatus = SD_SendSDStatus((uint32_t *)SDSTATUS_Tab);
	
	if (errorstatus  != SD_OK) {
		return(errorstatus);
	}
	
	/*!< Byte 0 */
	tmp = (uint8_t)((SDSTATUS_Tab[0] & 0xC0) >> 6);
	cardstatus->DAT_BUS_WIDTH = tmp;
	
	/*!< Byte 0 */
	tmp = (uint8_t)((SDSTATUS_Tab[0] & 0x20) >> 5);
	cardstatus->SECURED_MODE = tmp;
	
	/*!< Byte 2 */
	tmp = (uint8_t)((SDSTATUS_Tab[2] & 0xFF));
	cardstatus->SD_CARD_TYPE = tmp << 8;
	
	/*!< Byte 3 */
	tmp = (uint8_t)((SDSTATUS_Tab[3] & 0xFF));
	cardstatus->SD_CARD_TYPE |= tmp;
	
	/*!< Byte 4 */
	tmp = (uint8_t)(SDSTATUS_Tab[4] & 0xFF);
	cardstatus->SIZE_OF_PROTECTED_AREA = tmp << 24;
	
	/*!< Byte 5 */
	tmp = (uint8_t)(SDSTATUS_Tab[5] & 0xFF);
	cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 16;
	
	/*!< Byte 6 */
	tmp = (uint8_t)(SDSTATUS_Tab[6] & 0xFF);
	cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 8;
	
	/*!< Byte 7 */
	tmp = (uint8_t)(SDSTATUS_Tab[7] & 0xFF);
	cardstatus->SIZE_OF_PROTECTED_AREA |= tmp;
	
	/*!< Byte 8 */
	tmp = (uint8_t)((SDSTATUS_Tab[8] & 0xFF));
	cardstatus->SPEED_CLASS = tmp;
	
	/*!< Byte 9 */
	tmp = (uint8_t)((SDSTATUS_Tab[9] & 0xFF));
	cardstatus->PERFORMANCE_MOVE = tmp;
	
	/*!< Byte 10 */
	tmp = (uint8_t)((SDSTATUS_Tab[10] & 0xF0) >> 4);
	cardstatus->AU_SIZE = tmp;
	
	/*!< Byte 11 */
	tmp = (uint8_t)(SDSTATUS_Tab[11] & 0xFF);
	cardstatus->ERASE_SIZE = tmp << 8;
	
	/*!< Byte 12 */
	tmp = (uint8_t)(SDSTATUS_Tab[12] & 0xFF);
	cardstatus->ERASE_SIZE |= tmp;
	
	/*!< Byte 13 */
	tmp = (uint8_t)((SDSTATUS_Tab[13] & 0xFC) >> 2);
	cardstatus->ERASE_TIMEOUT = tmp;
	
	/*!< Byte 13 */
	tmp = (uint8_t)((SDSTATUS_Tab[13] & 0x3));
	cardstatus->ERASE_OFFSET = tmp;
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_SelectDeselect(uint32_t addr)
 @Description			SD_SelectDeselect							: 选择卡相对地址为addr的卡, 取消选择其它卡, 如果addr=0, 取消选择所有卡
 @Input				addr										: 选择卡的地址
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_SelectDeselect(uint32_t addr)
{
	SD_Error errorstatus = SD_OK;
	
	/*!< Send CMD7 SDIO_SEL_DESEL_CARD */
	SDIO_Cmdstruct.SDIO_Argument			= addr;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SEL_DESEL_CARD;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SEL_DESEL_CARD);
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_ProcessIRQSrc(void)
 @Description			SD_ProcessIRQSrc							: 数据传输结束中断
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_ProcessIRQSrc(void)
{
	if (StopCondition == 1) {
		SDIO->ARG = 0x00;
		SDIO->CMD = 0x44C;
		
		TransferError = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
	}
	else {
		TransferError = SD_OK;
	}
	
	SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
	SDIO_ITConfig(SDIO_IT_DATAEND, DISABLE);
	
	TransferEnd = 1;
	
	return (TransferError);
}

extern u8  Sdcard_Systime_Time_Meter;
extern u32 Sdcard_Runtime_Time_MS;
extern u32 Sdcard_Flgtime_Time_MS;

/**********************************************************************************************************
 @Function			SD_Error SD_WaitReadOperation(void)
 @Description			SD_WaitReadOperation						: This function waits until the SDIO DMA data transfer is finished
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_WaitReadOperation(void)
{
	SD_Error errorstatus = SD_OK;
	
	/* 等待DMA传输结束 */
	Sdcard_Systime_Time_Meter = 1;
	Sdcard_Runtime_Time_MS    = 0;
	while ((SD_DMAEndOfTransferStatus() == RESET) && (TransferEnd == 0) && (TransferError == SD_OK)) {
		if (Sdcard_Runtime_Time_MS > Sdcard_Flgtime_Time_MS) {
#ifdef SOFTWAREIWDG
			IWDG_ReloadCounter();
#endif
#ifdef HARDWAREIWDG
			MAX823_IWDGReloadCounter();
#endif
			Sdcard_Systime_Time_Meter = 0;
			return(TransferError);
		}
	}
	Sdcard_Systime_Time_Meter = 0;
	
	if (TransferError != SD_OK) {
		return(TransferError);
	}
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_WaitWriteOperation(void)
 @Description			SD_WaitWriteOperation						: This function waits until the SDIO DMA data transfer is finished
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_WaitWriteOperation(void)
{
	SD_Error errorstatus = SD_OK;
	
	/* 等待DMA传输结束 */
	Sdcard_Systime_Time_Meter = 1;
	Sdcard_Runtime_Time_MS    = 0;
	while ((SD_DMAEndOfTransferStatus() == RESET) && (TransferEnd == 0) && (TransferError == SD_OK)) {
		if (Sdcard_Runtime_Time_MS > Sdcard_Flgtime_Time_MS) {
#ifdef SOFTWAREIWDG
			IWDG_ReloadCounter();
#endif
#ifdef HARDWAREIWDG
			MAX823_IWDGReloadCounter();
#endif
			Sdcard_Systime_Time_Meter = 0;
			return(TransferError);
		}
	}
	Sdcard_Systime_Time_Meter = 0;
	
	if (TransferError != SD_OK) {
		return(TransferError);
	}
	
	/*!< Clear all the static flags */
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_EnableWideBusOperation(uint32_t WideMode)
 @Description			SD_EnableWideBusOperation		: 配置SD卡的数据宽度
 @Input				-WideMode						: 指定SD卡的数据线宽
					@arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
					@arg SDIO_BusWide_4b: 4-bit data transfer
					@arg SDIO_BusWide_1b: 1-bit data transfer (默认)
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_EnableWideBusOperation(uint32_t WideMode)
{
	SD_Error errorstatus = SD_OK;
	
	/*!< MMC Card doesn't support this feature */
	if (SDIO_MULTIMEDIA_CARD == CardType) {
		errorstatus = SD_UNSUPPORTED_FEATURE;
		return(errorstatus);
	}
	else if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
		/* 2.0 SD卡不支持8bits */
		if (SDIO_BusWide_8b == WideMode) {
			errorstatus = SD_UNSUPPORTED_FEATURE;
			return(errorstatus);
		}
		else if (SDIO_BusWide_4b == WideMode) {
			/* 使用ACMD6设置总线宽度, 设置卡的传输方式 */
			errorstatus = SDEnWideBus(ENABLE);
			
			if (SD_OK == errorstatus) {
				/*!< Configure the SDIO peripheral */
				SDIO_struct.SDIO_ClockDiv			= SDIO_TRANSFER_CLK_DIV;
				SDIO_struct.SDIO_ClockEdge			= SDIO_ClockEdge_Rising;
				SDIO_struct.SDIO_ClockBypass			= SDIO_ClockBypass_Disable;
				SDIO_struct.SDIO_ClockPowerSave		= SDIO_ClockPowerSave_Disable;
				SDIO_struct.SDIO_BusWide				= SDIO_BusWide_4b;
				SDIO_struct.SDIO_HardwareFlowControl	= SDIO_HardwareFlowControl_Disable;
				SDIO_Init(&SDIO_struct);
			}
		}
		else {
			/* 使用ACMD6设置总线宽度, 设置卡的传输方式 */
			errorstatus = SDEnWideBus(DISABLE);
			
			if (SD_OK == errorstatus) {
				/*!< Configure the SDIO peripheral */
				SDIO_struct.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;
				SDIO_struct.SDIO_ClockEdge			= SDIO_ClockEdge_Rising;
				SDIO_struct.SDIO_ClockBypass			= SDIO_ClockBypass_Disable;
				SDIO_struct.SDIO_ClockPowerSave		= SDIO_ClockPowerSave_Disable;
				SDIO_struct.SDIO_BusWide				= SDIO_BusWide_1b;
				SDIO_struct.SDIO_HardwareFlowControl	= SDIO_HardwareFlowControl_Disable;
				SDIO_Init(&SDIO_struct);
			}
		}
	}
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_ReadBlock(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize)
 @Description			SD_ReadBlock	: This operation should be followed by two functions to check if the DMA Controller and SD Card status.
 @Input				readbuff		: pointer to the buffer that will contain the received data
					ReadAddr		: Address from where data are to be read
					BlockSize		: the SD card Data block size. The Block size should be 512
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_ReadBlock(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize)
{
	SD_Error errorstatus = SD_OK;
#if defined (SD_POLLING_MODE)
	uint32_t count = 0, *tempbuff = (uint32_t *)readbuff;
#endif
	
	TransferError	= SD_OK;
	TransferEnd	= 0;
	StopCondition	= 0;
	
	SDIO->DCTRL = 0x00;
	
	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
		BlockSize = 512;
		ReadAddr /= 512;
	}
	
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t) BlockSize;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SET_BLOCKLEN;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
	if (SD_OK != errorstatus) {
		return(errorstatus);
	}
	
	SDIO_Datastruct.SDIO_DataTimeOut		= SD_DATATIMEOUT;
	SDIO_Datastruct.SDIO_DataLength		= BlockSize;
	SDIO_Datastruct.SDIO_DataBlockSize		= (uint32_t) 9 << 4;
	SDIO_Datastruct.SDIO_TransferDir		= SDIO_TransferDir_ToSDIO;
	SDIO_Datastruct.SDIO_TransferMode		= SDIO_TransferMode_Block;
	SDIO_Datastruct.SDIO_DPSM			= SDIO_DPSM_Enable;
	SDIO_DataConfig(&SDIO_Datastruct);
	
	/*!< Send CMD17 READ_SINGLE_BLOCK */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t)ReadAddr;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_READ_SINGLE_BLOCK;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_READ_SINGLE_BLOCK);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
#if defined (SD_POLLING_MODE)
	/*!< In case of single block transfer, no need of stop transfer at all.*/
	while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
		if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET) {
			for (count = 0; count < 8; count++) {
				*(tempbuff + count) = SDIO_ReadData();
			}
			tempbuff += 8;
		}
	}
	
	if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
		errorstatus = SD_DATA_TIMEOUT;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
		errorstatus = SD_DATA_CRC_FAIL;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
		errorstatus = SD_RX_OVERRUN;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_STBITERR);
		errorstatus = SD_START_BIT_ERR;
		return (errorstatus);
	}
	
	while (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) {
		*tempbuff = SDIO_ReadData();
		tempbuff++;
	}
	
	/*!< Clear all the static flags */
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
#elif defined (SD_DMA_MODE)
	SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
	SDIO_DMACmd(ENABLE);
	SD_DMA_RxConfig((uint32_t *)readbuff, BlockSize);
#endif
	
	return(errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_ReadMultiBlocks(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
 @Description			SD_ReadMultiBlocks	: This operation should be followed by two functions to check if the DMA Controller and SD Card status.
 @Input				readbuff			: pointer to the buffer that will contain the received data
					ReadAddr			: Address from where data are to be read
					BlockSize			: the SD card Data block size. The Block size should be 512
					NumberOfBlocks		: number of blocks to be read
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_ReadMultiBlocks(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	
	TransferError	= SD_OK;
	TransferEnd	= 0;
	StopCondition	= 1;
	
	SDIO->DCTRL = 0x00;
	
	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
		BlockSize = 512;
		ReadAddr /= 512;
	}
	
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t) BlockSize;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SET_BLOCKLEN;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
	if (SD_OK != errorstatus) {
		return(errorstatus);
	}
	
	SDIO_Datastruct.SDIO_DataTimeOut		= SD_DATATIMEOUT;
	SDIO_Datastruct.SDIO_DataLength		= NumberOfBlocks * BlockSize;
	SDIO_Datastruct.SDIO_DataBlockSize		= (uint32_t) 9 << 4;
	SDIO_Datastruct.SDIO_TransferDir		= SDIO_TransferDir_ToSDIO;
	SDIO_Datastruct.SDIO_TransferMode		= SDIO_TransferMode_Block;
	SDIO_Datastruct.SDIO_DPSM			= SDIO_DPSM_Enable;
	SDIO_DataConfig(&SDIO_Datastruct);
	
	/*!< Send CMD18 READ_MULT_BLOCK with argument data address */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t)ReadAddr;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_READ_MULT_BLOCK;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_READ_MULT_BLOCK);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
	SDIO_DMACmd(ENABLE);
	SD_DMA_RxConfig((uint32_t *)readbuff, (NumberOfBlocks * BlockSize));
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_WriteBlock(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize)
 @Description			SD_WriteBlock	: This operation should be followed by two functions to check if the DMA Controller and SD Card status.
 @Input				writebuff		: pointer to the buffer that contain the data to be transferred
					WriteAddr		: Address from where data are to be write
					BlockSize		: the SD card Data block size. The Block size should be 512
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_WriteBlock(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize)
{
	SD_Error errorstatus = SD_OK;
	
#if defined (SD_POLLING_MODE)
	uint32_t bytestransferred = 0, count = 0, restwords = 0;
	uint32_t *tempbuff = (uint32_t *)writebuff;
#endif
	
	TransferError	= SD_OK;
	TransferEnd	= 0;
	StopCondition	= 0;
	
	SDIO->DCTRL = 0x00;
	
	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
		BlockSize = 512;
		WriteAddr /= 512;
	}
	
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t) BlockSize;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SET_BLOCKLEN;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
	if (SD_OK != errorstatus) {
		return (errorstatus);
	}
	
	/*!< Send CMD24 WRITE_SINGLE_BLOCK */
	SDIO_Cmdstruct.SDIO_Argument			= WriteAddr;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_WRITE_SINGLE_BLOCK;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_WRITE_SINGLE_BLOCK);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	/* 配置SDIO写数据寄存器 */
	SDIO_Datastruct.SDIO_DataTimeOut		= SD_DATATIMEOUT;
	SDIO_Datastruct.SDIO_DataLength		= BlockSize;
	SDIO_Datastruct.SDIO_DataBlockSize		= (uint32_t) 9 << 4;
	SDIO_Datastruct.SDIO_TransferDir		= SDIO_TransferDir_ToCard;
	SDIO_Datastruct.SDIO_TransferMode		= SDIO_TransferMode_Block;
	SDIO_Datastruct.SDIO_DPSM			= SDIO_DPSM_Enable;
	SDIO_DataConfig(&SDIO_Datastruct);
	
#if defined (SD_POLLING_MODE)
	/*!< In case of single data block transfer no need of stop command at all */
	while (!(SDIO->STA & (SDIO_FLAG_DBCKEND | SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_STBITERR))) {
		if (SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET) {
			if ((512 - bytestransferred) < 32) {
				restwords = ((512 - bytestransferred) % 4 == 0) ? ((512 - bytestransferred) / 4) : (( 512 -  bytestransferred) / 4 + 1);
				for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4) {
					SDIO_WriteData(*tempbuff);
				}
			}
			else {
				for (count = 0; count < 8; count++) {
					SDIO_WriteData(*(tempbuff + count));
				}
				tempbuff += 8;
				bytestransferred += 32;
			}
		}
	}
	
	if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
		errorstatus = SD_DATA_TIMEOUT;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
		errorstatus = SD_DATA_CRC_FAIL;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_TXUNDERR);
		errorstatus = SD_TX_UNDERRUN;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_STBITERR);
		errorstatus = SD_START_BIT_ERR;
		return (errorstatus);
	}
	
#elif defined (SD_DMA_MODE)
	SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
	SD_DMA_TxConfig((uint32_t *)writebuff, BlockSize);
	SDIO_DMACmd(ENABLE);
#endif
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_WriteMultiBlocks(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
 @Description			SD_WriteMultiBlocks	: This operation should be followed by two functions to check if the DMA Controller and SD Card status.
 @Input				writebuff			: pointer to the buffer that will contain the received data
					WriteAddr			: Address from where data are to be write
					BlockSize			: the SD card Data block size. The Block size should be 512
					NumberOfBlocks		: number of blocks to be read
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_WriteMultiBlocks(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	__IO uint32_t count = 0;
	
	TransferError	= SD_OK;
	TransferEnd	= 0;
	StopCondition	= 1;
	
	SDIO->DCTRL = 0x00;
	
	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
		BlockSize = 512;
		WriteAddr /= 512;
	}
	
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t) BlockSize;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SET_BLOCKLEN;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
	if (SD_OK != errorstatus) {
		return (errorstatus);
	}
	
	/*!< To improve performance */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t) (RCA << 16);
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_APP_CMD;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
	if (errorstatus != SD_OK) {
		return (errorstatus);
	}
	
	/*!< To improve performance */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t)NumberOfBlocks;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SET_BLOCK_COUNT;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SET_BLOCK_COUNT);
	if (errorstatus != SD_OK) {
		return (errorstatus);
	}
	
	/*!< Send CMD25 WRITE_MULT_BLOCK with argument data address */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t)WriteAddr;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_WRITE_MULT_BLOCK;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_WRITE_MULT_BLOCK);
	if (SD_OK != errorstatus) {
		return (errorstatus);
	}
	
	SDIO_Datastruct.SDIO_DataTimeOut		= SD_DATATIMEOUT;
	SDIO_Datastruct.SDIO_DataLength		= NumberOfBlocks * BlockSize;
	SDIO_Datastruct.SDIO_DataBlockSize		= (uint32_t) 9 << 4;
	SDIO_Datastruct.SDIO_TransferDir		= SDIO_TransferDir_ToCard;
	SDIO_Datastruct.SDIO_TransferMode		= SDIO_TransferMode_Block;
	SDIO_Datastruct.SDIO_DPSM			= SDIO_DPSM_Enable;
	SDIO_DataConfig(&SDIO_Datastruct);
	
	SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
	SDIO_DMACmd(ENABLE);
	SD_DMA_TxConfig((uint32_t *)writebuff, (NumberOfBlocks * BlockSize));
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_StopTransfer(void)
 @Description			SD_StopTransfer							: Aborts an ongoing data transfer
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_StopTransfer(void)
{
	SD_Error errorstatus = SD_OK;
	
	/*!< Send CMD12 STOP_TRANSMISSION */
	SDIO->ARG = 0x0;
	SDIO->CMD = 0x44C;
	
	errorstatus = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_Erase(uint64_t startaddr, uint64_t endaddr)
 @Description			SD_Erase									: Allows to erase memory area specified for the given card
 @Input				startaddr									: the start address.
					endaddr									: the end address.
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_Erase(uint64_t startaddr, uint64_t endaddr)
{
	SD_Error errorstatus = SD_OK;
	__IO uint32_t maxdelay = 0;
	uint32_t delay = 0;
	uint8_t  cardstate = 0;
	
	/*!< Check if the card coomnd class supports erase command */
	if (((CSD_Tab[1] >> 20) & SD_CCCC_ERASE) == 0) {
		errorstatus = SD_REQUEST_NOT_APPLICABLE;
		return (errorstatus);
	}
	
	maxdelay = 120000 / ((SDIO->CLKCR & 0xFF) + 2);
	
	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED) {
		errorstatus = SD_LOCK_UNLOCK_FAILED;
		return (errorstatus);
	}
	
	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
		startaddr /= 512;
		endaddr /= 512;
	}
	
	/*!< According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
	if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
		/*!< Send CMD32 SD_ERASE_GRP_START with argument as addr */
		SDIO_Cmdstruct.SDIO_Argument		= startaddr;
		SDIO_Cmdstruct.SDIO_CmdIndex		= SD_CMD_SD_ERASE_GRP_START;
		SDIO_Cmdstruct.SDIO_Response		= SDIO_Response_Short;
		SDIO_Cmdstruct.SDIO_Wait			= SDIO_Wait_No;
		SDIO_Cmdstruct.SDIO_CPSM			= SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_Cmdstruct);
		
		errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_START);
		if (errorstatus != SD_OK) {
			return (errorstatus);
		}
		
		/*!< Send CMD33 SD_ERASE_GRP_END with argument as addr */
		SDIO_Cmdstruct.SDIO_Argument		= endaddr;
		SDIO_Cmdstruct.SDIO_CmdIndex		= SD_CMD_SD_ERASE_GRP_END;
		SDIO_Cmdstruct.SDIO_Response		= SDIO_Response_Short;
		SDIO_Cmdstruct.SDIO_Wait			= SDIO_Wait_No;
		SDIO_Cmdstruct.SDIO_CPSM			= SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_Cmdstruct);
		
		errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_END);
		if (errorstatus != SD_OK) {
			return (errorstatus);
		}
	}
	
	/*!< Send CMD38 ERASE */
	SDIO_Cmdstruct.SDIO_Argument			= 0;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_ERASE;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_ERASE);
	if (errorstatus != SD_OK) {
		return (errorstatus);
	}
	
	for (delay = 0; delay < maxdelay; delay++);
	
	/*!< Wait till the card is in programming state */
	errorstatus = IsCardProgramming(&cardstate);
	
	while ((errorstatus == SD_OK) && ((SD_CARD_PROGRAMMING == cardstate) || (SD_CARD_RECEIVING == cardstate))) {
		errorstatus = IsCardProgramming(&cardstate);
	}
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_SendStatus(uint32_t *pcardstatus)
 @Description			SD_SendStatus								: Returns the current card's status
 @Input				pcardstatus								: pointer to the buffer that will contain the SD card status (Card Status register)
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_SendStatus(uint32_t *pcardstatus)
{
	SD_Error errorstatus = SD_OK;
	
	SDIO->ARG = (uint32_t) RCA << 16;
	SDIO->CMD = 0x44D;
	
	errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);
	
	if (errorstatus != SD_OK) {
		return (errorstatus);
	}
	
	*pcardstatus = SDIO->RESP1;
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			SD_Error SD_SendSDStatus(uint32_t *psdstatus)
 @Description			SD_SendSDStatus							: Returns the current SD card's status
 @Input				psdstatus									: pointer to the buffer that will contain the SD card status (Card Status register)
 @Return				SD_Error
**********************************************************************************************************/
SD_Error SD_SendSDStatus(uint32_t *psdstatus)
{
	SD_Error errorstatus = SD_OK;
	uint32_t count = 0;
	
	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED) {
		errorstatus = SD_LOCK_UNLOCK_FAILED;
		return(errorstatus);
	}
	
	/*!< Set block size for card if it is not equal to current block size for card. */
	SDIO_Cmdstruct.SDIO_Argument			= 64;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SET_BLOCKLEN;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	/*!< CMD55 */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t) RCA << 16;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_APP_CMD;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	SDIO_Datastruct.SDIO_DataTimeOut		= SD_DATATIMEOUT;
	SDIO_Datastruct.SDIO_DataLength		= 64;
	SDIO_Datastruct.SDIO_DataBlockSize		= SDIO_DataBlockSize_64b;
	SDIO_Datastruct.SDIO_TransferDir		= SDIO_TransferDir_ToSDIO;
	SDIO_Datastruct.SDIO_TransferMode		= SDIO_TransferMode_Block;
	SDIO_Datastruct.SDIO_DPSM			= SDIO_DPSM_Enable;
	SDIO_DataConfig(&SDIO_Datastruct);
	
	/*!< Send ACMD13 SD_APP_STAUS  with argument as card's RCA.*/
	SDIO_Cmdstruct.SDIO_Argument			= 0;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SD_APP_STAUS;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SD_APP_STAUS);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
		if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET) {
			for (count = 0; count < 8; count++) {
				*(psdstatus + count) = SDIO_ReadData();
			}
			psdstatus += 8;
		}
	}
	
	if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
		errorstatus = SD_DATA_TIMEOUT;
		return(errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
		errorstatus = SD_DATA_CRC_FAIL;
		return(errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
		errorstatus = SD_RX_OVERRUN;
		return(errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_STBITERR);
		errorstatus = SD_START_BIT_ERR;
		return(errorstatus);
	}
	
	while (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) {
		*psdstatus = SDIO_ReadData();
		psdstatus++;
	}
	
	/*!< Clear all the static status flags*/
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
	return(errorstatus);
}

/**********************************************************************************************************
 @Function			SDCardState SD_GetState(void)
 @Description			SD_GetState							: Returns the current card's state.
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SDCardState SD_GetState(void)
{
	uint32_t resp1 = 0;
	
	if (SD_SendStatus(&resp1) != SD_OK) {
		return SD_CARD_ERROR;
	}
	else {
		return (SDCardState)((resp1 >> 9) & 0x0F);
	}
}

/**********************************************************************************************************
 @Function			SDTransferState SD_GetStatus(void)
 @Description			SD_GetStatus							: Gets the cuurent sd card data transfer status.
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SDTransferState SD_GetStatus(void)
{
	SDCardState cardstate = SD_CARD_TRANSFER;
	
	cardstate = SD_GetState();
	
	if (cardstate == SD_CARD_TRANSFER) {
		return(SD_TRANSFER_OK);
	}
	else if(cardstate == SD_CARD_ERROR) {
		return (SD_TRANSFER_ERROR);
	}
	else {
		return(SD_TRANSFER_BUSY);
	}
}

/**********************************************************************************************************
 @Function			SDTransferState SD_GetTransferState(void)
 @Description			SD_GetTransferState						: Gets the cuurent data transfer state.
 @Input				void
 @Return				SD_Error
**********************************************************************************************************/
SDTransferState SD_GetTransferState(void)
{
	if (SDIO->STA & (SDIO_FLAG_TXACT | SDIO_FLAG_RXACT)) {
		return (SD_TRANSFER_BUSY);
	}
	else {
		return (SD_TRANSFER_OK);
	}
}

/**********************************************************************************************************
 @Function			static SD_Error SDEnWideBus(FunctionalState NewState)
 @Description			SDEnWideBus							: 使能或关闭SDIO的4bit模式
 @Input				ENABLE or DISABLE
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error SDEnWideBus(FunctionalState NewState)
{
	SD_Error errorstatus = SD_OK;
	uint32_t scr[2] = {0, 0};
	
	/* 检测卡是否已上锁 */
	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED) {
		errorstatus = SD_LOCK_UNLOCK_FAILED;
		return(errorstatus);
	}
	
	/*!< Get SCR Register */
	errorstatus = FindSCR(RCA, scr);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	/*!< If wide bus operation to be enabled */
	if (NewState == ENABLE) {
		/*!< If requested card supports wide bus operation */
		if ((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO) {
			/*!< Send CMD55 APP_CMD with argument as card's RCA.*/
			SDIO_Cmdstruct.SDIO_Argument	= (uint32_t) RCA << 16;
			SDIO_Cmdstruct.SDIO_CmdIndex	= SD_CMD_APP_CMD;
			SDIO_Cmdstruct.SDIO_Response	= SDIO_Response_Short;
			SDIO_Cmdstruct.SDIO_Wait		= SDIO_Wait_No;
			SDIO_Cmdstruct.SDIO_CPSM		= SDIO_CPSM_Enable;
			SDIO_SendCommand(&SDIO_Cmdstruct);
			
			errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
			if (errorstatus != SD_OK) {
				return(errorstatus);
			}
			
			/*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
			SDIO_Cmdstruct.SDIO_Argument	= 0x2;
			SDIO_Cmdstruct.SDIO_CmdIndex	= SD_CMD_APP_SD_SET_BUSWIDTH;
			SDIO_Cmdstruct.SDIO_Response	= SDIO_Response_Short;
			SDIO_Cmdstruct.SDIO_Wait		= SDIO_Wait_No;
			SDIO_Cmdstruct.SDIO_CPSM		= SDIO_CPSM_Enable;
			SDIO_SendCommand(&SDIO_Cmdstruct);
			
			errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);
			if (errorstatus != SD_OK) {
				return(errorstatus);
			}
			
			return(errorstatus);
		}
		else {
			errorstatus = SD_REQUEST_NOT_APPLICABLE;
			return(errorstatus);
		}
	}
	/*!< If wide bus operation to be disabled */
	else {
		/*!< If requested card supports 1 bit mode operation */
		if ((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO) {
			/*!< Send CMD55 APP_CMD with argument as card's RCA.*/
			SDIO_Cmdstruct.SDIO_Argument	= (uint32_t) RCA << 16;
			SDIO_Cmdstruct.SDIO_CmdIndex	= SD_CMD_APP_CMD;
			SDIO_Cmdstruct.SDIO_Response	= SDIO_Response_Short;
			SDIO_Cmdstruct.SDIO_Wait		= SDIO_Wait_No;
			SDIO_Cmdstruct.SDIO_CPSM		= SDIO_CPSM_Enable;
			SDIO_SendCommand(&SDIO_Cmdstruct);
			
			errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
			if (errorstatus != SD_OK) {
				return(errorstatus);
			}
			
			/*!< Send ACMD6 APP_CMD with argument as 0 for single bus mode */
			SDIO_Cmdstruct.SDIO_Argument	= 0x00;
			SDIO_Cmdstruct.SDIO_CmdIndex	= SD_CMD_APP_SD_SET_BUSWIDTH;
			SDIO_Cmdstruct.SDIO_Response	= SDIO_Response_Short;
			SDIO_Cmdstruct.SDIO_Wait		= SDIO_Wait_No;
			SDIO_Cmdstruct.SDIO_CPSM		= SDIO_CPSM_Enable;
			SDIO_SendCommand(&SDIO_Cmdstruct);
			
			errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);
			if (errorstatus != SD_OK) {
				return(errorstatus);
			}
			
			return(errorstatus);
		}
		else {
			errorstatus = SD_REQUEST_NOT_APPLICABLE;
			return(errorstatus);
		}
	}
}

/**********************************************************************************************************
 @Function			static SD_Error IsCardProgramming(uint8_t *pstatus)
 @Description			IsCardProgramming							: 检测SD卡是否正在进行内部读写操作
 @Input				pstatus									: 用来装载SD state状态的指针
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error IsCardProgramming(uint8_t *pstatus)
{
	SD_Error errorstatus = SD_OK;
	__IO uint32_t respR1 = 0, status = 0;
	
	/* CMD13让卡发送卡状态寄存器, 存储到M3的位置为SDIO_STA寄存器 */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t) RCA << 16;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SEND_STATUS;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	status = SDIO->STA;
	while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT))) {
		status = SDIO->STA;
	}
	
	if (status & SDIO_FLAG_CTIMEOUT) {
		errorstatus = SD_CMD_RSP_TIMEOUT;
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
		return (errorstatus);
	}
	else if (status & SDIO_FLAG_CCRCFAIL) {
		errorstatus = SD_CMD_CRC_FAIL;
		SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
		return (errorstatus);
	}
	
	status = (uint32_t)SDIO_GetCommandResponse();
	
	/*!< Check response received is of desired command */
	if (status != SD_CMD_SEND_STATUS) {
		errorstatus = SD_ILLEGAL_CMD;
		return (errorstatus);
	}
	
	/*!< Clear all the static flags */
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
	/*!< We have received response, retrieve it for analysis */
	respR1 = SDIO_GetResponse(SDIO_RESP1);
	
	/*!< Find out card status */
	*pstatus = (uint8_t) ((respR1 >> 9) & 0x0000000F);
	
	if ((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO) {
		return(errorstatus);
	}
	
	if (respR1 & SD_OCR_ADDR_OUT_OF_RANGE) {
		return(SD_ADDR_OUT_OF_RANGE);
	}
	
	if (respR1 & SD_OCR_ADDR_MISALIGNED) {
		return(SD_ADDR_MISALIGNED);
	}
	
	if (respR1 & SD_OCR_BLOCK_LEN_ERR) {
		return(SD_BLOCK_LEN_ERR);
	}
	
	if (respR1 & SD_OCR_ERASE_SEQ_ERR) {
		return(SD_ERASE_SEQ_ERR);
	}
	
	if (respR1 & SD_OCR_BAD_ERASE_PARAM) {
		return(SD_BAD_ERASE_PARAM);
	}
	
	if (respR1 & SD_OCR_WRITE_PROT_VIOLATION) {
		return(SD_WRITE_PROT_VIOLATION);
	}
	
	if (respR1 & SD_OCR_LOCK_UNLOCK_FAILED) {
		return(SD_LOCK_UNLOCK_FAILED);
	}
	
	if (respR1 & SD_OCR_COM_CRC_FAILED) {
		return(SD_COM_CRC_FAILED);
	}
	
	if (respR1 & SD_OCR_ILLEGAL_CMD) {
		return(SD_ILLEGAL_CMD);
	}
	
	if (respR1 & SD_OCR_CARD_ECC_FAILED) {
		return(SD_CARD_ECC_FAILED);
	}
	
	if (respR1 & SD_OCR_CC_ERROR) {
		return(SD_CC_ERROR);
	}
	
	if (respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR) {
		return(SD_GENERAL_UNKNOWN_ERROR);
	}
	
	if (respR1 & SD_OCR_STREAM_READ_UNDERRUN) {
		return(SD_STREAM_READ_UNDERRUN);
	}
	
	if (respR1 & SD_OCR_STREAM_WRITE_OVERRUN) {
		return(SD_STREAM_WRITE_OVERRUN);
	}
	
	if (respR1 & SD_OCR_CID_CSD_OVERWRIETE) {
		return(SD_CID_CSD_OVERWRITE);
	}
	
	if (respR1 & SD_OCR_WP_ERASE_SKIP) {
		return(SD_WP_ERASE_SKIP);
	}
	
	if (respR1 & SD_OCR_CARD_ECC_DISABLED) {
		return(SD_CARD_ECC_DISABLED);
	}
	
	if (respR1 & SD_OCR_ERASE_RESET) {
		return(SD_ERASE_RESET);
	}
	
	if (respR1 & SD_OCR_AKE_SEQ_ERROR) {
		return(SD_AKE_SEQ_ERROR);
	}
	
	return (errorstatus);
}

/**********************************************************************************************************
 @Function			static SD_Error FindSCR(uint16_t rca, uint32_t *pscr)
 @Description			FindSCR									: 读取SD卡的SCR寄存器内容
 @Input				RCA										: RCA卡相对地址
					pscr										: 装载SCR内容指针
 @Return				SD_Error
**********************************************************************************************************/
static SD_Error FindSCR(uint16_t rca, uint32_t *pscr)
{
	SD_Error errorstatus = SD_OK;
	uint32_t tempscr[2] = {0, 0};
	uint32_t index = 0;
	
	/*!< Set Block Size To 8 Bytes */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t)8;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SET_BLOCKLEN;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	/*!< Send CMD55 APP_CMD with argument as card's RCA */
	SDIO_Cmdstruct.SDIO_Argument			= (uint32_t) RCA << 16;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_APP_CMD;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	/* 设置数据接收寄存器 */
	SDIO_Datastruct.SDIO_DataTimeOut		= SD_DATATIMEOUT;
	SDIO_Datastruct.SDIO_DataLength		= 8;
	SDIO_Datastruct.SDIO_DataBlockSize		= SDIO_DataBlockSize_8b;
	SDIO_Datastruct.SDIO_TransferDir		= SDIO_TransferDir_ToSDIO;
	SDIO_Datastruct.SDIO_TransferMode		= SDIO_TransferMode_Block;
	SDIO_Datastruct.SDIO_DPSM			= SDIO_DPSM_Enable;
	SDIO_DataConfig(&SDIO_Datastruct);
	
	/*!< Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
	SDIO_Cmdstruct.SDIO_Argument			= 0x0;
	SDIO_Cmdstruct.SDIO_CmdIndex			= SD_CMD_SD_APP_SEND_SCR;
	SDIO_Cmdstruct.SDIO_Response			= SDIO_Response_Short;
	SDIO_Cmdstruct.SDIO_Wait				= SDIO_Wait_No;
	SDIO_Cmdstruct.SDIO_CPSM				= SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_Cmdstruct);
	
	errorstatus = CmdResp1Error(SD_CMD_SD_APP_SEND_SCR);
	if (errorstatus != SD_OK) {
		return(errorstatus);
	}
	
	while (!(SDIO->STA & (SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND| SDIO_FLAG_STBITERR))) {
		if (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) {
			*(tempscr + index) = SDIO_ReadData();
			index++;
			
			if (index > 1) break;
		}
	}
	
	if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
		errorstatus = SD_DATA_TIMEOUT;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
		errorstatus = SD_DATA_CRC_FAIL;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
		errorstatus = SD_RX_OVERRUN;
		return (errorstatus);
	}
	else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
		SDIO_ClearFlag(SDIO_FLAG_STBITERR);
		errorstatus = SD_START_BIT_ERR;
		return (errorstatus);
	}
	
	/*!< Clear all the static flags */
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);
	
	*(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);
	
	*(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);
	
	return(errorstatus);
}

/********************************************** END OF FLEE **********************************************/
