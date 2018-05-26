#include <Servo.h>
#include "stackJson.h"

Servo servo_door;

boolean again;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  servo_door.attach(9);
  servo_door.write(UPangle);
  delay(2000);

}

void loop() {
  // windmilljson_data();
  if (!digitalRead(6))
    gamestart = true;

  if (gamestart)
  {
    if (!again)
    {
      servo_door.write(DOWNangle);
      delay(UPtime);
      servo_door.write(UPangle);
      delay(firstUPtime);
      servo_door.write(DOWNangle);
      delay(DOWNtime);
      servo_door.write(UPangle);
      delay(UPtime);
      again = true;
    }
    servo_door.write(DOWNangle);
    delay(DOWNtime);
    servo_door.write(UPangle);
    delay(UPtime);
    nunber++;
  }
  else
    servo_door.write(UPangle);
}
