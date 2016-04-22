/*
  =================================
  本作品采用知识共享 署名-非商业性使用-相同方式共享 3.0 未本地化版本 许可协议进行许可
  访问 http://creativecommons.org/licenses/by-nc-sa/3.0/ 查看该许可协议
  版权所有：
  @老潘orz  wasdpkj@hotmail.com
  =================================

  Microduino wiki:
  http://wiki.microduino.cc

  包含程序库、硬件支持包的IDE可在Getting Start中下载:
  http://www.microduino.cc/project/view?id=5482ba0248f8313548d82b8c

  串口信息（Serial Monitor），115200 baud，Both NL & CR

  所需硬件：
  Microduino Core+
  Microduino USBTLL
  Microduino SD
  Microduino RTC
  Microduino NFC
  Microduino OLED
  Solo底板
  矩阵键盘
  舵机
  钥匙检测传感器


  功能：
  一个钥匙借还系统，学生利用NFC刷校园卡，刷卡后用键盘输入银行卡号，即可开门借走钥匙；
  同理，学生可以刷卡还钥匙，有未还钥匙记录的，可不用再次输入银行卡，直接开门借还钥匙；
  系统会在关门时，将钥匙校园卡、银行卡、借走时间、还回时间、借走时长等信息记录到对应钥匙的xml数据库中；
  教师可以取下sd卡，得到数据库信息。

  校正时间：
  系统用到了RTC模块，用户需要在第一次使用时校正时间；
  首先下载NFC Tools（Android）软件，给对应有管理员权限的NFC标签写入文本信息；
  格式如下：{Year,Month,Day,Hour,Minute}，比如：{2015,03,14,15,15}；
  然后即可刷该NFC标签给系统校正时间。

*/

//================
#include "user.h"
#include "uid.h"
#include "RFID.h"
#include "door.h"
#include "eeprom.h"
#include "draw.h"

#include <Wire.h>

#include <Microduino_Key.h>

Key KeyA(6, INPUT_PULLUP);

//EEPROM=======================
#include <EEPROM.h>
//OLED=========================
#include "U8glib.h"

void setup() {
  Serial.begin(115200);  //初始化串口调试
  if (DOOR_PIN == 0)
    Serial.end();  //停用串口用作普通GPIO

  Door_begin();   //初始化门锁及其传感器
  uid_data_num = EEPROM.read(0);

  //RFID------------------------
  Wire.begin();
  rfid.init();
  //  Serial.println(get_data(uid_mother), HEX);
}

void loop() {
  uint32_t uid_data_cache = 0;
  //    if (step == 0) //在主界面时候，执行刷卡动作
  Serial.println("");
  Serial.println("===nfc_scan()===");
  uid_data_cache = RFID_Scan();
  if (uid_data_cache) {
    uid_data = uid_data_cache;
    //        step = 1;
    card_sta = true;    //开始刷卡
    Serial.println(F("\n====================="));
    Serial.print(F("    Card ["));
    Serial.print(uid_data, HEX);
    Serial.println(F("] is OK"));
    Serial.println(F("====================="));
    if (uid_data == get_data(uid_num_admin))
      eeprom_Clear_num++;
    else
      eeprom_Clear_num = 0;

    u8g.sleepOff();
    time_draw_sleep = millis();
    draw_switch = true;
  }
  else {
    Serial.println("NFC ERROR!");
  }

  ///////////////////////////////////////////////////////
  if (eeprom_Clear_num > 16) {
    eeprom_Clear_num = 0;

    Serial.println("Clear");
    step = 2;
    eeprom_Clear_time = millis();
  }
  if (step == 2) {
    if (millis() - eeprom_Clear_time > 3000) {
      for (int i = 0 ; i < (1 + (sizeof(uint32_t) * MAX_BANKCARD_NUM)) ; i++) {
        EEPROM.write(i, 0);
      }
      uid_data = 0;
      uid_data_num = 0;
      uid_time_admin = 0;
      step = 0;
    }
  }

  ///////////////////////////////////////////////////////
  if (uid_data == get_data(uid_num_admin) && !step) {
    step = 1;
    Serial.println("Mother OK!");
    uid_time_admin = millis();
  }
  if (step == 1) {
    if (millis() - uid_time_admin < 10000) {
      if (card_sta) {
        card_sta = false;
        if (uid_data != get_data(uid_num_admin) && !eeprom_Check(uid_data)) { //不是管理员卡 && 没有被收录过
          Serial.println("\t NEW");
          Serial.print("uid_data_num:");
          Serial.print(uid_data_num);
          Serial.print(",uid_data:");
          Serial.println(uid_data);
          uid_time_admin = millis();  //重新计时
          // eeprom_write();
          EEPROM.put(1 + (sizeof(uint32_t) * uid_data_num), uid_data);

          uid_data_num++; //序号递增
          if (uid_data_num >= MAX_BANKCARD_NUM) {
            Serial.println("eeprom out!");
            for (int i = 0 ; i < MAX_BANKCARD_NUM ; i++) {
              EEPROM.write(i, 0);
            }
          }
        }
      }
    }
    else {
      uid_data = 0;
      EEPROM.write(0, uid_data_num);
      Serial.println("\t EXIT");
      step = 0;
    }
  }

  ///////////////////////////////////////////////////////
  if (!step) {
    if (eeprom_Check(uid_data)) {
      step = 3;
      time_door = millis();
      uid_data = 0;
      Serial.println("\t OPEN");
      Door_Open();
      delay(500);
      Door_Close();
    }
  }

  if (KeyA.read() == SHORT_PRESS) {
    u8g.sleepOff();
    time_draw_sleep = millis();
    draw_switch = true;
    step = 3;
    time_door = millis();
    uid_data = 0;
    Serial.println("\t OPEN");
  }

  if (step == 3) {
    if (millis() < time_door) time_door = millis();
    if (millis() - time_door > 3000) {
      step = 0;
    }
  }

  drawMain(step, false);  //定时器刷新界面
  rfid.halt();
}
