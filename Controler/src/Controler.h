#ifndef CONTROLER_H
#define CONTROLER_H

#include <WiFi.h>
#include <list>
#include <string>
#include "Data/command.h"
#include "Data/room.h"
#include "Data/buffer.h"
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
    explicit Controler(const char *ssid, 
                        const char *password,
                        const char *host);

    static Controler *instance() { return m_instance; }
    static void setInstance(Controler *controler) { m_instance = controler; }

    void update();

    bool connectToWifi(const int &wait = 30000);
    bool connectToHost(const int &wait = 30000);

    void reconnectToWiFi(const char *ssid, const char *password);

    bool checkConnect();
    bool checkAvailable();
    void readAvailable();
    void monitorChanges();
    void parseCommand(const Command& command);

    void updateItems();
    void updateRooms();
    void triggerItem(const Item *item);

    void onConnected();

private:
    const char *m_key;

    const char *m_ssid;
    const char *m_password;

    const char *m_host;
    uint16_t m_port;

    WiFiClient *m_client;

    std::list<ArduinoObject *> m_objects;

    Rooms *m_rooms;

    static Controler *m_instance;
};

#endif