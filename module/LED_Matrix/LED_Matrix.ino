#include "colorLed.h"

void setup() {
  Serial.begin(9600);
  strip.begin();//初始化彩灯，开始控制彩灯
  strip.setBrightness(BRIGHT_MAX);
}

void loop() {
  ledBreath(COLOR_GREEN, 15);//彩灯呼吸
  //ledRainbow(10);
  //setAllLed(COLOR_NONE);
}
