#include "arduino.h"

#define MAX_BANKCARD_NUM 256
#define UID_NUM 4

//---------------
boolean door_sta = false;			//门状态
boolean card_sta = false;			//读卡状态
boolean bankcard_sta = false;		//银行卡状态

int step = 0;		//逻辑步骤
//---------------
unsigned long user_time[3], user_time_cache[3];		//系统计时

long countdown;		//超时函数
