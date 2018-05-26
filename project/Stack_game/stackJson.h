#include <aJSON.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

aJsonStream serial_stream(&mySerial);

String startS = "";
String DOWNangleS = "";
String UPangleS = "";
String DOWNtimeS = "";
String UPtimeS = "";
String firstUPtimeS = "";
String projtctid = "";

boolean gamestart;
boolean ssleep;
//uint8_t DOWNangle = 5;
//uint8_t UPangle = 20;
//uint16_t DOWNtime = 250;
//uint16_t UPtime = 550;
//uint16_t firstUPtime = 680;

uint8_t DOWNangle = 0;
uint8_t UPangle = 13;
uint16_t DOWNtime = 250;
uint16_t UPtime = 550;
uint16_t firstUPtime = 750;

String ID = "{\"ID\":\"59e56a3833f26d0001fd83cc\"}";

uint8_t nunber;

void IDupload()
{
  mySerial.print(ID.substring(0, 17));
  delay(30);
  mySerial.println(ID.substring(17, ID.length()));
  delay(30);
}

void Ctrldata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    mySerial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      mySerial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    mySerial.print("\n");
  }
}
//{"ID":"project"}
//{"DOWNangle":"5"}
//{"UPangle":"20"}
//{"DOWNtime":"250"}
//{"UPtime":"550"}
//{"firstUPtime":"680"}

void ComExecution(aJsonObject * msg)
{
  aJsonObject *IDval = aJson.getObjectItem(msg, "ID");
  if (IDval) {
    projtctid = IDval->valuestring;
    if (projtctid == "project")
    {
      IDupload();
      Ctrldata("DOWNangle", DOWNangle);
      Ctrldata("UPangle", UPangle);
      Ctrldata("DOWNtime", DOWNtime);
      Ctrldata("UPtime", UPtime);
      Ctrldata("firstUPtime", firstUPtime);
    }
  }

  aJsonObject *DOWNangleval = aJson.getObjectItem(msg, "DOWNangle");
  if (DOWNangleval) {
    DOWNangleS = DOWNangleval->valuestring;
    DOWNangle = DOWNangleS.toInt();
    Ctrldata("DOWNangle", DOWNangle);
  }

  aJsonObject *UPangleval = aJson.getObjectItem(msg, "UPangle");
  if (UPangleval) {
    UPangleS = UPangleval->valuestring;
    UPangle = UPangleS.toInt();
    Ctrldata("UPangle", UPangle);
  }

  aJsonObject *DOWNtimeval = aJson.getObjectItem(msg, "DOWNtime");
  if (DOWNtimeval) {
    DOWNtimeS = DOWNtimeval->valuestring;
    DOWNtime = DOWNtimeS.toInt();
    Ctrldata("DOWNtime", DOWNtime);
  }

  aJsonObject *UPtimeval = aJson.getObjectItem(msg, "UPtime");
  if (UPtimeval) {
    UPtimeS = UPtimeval->valuestring;
    UPtime = UPtimeS.toInt();
    Ctrldata("UPtime", UPtime);
  }

  aJsonObject *firstUPtimeval = aJson.getObjectItem(msg, "firstUPtime");
  if (firstUPtimeval) {
    firstUPtimeS = firstUPtimeval->valuestring;
    firstUPtime = firstUPtimeS.toInt();
    Ctrldata("firstUPtime", firstUPtime);
  }

  aJsonObject *startval = aJson.getObjectItem(msg, "start");
  if (startval) {
    startS = startval->valuestring;
    if (startS == "true")
      gamestart = true;
  }

}

void windmilljson_data()
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

