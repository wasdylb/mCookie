#include"Arduino.h"

float sensor_tem, sensor_hum, sensor_lux; //温度、湿度、海拔、气压、光照

void tsl_init()
{
  //初始化-光照
  Serial.println(tsl.begin() ? "TSL2561 successful" : "TSL2561 failed");
  tsl.enableAutoGain(true);                                  // 在1x 与16x 增益中切换
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      //13MS的采样速度

  sensors_event_t event;
  tsl.getEvent(&event);
  (event.light) ?  lux_max = event.light : Serial.println("Sensor overload");
}

void updateSensorData() {
  //获取温湿度==============================================
  am2321.read();
  sensor_tem = am2321.temperature / 10.0;
  sensor_hum = am2321.humidity / 10.0;

  //获取光照===============================================

  sensors_event_t event;
  tsl.getEvent(&event);
  (event.light) ?  sensor_lux = event.light : Serial.println("Sensor overload");
}
