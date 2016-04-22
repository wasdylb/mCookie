Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, A0, NEO_GRB + NEO_KHZ800);

//DEBUG-----------
#define Serial_DEBUG
//====================
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

//float Ax, Ay, Az; //单位 g(9.8m/s^2)
//int Angel_accX, Angel_accY, Angel_accZ; //存储加速度计算出的角度

boolean led_motion;

void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void getMPU()
{
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //获取三个轴的加速度和角速度
  /*
    //======一下三行是对加速度进行量化，得出单位为g的加速度值
    Ax = ax / 16384.00;
    Ay = ay / 16384.00;
    Az = az / 16384.00;
    //==========以下三行是用加速度计算三个轴和水平面坐标系之间的夹角
    Angel_accX = atan(Ax / sqrt(Az * Az + Ay * Ay)) * 180 / 3.14;
    Angel_accY = atan(Ay / sqrt(Ax * Ax + Az * Az)) * 180 / 3.14;
    Angel_accZ = atan(Az / sqrt(Ax * Ax + Ay * Ay)) * 180 / 3.14;
      colorWipe(strip.Color(abs(Angel_accX) * 2, abs(Angel_accY) * 2, abs(Angel_accZ) * 2));
  */
  if ((abs(gx / 180) + abs(gy / 180) + abs(gz / 180)) / 3 > 120)
  {
    delay(300);
    // accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    if ((abs(gx / 180) + abs(gy / 180) + abs(gz / 180)) / 3 > 120)
      led_motion = !led_motion;
    if (led_motion)
      colorWipe(strip.Color(255, 200, 0));
    else
      colorWipe(strip.Color(0, 0, 0));
  }

  //==============================
#ifdef Serial_DEBUG
  delay(5);//这个用来控制采样速度
  Serial.print("    X:");
  Serial.print(abs(gx / 180));
  Serial.print("    Y:");
  Serial.print(abs(gy / 180));
  Serial.print("    Z:");
  Serial.println(abs(gz / 180));
  /*
    Serial.print("    X:");
    Serial.print(Angel_accX);
    Serial.print("    Y:");
    Serial.print(Angel_accY);
    Serial.print("    Z:");
    Serial.println(Angel_accZ);
  */
#endif
}
