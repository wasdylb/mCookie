#include"arduino.h"

#define mySerial Serial1

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(4, -1); // RX, TX (D5与电机冲突 屏蔽 只用RX)

Protocol bleProtocol(&mySerial, TYPE_NUM);  //软串口,校验数据类

#define _DEBUG  //DEBUG调试

#define BLE_SPEED 9600  //蓝牙接口速度

#define SAFE_TIME_OUT 250   //失控保护时间
#define MAX_THROTTLE 255 //最大油门 < 255
#define MAX_STEERING 512 //最大转向 < 512
#define CHANNEL_THROTTLE  2 //油门通道
#define CHANNEL_STEERING  1 //转向通道

#if defined(__AVR_ATmega32U4__) || (__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define motor_pin0A 8  //PWM
#define motor_pin0B 6
#define motor_pin1A 7  //PWM 
#define motor_pin1B 5
#define motor_fixL 1  //速度修正 -1到1之间
#define motor_fixR 1  //速度修正 -1到1之间
#else
#define motor_pin0A 6  //PWM
#define motor_pin0B 8
#define motor_pin1A 5  //PWM
#define motor_pin1B 7
#define motor_fixL -1 //速度修正 -1到1之间
#define motor_fixR -1 //速度修正 -1到1之间
#endif

Motor MotorLeft(motor_pin0A, motor_pin0B);
Motor MotorRight(motor_pin1A, motor_pin1B);

uint16_t channal_data[CHANNEL_NUM]; //8通道数据
int16_t throttle = 0; //油门
int16_t steering = 0; //

unsigned long safe_ms = millis();

boolean _Error = true;

void ble_robot()
{
  switch (bleProtocol.parse(channal_data, MODE_WHILE)) {
    case P_NONE:  //DATA NONE
      break;
    case P_FINE:  //DATA OK
      _Error = false;
      break;
    case P_ERROR: //DATA ERROR
      break;
    case P_TIMEOUT: //DATA TIMEOUT
      break;
  }
  if (!_Error) {
    safe_ms = millis();

    throttle = map(channal_data[CHANNEL_THROTTLE - 1], 1000, 2000, -MAX_THROTTLE, MAX_THROTTLE);
    steering = map(channal_data[CHANNEL_STEERING - 1], 1000, 2000, -MAX_STEERING, MAX_STEERING);

    MotorLeft.Driver(MotorLeft.GetData(throttle, steering, CHAN_LEFT));
    MotorRight.Driver(MotorRight.GetData(throttle, steering, CHAN_RIGHT));

#ifdef _DEBUG
    Serial.print(" \t DATA OK :[");
    for (int a = 0; a < CHANNEL_NUM; a++) {
      Serial.print(channal_data[a]);
      Serial.print(" ");
    }
    Serial.print("],throttle:");
    Serial.print(throttle);
    Serial.print(",throttle:");
    Serial.println(throttle);
#endif
  }

  if (safe_ms > millis()) safe_ms = millis();
  if (millis() - safe_ms > SAFE_TIME_OUT) {
    MotorLeft.Free();
    MotorRight.Free();
    //MotorLeft.Brake();
    //MotorRight.Brake();
  }
}

boolean check_ble()
{
  
  for (int a = 0; a < 4; a++)
  {
    // Open serial communications and wait for port to open:
    mySerial.begin(BLE_SPEED);
    delay(200);
    mySerial.flush();

    Serial.print("2 Go.. ");
    mySerial.println("AT");
    delay(20);

    if (mySerial.findUntil("OK\r", "\n"))
    {
      Serial.println(" is OK");
      mySerial.flush();
      return true;
    }
  }
  {
    Serial.println(" is ERROR");
    mySerial.end();
    return false;
  }
}

