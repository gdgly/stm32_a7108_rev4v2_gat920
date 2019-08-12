# stm32F103_A7108_tmote_app_recv
> ##### Copyright (C) 2019 Movebroad Version:1.19 Design by Movebroad

* 2019-08-12 :
	* Socket协议中Pcu字段改为Voltage字段，表示地磁电压值。
* 2019-07-19 :
	* IO Output 模式6中，修复输出IO为车辆进车辆出最后一次改变状态，输出设定时长。
	* IOOutput_Mode6_Check(): 
	* ++ IOOutputStruct[dici_index].SupplyingFlag = 1;
	* ++ IOOutputStruct[dici_index].SupplyUseTime = 0;
	* ++ IOOutputStruct[dici_index].SupplyingCarNum = 0;
* 2019-07-17 :
	* SD卡数据缓存功能初步完成。
* 2019-07-03 :
	* SD卡Fatfs文件系统操作文件，进行封装，方便应用层调用写入数据。
* 2019-07-02 :
	* 车长计算增加一种算法。
	* 车长 = 速度*（第一地磁进入至第二地磁离开时间）-3。
	* FATFS默认只支持4G，32位系统只有4寻址空间，修改FATFS底层库，支持4G*512内存。
	* 修改车长-3为-2。
* 2019-07-01 :
	* 车速限定120KM/h。
* 2019-06-28 :
	* SDIO驱动中SDCardInfo中卡的容量字节数，由于32为最大4G，所以计算容量上，先不乘以1024。
	* SDIO驱动读写测试正常。
* 2019-06-27 :
	* 修改启动汇编文件，加入其他中断标志号。
	* SDIO驱动基本完成，还需测试。
* 2019-06-26 :
	* SDIO驱动测试卡信息已读取，硬件电路无问题。
* 2019-06-25 :
	* Socket流量脉冲数据，未收到应答继续发送3次。
	* Socket流量脉冲数据，地磁心跳包收到不再以脉冲数据发送。
	* 串口1作为调试串口。
* 2019-06-24 :
	* Socket普通协议脉冲数据上传中车场上传修复。
	* 讯飞协议账号密码修改。
* 2019-06-24 :
	* 版本号修改为 1.19。
* 2019-06-19 :
	* 讯飞协议初次架构完成。
* 2019-06-12 :
	* SocketExtend协议(银江Socket扩展协议)，增加普通流量数据包应答处理，无应答重复发送之前数据，重复次数3次，重复时间5秒。
* 2019-06-10 :
	* GAT协议脉冲数据发送各个包延时100ms发送。
* 2019-05-29 :
	* 修改GAT脉冲数据发送增加地磁数据丢失补包功能。
* 2019-05-21 :
	* 修改socketextend协议中路口编号排列方式根据上位机配置而定。
* 2019-05-20 :
	* 修复socketextend协议中datalength计算错误。
* 2019-05-17 :
	* socketextend协议中，小车数量数跟随一分钟总得车辆数。
* 2019-05-16 :
	* 版本号修改为 1.18。
	* socket普通交通信息采集协议中，小车数量数跟随一分钟总得车辆数。
	* 编码方式修改为C99 Mode，One ELF Section per Function。