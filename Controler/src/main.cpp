#include <Arduino.h>
#include "Controler.h"

const int SIOD = 21; //SDA
const int SIOC = 22; //SCL

const int VSYNC = 34;
const int HREF = 35;

const int XCLK = 32;
const int PCLK = 33;

const int D0 = 27;
const int D1 = 17;
const int D2 = 16;
const int D3 = 15;
const int D4 = 14;
const int D5 = 13;
const int D6 = 12;
const int D7 = 2;

Controler *controler;

const char *ssid = "TP-LINK_6266";
const char *password = "16082228";
const char *ip = "192.168.0.106";

WiFiClient client;
  ArduinoCameraOV7670Object *camera;

void setup() 
{
  Serial.begin(115200);
  controler = new Controler(ssid, password, ip);

  
}

void loop() 
{controler->update();

}