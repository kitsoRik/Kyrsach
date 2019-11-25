#include "Controler.h"

std::vector<std::string> split(const std::string &str, const char sep);

Controler *Controler::m_instance = nullptr;

Controler::Controler(const char* host) 
	: m_key("KEYROSTIK"),
	  m_host(host),
	  m_port(3000),
		m_connectedToWiFi(false),
		m_connectedToHost(false)
{
	EEPROM.commit();
	int ssidlength = readString(0, m_ssid);
	int passLength = readString(ssidlength, m_password);
	
	Serial.print("Start SSID: ");
	Serial.println(m_ssid);
	Serial.print("Start Password: ");
	Serial.println(m_password);
	
	m_instance = this;
	
	m_client = new WiFiClient();
	m_rooms = new Rooms;
	
	m_apServer = new WiFiServer();
	
	m_apServer->begin(3000);
	
	if (connectToWifi(1000))
	{
		delay(1000);
		connectToHost(1000);
	}
}

bool Controler::connectToWifi(const int &wait)
{
	ulong timer = millis();
	WiFi.disconnect();
	WiFi.begin(m_ssid, m_password);
	
	if (WiFi.isConnected())
	{
		Serial.println("Connected to ");
		Serial.println(WiFi.localIP());
	}
	else
	{
		Serial.println("Error connected WiFi.");
		delay(1000);
	}
	
	return WiFi.isConnected();
}

bool Controler::connectToHost(const int &wait)
{
	ulong timer = millis();
	m_client->connect(m_host, 3000);
	Serial.println("");
	if (m_client->connected())
	{
		Serial.print("Connected to ");
		Serial.println(m_host);
	}
	else
	{
		m_client->connect(m_host, 3000);
		Serial.println("Error connected client.");
	}
	
	if (m_client->connected())
	{
		onConnected();
	}
	
	return m_client->connected();
}

void Controler::reconnectToWiFi(const char *ssid, 
								const char *password)
{
	
    int ssidSize = -1, passSize = -1;
    while(ssid[++ssidSize]);
    while(password[++passSize]);

    m_ssid = new char[ssidSize + 1];
    m_password = new char[passSize + 1];

    m_ssid[ssidSize] = 0;
    m_password[passSize] = 0;

    for(int i = 0; i < ssidSize; i++)
        m_ssid[i] = ssid[i];
    for(int i = 0; i < passSize; i++)
        m_password[i] = password[i];
	
	WiFi.disconnect();
	checkConnect();

	int t = writeString(m_ssid, 0);
	writeString(m_password, t);
}

void Controler::update()
{
	updateAP();
	if(!checkConnect())
		return;
	if (checkAvailable())
	{
		readAvailable();
	}
	monitorChanges();
}

void Controler::updateAP()
{
	delay(20);
	WiFiClient t_client = m_apServer->available();
	if(t_client)
	{
		WiFiClient *client = new WiFiClient(t_client);
		
		Serial.println("NEW APSERVER CLIENT");
		
		m_apClients.push_back(client);

        updateAPClients();
	}
	
	for(WiFiClient *client : m_apClients)
	{
		if(!client)
			continue;
		if(!client->connected())
			continue;
		if(client->available())
		{
            Buffer buffer;
            char *s = new char[4];
            for(int i = 0; i < 4; i++)
                s[i] = client->read();
            int readSize = *reinterpret_cast<int *>(s);
            Serial.println(readSize);
            while (client->available())
            {
                buffer.append((char)client->read());
            }

            ControlerSettings settings = ControlerSettings::fromBuffer(buffer);

            reconnectToWiFi(settings.ssid.c_str(), settings.password.c_str());
		}
	}
}

bool Controler::checkConnect()
{
	if (!WiFi.isConnected())
	{
		connectToWifi(1000);
	}
	else if (!m_client->connected())
	{
		connectToHost(1000);
	}

	bool connectToWifi = WiFi.isConnected();
	bool connectToHost = m_client->connected();

	bool flag = false;

	if(connectToWifi != m_connectedToHost)
	{
		m_connectedToWiFi = connectToWifi;
		flag = true;
	}
	if(connectToHost != m_connectedToHost)
	{
		m_connectedToHost = connectToHost;
		flag = true;
	}

	if(flag)
		updateAPClients();

	return (connectToWifi && connectToHost);
}

bool Controler::checkAvailable()
{
	return m_client->available() > 0;
}

