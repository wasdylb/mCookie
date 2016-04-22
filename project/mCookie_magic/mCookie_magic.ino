#include "U8glib.h"
#include <EEPROM.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>//添加必须的库文件
#include <Adafruit_NeoPixel.h>
//#include <Microduino_Protocol_SoftSer.h>
#include <Microduino_Protocol_HardSer.h>
#include <Microduino_Motor.h>
#include <SoftwareSerial.h>
#include <Microduino_Key.h>
#include <AM2321.h>
AM2321 am2321;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号

#include "mpu.h"
#include "robot.h"
#include "weather.h"
#include "lcdoled.h"
#include "music.h"

uint8_t sys_mode;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  accelgyro.initialize();
  if (accelgyro.testConnection())
  {
    sys_mode = 1;
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
  }

  else if (check_ble())
  {
    sys_mode = 2;
    bleProtocol.begin(BLE_SPEED);
    MotorLeft.Fix(motor_fixL);
    MotorRight.Fix(motor_fixR);
  }
  else if (check_temp())
  {
    sys_mode = 3;
    //u8g.setRot90();
  }
  else
  {
    sys_mode = 0;
    eeprom_READ();
    if (music_num > music_num_MAX)music_num = music_num_MAX;
    else if (music_num < 0)music_num = 1;
    if (music_vol > 30)music_vol = 30;
    else if (music_vol < 0)music_vol = 10;
    audio_init(DEVICE_TF, MODE_loopOne, music_vol);
  }
}

void loop() {
  if (sys_mode == 1)
    getMPU();
  else if (sys_mode == 2)
    ble_robot();
  else if (sys_mode == 3)
  {
    updateSensorData();
    volcd_weather();
  }
  else
    music_play();
}
