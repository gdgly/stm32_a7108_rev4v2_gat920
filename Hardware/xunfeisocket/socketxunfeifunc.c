/**
  *********************************************************************************************************
  * @file    socketxunfeifunc.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socket xunfei 内部协议
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "socketxunfeifunc.h"
#include "socketxunfeiinstantia.h"
#include "sock.h"
#include "string.h"
#include "stdio.h"

/**********************************************************************************************************
 @Function			unsigned int SOCKET_Xunfei_GetNextMsgId(SOCKET_Xunfei_ClientsTypeDef* pClient)
 @Description			SOCKET_Xunfei_GetNextMsgId			: 获取下一个消息ID
 @Input				pClient							: Socket Xunfei客户端实例
 @Return				MsgId							: 消息ID
**********************************************************************************************************/
unsigned int SOCKET_Xunfei_GetNextMsgId(SOCKET_Xunfei_ClientsTypeDef* pClient)
{
	pClient->MsgId = (pClient->MsgId == MAX_MSG_ID) ? 1 : pClient->MsgId + 1;
	return pClient->MsgId;
}






/**********************************************************************************************************
 @Function			SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Serialize_LoginRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, const char* account, const char* passwd)
 @Description			SOCKET_Xunfei_Serialize_LoginRequest	: 序列化登入请求
 @Input				pClient							: Socket Xunfei客户端实例
					account							: 账号
					passwd							: 密码
 @Return				SOCKET_Xunfei_StatusTypeDef			: 状态
**********************************************************************************************************/
SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Serialize_LoginRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, const char* account, const char* passwd)
{
	SOCKET_Xunfei_StatusTypeDef status = SocketXunfei_OK;
	
	memset((void*)pClient->DataProcessStack, 0x00, pClient->DataProcessStack_size);
	SOCKET_Xunfei_Data_TypeDef* data = (SOCKET_Xunfei_Data_TypeDef*)pClient->DataProcessStack;
	
	char* databuffer = (char*)&data->dataBody;
	
	sprintf(databuffer, "%d,%s,%s,%s", SOCKET_Xunfei_GetNextMsgId(pClient), SOCKET_XUNFEI_DATA_TYPE_LOGIN_CON, account, passwd);
	data->dataHead = htonl(strlen(databuffer));
	
	pClient->DataProcessStack_len = sizeof(data->dataHead) + strlen(databuffer);
	
	return status;
}

/**********************************************************************************************************
 @Function			SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Deserialize_LoginRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, char* buf, int len)
 @Description			SOCKET_Xunfei_Deserialize_LoginRequest	: 反序列化登入请求应答
 @Input				pClient							: Socket Xunfei客户端实例
 @Return				SOCKET_Xunfei_StatusTypeDef			: 状态
**********************************************************************************************************/
SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Deserialize_LoginRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, char* buf, int len)
{
	SOCKET_Xunfei_StatusTypeDef status = SocketXunfei_OK;
	
	SOCKET_Xunfei_Data_TypeDef* data = (SOCKET_Xunfei_Data_TypeDef*)buf;
	
	char* databuffer = (char*)&data->dataBody;
	int msgid = 0;
	int msgtype = 0;
	char msgresult[30] = {0};
	
	if (!(ntohl(data->dataHead) > 0)) {
		status = SocketXunfei_ERROR;
		goto exit;
	}
	
	if (sscanf(databuffer, "%d,%d,%s", &msgid, &msgtype, msgresult) <= 0) {
		status = SocketXunfei_ERROR;
		goto exit;
	}
	
	if (msgtype != 201) {
		status = SocketXunfei_ERROR;
		goto exit;
	}
	
	if (strncmp(msgresult, "0", 1) != 0) {
		status = SocketXunfei_ERROR;
		goto exit;
	}
	
exit:
	return status;
}

