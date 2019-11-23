#ifndef CONTROLER_H
#define CONTROLER_H

#include <WiFi.h>
#include <EEPROM.h>
#include <list>
#include <string>
#include "Data/command.h"
#include "Data/room.h"
#include "Data/buffer.h"
#include "Data/controlersettings.h"
#include "Objects/MagSigObject.h"
#include "Objects/ReleObject.h"
#include "Objects/ServoObject.h"
#include "Objects/PiezoObject.h"
#include "Objects/LightSensorObject.h"
#include "Objects/TermistorObject.h"
#include "Objects/LedObject.h"
#include "Objects/ArduinoCameraOV7670.h"

class Controler 
{
public:
    explicit Controler(const char *host);

    static Controler *instance() { return m_instance; }
    static void setInstance(Controler *controler) { m_instance = controler; }

    void update();

    bool connectToWifi(const int &wait = 30000);
    bool connectToHost(const int &wait = 30000);

    void reconnectToWiFi(const char *ssid, const char *password);

    void updateAP();
    bool checkConnect();
    bool checkAvailable();
    void readAvailable();
    void monitorChanges();
    void parseCommand(const Command& command);

    bool checkItemForPassability(Item &item);

    void updateAPClients();

    void updateItems();
    void updateRooms();
    void triggerItem(const Item *item);

    void onConnected();

    int writeString(const char *s, int address)
    {
    int size = -1;
    while(s[++size]);
    EEPROM.write(address, size);
    for(int i = 0, j = address + 1; i < size; i++, j++)
    {
        EEPROM.write(j, s[i]);
    }
    EEPROM.commit();

    int lastAddress = address + size + 2;

    return lastAddress;
    }

    int readString(int address, char *&s)
    {
    int size = EEPROM.read(address);
    s = new char[size + 1];
    s[size] = 0;
    for(int i = 0, j = address + 1; i < size; i++, j++)
        s[i] = EEPROM.read(j);

    int lastAddress = address + size + 2;

    return lastAddress;
    }

private:
    const char *m_key;

    char *m_ssid;
    char *m_password;

    const char *m_host;
    uint16_t m_port;

    WiFiClient *m_client;
    

    WiFiServer *m_apServer;
    std::list<WiFiClient *> m_apClients;

    std::list<ArduinoObject *> m_objects;

    Rooms *m_rooms;

    static Controler *m_instance;
};

#endif