#ifndef _MB_CONFIG_H
#define _MB_CONFIG_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
/* ----------------------- Defines ------------------------------------------*/
/*! \defgroup modbus_cfg Modbus Configuration
 *
 * Most modules in the protocol stack are completly optional and can be
 * excluded. This is specially important if target resources are very small
 * and program memory space should be saved.<br>
 *
 * All of these settings are available in the file <code>mbconfig.h</code>
 */
/*! \addtogroup modbus_cfg
 *  @{
 */
/*! \brief If Modbus ASCII support is enabled. */
#define MB_ASCII_ENABLED                        (  0 )

/*! \brief If Modbus RTU support is enabled. */
#define MB_RTU_ENABLED                          (  1 )

/*! \brief If Modbus TCP support is enabled. */
#define MB_TCP_ENABLED                          (  0 )

/*! \brief The character timeout value for Modbus ASCII.
 *
 * The character timeout value is not fixed for Modbus ASCII and is therefore
 * a configuration option. It should be set to the maximum expected delay
 * time of the network.
 */
#define MB_ASCII_TIMEOUT_SEC                    (  1 )

/*! \brief Timeout to wait in ASCII prior to enabling transmitter.
 *
 * If defined the function calls vMBPortSerialDelay with the argument
 * MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS to allow for a delay before
 * the serial transmitter is enabled. This is required because some
 * targets are so fast that there is no time between receiving and
 * transmitting the frame. If the master is to slow with enabling its 
 * receiver then he will not receive the response correctly.
 */
#ifndef MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS
#define MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS    ( 0 )
#endif

/*! \brief Maximum number of Modbus functions codes the protocol stack
 *    should support.
 *
 * The maximum number of supported Modbus functions must be greater than
 * the sum of all enabled functions in this file and custom function
 * handlers. If set to small adding more functions will fail.
 */
#define MB_FUNC_HANDLERS_MAX                    ( 16 )

/*! \brief Number of bytes which should be allocated for the <em>Report Slave ID
 *    </em>command.
 *
 * This number limits the maximum size of the additional segment in the
 * report slave id function. See eMBSetSlaveID(  ) for more information on
 * how to set this value. It is only used if MB_FUNC_OTHER_REP_SLAVEID_ENABLED
 * is set to <code>1</code>.
 */
#define MB_FUNC_OTHER_REP_SLAVEID_BUF           ( 32 )

/*! \brief If the <em>Report Slave ID</em> function should be enabled. */
#define MB_FUNC_OTHER_REP_SLAVEID_ENABLED       (  1 )

/*! \brief If the <em>Read Input Registers</em> function should be enabled. */
#define MB_FUNC_READ_INPUT_ENABLED              (  1 )

/*! \brief If the <em>Read Holding Registers</em> function should be enabled. */
#define MB_FUNC_READ_HOLDING_ENABLED            (  1 )

/*! \brief If the <em>Write Single Register</em> function should be enabled. */
#define MB_FUNC_WRITE_HOLDING_ENABLED           (  1 )

/*! \brief If the <em>Write Multiple registers</em> function should be enabled. */
#define MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )

/*! \brief If the <em>Read Coils</em> function should be enabled. */
#define MB_FUNC_READ_COILS_ENABLED              (  1 )

/*! \brief If the <em>Write Coils</em> function should be enabled. */
#define MB_FUNC_WRITE_COIL_ENABLED              (  1 )

/*! \brief If the <em>Write Multiple Coils</em> function should be enabled. */
#define MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED    (  1 )

/*! \brief If the <em>Read Discrete Inputs</em> function should be enabled. */
#define MB_FUNC_READ_DISCRETE_INPUTS_ENABLED    (  1 )

/*! \brief If the <em>Read/Write Multiple Registers</em> function should be enabled. */
#define MB_FUNC_READWRITE_HOLDING_ENABLED       (  1 )

/* ----------------------- Defines  Register---------------------------------*/
#define REG_RECV_START 0
#define REG_RECV_NREGS 32

#define REG_NET_START 0x0100
#define REG_NET_NREGS 16

#define REG_WVD_START 0x1000
#define REG_WVD_NREGS 16

#define REG_OUTPUT_START 0x2000


