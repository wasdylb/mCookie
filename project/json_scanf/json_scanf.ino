String str = "{\"deviceId\":\"598c003c8673b9000150704f\",\"secureToken\":\"1cuBFO68G5uV\"}";
String str1 = "{\"deviceId\":\"598c003c8673b9000150704f\"}";

char device[30], Token[15];
char deviceID[30];

void solution2(String str)
{
  String josn1, josn2;
  boolean change;
  if (str.startsWith("{") && str.endsWith("}"))
  {
    Serial.println(str);
    str = str.substring(1, str.length() - 1);
    for (uint8_t i = 0; i < str.length(); i++)
    {
      if (!change)
      {
        if (str[i] == ',')
          change = true;
        else
          josn1 += str[i];
      }
      else
      {
        josn2 += str[i];
        if (i == (str.length() - 1))
          change = false;
      }
    }
    josn1.replace("\"", "");
    josn2.replace("\"", "");
    sscanf(josn1.c_str(), "deviceId:%s", &device);
    sscanf(josn2.c_str(), "secureToken:%s", &Token);
    Serial.println(device);
    Serial.println(Token);
  }
}

String solution(String _sta, char *c)
{
  String data;
  if (_sta.startsWith("{") && _sta.endsWith("}"))
  {
    _sta = _sta.substring(1, _sta.length() - 1);
    _sta.replace("\"", "");
    uint8_t _length = _sta.length();
    char buf[_length];

    char c_all[30] = "";
    char data1[] = ":%s";

    strcat(c_all, c);
    strcat(c_all, data1);

    sscanf(_sta.c_str(), c_all, &buf);

    data = String(buf);
  }
  if (data != NULL)
    return data;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  solution2(str);
  Serial.println("--------");
  Serial.println(solution(str1, "deviceId"));
  delay(3000);
  Serial.println("--------");
}
