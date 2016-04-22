#include "audio.h"

Key KeyA(A6, INPUT);
Key KeyB(A6, INPUT);
Key KeyC(A6, INPUT);

#define music_num_MAX 9

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  int EEPROM_music_num;       //歌曲的数目
  int EEPROM_music_vol;       //歌曲的音量
};

#define init_draw 500  //主界面刷新时间
unsigned long timer_draw;

int MENU_FONT = 1;  //初始化字体大小 0：小，1：中，2：大

boolean music_status = false; //歌曲播放状态
int music_num = 1;    //歌曲序号
int music_vol = 26;             //音量0~30

void eeprom_WRITE()
{
  config_type config;      // 定义结构变量config，并定义config的内容
  config.EEPROM_music_num = music_num;
  config.EEPROM_music_vol = music_vol;

  EEPROM_write(0, config);  // 变量config存储到EEPROM，地址0写入
}

void eeprom_READ()
{
  config_type config_readback;
  EEPROM_read(0, config_readback);
  music_num = config_readback.EEPROM_music_num;
  music_vol = config_readback.EEPROM_music_vol;
}

void music_play()
{
  switch (KeyA.read(0, 80)) {
    case SHORT_PRESS:
      delay(200);
      music_status = !music_status;
      music_status ? audio_play() : audio_pause();
      break;
  }
  switch (KeyB.read(80, 180)) {
    case SHORT_PRESS:
      music_status = true;
      music_num++;
      if (music_num > music_num_MAX)
        music_num = 1;
      audio_choose(music_num);
      audio_play();
      eeprom_WRITE();
      break;
    case LONG_PRESS:
      music_status = true;
      music_vol++;
      if (music_vol > 30) music_vol = 1;
      audio_vol(music_vol);
      eeprom_WRITE();
      delay(500);
      break;
  }
  switch (KeyC.read(180, 280)) {
    case SHORT_PRESS:
      music_status = true;
      music_num--;
      if (music_num < 1)
        music_num = music_num_MAX;
      audio_choose(music_num);
      audio_play();
      eeprom_WRITE();
      break;
    case LONG_PRESS:
      music_status = true;
      music_vol--;
      if (music_vol < 1) music_vol = 30;
      audio_vol(music_vol);
      eeprom_WRITE();
      delay(500);
      break;
  }
  delay(15);

  if (millis() - timer_draw > init_draw)
  {
    u8g.firstPage();
    do {
      setFont_L;

      u8g.setPrintPos(4, 16);
      u8g.print("Music_sta:");
      u8g.print(music_status ? "play" : "pause");

      u8g.setPrintPos(4, 16 * 2);
      u8g.print("Music_vol:");
      u8g.print(music_vol);
      u8g.print("/30");
      u8g.setPrintPos(4, 16 * 3);
      u8g.print("Music_num:");
      u8g.print(music_num);
      u8g.print("/");
      u8g.print(music_num_MAX);
      u8g.setPrintPos(4, 16 * 4);
      u8g.print("....Microduino....");
    }
    while ( u8g.nextPage() );
    timer_draw = millis();
  }
}

