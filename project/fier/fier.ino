#include <aJSON.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5); // RX, TX

aJsonStream serial_stream(&mySerial);

#define my_Serial mySerial

#define PIN 6

String startS = "";
boolean gamestart;

void ComExecution(aJsonObject * msg)
{
  aJsonObject *startval = aJson.getObjectItem(msg, "start");
  if (startval) {
    startS = startval->valuestring;
    Serial.println(startS);
    if (startS == "1")
      gamestart = true;
    else
      gamestart = false;
  }
}

void json_data()
{
  if (serial_stream.available()) {
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}

void setup() {
  Serial.begin(9600);
  my_Serial.begin(9600);
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
}

void loop() {

  json_data();

  if (gamestart)
    digitalWrite(PIN, HIGH);
  else
    digitalWrite(PIN, LOW);
    
  delay(1);
}

