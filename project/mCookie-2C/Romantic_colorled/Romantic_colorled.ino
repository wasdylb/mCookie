/*
   功能描述：乐高搭建灯塔，通过红外遥控改变灯的显示效果。
-------------------------------------------------------
   遥控器按钮说明：
     电源-关灯
     A-全彩呼吸
     B-暖色呼吸
     C-冷色呼吸
     D-单色不呼吸
     E-单色呼吸
     --单色颜色可选：红、橙、黄、绿、青、蓝、紫；
       D，E模式下左右调颜色
-------------------------------------------------------
   红外接收引脚：D6
   彩灯控制引脚：D8
*/

#include <Adafruit_NeoPixel.h>//调用彩灯API库
#include <IRremote.h>//调用红外接收库

#define RECV_PIN 6 //红外接收引脚
#define PIN 8   //led灯控制引脚
#define PIN_NUM 2 //允许接的led灯的个数

IRrecv irrecv(RECV_PIN);  //定义红外
decode_results results;

#define  power 0x1FE48B7  //电源
#define  mode_A 0x1FE807F //A按键
#define  mode_B 0x1FE40BF //B按键
#define  mode_C 0x1FEC03F //C按键
#define  mode_D 0x1FE20DF //D按键
#define  mode_E  0x1FE609F //E按键
#define  mode_left 0x1FEE01F //左按键
#define  mode_right 0x1FE906F //右按键

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIN_NUM, PIN, NEO_GRB + NEO_KHZ800);//定义彩灯控制方式

//定义彩灯颜色
uint32_t color_set[10][3] = {
  {155, 100, 0},//暖灯 (R,G,B)
  {100, 130, 100},//冷灯
  {255, 0, 0},//红
  {234, 139, 23},//橙
  {255, 255, 0},//黄
  {0, 255, 0},//绿
  {0, 255, 255},//青
  {0, 0, 255},//蓝
  {255, 0, 255},//紫
  {0, 0, 0}//关灯
};

unsigned long timer; //存放A模式下彩灯切换时间
unsigned long recv_code = 0; //存放红外编码
boolean color_single, color_breath;//判断单色调节模式
boolean one_single, one_breath;//判断左右切换颜色执行一次
int single_clor_number = 2;//单色不呼吸模式下颜色选择序号
int breath_clor_number = 5;//单色呼吸模式下颜色选择序号

int led_pos = 0, led_num = 1;//彩灯呼吸模式切换
unsigned long led_time = 0;//存放彩灯呼吸

#define val_max 255 //彩灯呼吸最大颜色
#define val_min 0  //彩灯呼吸最小颜色

//------------彩灯点亮函数-------------//
void colorSet(uint32_t c) {
  for (int i = 0; i < strip.numPixels(); i++)//选择所有灯
  {
    strip.setPixelColor(i, c);//设置指定灯颜色
    strip.show();  //点亮
  }
}

//------------颜色切换：红-蓝-绿循环-------------//
uint32_t Wheel(byte WheelPos) {
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
//------------彩灯呼吸，设置r,g,b值-------------//
void breath(int r, int g, int b) {
  if (millis() < led_time) led_time = millis();
  if (millis() - led_time > 10) {
    led_pos += led_num;
    if (led_pos >= 255 || led_pos <= 0)
      led_num = -led_num;
    led_time = millis();
  }
  colorSet(strip.Color(map(led_pos, val_min, val_max, 0, r), map(led_pos, val_min, val_max, 0, g), map(led_pos, val_min, val_max, 0, b)));
}

//------------彩灯切换函数-------------//
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    colorSet(Wheel(j & 255));
    if (millis() != timer) timer = millis();
    while (millis() - timer <= wait)
    {
      if (irrecv.decode(&results))
        return;
    }
  }
}
//------------红外接收函数-------------//
void recv()
{
  if (irrecv.decode(&results)) {//如果有红外信号
    if (results.value != 0xFFFFFFFF && results.value != 0)//排除乱码
      recv_code = results.value;//存储红外
    delay(100); //延时接收时间
    one_single = true; //允许D模式更换颜色
    one_breath = true; //允许E模式更换颜色
    if (recv_code == mode_D)//存储D模式
    {
      color_single = true; 
      color_breath = false;
    }
    else if (recv_code == mode_E)//存储E模式
    {
      color_single = false;
      color_breath = true;
    }
    else if (recv_code == power || recv_code == mode_A || recv_code == mode_B || recv_code == mode_C)//存储A，B，C，模式
    {
      color_single = false;
      color_breath = false;
    }
    if (recv_code == power)//电源键关灯
      colorSet(strip.Color(0, 0, 0));

    Serial.println(recv_code, HEX);//串口打印接收的红外信号值
    irrecv.resume(); // 接收下一个值
  }
}

//初始化函数，只执行一次
void setup() {
  Serial.begin(9600);
  strip.begin();//初始化彩灯，开始控制彩灯
  irrecv.enableIRIn();//初始化红外接收，开始接受数据
  //初始化所有颜色亮一段时间
  for (int i = 0; i < 10; i++)
  {
    colorSet(strip.Color(color_set[i][0], color_set[i][1], color_set[i][2]));//从数组里面选出颜色
    delay(500);//每个颜色持续时间
  }
}

void loop() {
  recv();//红外接收
  //------------------A，B，C三模式-----------------------//
  if (!color_single && !color_breath)
  {
    if (recv_code == mode_A)//判断A模式
      rainbow(20);//彩灯切换，每个颜色20ms时间
    if (recv_code == mode_B)//判断B模式
      breath(color_set[0][0], color_set[0][1], color_set[0][2]);//暖灯呼吸
    else if (recv_code == mode_C)//判断C模式
      breath(color_set[1][0], color_set[1][1], color_set[1][2]);//冷灯
  }
  //-------------------D 模式----------------------//
  else if (color_single && !color_breath)
  {
    if (one_single)//允许D模式更换颜色
    {
      if (recv_code == mode_left)//判断左边按钮按下
      {
        //单色不呼吸模式下颜色选择序号往左切换
        single_clor_number--;
        if (single_clor_number < 2)
          single_clor_number = 8;
      }
      if (recv_code == mode_right)//判断右边按钮按下
      {
        //单色不呼吸模式下颜色选择序号往左切换
        single_clor_number++;
        if (single_clor_number > 8)
          single_clor_number = 2;
      }
      //显示切换的颜色，从数组里面选择
      colorSet(strip.Color(color_set[single_clor_number][0], color_set[single_clor_number][1], color_set[single_clor_number][2]));
      one_single = false;//不允许D模式更换颜色，这样颜色是能切换一次
    }
  }
  //------------------E 模式------------------------//
  else if (!color_single && color_breath)
  {
    if (recv_code == mode_left && one_breath)//判断左边按钮按下
    {
      //单色呼吸模式下颜色选择序号往左切换
      breath_clor_number--;
      if (breath_clor_number < 2)
        breath_clor_number = 8;
    }
    if (recv_code == mode_right && one_breath)//判断右边按钮按下
    {
       //单色呼吸模式下颜色选择序号往右切换
      breath_clor_number++;
      if (breath_clor_number > 8)
        breath_clor_number = 2;
    }
    one_breath = false;////不允许E模式更换颜色，这样颜色是能切换一次
    //呼吸显示切换的颜色，从数组里面选择
    breath(color_set[breath_clor_number][0], color_set[breath_clor_number][1], color_set[breath_clor_number][2]);
  }
}
