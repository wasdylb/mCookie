#include <Arduino.h>

#define HX711_SCK 4
#define HX711_DT 5
#define GapValue 1030  //24.3

long Weight_Maopi = 0,Weight_Shiwu = 0;
      
//****************************************************
//初始化HX711
//****************************************************
void Init_Hx711()
{
  pinMode(HX711_SCK, OUTPUT); 
  pinMode(HX711_DT, INPUT);
}

//****************************************************
//读取HX711
//****************************************************
unsigned long HX711_Read(void)  //增益128
{
  unsigned long count; 
  unsigned char i;

  digitalWrite(HX711_DT, HIGH);
  delayMicroseconds(1);

  digitalWrite(HX711_SCK, LOW);
  delayMicroseconds(1);

    count=0; 
    while(digitalRead(HX711_DT)); 
    for(i=0;i<24;i++)
  { 
      digitalWrite(HX711_SCK, HIGH); 
    delayMicroseconds(1);
      count=count<<1; 
    digitalWrite(HX711_SCK, LOW); 
    delayMicroseconds(1);
      if(digitalRead(HX711_DT))
      count++; 
  } 
  digitalWrite(HX711_SCK, HIGH); 
  count ^= 0x800000;
  delayMicroseconds(1);
  digitalWrite(HX711_SCK, LOW); 
  delayMicroseconds(1);
  
  return(count);
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
  Weight_Maopi = HX711_Read();  
  if(Weight_Maopi!=0)
  {
    Serial.println("GET!");
  }
} 

//****************************************************
//称重
//****************************************************
float Get_Weight()
{
    Weight_Shiwu = (HX711_Read() - Weight_Maopi)+800;       //获取实物的AD采样数值。
    Serial.println(Weight_Shiwu);
    if(Weight_Shiwu < 0)
    {
      return 0;
    }
    else
    {
      return Weight_Shiwu/GapValue;
    }   
}

