#include <Microduino_Motor.h>
#if defined(__AVR_ATmega32U4__) || (__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define motor_pin0A 8
#define motor_pin0B 6
#define motor_pin1A 7
#define motor_pin1B 5
#else
#define motor_pin0A 6
#define motor_pin0B 8
#define motor_pin1A 5
#define motor_pin1B 7
#endif
Motor MotorLeft(motor_pin0A, motor_pin0B);
Motor MotorRight(motor_pin1A, motor_pin1B);
#define MAX_THROTTLE 255
#define MAX_STEERING 200
int16_t throttle = 0;
int16_t steering = 0;

#include <Microduino_Protocol_HardSer.h>

Protocol ProtocolB(&Serial1, TYPE_NUM);
uint16_t Data[8];
void setup()
{
  MotorLeft.Fix(1);
  MotorRight.Fix(1);
  delay(1000);

  ProtocolB.begin(9600);
  Serial.begin(9600);
}


void loop()
{

  int sta = ProtocolB.parse(Data, MODE_WHILE);
  if (sta == P_FINE) {
    for (int a = 0; a < CHANNEL_NUM; a++) {
      Serial.print(Data[a]);
      Serial.print(" ");
    }
    Serial.println("    DATA OK");
    throttle = map(Data[1], 1000, 2000, -255, 255);
    steering = map(Data[0], 1000, 2000, -255, 255);
    MotorLeft.Driver(MotorLeft.GetData(throttle, steering, CHAN_LEFT));
    MotorRight.Driver(MotorRight.GetData(throttle, steering, CHAN_RIGHT));
  }
  delay(10);

}
