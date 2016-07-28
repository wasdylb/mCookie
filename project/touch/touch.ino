#include "touch.h"
#include "color.h"

int pin[4] = {
  7, 8, 9, 10
};

int  capval_sta[4];
void setup()
{
  Serial.begin(9600);
  strip.begin();//初始化彩灯，开始控制彩灯
  strip.setBrightness(BRIGHT_MAX);
}

void loop ()
{
  for (int i = 0; i < 4; i++)
  {
    capval_sta[i] = readCapacitivePin(pin[i]);
    Serial.print(capval_sta[i]);
    Serial.print(" , ");
    if (i == 3)
      Serial.println("");
  }
  if (capval_sta[0] > 10)
    setAllLed(COLOR_PURPLE);
  else if (capval_sta[1] > 10)
    ledRainbow(10);
  else if (capval_sta[2] > 10)
    ledBreath(COLOR_INDIGO, 1);
  else if (capval_sta[3] > 10)
    allLedBlinkNum(2, COLOR_WARM, 500);
  else
    setAllLed(COLOR_NONE);
}


