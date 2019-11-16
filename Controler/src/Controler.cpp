#include "Controler.h"

std::vector<std::string> split(const std::string &str, const char sep);

Controler *Controler::m_instance = nullptr;

Controler::Controler(const char *ssid, const char *password, const char* host) 
    : m_key("KEYROSTIK"),
      m_ssid(ssid),
      m_password(password),
      m_host(host),
      m_port(3000)
{
    m_instance = this;

    m_client = new WiFiClient();
    m_rooms = new Rooms;

    if (connectToWifi(1000))
    {
        delay(1000);
        connectToHost(1000);
    }
}

bool Controler::connectToWifi(const int &wait)
{
    ulong timer = millis();
    WiFi.begin(m_ssid, m_password);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(50);
        if (wait == -1) continue;
        if (timer + wait < millis())  break;
    }
    Serial.println("");
    if (WiFi.isConnected())
    {
        Serial.println("Connected to ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("Error connected WiFi.");
    }

    return WiFi.isConnected();
}

bool Controler::connectToHost(const int &wait)
{
    ulong timer = millis();
    m_client->connect(m_host, 3000);
    while (!m_client->connected())
    {
        Serial.print('.');
        delay(50);
        if (wait == -1)
            continue;
        if (timer + wait < millis())
            break;
    }
    Serial.println("");
    if (m_client->connected())
    {
        Serial.print("Connected to ");
        Serial.print(m_host);
    }
    else
    {
        Serial.println("Error connected client.");
    }

    if (m_client->connected())
    {
        onConnected();
    }

    return m_client->connected();
}

void Controler::update()
{
    checkConnect();
    if (checkAvailable())
    {
        readAvailable();
    }
    monitorChanges();
}

void Controler::checkConnect()
{
    while (!m_client->connected() || !WiFi.isConnected())
    {
        if (!WiFi.isConnected())
        {
            connectToWifi(5000);
        }
        else if (!m_client->connected())
        {
            connectToHost(1000);
        }
    }
}

bool Controler::checkAvailable()
{
    return m_client->available() > 0;
}

void Controler::readAvailable()
{
    Buffer buffer;
    for(int i = 0; i < 4; i++)
        m_client->read();
    while (m_client->available())
    {
        buffer.append((char)m_client->read());
    }
    // for (int i = 0; i < buffer.size; i++)
    // {
    //     Serial.print(int(buffer.array[i]));
    // }
    Commands commands = Commands::fromBuffer(buffer);
    for (unsigned int i = 0; i < commands.commands.size(); i++)
        parseCommand(commands.commands.at(i));
}

void Controler::monitorChanges()
{
    bool changes = false;
    for(Room *room : m_rooms->rooms)
    {
        for(Item *item : room->items.m_items)
        {
            if(!item->monitor)
                continue;
            ArduinoObject *object;
            for(ArduinoObject *obj : m_objects)
            {
                if(obj->pin() == item->pins[0])
                {
                    object = obj;
                    break;
                }
            }
            if(object == nullptr)
            {
                Serial.print("NULL OF ");
                Serial.print(item->pins[0]);
                Serial.println(" PIN");
                continue;
            }
            switch (item->type)
            {
            case Item::MagSig:
            {
                MagSigObject *mag = static_cast<MagSigObject *>(object);
                if(mag->value() != item->on)
                {
                    changes = true;
                    item->on = mag->value();
                    triggerItem(item);
                    Serial.println("MAGCHANGED");
                }
                break;
            }
            default:
                break;
            }
            
        }
    }
    if(changes)
        updateRooms();
}

