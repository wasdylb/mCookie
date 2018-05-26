#include <ESP8266.h>
#define DEBUG  1

/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif

/**
**Core UART Port: [SoftSerial] [D2,D3]
**/
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */

#define EspSerial mySerial
#define UARTSPEED  9600
#endif

ESP8266 wifi(&EspSerial);

bool wifimCottonbegin(const char* ssid, const char* pass, const char* mqserver, const char* mqid, const char* mquser, const char* mqpass , const char* subcom)
{
  WifiInit(EspSerial, UARTSPEED);
  Serial.print(F("Start Connection homeassistant...Waiting for 5S...\r\n"));
  //--------------------1------------------//
  while (!wifi.setOprToStation());
#if DEBUG
  Serial.print(F("to station ok\r\n"));
#endif
  //--------------------2------------------//
  while (!wifi.joinAP(ssid, pass));
  wifi.setWiFiconnected(true);
#if DEBUG
  Serial.print(F("Join AP success\r\n"));
#endif
  //--------------------3------------------//
  while (!wifi.mqttSetServer(mqserver, (1883)));
#if DEBUG
  Serial.print(F("mqtt set server ok\r\n"));
#endif
  //--------------------4------------------//
  while (!wifi.mqttConnect(mqid, mquser, mqpass));
  wifi.setMqttConnected(true);
#if DEBUG
  Serial.print(F("mqtt connect ok\r\n"));
#endif
  //--------------------5------------------//
  while (!wifi.mqttSetSubscrib(subcom));
#if DEBUG
  Serial.print(F("mqtt set subscrib topic ok\r\n"));
#endif
}

bool wifimCottonagain(const char* mqserver, const char* mqid, const char* mquser, const char* mqpass , const char* subcom)
{
  //--------------------3------------------//
  while (!wifi.mqttSetServer(mqserver, (1883)));
#if DEBUG
  Serial.print(F("mqtt set server ok\r\n"));
#endif
  //--------------------4------------------//
  while (!wifi.mqttConnect(mqid, mquser, mqpass));
  wifi.setMqttConnected(true);
#if DEBUG
  Serial.print(F("mqtt connect ok\r\n"));
#endif
  //--------------------5------------------//
  while (!wifi.mqttSetSubscrib(subcom));
#if DEBUG
  Serial.print(F("mqtt set subscrib topic ok\r\n"));
#endif
}
