/*BY Microduino @老潘orz*/

// 协议接收部分，坐垫接收蓝牙串口传入数据(并且解析出马达震动方式、时间、强度)
//格式如下
//{"REQUEST":"0","MOTOR_KEEP":"0","MOTOR_SW":"1","MOTOR_TIME":"1000","MOTOR_VOL":{"A":"50","B":"100","C":"150","D":"200"},"CHECK":"THU"}
//协议解析：
//REQUEST为请求获取坐垫数据的作用，开启/关闭，取值范围：0/1
//MOTOR_KEEP为震动是否在接收到一个指令周期后持续开启/关闭，取值范围：0/1
//MOTOR_SW为震动使能作用，开启/关闭，取值范围：0/1
//MOTOR_TIME为一个震动周期持续时间，吃在MOTOR_KEEP开启时，此函数将无效，取值范围：0~3600（不推荐太久）
//MOTOR_VOL为四个震动马达的震动强度，取值范围：0~255
//CHECK为校验部分，以验证Json命令的完整性，取值：“THU”


//协议发送部分，坐垫向通过串口向蓝牙发送数据(坐垫收到请求命令时，采集几个传感器数值，并且发送出去)
//参照接收部分的协议解析，最简单的是发送以下内容即可请求数据：{"REQUEST":"1","CHECK":"THU"}
//格式如下
//{"ID":"19920929","RSSI":"1","VOL":{"a":"250","b":"250","c":"150","d":"150","e":"150","f":"150"},"CHECK":"THU"}
//协议解析：
//ID为坐垫唯一ID，数字字母均可，不推荐太长
//RSSI为蓝牙信号强度，暂无实际作用
//VOL为六个应变片传感器的模拟值，取值范围：0~1023
//CHECK为校验部分，以验证Json命令的完整性，取值：数字字母均可，不推荐太长

#define DEBUG  //是否开启串口监视，以便于调试

#include <aJSON.h>

boolean _check;		//校验
//----------------------------------
aJsonStream serial_stream(&Serial);		//协议传输串口，默认串口0：Serial，如果用ATMEGA32u4，就应该改为Serial1

//----------------------------------
#define PIN_MOTOR_1 6	//引脚：震动马达
#define PIN_MOTOR_2 9	//引脚：震动马达
#define PIN_MOTOR_3 10	//引脚：震动马达
#define PIN_MOTOR_4 11	//引脚：震动马达

boolean motor_keep=false;	//震动是否持续开始
boolean motor_sw=false;		//震动是否开启/关闭
unsigned long motor_time=0;	//震动持续时间
int motor_vol[4] = 	//四个震动强度
{
  0,0,0,0
};
boolean run_motor=false;	//开始震动
unsigned long _motor_delay=millis();	//震动持续时间定时器
//----------------------------------
#define TIME_SENSOR_MIN 200		//传感器采集时间最小间隔
int vol_sensor[6];			//六个传感器数值
unsigned long sensor_delay=millis();	//传感器采集时间定时器

#define PIN_SENSOR_1 A0		//引脚：传感器
#define PIN_SENSOR_2 A1		//引脚：传感器
#define PIN_SENSOR_3 A2		//引脚：传感器
#define PIN_SENSOR_4 A3		//引脚：传感器
#define PIN_SENSOR_5 A6		//引脚：传感器
#define PIN_SENSOR_6 A7		//引脚：传感器
//----------------------------------
boolean request=false;		//请求获取坐垫数据

int rssi=1024;				//蓝牙信号强度，暂无实际作用
char* id="19920929";		//坐垫唯一ID，数字字母均可，不推荐太长
char* check="THU";			//校验部分，以验证Json命令的完整性


//Code to print out the free memory
void freeMem(char* message) {
  extern unsigned int __heap_start, *__brkval; 
  unsigned int v;
  Serial.print(message);
  Serial.print(":");
  Serial.println((unsigned int) &v - (__brkval == 0 ? (unsigned int) &__heap_start : (unsigned int) __brkval));
}

