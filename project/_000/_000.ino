#include "HX711.h"
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#include <Servo.h>


//硬件定义===============================================

Servo myservo;  // 创建一个舵机，命名myservo

int pos = 0;// 设置舵机角度的变量pos
float warnWeight = 100;// 设置体重报警值
float curWeight = 0; //设置当前重量变量curWeight


void setup()
{
  myservo.attach(9);// 将舵机接口定义为9
  Init_Hx711();
  Get_Maopi();// 获取初始重量
  myservo.write(0);// 设置舵机初始角度
  delay(2000);// 设置舵机
}
void loop() {
  curWeight = Get_Weight();
  Serial.println(curWeight);
  if (curWeight >= warnWeight)
    myservo.write(90);           
  else
    myservo.write(0);

  u8g.firstPage();
  do {
    //u8g.setFont(u8g_font_fub20);
    u8g.setPrintPos(4, 32);

    if (curWeight >= warnWeight) {
      u8g.print("!!!!!!!");
    }
    else {
      u8g.print(curWeight);
      u8g.print("g");
    }
  } 
  while ( u8g.nextPage());
}


