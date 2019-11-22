#include <Arduino.h>
#include "Controler.h"

Controler *controler;

char *ip = "31.131.28.232";

void setup() 
{
    Serial.begin(115200);
    EEPROM.begin(512);

    WiFi.enableAP(true);
    WiFi.softAP("ESP32AP", "11223344");
    Serial.print("AP: ");
    Serial.println(WiFi.softAPIP());

    controler = new Controler(ip);
}

void loop() 
{
    controler->update();
}

