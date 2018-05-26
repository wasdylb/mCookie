#include "wifiJson.h"
#include <ArduinoJson.h>
#include <Microduino_ColorLED.h> //引用彩灯库

#define PIN            A0         //彩灯引脚
#define NUMPIXELS      12        //级联彩灯数量

ColorLED strip = ColorLED(NUMPIXELS, PIN);

#define SSID      "ylb"
#define PASSWORD  "ylb12345"

const char* client_id = "mCookieLED";

const char* mqtt_server = "192.168.10.152";
const char* mqtt_username = "ylb";
const char* mqtt_password = "microduino";

const char* light_state_topic = "home/light1";
const char* light_set_topic = "home/light1/set";

const char* LEDON = "ON";
const char* LEDOFF = "OFF";

int brightness;
boolean stateOn;

void msgMqtt(String _str) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(_str);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return false;
  }

  if (root.containsKey("state")) {
    if (strcmp(root["state"], LEDON) == 0) {
      stateOn = true;
    }
    else if (strcmp(root["state"], LEDOFF) == 0) {
      stateOn = false;
    }
  }

  if (root.containsKey("brightness")) {
    brightness = root["brightness"];
    strip.setBrightness(brightness);
  }

}

void setup() {
#if DEBUG
  Serial.begin(115200);
#endif
  strip.begin();
  strip.setAllLED(COLOR_NONE);
  strip.show();
  boolean wifista = wifimCottonbegin(SSID, PASSWORD, mqtt_server, client_id, mqtt_username, mqtt_password, light_set_topic);
  strip.setAllLED(COLOR_GREEN);
  strip.show();
  delay(1000);
  strip.setAllLED(COLOR_NONE);
  strip.show();
}

void loop() {

  String mCottenData = wifi.getMqttJson();
  mCottenData.trim();
  if (mCottenData.startsWith("{") && mCottenData.endsWith("}")) {

#if DEBUG
    Serial.println(mCottenData);
#endif

    msgMqtt(mCottenData);

    if (stateOn) {
      strip.setAllLED(0x00FFFF);
      strip.show();
    }
    else {
      strip.setAllLED(COLOR_NONE);
      strip.show();
    }

    if (mCottenData.equals("WIFI DISCONNECT")) {
      wifi.setWiFiconnected(false);
      wifi.setMqttConnected(false);
    } else if (mCottenData.equals("WIFI Connected")) {
      wifi.setWiFiconnected(true);
    } else if (mCottenData.equals("MQTT: Disconnected")) {
      wifi.setMqttConnected(false);
    } else if (mCottenData.equals("MQTT: Connected")) {
      wifimCottonagain(mqtt_server, client_id, mqtt_username, mqtt_password, light_set_topic);
      wifi.setMqttConnected(true);
    }
  }

}
