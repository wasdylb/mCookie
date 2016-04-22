#include "arduino.h"
#include <Servo.h>
Servo myservo;

#define servo_pin 8

#define servo_max 25//开
#define servo_min 110//关

#define DOOR_TIMEOUT 10000  //10s

#define DOOR_SENSOR_PIN A0
#define DOOR_PIN 0

unsigned long time_door;

void Door_begin() {
  pinMode(DOOR_PIN, OUTPUT);
  digitalWrite(DOOR_PIN, LOW);
  myservo.attach(servo_pin);
}

void Door_Open() {
  Serial.println("Door_Open");
  digitalWrite(DOOR_PIN, HIGH);
  //delay(2000);
}

void Door_Close() {
  Serial.println("Door_Close");
  digitalWrite(DOOR_PIN, LOW);
  
  //delay(2000);
}

boolean Door_Check() {
  Serial.println("Door_Check");
  //  //  Serial.print("\nDoor_Check Program IN!");
  //  return digitalRead(DOOR_SENSOR_PIN);
}
