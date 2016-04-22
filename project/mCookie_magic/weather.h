#include"Arduino.h"

float sensor_tem, sensor_hum, sensor_light, sensor_Etoh; //温度、湿度,光照

char s_data;

void updateSensorData() {

  am2321.read();
  sensor_tem = am2321.temperature / 10.0;
  sensor_hum = am2321.humidity / 10.0;

  sensor_light = map(analogRead(A0), 0, 1023, 0, 255);
  sensor_Etoh = map(analogRead(A2), 0, 1023, 0, 255);
  /*
    Serial.print(sensor_light);
    Serial.print("   ,   ");
    Serial.println(sensor_Etoh);

    if (sensor_tem > 30)
      s_data = 1;
    else
      s_data = 0;
  */
}

boolean check_temp()
{
  am2321.read();
  sensor_tem = am2321.temperature / 10.0;
  sensor_hum = am2321.humidity / 10.0;
  if (sensor_tem > 10 || sensor_hum > 10)
    return true;
  else
    return false;
}
