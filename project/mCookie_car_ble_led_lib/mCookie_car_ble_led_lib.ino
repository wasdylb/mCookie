#include <Microduino_Motor.h>
#if defined(__AVR_ATmega32U4__) || (__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define motor_pin0A 8
#define motor_pin0B 6
#define motor_pin1A 7
#define motor_pin1B 5
#else
#define motor_pin0A 6
#define motor_pin0B 8
#define motor_pin1A 5
#define motor_pin1B 7
#endif
Motor MotorLeft(motor_pin0A, motor_pin0B);
Motor MotorRight(motor_pin1A, motor_pin1B);
#define MAX_THROTTLE 255
#define MAX_STEERING 200
int16_t throttle = 0;
int16_t steering = 0;

unsigned long timer;
unsigned long timer_start;
int i, num, num_c;
boolean rc, lc;

int steer, steer_c;

#include <Adafruit_NeoPixel.h>

#define PIN A0   //led灯控制引脚
#define PIN_NUM 6 //允许接的led灯的个数

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIN_NUM, PIN, NEO_GRB + NEO_KHZ800);

#include <Microduino_Protocol_HardSer.h>

Protocol ProtocolB(&Serial1, TYPE_NUM);
uint16_t Data[8];

void colorWipe(uint32_t c, int i) {
  strip.setPixelColor(i, c);
  strip.show();
}

void go_f()
{
  if (millis() - timer > 200)
  {
    timer = millis();
    colorWipe(strip.Color(0, 40, 0), 6 - i);
    colorWipe(strip.Color(0, 40, 0), 6 - i + 1);
    i += 2;
    if (i > 8)
    {
      i = 0;
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
      }
    }
  }
}

void go_b()
{
  if (millis() - timer > 200)
  {
    timer = millis();
    colorWipe(strip.Color(100, 0, 0), i);
    colorWipe(strip.Color(100, 0, 0), i + 1);
    i += 2;
    if (i > 6)
    {
      i = 0;
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
      }
    }
  }
}

void go_l()
{
  if (millis() - timer > 300)
  {
    timer = millis();
    lc = !lc;
    if (lc)
    {
      colorWipe(strip.Color(100, 0, 0), 1);
      colorWipe(strip.Color(100, 0, 0), 3);
      colorWipe(strip.Color(100, 0, 0), 5);
      colorWipe(strip.Color(100, 0, 0), 0);
    }
    else
    {
      colorWipe(strip.Color(0, 0, 0), 1);
      colorWipe(strip.Color(0, 0, 0), 3);
      colorWipe(strip.Color(0, 0, 0), 5);
      colorWipe(strip.Color(0, 0, 0), 0);
    }
  }
}

void go_r()
{
  if (millis() - timer > 300)
  {
    timer = millis();
    rc = !rc;
    if (rc)
    {
      colorWipe(strip.Color(100, 0, 0), 0);
      colorWipe(strip.Color(100, 0, 0), 2);
      colorWipe(strip.Color(100, 0, 0), 4);
      colorWipe(strip.Color(100, 0, 0), 1);
    }
    else
    {
      colorWipe(strip.Color(0, 0, 0), 0);
      colorWipe(strip.Color(0, 0, 0), 2);
      colorWipe(strip.Color(0, 0, 0), 4);
      colorWipe(strip.Color(0, 0, 0), 1);
    }
  }
}
void stop()
{
  for (int j = 0; j < 6; j++)
    colorWipe(strip.Color(0, 0, 0), j);
}

void setup()
{
  MotorLeft.Fix(-1);
  MotorRight.Fix(-1);
  delay(1000);
  strip.begin();
  ProtocolB.begin(9600);
  Serial.begin(9600);
  //f
  if (millis() != timer_start) timer_start = millis();
  while (millis() - timer_start <= 2 * 1000)
  {
    MotorLeft.Driver(MotorLeft.GetData(125, 0, CHAN_LEFT));
    MotorRight.Driver(MotorRight.GetData(125, 0, CHAN_RIGHT));
    go_f();
  }
  stop();
  //b
  if (millis() != timer_start) timer_start = millis();
  while (millis() - timer_start <= 2 * 1000)
  {
    MotorLeft.Driver(MotorLeft.GetData(-80, 0, CHAN_LEFT));
    MotorRight.Driver(MotorRight.GetData(-80, 0, CHAN_RIGHT));
    go_b();
  }
  stop();
  //l
  if (millis() != timer_start) timer_start = millis();
  while (millis() - timer_start <= 2 * 1000)
  {
    MotorLeft.Driver(MotorLeft.GetData(0, -170, CHAN_LEFT));
    MotorRight.Driver(MotorRight.GetData(0, -170, CHAN_RIGHT));
    go_l();
  }
  stop();
  //r
  if (millis() != timer_start) timer_start = millis();
  while (millis() - timer_start <= 2 * 1000)
  {
    MotorLeft.Driver(MotorLeft.GetData(0, 170, CHAN_LEFT));
    MotorRight.Driver(MotorRight.GetData(0, 170, CHAN_RIGHT));
    go_r();
  }
  MotorLeft.Driver(MotorLeft.GetData(0, 0, CHAN_LEFT));
  MotorRight.Driver(MotorRight.GetData(0, 0, CHAN_RIGHT));
  stop();
}

void loop()
{
  int sta = ProtocolB.parse(Data, MODE_WHILE);
  if (sta == P_FINE) {
    for (int a = 0; a < CHANNEL_NUM; a++) {
      Serial.print(Data[a]);
      Serial.print(" ");
    }
    Serial.println("    DATA OK");
    if (Data[2] == 1500 && Data[3] == 1500)
    {
      steer = 0;
      stop();
    }
    else if (Data[2] < 1500)
    {
      if (Data[3] > 1500)
      {
        if (Data[2] == 1000 || ((Data[3] - 1500) < (1500 - Data[2]))) //left
          steer = 3;
        //go_l();
        else
          steer = 1;
        //go_f();
      }
      else if (Data[3] < 1500)
      {
        if (Data[2] == 1000 || ((1500 - Data[3]) < (1500 - Data[2]))) //left
          steer = 3;
        //go_l();
        else
          steer = 2;
        //go_b();
      }
    }
    else if (Data[2] > 1500)
    {
      if (Data[3] > 1500)
      {
        if (Data[2] == 2000 || ((Data[3] - 1500) < (Data[2] - 1500))) //left
          steer = 4;
        // go_r();
        else
          steer = 1;
        //go_f();
      }
      else if (Data[3] < 1500)
      {
        if (Data[2] == 2000 || ((1500 - Data[3]) < (Data[2] - 1500))) //left
          steer = 4;
        //go_r();
        else
          steer = 2;
        //go_b();
      }
    }
    if (steer_c != steer)
    {
      stop();
      steer_c = steer;
    }
    switch (steer)
    {
      case 0:
        stop();
        break;
      case 1:
        go_f();
        break;
      case 2:
        go_b();
        break;
      case 3:
        go_l();
        break;
      case 4:
        go_r();
        break;
    }
    throttle = map(Data[3], 1000, 2000, -255, 255);
    steering = map(Data[2], 1000, 2000, -255, 255);
    MotorLeft.Driver(MotorLeft.GetData(throttle, steering, CHAN_LEFT));
    MotorRight.Driver(MotorRight.GetData(throttle, steering, CHAN_RIGHT));
  }
  delay(10);
}