/**********************************************************************************************************
 @Function			SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Serialize_LenDataRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_Serialize_LenDataRequest	: 序列化车道交通流量数据
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				SOCKET_Xunfei_StatusTypeDef			: 状态
**********************************************************************************************************/
SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Serialize_LenDataRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	SOCKET_Xunfei_StatusTypeDef status = SocketXunfei_OK;
	
	memset((void*)pClient->DataProcessStack, 0x00, pClient->DataProcessStack_size);
	SOCKET_Xunfei_Data_TypeDef* data = (SOCKET_Xunfei_Data_TypeDef*)pClient->DataProcessStack;
	
	char* databuffer = (char*)&data->dataBody;
	int   datalen    = 0;
	
	/* 流水号, 数据类型 */
	sprintf(databuffer + datalen, "%d,%s", SOCKET_Xunfei_GetNextMsgId(pClient), SOCKET_XUNFEI_DATA_TYPE_FLOWT_CON);
	datalen = strlen(databuffer);
	
	/* 设备编号 */
	sprintf(databuffer + datalen, ",%d", SOCKET_XUNFEI_DEVICE_CODE);
	datalen = strlen(databuffer);
	
	/* 车道编号 */
	sprintf(databuffer + datalen, ",%d", pClient->Parameter[inlen].DirverWayCode);
	datalen = strlen(databuffer);
	
	/* 统计/采集时间 */
	sprintf(databuffer + datalen, ",%s", (char*)pClient->Parameter[inlen].DateTime);
	datalen = strlen(databuffer);
	
	/* 统计周期 */
	sprintf(databuffer + datalen, ",%d", pClient->Parameter[inlen].Interval);
	datalen = strlen(databuffer);
	
	/* 车流量 */
	sprintf(databuffer + datalen, ",%d", pClient->Parameter[inlen].Volume);
	datalen = strlen(databuffer);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	data->dataHead = htonl(strlen(databuffer));
	
	pClient->DataProcessStack_len = sizeof(data->dataHead) + strlen(databuffer);
	
	return status;
}

/**********************************************************************************************************
 @Function			SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Deserialize_LenDataRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, char* buf, int len)
 @Description			SOCKET_Xunfei_Deserialize_LenDataRequest: 反序列化登入请求应答
 @Input				pClient							: Socket Xunfei客户端实例
 @Return				SOCKET_Xunfei_StatusTypeDef			: 状态
**********************************************************************************************************/
SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Deserialize_LenDataRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, char* buf, int len)
{
	SOCKET_Xunfei_StatusTypeDef status = SocketXunfei_OK;
	
	SOCKET_Xunfei_Data_TypeDef* data = (SOCKET_Xunfei_Data_TypeDef*)buf;
	
	char* databuffer = (char*)&data->dataBody;
	int msgid = 0;
	int msgtype = 0;
	char msgresult[30] = {0};
	
	if (!(ntohl(data->dataHead) > 0)) {
		status = SocketXunfei_ERROR;
		goto exit;
	}
	
	if (sscanf(databuffer, "%d,%d,%s", &msgid, &msgtype, msgresult) <= 0) {
		status = SocketXunfei_ERROR;
		goto exit;
	}
	
	if (msgtype != 205) {
		status = SocketXunfei_ERROR;
		goto exit;
	}
	
	if (strncmp(msgresult, "0", 1) != 0) {
		status = SocketXunfei_ERROR;
		goto exit;
	}
	
exit:
	return status;
}














































/**********************************************************************************************************
 @Function			SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_GetOutputID(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 *outputid)
 @Description			SOCKET_Xunfei_GetOutputID			: 读取OutputID输出端口
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: Output_ID存放地址
 @Return				SOCKET_Xunfei_StatusTypeDef			: 状态
**********************************************************************************************************/
SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_GetOutputID(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 *outputid)
{
	SOCKET_Xunfei_StatusTypeDef status = SocketXunfei_OK;
	
	for (int i = 0; i < OUTPUT_MAX; i++) {
		pClient->OutputID[i] = outputid[i];
	}
	
	return status;
}


















/********************************************** END OF FLEE **********************************************/