//Modbus�Ĵ�����ַ
/*
0x0000	��ַ	1-254	R,W	
0x0001	������	0-9	R,W	
0x0002	�����ϴ���ͨ��		R,W	
0x0003	�������ĳ�����ŵĸ�λ		R	
0x0004	�������ĳ�����ŵĵ�λ		R	
0x0005	���������������		R	
0x0006	����������������		R	
0x0007	��ʼ��������ģ���־		W	
0x0008	����������Եķ������Ķ˿���		R	
0x0009	��������⵽��Ӧ�������������źŷ�ʽ	0-6	R,W	
0x000A	�������������ĸߵ�ƽʱ�䣨ģʽ0ʱ���ڼ�⵽�����������ʧʱ�����ʱ�䣩����100msΪ��λ������ģʽ0/1/3/4/6	1-255	R,W	
0x000B	�����뿪������ĸߵ�ƽʱ�䣬��100msΪ��λ������ģʽ2/3/5/6��	1-255	R,W	
0x000C	����ʱ�ĵ͵�ƽ���ʱ�䣬��100msΪ��λ������ģʽ0/1/2/3/4/5/6	0-255	R,W	
0x000D	�ߵ�ƽ������ʱ��,����0	0-255	R,W	
0x000E	��⵽����������ʱ���Ƿ�Զ������д���Ϊ0��ʾֻ�Խ��յ������ݰ����д���Ϊ1��ʾ��⵽����ʱ����ͨ���߼��ж϶Զ�Ӧ�Ķ������д���	0/1	R,W	
0x000F	������ݰ��Ƿ��ظ���ʱ�䣬��200msΪ��λ��Ĭ��Ϊ0��	0-255	R,W	
0x0010	����������汾��			0-65535	R,O
0x0011	������Ӳ���汾��			0-65535	R,O
0x0012	��������ϵͳʱ��ĸ�λ		0-65535	R,W
0x0013	��������ϵͳʱ��ĵ�λ		0-65535	R,W
0x0014	�������ĳ���·�ڱ���ĸ�λ	0-65535	R,O
0x0015	�������ĳ���·�ڱ���ĵ�λ	0-65535	R,O
0x0016	SocketDirverWayCode��������	0-65535	R,W

0x1000	Ҫ���õĳ������������				R,W
0x1001	Ҫ���õĳ�������Ϣ��־λ				R,W
0x1002	Ҫ���õĳ���������ģʽ��ͨ�Ų���		R,W
0x1003	Ҫ���õĳ���������ͳ�ƻ�����ʱ����	R,W
0x1004	Ҫ���õĳ���������ʱ����			R,W
0x1005	Ҫ���õĳ�������������ֵ				R,W
0x1006	Ҫ���õĳ����������뿪����			R,W
0x1007	�شż�����						R,W
0x1008	Ҫ���õĳ�������׼ֵ������ֵ			R,W
0x1009	Ҫ���õĳ��������¼����׼ֵ�Ĳ���		R,W

0x0100	�豸����ӿ�IPv4��ַ���ֽ�			R,W
0x0101	�豸����ӿ�IPv4��ַ���ֽ�			R,W
0x0102	�豸����ӿ�IPv4����������ֽ�		R,W
0x0103	�豸����ӿ�IPv4����������ֽ�		R,W
0x0104	�豸����ӿ�IPv4���ظ��ֽ�			R,W
0x0105	�豸����ӿ�IPv4���ص��ֽ�			R,W
0x0106	SOCKET�ӿ�A�����ӷ�ʽ			0-3	R,W
0x0107	SOCKET�ӿ�A�ķ�����IP���ֽ�			R,W
0x0108	SOCKET�ӿ�A�ķ�����IP���ֽ�			R,W
0x0109	SOCKET�ӿ�A�ķ������˿�				R,W
0x010A	SOCKET�ӿ�A������״̬			0-3	R,O
*/

#define REG_READ_SIMPLE_DATA					0xFC00	//��ȡ����Ϣ��ַ
#define REG_READ_COMPLETE_DATA				0xFD00	//��ȡ������Ϣ��ַ
#define REG_READ_MAC_ADDRESS					0xFFF0	//��ȡ������Ϣ��ַ

#define REG_RW_ADDR							0x0000
#define REG_RW_BAUD							0x0001
#define REG_RW_RT_CHANNEL					0x0002
#define REG_RO_MAC_H						0x0003
#define REG_RO_MAC_L						0x0004
#define REG_RO_PRODUCE_YEAR					0x0005
#define REG_RO_PRODUCE_DAY					0x0006
#define REG_RW_INIT_FLAG						0x0007
#define REG_RO_PORT_NUM						0x0008
#define REG_RW_OUTPUT_MODE					0x0009
#define REG_RW_VI_OUTPUT_HIGH_TIME				0x000A
#define REG_RW_VO_OUTPUT_HIGH_TIME				0x000B
#define REG_RW_OUTPUT_LOW_MIN					0x000C
#define REG_RW_OUTPUT_HIGH_MAX				0x000D
#define REG_RW_HANDLE_LOST					0x000E
#define REG_RW_CHECK_REPEAT_TIME				0x000F
#define REG_RO_SOFTWARE_VERSION				0x0010
#define REG_RO_HARDWARE_VERSION				0x0011
#define REG_RW_RTC_TIME_H					0x0012
#define REG_RW_RTC_TIME_L					0x0013
#define REG_RO_CROSSID_H						0x0014
#define REG_RO_CROSSID_L						0x0015
#define REG_RW_DIRVERWAY_CONFIG				0x0016

#define REG_RW_WVD_ID						0x1000
#define REG_RW_WVD_CONFIG_FLAG				0x1001
#define REG_RW_WVD_WORKMODE					0x1002
#define REG_RW_WVD_HEARTBEAT_INTERVAL			0x1003
#define REG_RW_WVD_CONFIG_INTERVAL				0x1004
#define REG_RW_WVD_VI_THRESHHOLD				0x1005
#define REG_RW_WVD_VO_THRESHHOLD				0x1006
#define REG_RW_WVD_MEASURE_MODE				0x1007
#define REG_RW_WVD_REBASE_VALUE				0x1008
#define REG_RW_WVD_REBASE_TIME				0x1009

#define REG_RW_NET_DEVICE_ADDR_H				0x0100
#define REG_RW_NET_DEVICE_ADDR_L				0x0101
#define REG_RW_NET_DEVICE_MASK_H				0x0102
#define REG_RW_NET_DEVICE_MASK_L				0x0103
#define REG_RW_NET_DEVICE_BCAST_H				0x0104
#define REG_RW_NET_DEVICE_BCAST_L				0x0105
#define REG_RW_NET_SOCKETA_CONNECT_MODE			0x0106
#define REG_RW_NET_SOCKETA_ADDR_H				0x0107
#define REG_RW_NET_SOCKETA_ADDR_L				0x0108
#define REG_RW_NET_SOCKETA_PORT				0x0109
#define REG_RO_NET_SOCKETA_CONNECT_STATE		0x010A



/*! @} */
#ifdef __cplusplus
    PR_END_EXTERN_C
#endif
#endif
