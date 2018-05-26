#include "Microduino_Matrix.h"
#include "bird.h"

uint8_t Addr[MatrixPix_X][MatrixPix_Y] = {
  { 64}
};

Matrix display = Matrix(Addr);

uint8_t button_cache[2], button_ret[2];
uint32_t end_times;

void setup() {
//    .begin(115200);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  Wire.begin();
  display.clearDisplay();
  timer1Setup();
  game_status = GAME_PAUSE;
  game_flag = 0;
  bird_counter = 0;
  pipe_counter = 0;
  score = 0;
  
}

void loop() {
    button_cache[0] = button_ret[0];
    button_cache[1] = button_ret[1];
    button_ret[0] = button[0];
    button_ret[1] = button[1];


   if(game_status == GAME_RUNNING)
   {
       if(!map_test())
       {
           game_status = GAME_END;
           game_flag = 0;
           bird_counter = 0;
           pipe_counter = 0;
           end_times = millis()+1000;
       }
       else if(button_cache[0]&&!button_ret[0])
       {
           game_status = GAME_PAUSE; 
           game_flag = 0;
           bird_counter = 0;
           pipe_counter = 0;
       }
       update_map();
       sensor_loop();
   }
   else if(game_status == GAME_PAUSE)
   {
       if(button_cache[0]&&!button_ret[0])
       {
           game_status = GAME_RUNNING; 
           game_flag = 1;
       }
       update_map();
   }
   else if(game_status == GAME_END)
   {
        if(millis() > end_times)
        {
             update_score();   
        }
        if(button_cache[0]&&!button_ret[0])
        {
             game_status = GAME_RUNNING;
             game_flag = 1; 
             memset(pipe_map, 0, sizeof(pipe_map));
             bird_flag[0] = 3;
             bird_flag[1] = 3;
             score = 0;
        }
   }  
}


void update_map()
{
    for(int pix = 0; pix < 8; pix++)
    {
        for(int row = 0; row < 8; row++)
        {        
            if(pix==bird_flag[0]&&row==5)
            display.setLedColor(7-row, pix, 160, 0, 0);  
            else if(pix==bird_flag[1]&&row==6)
            display.setLedColor(7-row, pix, 80, 80, 80);  
            else if((pipe_map[pix]>>row)&0x01)
            display.setLedColor(7-row, pix, 0, 80, 0); 
            else
            display.setLedColor(7-row, pix, 0, 0, 80); 
        }          
    }   
}


void update_score()
{
    for(int pix = 0; pix < 8; pix++)
    {
        for(int row = 0; row < 8; row++)
        {
            if((score_map[pix]>>row)&0x01)
            display.setLedColor(7-row, pix, 80, 80, 0); 
            else
            display.setLedColor(7-row, pix, 0, 0, 80); 
        }          
    }   
}

void sensor_loop()
{   
    if(!flag_Up&&bird_flag[1]>0)
    {
        if(button_cache[1]&&!button_ret[1])
        {
             bird_flag[0] = bird_flag[1]-1; 
             flag_Up = true;
             bird_counter = 0; 
        }
    }    
}

boolean map_test()
{
    if((bird_flag[1] == 7)||((pipe_map[bird_flag[0]]>>5)&0x01)||((pipe_map[bird_flag[1]]>>6)&0x01))
       return false;
    else 
       return true;  
}


