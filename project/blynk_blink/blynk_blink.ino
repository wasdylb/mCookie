/**************************************************************
   Blynk is a platform with iOS and Android apps to control
   Arduino, Raspberry Pi and the likes over the Internet.
   You can easily build graphic interfaces for all your
   projects by simply dragging and dropping widgets.

     Downloads, docs, tutorials: http://www.blynk.cc
     Blynk community:            http://community.blynk.cc
     Social networks:            http://www.fb.com/blynkapp
                                 http://twitter.com/blynk_app

   Blynk library is licensed under MIT license
   This example code is in public domain.

 **************************************************************
   You can use this sketch as a debug tool that prints all incoming values
   sent by a widget connected to a Virtual Pin 1 in the Blynk App.

   App dashboard setup:
     Slider widget (0...100) on V1

 **************************************************************/

#define BLYNK_PRINT Serial
#include <ESP8266_HardSer.h>
#include <BlynkSimpleShieldEsp8266_HardSer.h>
#include <Adafruit_NeoPixel.h>

#define PIN A0   //led灯控制引脚
#define PIN_NUM 6 //允许接的led灯的个数

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIN_NUM, PIN, NEO_GRB + NEO_KHZ800);

// Set ESP8266 Serial object
#define EspSerial Serial

ESP8266 wifi(EspSerial);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "b46e14a9f65845a98153218d6a9fb425";

void setup()
{
  Serial.begin(9600);
  // Set ESP8266 baud rate
  EspSerial.begin(115200);
  delay(10);
  strip.begin();
  Blynk.begin(auth, wifi, "Microduino_IoT", "MakerModule2016");
  Serial.println("start");
}

// This function will be called every time
// when App writes value to Virtual Pin 1
BLYNK_WRITE(V1)
{
  BLYNK_LOG("Got a value: %s", param.asStr());
  // You can also use:
  int i = param.asInt();//or
  Serial.println(i);
  // double d = param.asDouble()
  if (i)
  {
    strip.setPixelColor(0, strip.Color(255, 0, 0));//红
    strip.show();
  }
  else
  {
    strip.setPixelColor(0, strip.Color(0, 0, 0));//灭
    strip.show();
  }
}

void loop()
{
  Blynk.run();
}

