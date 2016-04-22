#include "U8glib.h"

#include <Wire.h>
#include "I2Cdev.h"

#include <AM2321.h>
AM2321 am2321;

#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);

#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);

#define door_pin 4
#include <Microduino_Key.h>
Key KeyA(door_pin, INPUT_PULLUP);

int lux_max = 1000;
#define lux_c 5

#include "weather.h"
#include "oled.h"

#define servo_max 150
#define servo_min 90

#define servo_pin 8

#define INTERVAL_SENSOR  50    //定义传感器采样时间间隔  
#define INTERVAL_LCD     100   //定义OLED刷新时间间隔  
#define INTERVAL_BUTTON 1000  //定义按键关闭时间间隔  


unsigned long lcd_time = millis();                 //OLED刷新时间计时器
unsigned long sensor_time = millis();             //传感器采样时间计时器
unsigned long button_time = millis();

boolean door_on, door_off, door_press;
boolean door_up, door_down;
boolean door;

void servopulse(int sp1, int val1) //Define a pulse function
{
  int myangle1 = map(val1, 0, 180, 500, 2480);

  digitalWrite(sp1, HIGH); //set servo interface HIGH
  delayMicroseconds(myangle1);
  digitalWrite(sp1, LOW); //set servo interface LOW
  delay(20 - val1 / 1000);
}

void colorSet(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void servo_up(int wait)
{
  for (int pos = servo_min; pos < servo_max; pos += 1) // goes from 0 degrees to 180 degrees
  {
    servopulse(servo_pin, pos);
    delay(wait);                       // waits 15ms for the servo to reach the position
  }
}
void servo_down(int wait)
{
  for (int pos =  servo_max; pos >= servo_min; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    servopulse(servo_pin, pos);             // tell servo to go to position in variable 'pos'
    delay(wait);                       // waits 15ms for the servo to reach the position
  }
}

void setup() {
  Serial.begin(115200);
  strip.begin();
  colorSet(strip.Color(0, 0, 0));
  tsl_init();
  pinMode(servo_pin, OUTPUT);
  servopulse(servo_pin, servo_min);
}

void loop() {
  Serial.println(door);

  if (digitalRead(door_pin))
  {
    door = true;
    door_on = false;
    button_time = millis();
  }
  else
  {
    if (button_time > millis()) button_time = millis();
    if (millis() - button_time > INTERVAL_BUTTON)             //传感器采样时间间隔
    {
      door_press = false;//开关按下执行一次
      door = false;//开关关
      door_up = false;//上可以执行
      door_down = false;//下可以执行

      button_time = millis();
    }
  }

  if (door  && !door_press && !door_up && sensor_lux > lux_max - lux_c)
  {
    door_off = true;
    servo_up(15);
    door_up = true;
    door_down = false;
  }

  if (door  && !door_press && !door_down && sensor_lux < (lux_max / 2) )
  {
    door_off = false;
    servo_down(15);
    door_down = true;
    door_up = false;
  }

  if (KeyA.read() == SHORT_PRESS && !door_press)
  {
    door_press = true;//只能按一次
    door = false;//关门了
    door_on = true;//关闭不让门又下一次
    door_off = false;//关闭不让门又下一次
    servo_down(15);
  }

  if (door)
  {
    if (sensor_hum < 50)
      colorSet(strip.Color(0, 100, 0));
    else if (sensor_hum > 50 && sensor_hum < 75)
      colorSet(strip.Color(0, 100, 100));
    else if (sensor_hum > 75)
      colorSet(strip.Color(100, 100, 100));
  }
  else if (!door && !door_on)
  {
    colorSet(strip.Color(0, 0, 0));
    if (door_off)
    {
      servo_down(15);
      door_off = false;
    }
    door_on = true;
  }

  if (sensor_time > millis()) sensor_time = millis();
  if (millis() - sensor_time > INTERVAL_SENSOR)             //传感器采样时间间隔
  {
    updateSensorData();
    sensor_time = millis();
  }

  if (lcd_time > millis()) lcd_time = millis();
  if (millis() - lcd_time > INTERVAL_LCD)
  {
    volcd();                       //调用显示库
    lcd_time = millis();
  }
  delay(20);
}