void Controler::parseCommand(const Command &command)
{
    Serial.println("PC1");
    switch (command.title())
    {
    case Command::Control:
    {
        switch (command.controlAction())
        {
        case Command::AddRoom:
        {
            Room temp_room = Room::fromBuffer(command.buffer());
            Room *room = m_rooms->addRoom(temp_room);
            Serial.println("ROOM ADDED");
            updateRooms();
            break;
        }
        case Command::AddItem:
        {
            Item temp_item = Item::fromBuffer(command.buffer());
            Room* room = m_rooms->roomFromIdentifier(temp_item.roomIdentifier);
          
            Item *item = room->addItem(temp_item);
            ArduinoObject *object;
            if (item->type == Item::Led)
            {
                Serial.print("ADD LED: ");
                Serial.println(item->pins[0]);
                object = new LedObject(item->pins[0]);
            }
            else if (item->type == Item::Servo)
            {
                object = new ServoObject(item->pins[0]);
                static_cast<ServoObject *>(object)->rotate(90);
            }else if(item->type == Item::MagSig)
            {
                Serial.print("ADD MAGSIG: ");
                Serial.println(item->pins[0]);
                item->monitor = true;
                object = new MagSigObject(item->pins[0]);
            }else if(item->type == Item::Camera)
            {
                Serial.println("SADD CAMERA");
                object = new ArduinoCameraOV7670Object(item->pins[0], 
                item->pins[1], 
                item->pins[2], 
                item->pins[3], 
                item->pins[4], 
                item->pins[5], 
                item->pins[6], 
                item->pins[7], 
                item->pins[8], 
                item->pins[9], 
                item->pins[10], 
                item->pins[11], 
                item->pins[12], 
                item->pins[13]);
                Serial.println("SADD CAMERA");
            }else 
            {
                Serial.println("ADD ERROR");
                return;
            }
            object->setIdentifier(item->identifier);
            m_objects.push_back(object);
            updateRooms();
            break;
        }

        case Command::TurnItem:
        {
            Item temp_item = Item::fromBuffer(command.buffer());
            Room *room = m_rooms->roomFromIdentifier(temp_item.roomIdentifier);
            Item *item = room->items.itemFromIdentifier(temp_item.identifier);
            
            Serial.println(temp_item.type);
            Serial.println(temp_item.pins[0]);
            for (ArduinoObject *obj : m_objects)
            {
                if (obj->identifier() == temp_item.identifier)
                {
                    Serial.print("CHECK PIN: ");
                    Serial.println(obj->pin());
                    switch (temp_item.type)
                    {
                    case Item::Servo:
                    {
                        ServoObject *servo = static_cast<ServoObject *>(obj);
                        servo->rotate(temp_item.angle);
                        item->angle = temp_item.angle;
                        break;
                    }
                    case Item::Led:
                    {
                        if (temp_item.on)
                            obj->turnOn();
                        else
                            obj->turnOff();
                        item->on = obj->value();
                        break;
                    }
                    case Item::MagSig:
                    {
                        MagSigObject *mag = static_cast<MagSigObject *>(obj);
                        item->on = mag->value();
                        break;
                    }
                    case Item::Camera:
                    {
                            Serial.println("UPDCAM1");
                        ArduinoCameraOV7670Object *cam = static_cast<ArduinoCameraOV7670Object *>(obj);
                        item->dataSize = 9666;
                        cam->oneFrame();
                            Serial.println("UPDCAM2");
                        item->data = new unsigned char[9666];
                            Serial.println("UPDCAM3");

                            Serial.println( ArduinoCameraOV7670Object::headerSize + cam->width() * cam->height() * 2);

                        for(int i = 0; i < ArduinoCameraOV7670Object::headerSize; i++)
                            item->data[i] = ArduinoCameraOV7670Object::bmpHeader[i];
                        for(int i = ArduinoCameraOV7670Object::headerSize; i < 9666; i++)
                            item->data[i] = cam->frame()[i-ArduinoCameraOV7670Object::headerSize];
                        Serial.println("UPDCAM4");

                        break;
                    }
                    default:
                        Serial.println("NO HAS TYPE");
                        return;
                    }
                    break;
                }
            }
            updateRooms();
            if(item->dataSize != 0)
            {
                item->dataSize = 0;
                delete[] item->data;
            }
            break;
        }
        case Command::UpdateItems:
        {
            return;
            updateItems();
            break;
        }
        case Command::UpdateRooms:
        {
            Buffer buffer = m_rooms->toBuffer();
            Command command(Command::Control, Command::UpdateRooms, buffer);
            buffer = command.toBuffer();
            m_client->write(buffer.toBytes(), buffer.fullSize());
            break;
        }
        break;
        }
    case Command::ConfirmConnection:
    {
        Command command(Command::ConfirmConnection);
        Buffer buffer = command.toBuffer();
        m_client->write(buffer.toBytes(), buffer.fullSize());
        break;
    }
    }
    break;

    default:
        break;
    }
}

void Controler::updateRooms()
{
    Serial.println("START UPD");
    Buffer buffer;
    BufferStream stream(&buffer, BufferStream::WriteOnly);
    Serial.println("1 UPD");
    stream << *m_rooms;
    Serial.println("1 UPD");
    Command c(Command::Control, Command::UpdateRooms, buffer);
    Buffer newb = c.toBuffer();
    auto s = newb.toBytes();
    m_client->write(s, newb.fullSize());
    Serial.println("END UPD");

    Serial.println(newb.fullSize());
}

void Controler::triggerItem(const Item *item)
{
    Buffer buffer = item->toBuffer();
    Command command(Command::Control, Command::TriggeredItem, buffer);
    buffer = command.toBuffer();
    m_client->write(buffer.toBytes(), buffer.fullSize());
}

void Controler::onConnected()
{
    Command c(Command::Confirm, Command::Controler, m_key);
    auto ds = c.toBuffer();
    m_client->write(ds.toBytes(), ds.fullSize());

    updateRooms();
}

std::vector<std::string> split(const std::string &str, const char sep)
{
    std::vector<std::string> list;
    int lastSepN = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == sep)
        {
            std::string temp;
            for (int j = lastSepN; j < i; j++)
            {
                temp += str[j];
            }
            list.push_back(temp);
            lastSepN = i + 1;
        }
    }
    std::string temp;
    for (int j = lastSepN; j < str.size(); j++)
    {
        temp += str[j];
    }
    list.push_back(temp);
    return list;
}