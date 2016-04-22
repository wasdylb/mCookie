#include "arduino.h"

#include <EEPROM.h>

unsigned long eeprom_Clear_num = 0;
unsigned long eeprom_Clear_time = 0;

int eeprom_Check(uint32_t _c) {
  if (!_c)   return 0;
  for (int b = 0; b < MAX_BANKCARD_NUM; b++) {
    uint32_t _d;
    EEPROM.get(1 + (sizeof(uint32_t) * b), _d);
    if (_d == _c) {
      return (b + 1);
    }
  }
  return 0;
}

int eeprom_Num() {
  uint32_t _c = 0, _d;
  for (int b = 0; b < MAX_BANKCARD_NUM; b++) {
    EEPROM.get(1 + (sizeof(uint32_t) * b), _d);
    if (_d) {
      _c++;
    }
  }
  return (int)_c;
}
