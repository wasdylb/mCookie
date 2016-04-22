int temp0 = 30;
#define temp_c 3

float temp, hum;

int temp_h;
int i = -1;

int temp_s()
{
  if (temp < temp0 + temp_c)
    return 0;
  if (temp < temp0 + temp_c * 2 && temp > temp0 + temp_c)
    return 1;
  if (temp < temp0 + temp_c * 3 && temp > temp0 + temp_c * 2)
    return 2;
  if (temp < temp0 + temp_c * 4 && temp > temp0 + temp_c * 3)
    return 3;
  if (temp < temp0 + temp_c * 5 && temp > temp0 + temp_c * 4)
    return 4;
  if (temp < temp0 + temp_c * 6 && temp > temp0 + temp_c * 5)
    return 5;
  if (temp < temp0 + temp_c * 7 && temp > temp0 + temp_c * 6)
    return 6;
  if (temp < temp0 + temp_c * 8 && temp > temp0 + temp_c * 7)
    return 7;
  if (temp > temp0 + temp_c * 9)
    return 8;
}

void temp_read()
{
  am2321.read();
  hum = am2321.temperature / 10.0;
  temp = am2321.humidity / 10.0;

  if (temp_s() == 0)
    display.clearDisplay();
  else if (temp_s() > 0 && temp_s() <= 8)
  {
    for (int j = 0; j < 8; j++)
    {
      for (int i = 0; i <= temp_s() - 1; i++)
        display.setLedColor( j, 7 - i, 100, 0, 80);
      for (int i = temp_s(); i < 8 ; i++)
        display.setLedColor( j, 7 - i, 0, 0, 0);
    }
  }
  Serial.print("(");
  Serial.print(temp);
  Serial.print(", ");
  Serial.print(hum);
  Serial.print(", ");
  Serial.print(temp_s());
  Serial.println(')');
  delay(200);
}

