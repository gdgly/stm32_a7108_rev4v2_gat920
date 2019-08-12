#ifndef __SOCKET_XUNFEI_CONFIG_H
#define   __SOCKET_XUNFEI_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

#define	MAX_MSG_ID						255

#define	SOCKET_XUNFEI_SEND_BUFFER_SIZE		2048
#define	SOCKET_XUNFEI_RECV_BUFFER_SIZE		512
#define	SOCKET_XUNFEI_BUFFER_SIZW			1024

#define	SOCKET_XUNFEI_AGAINSENDTIME			5

#define	SOCKET_XUNFEI_LOGIN_REQUEST			1
#define	SOCKET_XUNFEI_LOGIN_OVER				0

#define	SOCKET_XUNFEI_LOGIN_ACCOUNT			"wt_magnetic"
#define	SOCKET_XUNFEI_LOGIN_PASSWD			""

#define	SOCKET_XUNFEI_DATA_TYPE_LOGIN_CON		"101"
#define	SOCKET_XUNFEI_DATA_TYPE_LOGIN_ACK		"201"
#define	SOCKET_XUNFEI_DATA_TYPE_FLOWT_CON		"105"
#define	SOCKET_XUNFEI_DATA_TYPE_FLOWT_ACK		"205"

#define	SOCKET_XUNFEI_DEVICE_CODE			CROSSID

typedef struct SOCKET_Xunfei_ParameterTypeDef	SOCKET_Xunfei_ParameterTypeDef;
typedef struct SOCKET_Xunfei_ClientsTypeDef		SOCKET_Xunfei_ClientsTypeDef;

/* Socket Xunfei Status */
typedef enum
{
	SocketXunfei_OK       					= 0x00,
	SocketXunfei_ERROR    					= 0x01
}SOCKET_Xunfei_StatusTypeDef;

typedef __packed struct
{
	unsigned int							dataHead;
	unsigned char							dataBody;
}SOCKET_Xunfei_Data_TypeDef;

struct SOCKET_Xunfei_ParameterTypeDef
{
	int									DirverWayCode;
	char									DateTime[15];
	int									Interval;
	int									Volume;
	unsigned short							AvgOccupancy;
	unsigned short							AvgHeadTime;
	float								AvgSpeed;
	float								AvgLength;
};

struct SOCKET_Xunfei_ClientsTypeDef
{
	unsigned char*							Sendbuf;
	unsigned char*							Recvbuf;
	unsigned int							Sendbuf_size;
	unsigned int							Recvbuf_size;
	short								Sendlen;
	short								Recvlen;
	unsigned char*							DataProcessStack;
	unsigned int							DataProcessStack_size;
	short								DataProcessStack_len;
	
	unsigned int							MsgId;
	
	unsigned short							OutputID[OUTPUT_MAX];
	
	SOCKET_Xunfei_ParameterTypeDef			Parameter[OUTPUT_MAX];
};

void SOCKET_Xunfei_Client_Init(SOCKET_Xunfei_ClientsTypeDef* pClient);

#endif
