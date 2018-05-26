
void charadd(char* id)
{

  char SUBSCRIBTOPICA[30] = "";
  strcat(SUBSCRIBTOPICA, "ca/");
  strcat(SUBSCRIBTOPICA, id);
  Serial.println(SUBSCRIBTOPICA);

  char SUBSCRIBTOPICP[30] = "";
  strcat(SUBSCRIBTOPICP, "cp/");
  strcat(SUBSCRIBTOPICP, id);
  Serial.println(SUBSCRIBTOPICP);

}

String uploadData(char* _st, int _data)
{
  String send_data = "";
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"}";
  return send_data;
}

String uploadData(char* _st, char* _data)
{
  String send_data = "";
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"}";
  return send_data;
}

void setup() {
  Serial.begin(115200);
  charadd("5998ee14c766bf00014d65db");
}

void loop() {

  Serial.println(uploadData("show_S1", millis() / 1000));
  delay(1000);
  Serial.println(uploadData("show_S2", "mCotton"));
  delay(1000);
}
