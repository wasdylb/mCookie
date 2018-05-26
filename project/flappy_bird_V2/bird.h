#include <TimerOne.h>

#define GAME_END 0
#define GAME_RUNNING 1
#define GAME_PAUSE 2


uint8_t pipe_map[8];
uint8_t score_map[8];
uint8_t height;
uint8_t bird_flag[2] = {3, 3};
uint8_t bird_counter, pipe_counter;
uint8_t button[2], fliter[2];
boolean flag_Up;
uint8_t game_flag;
uint16_t score;
uint8_t game_status;

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

void move_pipe()
{
   uint8_t i;
   for(i = 0; i < 8; i++)
   {
       pipe_map[i] <<= 1; 
   }
   if((pipe_map[0]&0x0f) == 0x00)
   {
       height = random(4);
       for(i = 0; i < 8; i++)
       {
          if((i<=height)||(i>height+3))
             pipe_map[i] |= 0x01; 
       }
   }
   else  if((pipe_map[0]&0x0f) == 0x02)
   {
       for(i = 0; i < 8; i++)
       {
          if((i<=height)||(i>height+3))
             pipe_map[i] |= 0x01; 
       }
   }
}

void SensorScan()
{
    fliter[0] = (fliter[0]<<1)|digitalRead(A0);
    fliter[1] = (fliter[1]<<1)|digitalRead(A2);
    if((fliter[0]&0x0F) == 0x0F) 
    {
        button[0] = 1;  
    } 
    else if((fliter[0]|0xF0) == 0xF0)
    {
        button[0] = 0;  
    }

    if((fliter[1]&0x0F) == 0x0F)
    {
        button[1] = 1;
    }
    else if((fliter[1]|0xF0) == 0xF0)
    {
        button[1] = 0;
    }
}

void disp_score(uint16_t num)    //把分数写入地图
{
    uint8_t score1,score2 ,score3;
    uint8_t tmp_x, i, j, m, n;
    score1 = num/100;
    score2 = (num%100)/10;
    score3 = num%10;
    m = score1/8;
    n = score1%8;

    memset(score_map, 0, sizeof(score_map));

   for(i = 0; i< m; i++)
      score_map[i] = 0xff; 
      
   for(j = 0; j < n; j++)
      score_map[m] |= (0x80>>j); 
   
   for(i = 0; i < 5; i++)
   {
       for(j = 0; j < 4; j++)
       {
            if((score_num[score2][i] & (0x80>>j)) == (0x80>>j))
            {
                score_map[3+i] |= 0x80>>j;
            }
       }
   }
   
   for(i = 0; i < 5; i++)
   {
       for(j = 0; j < 4; j++)
       {
            if((score_num[score3][i] & (0x80>>j)) == (0x80>>j))
            {
                score_map[3+i] |= 0x80>>(j+4);
            }
       }
   }
}


void timer1Isr()
{
    bird_counter += game_flag;
    pipe_counter += game_flag;
    SensorScan();
    if(bird_counter > 25)
    {
        bird_counter = 0; 
        if(bird_flag[1]<7)
        {
          if(bird_flag[1] == bird_flag[0])
          {
              bird_flag[0]++; 
          }
          else
          {
              bird_flag[1] = bird_flag[0];
              flag_Up = false;
          }
       }     
    } 
    if(pipe_counter > 50)
    {
        pipe_counter = 0; 
        move_pipe();
        if((pipe_map[0]&0xE0) == 0x80)
        {
            score++;  
        }
        disp_score(score);
    }
}

void timer1Setup()
{
    Timer1.initialize(10000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt( timer1Isr ); // attach the service routine here
}



