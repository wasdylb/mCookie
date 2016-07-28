#include <Adafruit_NeoPixel.h>//调用彩灯API库
#define PIN_LED     A0      //led灯控制引脚

#define LED_NUM 6           //允许接的led灯的个数
#define BRIGHT_MAX  128     //led灯的最大亮度

#define COLOR_MAX 255   //彩灯呼吸最大颜色
#define COLOR_MIN 0     //彩灯呼吸最小颜色

#define COLOR_NONE    0    //黑色（关灯）
#define COLOR_WARM    1    //暖色
#define COLOR_COLD    2    //冷色
#define COLOR_RED     3    //红色
#define COLOR_ORANGE  4    //橙色
#define COLOR_YELLOW  5  //黄色
#define COLOR_GREEN   6   //绿色
#define COLOR_BLUE    7    //蓝色
#define COLOR_INDIGO  8  //靛色
#define COLOR_PURPLE  9  //紫色
#define COLOR_WHITE   10   //白色

int led_pos = 0, led_num = 1;
unsigned long led_time = 0;

//定义彩灯颜色
const uint16_t colorArray[11][3] = {
  {0,   0,   0   },        //灭
  {155, 100, 0   },        //暖灯 (R,G,B)
  {100, 130, 100 },        //冷灯
  {255, 0,   0   },        //红
  {234, 139, 23  },        //橙
  {255, 255, 0   },        //黄
  {0,   255, 0   },        //绿
  {0,   255, 255 },        //青
  {0,   0,   255 },        //蓝
  {255, 0,   255 },        //紫
  {255, 255, 255 }         //白
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, PIN_LED, NEO_GRB + NEO_KHZ800);//定义彩灯控制方式

uint32_t rainTimer;       //存放A模式下彩灯切换时间
uint32_t breathTimer;     //存放A模式下彩灯切换时间
uint32_t blinkTimer;

//------------所有彩灯点亮函数-------------//
void setAllColor(uint32_t c)
{
  for (uint8_t i = 0; i < strip.numPixels(); i++)//选择所有灯
  {
    strip.setPixelColor(i, c);//设置指定灯颜色
  }
  strip.show();  //点亮
}

//------------所有彩灯点亮函数-------------//
void setAllLed(uint8_t color)
{
  setAllColor(strip.Color(colorArray[color][0], colorArray[color][1], colorArray[color][2]));
}


//------------单个彩灯点亮函数-------------//
void setColor(uint32_t c, uint8_t i)
{
  strip.setPixelColor(i, c);//设置指定灯颜色
  strip.show();  //点亮
}

//------------单个彩灯点亮函数-------------//
void setLed(uint8_t color, uint8_t i)
{
  setColor(strip.Color(colorArray[color][0], colorArray[color][1], colorArray[color][2]), i);
}


//------------颜色切换：红-蓝-绿循环-------------//
uint32_t Wheel(uint8_t  WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


//------------彩灯切换函数-------------//
void ledRainbow(uint8_t wait) {
  if (millis() - rainTimer > wait)
  {
    uint8_t i = (millis() / wait) & 0xFF;
    setAllColor(Wheel(i));
    rainTimer = millis();
  }
}


//------------彩灯呼吸，设置r,g,b值-------------//
void ledBreath(uint8_t color, uint8_t wait) {
  if (millis() < led_time) led_time = millis();
  if (millis() - led_time > wait) {
    led_pos += led_num;
    if (led_pos >= 255 || led_pos <= 0)
      led_num = -led_num;
    led_time = millis();
  }
  setAllColor(strip.Color(map(led_pos, COLOR_MIN, COLOR_MAX, 0, colorArray[color][0]), map(led_pos, COLOR_MIN, COLOR_MAX, 0, colorArray[color][1]), map(led_pos, COLOR_MIN, COLOR_MAX, 0, colorArray[color][2])));
}

void ledBlink(uint8_t wait, uint8_t color, uint8_t j)
{
  if (millis() - blinkTimer > wait)
  {
    if ((millis() / wait) % 2)
      setLed(color, j);
    else
      setLed(0, j);
    blinkTimer = millis();
  }
}

//------------彩灯闪烁，设置r,g,b值-------------//
void allLedBlinkNum(uint8_t num, uint8_t color, uint16_t wait)
{
  for (uint8_t i = 0; i < num; i++)
  {
    setAllLed(COLOR_NONE);
    delay(wait);
    setAllLed(color);
    delay(wait);
  }
  setAllLed(COLOR_NONE);
}

//------------彩灯闪烁，设置r,g,b值-------------//
void ledBlinkNum(uint8_t num, uint8_t color, uint8_t index, uint16_t wait)
{
  for (uint8_t i = 0; i < num; i++)
  {
    setLed(COLOR_NONE, index);
    delay(wait);
    setLed(color, index);
    delay(wait);
  }
  setLed(COLOR_NONE, index);
  delay(wait);
}


