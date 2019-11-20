#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include "Controler.h"

WiFiServer server;
Controler *controler;

char *ssid = "11111111";
char *password = "11111111";
char *ip = "192.168.0.106";

void readSsidPassword();
void writeSsidPassword();

void setup() 
{
  Serial.begin(115200);

  EEPROM.begin(100);

  readSsidPassword();

  Serial.println("SSID");
  Serial.println(ssid);
  Serial.println("PASS");
  Serial.println(password);

  controler = new Controler(ssid, password, ip);

  WiFi.enableAP(true);
  WiFi.softAP("ESP32AP", "11223344");
  Serial.println(WiFi.softAPIP());

  server.begin(3000);
}

bool hasClient = false;
WiFiClient *client;

void loop() 
{
  Serial.println("LOOP");
    if(!hasClient)
    {
      WiFiClient c = server.available();
      if(c)
      {
        client = new WiFiClient(c);
        Serial.println("CONN");
        client->write("Hello from controler");
        hasClient = true;
      }else {Serial.println("HAS NO CLIENTS");}
    }else
    {
      int size = client->available();
      if(size)
      {
        char *s = new char[size + 1];
        int splitIndex;
        for(int i = 0; i < size; i++)
        {
          s[i] = client->read();
          if(s[i] == '|')
            splitIndex = i;
        }
        s[size] = 0;
        Serial.println(s);
        char *t_ssid = new char[splitIndex + 1];
        char *t_password = new char[size - splitIndex + 1];

        for(int i = 0; i < splitIndex; i++)
        {
          t_ssid[i] = s[i];
        }
        t_ssid[splitIndex] = 0;
        for(int i = splitIndex + 1, j = 0; i < size + 1; i++, j++)
        {
          t_password[j] = s[i];
        }
        t_password[size] = 0;

        ssid = t_ssid;
        password = t_password;
        
        writeSsidPassword();

        controler->reconnectToWiFi(ssid, password);

      }
    }
  controler->update();
}

void readSsidPassword()
{
  uint8_t ssidSize = 0, passSize = 0;

  ssidSize = EEPROM.read(0);
  ssid = new char[ssidSize + 1];
  passSize = EEPROM.read(ssidSize + 1);
  password = new char[passSize + 1];
  for(int i = 0; i < ssidSize; i++)
    ssid[i] = EEPROM.read(i+1);
  for(int i = 0; i < passSize; i++)
    password[i] = EEPROM.read(i + 1 + ssidSize + 1);

    ssid[ssidSize] = 0;
    password[passSize] = 0;
}

void writeSsidPassword()
{
  uint8_t ssidSize = -1, passSize = -1;
  while(ssid[++ssidSize]);
  while(password[++passSize]);

  EEPROM.write(0, ssidSize); // 0
  for(int i = 0; i < ssidSize; i++)
    EEPROM.write(i + 1, ssid[i]); // 1,2,3
  EEPROM.write(ssidSize + 1, passSize); // 4
  for(int i = 0; i < passSize; i++)
    EEPROM.write(i + 1 + ssidSize + 1, password[i]); // 5,6,7

  Serial.print("WRITE: ");
  Serial.println(ssid);
  Serial.print("WRITE: ");
  Serial.println(password);
  Serial.println("WRITEEND");

  Serial.println(passSize);

  EEPROM.commit();
}