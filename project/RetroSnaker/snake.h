#include <TimerOne.h>
#include <Microduino_Key.h>

#define GAME_END        0
#define GAME_RUNNING    1
#define GAME_PAUSE      2

#define DIR_NONE        0
#define DIR_FORWARD     1
#define DIR_BACK        2
#define DIR_RIGHT       3
#define DIR_LEFT        4

Key KeyA(A0, INPUT);
Key KeyB(A0, INPUT);
Key KeyC(A0, INPUT);
Key KeyD(A0, INPUT);
Key KeyE(A0, INPUT);

uint8_t snakeArray[64][2] = {0};
uint8_t tempArray[128] = {0};
uint8_t snakeMap[8][8] = {0};
uint8_t scoreMap[8]= {0};
int8_t food[2], snakeHead[2];
uint8_t sLength = 3;
uint8_t direction = 0; //1-up. 2-down. 3-left. 4-right. default-not move, which means no direction
uint8_t score = 0; //number of food have eaten; also can be the score
uint8_t snakeSpeed = 0;
uint8_t cnt = 0;
uint8_t gameStatus;
uint32_t endTimer = 0;
uint8_t snakeColor, foodColor;


//定义彩灯颜色
const uint16_t colorArray[10][3] = {
  {0,   0,   0   },
  {155, 100, 0   },        //暖灯 (R,G,B)
  {100, 130, 100 },        //冷灯
  {255, 0,   0   },        //红
  {234, 139, 23  },        //橙
  {255, 255, 0   },        //黄
  {0,   255, 0   },        //绿
  {0,   255, 255 },        //青
  {0,   0,   255 },        //蓝
  {255, 0,   255 }         //紫
};

uint8_t score_num[10][5] = {
  0x70, 0x50, 0x50, 0x50, 0x70,
  0x10, 0x10, 0x10, 0x10, 0x10,
  0x70, 0x10, 0x70, 0x40, 0x70,
  0x70, 0x10, 0x70, 0x10, 0x70,
  0x50, 0x50, 0x70, 0x10, 0x10,
  0x70, 0x40, 0x70, 0x10, 0x70,
  0x70, 0x40, 0x70, 0x50, 0x70,
  0x70, 0x10, 0x10, 0x10, 0x10,
  0x70, 0x50, 0x70, 0x50, 0x70,
  0x70, 0x50, 0x70, 0x10, 0x70,
};

void readButton(){
  if (KeyA.read(800, 900) == SHORT_PRESS && direction != DIR_BACK){
      direction = DIR_FORWARD;
  }else if (KeyB.read(600, 750) == SHORT_PRESS && direction != DIR_LEFT){
      direction = DIR_RIGHT;
  }else if (KeyC.read(480, 650) == SHORT_PRESS && direction != DIR_FORWARD){
     direction = DIR_BACK;
  }else if (KeyD.read(250, 450) == SHORT_PRESS && direction != DIR_RIGHT){
     direction = DIR_LEFT;
  }
}


bool arraySearch(){
   for (uint8_t i = 0; i < sLength; i++) {
     if (snakeArray[i][0] == snakeHead[0] && snakeArray[i][1] == snakeHead[1])
       return true;
   }   
   return false;
}

void randFood() {//generate random food location
  food[0] = random(0, 8); food[1] = random(0, 8);
  for (uint8_t  i = 0; i < sLength; i++) {//food can't be on the snack
    if (snakeArray[i][0] == food[0] && snakeArray[i][1] == food[1]) {
      food[0] = random(0, 8); food[1] = random(0, 8);
      i = 0;
    }
  }
  foodColor = random(1, 10);
  while(foodColor == snakeColor)
     foodColor = random(1, 10);
}


void dispScore(uint16_t num)    //把分数写入地图
{
    uint8_t score1,score2 ,score3; 
    uint8_t tmp_x, i, j, m, n;
    score1 = num/100;
    score2 = (num%100)/10;
    score3 = num%10;
    m = score1/8;
    n = score1%8;

    memset(scoreMap, 0, sizeof(scoreMap));

   for(i = 0; i< m; i++)
      scoreMap[i] = 0xff; 
      
   for(j = 0; j < n; j++)
      scoreMap[m] |= (0x80>>j); 
   
   for(i = 0; i < 5; i++){
      for(j = 0; j < 4; j++){
         if((score_num[score2][i] & (0x80>>j)) == (0x80>>j))
            scoreMap[3+i] |= 0x80>>j;
       }
   }
   
   for(i = 0; i < 5; i++){
      for(j = 0; j < 4; j++){
         if((score_num[score3][i] & (0x80>>j)) == (0x80>>j))
            scoreMap[3+i] |= 0x80>>(j+4);
       }
   }
}


void gameReset() {
  snakeArray[0][0] = 0;
  snakeArray[0][1] = 4;
  snakeArray[1][0] = 1;
  snakeArray[1][1] = 4;
  snakeArray[2][0] = 2;
  snakeArray[2][1] = 4;
  snakeHead[0] = snakeArray[2][0]; snakeHead[1] = snakeArray[2][1];
  sLength = 3;
  direction = 0;// default-not move, which means no direction
  score = 0;
  snakeSpeed = 0;
  randFood();//generate random food location
}

void updateSnake() {
  switch (direction){
    case DIR_FORWARD:
      snakeHead[1]--;
      break;
    case DIR_BACK:
      snakeHead[1]++;
      break;
    case DIR_RIGHT:
      snakeHead[0]--;
      break;
    case DIR_LEFT:
      snakeHead[0]++;
      break;
  }

  if(direction)
  if (snakeHead[0] > 7 || snakeHead[0] < 0 || snakeHead[1] > 7 || snakeHead[1] < 0 || arraySearch()) {
     endTimer = millis() + 1000; 
     gameStatus = GAME_END;
     dispScore(score);

  }else{
     if (snakeHead[0] == food[0] && snakeHead[1] == food[1]) { //if eat the food
        snakeColor = foodColor;
        score++;
      if (score % 2 == 0) {//add length to the snack
        sLength++;
        memcpy(tempArray, snakeArray[0], (sLength-1)*2); 
        memcpy(snakeArray[1], tempArray, (sLength-1)*2);  
      }
      if(score % 3 == 0 && snakeSpeed < 7)
        snakeSpeed++;    
      randFood();
    }
    snakeArray[sLength][0] = snakeHead[0];
    snakeArray[sLength][1] = snakeHead[1];
    memcpy(snakeArray[0], snakeArray[1], sLength*2);    
  }
  memset(snakeMap, 0, sizeof(snakeMap));
  for(uint8_t i = 0; i < sLength; i++)
     snakeMap[snakeArray[i][0]][snakeArray[i][1]] = 1;
}

void timer1Isr()
{
    if(gameStatus == GAME_RUNNING){
       readButton();
       cnt++;
       if(cnt + snakeSpeed > 10){
         cnt = 0;
         updateSnake();
       }
    }
}

void timer1Setup()
{
    Timer1.initialize(50000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt( timer1Isr ); // attach the service routine here
}


