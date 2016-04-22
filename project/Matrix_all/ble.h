String msg = "";

int hor[8][8], ver[8][8];

int color;

char msg_c;

char _buffer[128];
String buffer;

void draw()
{
  int x = msg.charAt(2) - 48;
  int y = msg.charAt(4) - 48;
  int color = msg.charAt(6) - 48;

  if (color == 0)
    display.setLedColor( y, x, 0, 0, 0);
  else if (color == 1)
    display.setLedColor( y, x, 255, 0, 0);
  else if (color == 2)
    display.setLedColor( y, x, 255, 255, 0);
  else if (color == 3)
    display.setLedColor( y, x, 0, 255, 0);
}

void image()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      hor[i][j] = msg[8 * i + j + 2] - 48;
      if (hor[i][j] != ver[i][i])
      {
        display.clearDisplay();
        ver[i][j] = hor[i][i];
      }
    }
  }

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (hor[i][j] / 1 == 1)
        display.setLedColor( j, i, 255, 0, 0);
      else if (hor[i][j] / 1 == 2)
        display.setLedColor( j, i, 255, 255, 0);
      else if (hor[i][j] / 1 == 3)
        display.setLedColor( j, i, 0, 255, 0);
    }
  }
}

void ble_data()
{
  if (my_serial.available() > 0) {
    msg = my_serial.readStringUntil('\n');
    Serial.println(msg);
    if (msg[0] != msg_c)
    {
      display.clearDisplay();
      msg_c = msg[0];
    }
    if (msg[0] == 'B')
      image();
    else if (msg[0] == 'L')
      draw();
    if (msg[0] == 'M')
    {
      buffer = String(msg.substring(2, msg.length()));
      for (int a = 0; a < buffer.length(); a++)
        _buffer[a] = buffer[a];
      for (int a = buffer.length(); a < 128 ; a++)
        _buffer[a] = NULL;
      display.writeString(_buffer, MODE_H, 100, 0);
    }
  }
}