// the setup routine runs once when you press reset:
void setup() 
{
  Serial.begin(115200);				//初始化波特率
  //-------------------------------				//四个震动引脚初始化
  pinMode(PIN_MOTOR_1, OUTPUT);     
  pinMode(PIN_MOTOR_2, OUTPUT);     
  pinMode(PIN_MOTOR_3, OUTPUT);     
  pinMode(PIN_MOTOR_4, OUTPUT);     
  //-------------------------------				//六个传感器引脚初始化
  pinMode(PIN_SENSOR_1, INPUT);     
  pinMode(PIN_SENSOR_2, INPUT);     
  pinMode(PIN_SENSOR_3, INPUT);     
  pinMode(PIN_SENSOR_4, INPUT);     
  pinMode(PIN_SENSOR_5, INPUT);     
  pinMode(PIN_SENSOR_6, INPUT);     
}

// the loop routine runs over and over again forever:
void loop() 
{
  //协议接收部分============================

  if (serial_stream.available()) 
  {
    /* First, skip any accidental whitespace like newlines. */
    serial_stream.skip();
  }

  if (serial_stream.available()) 
  {
    _check = parseJson();				//解析协议
    if (_check) 
    {
#ifdef DEBUG
      Serial.println(F("JSON:OK"));		//校验通过
#endif
      if(!request)
      {
        run_motor=true;
        _motor_delay=millis();
      }
    }
    else 
    {
#ifdef DEBUG
      Serial.println(F("JSON:ER"));		//校验失败
#endif
    }

#ifdef DEBUG
    Serial.println("");
#endif

  }

  //震动控制部分============================
  if(run_motor)
  {
    _MOTOR();
  }


  //协议请求部分============================
  if(request)
  {
    //传感器采集部分===========================
    if(sensor_delay>millis()) sensor_delay=millis();
    if(millis()-sensor_delay>TIME_SENSOR_MIN)      //最小刷新间隔
    {
      sensor_delay=millis();		//定时器

      vol_sensor[0]=Filter(PIN_SENSOR_1);
      vol_sensor[1]=Filter(PIN_SENSOR_2);
      vol_sensor[2]=Filter(PIN_SENSOR_3);
      vol_sensor[3]=Filter(PIN_SENSOR_4);
      vol_sensor[4]=Filter(PIN_SENSOR_5);
      vol_sensor[5]=Filter(PIN_SENSOR_6);

#ifdef DEBUG
      Serial.println(millis()-sensor_delay);
#endif
    }

    //协议发送部分============================
    aJsonObject *msg = createMessage();		//创建协议
    aJson.print(msg, &serial_stream);			//发送协议

#ifdef DEBUG
    Serial.println(); 
#endif
    aJson.deleteItem(msg);					//删除缓存

    request=false;
  }
}

/*
A、名称：中位值滤波法
B、方法：
    连续采样N次（N取奇数），把N次采样值按大小排列，
    取中间值为本次有效值。
C、优点：
    能有效克服因偶然因素引起的波动干扰；
    对温度、液位的变化缓慢的被测参数有良好的滤波效果。
D、缺点：
    对流量、速度等快速变化的参数不宜。
E、整理：shenhaiyu 2013-11-01
*/
#define FILTER_N 3
int Filter(int _AD) {
  int filter_buf[FILTER_N];
  int i, j;
  int filter_temp;
  for(i = 0; i < FILTER_N; i++) {
    filter_buf[i] = analogRead(_AD);
    delay(1);
  }
  // 采样值从小到大排列（冒泡法）
  for(j = 0; j < FILTER_N - 1; j++) {
    for(i = 0; i < FILTER_N - 1 - j; i++) {
      if(filter_buf[i] > filter_buf[i + 1]) {
        filter_temp = filter_buf[i];
        filter_buf[i] = filter_buf[i + 1];
        filter_buf[i + 1] = filter_temp;
      }
    }
  }
  return filter_buf[(FILTER_N - 1) / 2];
}

aJsonObject *createMessage()
{
  aJsonObject *msg = aJson.createObject();

  aJson.addStringToObject(msg, "ID", id);
  aJson.addNumberToObject(msg, "RSSI", rssi);
  aJsonObject *VOL = aJson.createIntArray(vol_sensor, 6);
  aJson.addItemToObject(msg, "VOL", VOL);
  aJson.addStringToObject(msg, "CHECK", check);
  return msg;
}

