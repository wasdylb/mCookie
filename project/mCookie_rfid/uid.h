#include "arduino.h"

unsigned long uid_time_admin = 0;

uint8_t uid_num_admin[5] = {
  0xB4, 0x89, 0xCD, 0xA4, 0x54
};

uint32_t uid_data;
uint8_t uid_data_num = 0;
//uint8_t uid_data_num = 0;

uint32_t get_data(uint8_t _data[4]) {
  return ((uint32_t)_data[0] << 32) | ((uint32_t)_data[1] << 24) | ((uint32_t)_data[2] << 16) | ((uint32_t)_data[3] << 8 | ((uint32_t)_data[4]));
}

