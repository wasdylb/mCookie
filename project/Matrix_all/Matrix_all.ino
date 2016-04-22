#include <Wire.h>
#include <AM2321.h>

AM2321 am2321;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5);

//#define my_serial mySerial
#define my_serial Serial1

#include <Microduino_Key.h>

#include "Microduino_Matrix.h"

uint8_t Addr[MatrixPix_X][MatrixPix_Y] = {
  { 64}
};

Matrix display = Matrix(Addr);

#include "mic.h"
#include "Temp.h"
#include "ble.h"

Key KeyA(6, INPUT_PULLUP);

int num, num_c=-1;

void setup() {
  Serial.begin(9600);
  my_serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  delay(6000);
  display.clearDisplay();
  display.clearFastMode();
  display.setColor(0, 100, 200);
  display.writeString("mCookie", MODE_H, 80, 0);
  display.clearDisplay();
  am2321.read();
  hum = am2321.temperature / 10.0;
  temp = am2321.humidity / 10.0;
  temp0 = temp;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (KeyA.read()==SHORT_PRESS)
  {
    num++;
    if (num > 2)
      num = 0;
  }

  if (num_c != num)
  {
    num_c = num;
    display.clearDisplay();
    if (num == 0)
      display.writeString("mic", MODE_H, 80, 0);
    else if (num == 1)
      display.writeString("temp", MODE_H, 80, 0);
    else if (num == 2)
      display.writeString("ble", MODE_H, 80, 0);
  }

  if (num == 0)
    mic_led();
  else if (num == 1)
    temp_read();
  else if (num == 2)
    ble_data();
  }
