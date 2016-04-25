/***************************************************
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Microduino_Key.h>
Key KeyA(A6, INPUT_PULLUP);
Key KeyB(A6, INPUT_PULLUP);
Key KeyC(A6, INPUT_PULLUP);

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#include <EEPROM.h>
#include <Wire.h>
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  int EEPROM_music;
  int EEPROM_val;
};

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins.
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  -1      // VS1053 reset pin (output)
#define BREAKOUT_CS     A3     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    A2      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 7     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
// create shield-example object!
//Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

#define INTERVAL_timer1 1000
#define INTERVAL_timer2 1000
unsigned long timer1lastTime = millis();
unsigned long timer2lastTime = millis();

boolean play_pose;
boolean clean = true;

int volnumber = 4;
int vol_num[11] = {120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20};
int musicnumber;
String music_num[15] = {"1.mp3", "2.mp3", "3.mp3", "4.mp3", "5.mp3", "6.mp3", "7.mp3", "8.mp3", "9.mp3", "10.mp3", "12.mp3", "12.mp3", "13.mp3", "14.mp3", "15.mp3"};

void choose_music(const char *_name)
{
  musicPlayer.startPlayingFile(_name);
}

void eeprom_write()
{
  config_type config;// 定义结构变量config，并定义config的内容
  if (clean == true)
  {
    config.EEPROM_music = musicnumber;
    config.EEPROM_val = volnumber;
  }
  else
  {
    config.EEPROM_music = 0;
    config.EEPROM_val = 0;
  }
  EEPROM_write(0, config);         // 变量config存储到EEPROM，地址0写入
}

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  SD.begin(CARDCS);    // initialise the SD card

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  /*EEPROM读取赋值*/
  config_type config_readback;
  EEPROM_read(0, config_readback);

  musicnumber = config_readback.EEPROM_music;
  volnumber = config_readback.EEPROM_val;

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(vol_num[volnumber], vol_num[volnumber]);

  // Play one file, don't return until complete
  Serial.println(F("Playing track 001"));
  choose_music(music_num[musicnumber].c_str());
}

void loop() {
  //Serial.println(analogRead(A6));
  // put your main code here, to run repeatedly:

  if (KeyC.read(0, 30) == SHORT_PRESS)
  {
    play_pose = !play_pose;
    Serial.println("---play_pose---");
  }
  if (play_pose)
    musicPlayer.pausePlaying(false);//paly
  else
    musicPlayer.pausePlaying(true);//pose

  switch (KeyA.read(170, 270)) {
    case SHORT_PRESS:
      Serial.println("---down_SHORT_PRESS---");
      play_pose = true;
      musicnumber--;
      if (musicnumber < 0)
        musicnumber = 14;
      eeprom_write();
      choose_music(music_num[musicnumber].c_str());
      break;
    case LONG_PRESS:
      if (timer1lastTime > millis()) timer1lastTime = millis();
      if (millis() - timer1lastTime > INTERVAL_timer1) {
        Serial.println("---down_LONG_PRESS---");
        volnumber--;
        if (volnumber < 0)
          volnumber = 0;
        eeprom_write();
        musicPlayer.setVolume(vol_num[volnumber], vol_num[volnumber]);
        timer1lastTime = millis();
      }
      break;
  }

  switch (KeyB.read(270, 370)) {
    case SHORT_PRESS:
      play_pose = true;
      musicnumber++;
      if (musicnumber > 14)
        musicnumber = 0;
      eeprom_write();
      choose_music(music_num[musicnumber].c_str());
      Serial.println("---up_SHORT_PRESS---");
      break;
    case LONG_PRESS:
      if (timer2lastTime > millis()) timer2lastTime = millis();
      if (millis() - timer2lastTime > INTERVAL_timer2) {
        Serial.println("---up_LONG_PRESS---");
        volnumber++;
        if (volnumber > 10)
          volnumber = 10;
        eeprom_write();
        musicPlayer.setVolume(vol_num[volnumber], vol_num[volnumber]);
        timer2lastTime = millis();
      }
      break;
  }
  delay(15);
}
