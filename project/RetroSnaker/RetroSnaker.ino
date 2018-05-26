#include "Microduino_Matrix.h"
#include "snake.h"

static const uint8_t logoA[] PROGMEM = {0x00, 0x66, 0x66, 0xDB, 0xDB, 0xDB, 0xDB, 0x00};

uint8_t Addr[MatrixPix_X][MatrixPix_Y] = {
  { 64}
};

Matrix display = Matrix(Addr);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  display.drawBMP(0, 0, 8, 8, logoA);
  delay(3000);
  randomSeed(analogRead(0));
  snakeColor = 1;
  gameStatus = GAME_RUNNING; 
  gameReset();
  timer1Setup();
}

void loop() {
  if(gameStatus == GAME_RUNNING){  
     if(KeyE.read(0, 50) == SHORT_PRESS)
        gameStatus = GAME_PAUSE; 
     updateMap();
  }else if(gameStatus == GAME_PAUSE){
     if(KeyE.read(0, 50) == SHORT_PRESS)
        gameStatus = GAME_RUNNING; 
     updateMap();
  }else if(gameStatus == GAME_END){
     if(millis() > endTimer)
        updateScore();   
     if(KeyE.read(0, 50) == SHORT_PRESS){
        gameStatus = GAME_RUNNING;
        gameReset();
     }
   }  
}


void updateMap(){
  for(uint8_t pix = 0; pix < 8; pix++){
     for(uint8_t row = 0; row < 8; row++){
        if(snakeMap[row][pix] > 0)
           display.setLedColor(row, pix, colorArray[snakeColor][0], colorArray[snakeColor][1], colorArray[snakeColor][2]);
        else if(row==food[0] && pix==food[1])
           display.setLedColor(row, pix, colorArray[foodColor][0], colorArray[foodColor][1], colorArray[foodColor][2]); 
        else 
           display.setLedColor(row, pix, 0, 0, 0); 
     }  
  }  
}


void updateScore(){
  for(uint8_t pix = 0; pix < 8; pix++){
     for(uint8_t row = 0; row < 8; row++){
        if((scoreMap[pix]>>row)&0x01)
           display.setLedColor(7-row, pix, 160, 160, 0); 
        else
           display.setLedColor(7-row, pix, 0, 0, 80); 
     }          
  }     
}









