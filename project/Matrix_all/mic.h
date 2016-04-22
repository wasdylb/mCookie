#define mic_pin A0

int sta[8];

void mic_led()
{
  sta[0] = map(analogRead(mic_pin), 0, 900, 0, 8);
  sta[1] = map(analogRead(mic_pin), 0, 900, 0, 7);
  sta[2] = map(analogRead(mic_pin), 0, 900, 0, 6);
  sta[3] = map(analogRead(mic_pin), 0, 900, 0, 6);
  sta[4] = map(analogRead(mic_pin), 0, 900, 0, 5);
  sta[5] = map(analogRead(mic_pin), 0, 900, 0, 5);
  sta[6] = map(analogRead(mic_pin), 0, 900, 0, 4);
  sta[7] = map(analogRead(mic_pin), 0, 900, 0, 4);

  for (int i = 0; i < sta[0]; i++)
    display.setLedColor( 0, 7 - i, 100, 0, 80);
  for (int i = sta[0]; i < 8; i++)
    display.setLedColor( 0, 7 - i, 0, 0, 0);

  for (int i = 0; i < sta[1]; i++)
    display.setLedColor( 1, 7 - i, 100, 0, 80);
  for (int i = sta[1]; i < 8; i++)
    display.setLedColor( 1, 7 - i, 0, 0, 0);

  for (int i = 0; i < sta[2]; i++)
    display.setLedColor( 2, 7 - i, 100, 0, 80);
  for (int i = sta[2]; i < 8; i++)
    display.setLedColor( 2, 7 - i, 0, 0, 0);

  for (int i = 0; i < sta[3]; i++)
    display.setLedColor( 3, 7 - i, 100, 0, 80);
  for (int i = sta[3]; i < 8; i++)
    display.setLedColor( 3, 7 - i, 0, 0, 0);

  for (int i = 0; i < sta[4]; i++)
    display.setLedColor( 4, 7 - i, 100, 0, 80);
  for (int i = sta[4]; i < 8; i++)
    display.setLedColor( 4, 7 - i, 0, 0, 0);

  for (int i = 0; i < sta[5]; i++)
    display.setLedColor( 5, 7 - i, 100, 0, 80);
  for (int i = sta[5]; i < 8; i++)
    display.setLedColor( 5, 7 - i, 0, 0, 0);

  for (int i = 0; i < sta[6]; i++)
    display.setLedColor( 6, 7 - i, 100, 0, 80);
  for (int i = sta[6]; i < 8; i++)
    display.setLedColor( 6, 7 - i, 0, 0, 0);

  for (int i = 0; i < sta[7]; i++)
    display.setLedColor( 7, 7 - i, 100, 0, 80);
  for (int i = sta[7]; i < 8 ; i++)
    display.setLedColor( 7, 7 - i, 0, 0, 0);
}
