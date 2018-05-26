String str;

String command = "{\"switch\":\"true\"}";

String solution(String _sta, char *c)
{
  _sta = _sta.substring(1, _sta.length() - 1);
  uint8_t length = _sta.length();
  char buf[length + 1];
  String data;
  char c_all[50] = "";
  char data1[] = "\"";
  char data2[] = "\":%s";
  
  strcat(c_all, data1);
  strcat(c_all, c);
  strcat(c_all, data2);
  
  sscanf(_sta.c_str(), c_all, &buf);
  
  data = String(buf);
  data = data.substring(1, data.length() - 1);
  if (data != NULL)
    return data;
}

void setup() {
  Serial.begin(115200);
  str = solution(command, "switch");
  Serial.println(str);
}

void loop() {

}