boolean parseJson() 
{
  char* _ST;
  String _SST;

  /* Something real on input, let's take a look. */
  aJsonObject* root = aJson.parse(&serial_stream);
  if (root != NULL) {
    //      Serial.println("Parsed successfully 1 " );

    aJsonObject* REQUEST = aJson.getObjectItem(root, "REQUEST"); 
    _ST=REQUEST->valuestring;
    //    Serial.println(_ST);
    _SST=_ST;
    request=(int)_SST.toInt();

    if(!request)
    {
      aJsonObject* MOTOR_KEEP = aJson.getObjectItem(root, "MOTOR_KEEP"); 
      _ST=MOTOR_KEEP->valuestring;
      //    Serial.println(_ST);
      _SST=_ST;
      motor_keep=(int)_SST.toInt();

      aJsonObject* MOTOR_SW = aJson.getObjectItem(root, "MOTOR_SW"); 
      _ST=MOTOR_SW->valuestring;
      //    Serial.println(_ST);
      _SST=_ST;
      motor_sw=(int)_SST.toInt();

      aJsonObject* MOTOR_TIME = aJson.getObjectItem(root, "MOTOR_TIME"); 
      _ST=MOTOR_TIME->valuestring;
      //    Serial.println(_ST);
      _SST=_ST;
      motor_time=(int)_SST.toInt();

      aJsonObject* MOTOR_VOL = aJson.getObjectItem(root, "MOTOR_VOL");       
      if (MOTOR_VOL != NULL) {
        //      Serial.println("Parsed successfully 2 " );

        aJsonObject* A = aJson.getObjectItem(MOTOR_VOL, "A"); 
        _ST=A->valuestring;
        //      Serial.print("A:");
        //      Serial.print(_ST);
        _SST=_ST;
        motor_vol[0]=(int)_SST.toInt();

        aJsonObject* B = aJson.getObjectItem(MOTOR_VOL, "B"); 
        _ST=B->valuestring;
        //      Serial.print(",B:");
        //      Serial.print(_ST);
        _SST=_ST;
        motor_vol[1]=(int)_SST.toInt();

        aJsonObject* C = aJson.getObjectItem(MOTOR_VOL, "C"); 
        _ST=C->valuestring;
        //      Serial.print(",C:");
        //      Serial.print(_ST);
        _SST=_ST;
        motor_vol[2]=(int)_SST.toInt();

        aJsonObject* D = aJson.getObjectItem(MOTOR_VOL, "D"); 
        _ST=D->valuestring;
        //      Serial.print(",D:");
        //      Serial.println(_ST);
        _SST=_ST;
        motor_vol[3]=(int)_SST.toInt();

      }
    }

    aJsonObject* CHECK = aJson.getObjectItem(root, "CHECK"); 
    _ST=CHECK->valuestring;
    //    Serial.println(_ST);
    //    Serial.print("check:");
    //    Serial.println(_ST);
  }

  String S_check=_ST;
  // Serial.println(S_check);

  aJson.deleteItem(root);		//删除缓存
#ifdef DEBUG
  freeMem("FREEMEM");
#endif
  if (S_check==check) 
    return true;
  else
    return false;
}

void _MOTOR()
{
  if(_motor_delay>millis()) _motor_delay=millis();
  if(millis()-_motor_delay>motor_time)
  {
    //    _motor_delay=millis();
    if(!motor_keep)
    {
      analogWrite(PIN_MOTOR_1,0);
      analogWrite(PIN_MOTOR_2,0);
      analogWrite(PIN_MOTOR_3,0);
      analogWrite(PIN_MOTOR_4,0);
    }
    run_motor=false;
  }
  else if(motor_sw)
  {
    analogWrite(PIN_MOTOR_1, motor_vol[0]);
    analogWrite(PIN_MOTOR_2,motor_vol[1]);
    analogWrite(PIN_MOTOR_3,motor_vol[2]);
    analogWrite(PIN_MOTOR_4,motor_vol[3]);
  }
}
