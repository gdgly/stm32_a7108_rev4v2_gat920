# stm32F103_A7108_tmote_app_recv
> ##### Copyright (C) 2019 Movebroad Version:1.18 Design by Movebroad

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