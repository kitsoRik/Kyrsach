#include <Arduino.h>
#include "Controler.h"

Controler *controler;

const char *ssid = "AndroidAP4BB3";
const char *password = "ctji4432";
const char *ip = "31.131.31.228";

void setup() 
{
  Serial.begin(115200);
  controler = new Controler(ssid, password, ip);
}

void loop() 
{
  controler->update();
}