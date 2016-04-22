#include <Wire.h>
#include <RFID_IIC.h>

RFID rfid(6, 0x3F);

void PrintHex(const byte * data, const uint32_t numBytes)
{
  for (uint32_t szPos = 0; szPos < numBytes; szPos++)
  {
    Serial.print("0x");
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      Serial.print("0");
    Serial.print(data[szPos] & 0xff, HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      Serial.print(" ");
    }
  }
  Serial.println("");
}

uint32_t RFID_Scan() {
  if (rfid.isCard())  {
    Serial.println("Find the card!");
    delay(200);
    //读取卡序列号
    if (rfid.readCardSerial()) {
      Serial.print("The card's number is  : ");
      /*
            Serial.print(rfid.serNum[0], HEX);
            Serial.print(rfid.serNum[1], HEX);
            Serial.print(rfid.serNum[2], HEX);
            Serial.print(rfid.serNum[3], HEX);
            Serial.print(rfid.serNum[4], HEX);
            Serial.println(" ");
      */
      PrintHex(rfid.serNum, 5);
    }
    rfid.selectTag(rfid.serNum);
    return get_data(rfid.serNum);
  }
  else
    return 0;
}