void Controler::readAvailable()
{
	Serial.println(m_client->available());
	Buffer buffer;
	char *s = new char[4];
	for(int i = 0; i < 4; i++)
		s[i] = m_client->read();
	int readSize = *reinterpret_cast<int *>(s);
	Serial.println(readSize);
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

bool Controler::checkItemForPassability(Item &checkItem)
{
	for(auto room : m_rooms->rooms)
	{
		for(Item *item : room->items.m_items)
		{
			for(int checkPin : checkItem.pins)
			{
				for(int pin : item->pins)
				{
					if(pin == checkPin)
						return false;
				}
			}
		}
	}
	return true;
}

void Controler::parseCommand(const Command &command)
{
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
					updateRooms();
					break;
				}
				case Command::AddItem:
				{
					Item temp_item = Item::fromBuffer(command.buffer());
					Room* room = m_rooms->roomFromIdentifier(temp_item.roomIdentifier);
					
					if(!checkItemForPassability(temp_item))
					{
						Serial.println("Bad item");
						return;
					}
					
					if(!room)
					{
						Serial.println("Bad room");
						return;
					}
					
					Item *item = room->addItem(temp_item);
					ArduinoObject *object;
					if (item->type == Item::Led)
					{
						object = new LedObject(item->pins[0]);
					}
					else if (item->type == Item::Servo)
					{
						object = new ServoObject(item->pins[0]);
						static_cast<ServoObject *>(object)->rotate(90);
					}else if(item->type == Item::MagSig)
					{
						item->monitor = true;
						object = new MagSigObject(item->pins[0]);
					}else if(item->type == Item::Camera)
					{
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
					
					if(!room)
						return;
					
					Item *item = room->items.itemFromIdentifier(temp_item.identifier);
					
					if(!item)
						return;
					
					Serial.println(temp_item.type);
					Serial.println(temp_item.pins[0]);
					for (ArduinoObject *obj : m_objects)
					{
						if(!obj)
						{
							continue;
						}
						if (obj->identifier() == temp_item.identifier)
						{
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
									ArduinoCameraOV7670Object *cam = static_cast<ArduinoCameraOV7670Object *>(obj);
									Serial.println((cam == nullptr));
									item->dataSize = 9666;
									cam->oneFrame();
									item->data = new unsigned char[9666];
									
									Serial.println( ArduinoCameraOV7670Object::headerSize + cam->width() * cam->height() * 2);
									
									for(int i = 0; i < ArduinoCameraOV7670Object::headerSize; i++)
										item->data[i] = ArduinoCameraOV7670Object::bmpHeader[i];
									for(int i = ArduinoCameraOV7670Object::headerSize; i < 9666; i++)
										item->data[i] = cam->frame()[i-ArduinoCameraOV7670Object::headerSize];
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
						item->data = nullptr;
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
					char *s = buffer.toBytes();
					m_client->write(s, buffer.fullSize());
					delete[] s;
					break;
				}
					break;
			}
		case Command::ConfirmConnection:
			{
				Command command(Command::ConfirmConnection);
				Buffer buffer = command.toBuffer();
				char *s = buffer.toBytes();
				m_client->write(s, buffer.fullSize());
				delete[] s;
				break;
			}
			}
			break;
			
		default:
			break;
	}
}

void Controler::updateAPClients()
{
    ControlerSettings settings;
    
    settings.ssid = m_ssid;
    settings.password = m_password;
    settings.connectedToWiFi = m_connectedToWiFi;
    settings.connectedToServer = m_connectedToHost;

    Buffer buffer = settings.toBuffer();

	for(WiFiClient *client : m_apClients)
	{
		if(!client)
			continue;
		if(!client->connected())
			continue;
		char *s = buffer.toBytes();
		client->write(s, buffer.fullSize());
		delete[] s;
	}
}

void Controler::updateRooms()
{
	Buffer buffer;
	BufferStream stream(&buffer, BufferStream::WriteOnly);
	stream << *m_rooms;
	Command c(Command::Control, Command::UpdateRooms, buffer);
	Buffer newb = c.toBuffer();
	auto s = newb.toBytes();
	m_client->write(s, newb.fullSize());
	delete[] s;
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
	Serial.print("BUFFSIZE:");
	Serial.println(c.buffer().size);
	char *bytes = ds.toBytes();
	m_client->write(bytes, ds.fullSize());
	delete[] bytes;
	updateRooms();
}

void readSsidPassword()
{
	//   uint8_t ssidSize = 0, passSize = 0;
	
	//   ssidSize = EEPROM.read(0);
	//   ssid = new char[ssidSize + 1];
	//   passSize = EEPROM.read(ssidSize + 1);
	//   password = new char[passSize + 1];
	//   for(int i = 0; i < ssidSize; i++)
	//     ssid[i] = EEPROM.read(i+1);
	//   for(int i = 0; i < passSize; i++)
	//     password[i] = EEPROM.read(i + 1 + ssidSize + 1);
	
	//     ssid[ssidSize] = 0;
	//     password[passSize] = 0;
}

void writeSsidPassword()
{
	// uint8_t ssidSize = -1, passSize = -1;
	// while(ssid[++ssidSize]);
	// while(password[++passSize]);
	
	// EEPROM.write(0, ssidSize); // 0
	// for(int i = 0; i < ssidSize; i++)
	//     EEPROM.write(i + 1, ssid[i]); // 1,2,3
	// EEPROM.write(ssidSize + 1, passSize); // 4
	// for(int i = 0; i < passSize; i++)
	//     EEPROM.write(i + 1 + ssidSize + 1, password[i]); // 5,6,7
	
	// Serial.print("WRITE: ");
	// Serial.println(ssid);
	// Serial.print("WRITE: ");
	// Serial.println(password);
	// Serial.println("WRITEEND");
	
	// Serial.println(passSize);
	
	// EEPROM.commit();
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