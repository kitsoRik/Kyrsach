#include "clientsocket.h"
#include <QTimer>
#include "Data/buffer.h"
#include "Data/item.h"
#include "client.h"
#include "Data/user.h"
#if defined(Q_OS_ANDROID)
#include <QtAndroidExtras>
#endif

#if defined(Q_OS_WIN)
const QString HOST = "192.168.0.106";
#elif defined(Q_OS_ANDROID)
const QString HOST = "192.168.0.106";
#endif
const int PORT = 3000;

ClientSocket::ClientSocket(QObject *parent)
	: QObject(parent), m_connectedToServer(false), m_connectedToControler(false)
{
	m_socket = new QTcpSocket(this);

	connect(m_socket, &QTcpSocket::connected, this, &ClientSocket::onConnected);
	connect(m_socket, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &ClientSocket::onReadyRead);
}

void ClientSocket::connectToServer(const QString &host, const quint16 &port)
{
	m_socket->disconnectFromHost();
	m_socket->connectToHost(host, port);
}

void ClientSocket::connectToControler(const QString &key, const QString &login, const QString &password)
{
	m_key = key;
	Client::instance()->setLogin(login);
	keyChanged();

	Command command(Command::Connect2Controler, (key + '|' + login + '|' + password).toStdString());

	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
}

void ClientSocket::addRoom()
{
	static int n = 0;
	Room room;
	room.name = ("Room " + QString::number(n++)).toStdString();


	Command command(Command::Control, Command::AddRoom, room.toBuffer());
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
}

void ClientSocket::addItem(const QList<int> &pins, const QString &type, const QString &roomName)
{
	qDebug() << "ADD ITEM IN ROOMNAME" << roomName;
	Item item;
	item.pins.resize(pins.size());
	for(int i =0; i < pins.size(); i++)
		item.pins[i] = pins[i];
	item.roomIdentifier = roomName.toStdString();
	qDebug() << pins;
	qDebug() << item.pins;
	if(type == "LED")
		item.type = Item::Led;
	else if(type == "SERVO")
		item.type = Item::Servo;
	else if(type == "MAGSIG")
		item.type = Item::MagSig;
	else
		item.type = Item::Camera;
	item.monitor = type == "MAGSIG";

	Command command(Command::Control, Command::AddItem, item.toBuffer());
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
	qDebug() << "ADD" << type;
}

void ClientSocket::turnItem(const Item &item)
{
	Command command(Command::Control, Command::TurnItem, item.toBuffer());
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
}

void ClientSocket::updateItems()
{
	Command command(Command::Control, Command::UpdateItems);
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
}

void ClientSocket::updateRooms()
{
	Command command(Command::Control, Command::UpdateRooms);
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
}

void ClientSocket::addUser(const QString &login, const QString &password)
{
	User u(login.toStdString(), password.toStdString());
	Buffer buffer;
	BufferStream stream (&buffer, BufferStream::WriteOnly);
	stream << u;
	Command command(Command::Settings, Command::AddUser, buffer);
	buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
}

void ClientSocket::onConnected()
{
	qDebug() << "CLIENT CONNECTED";
	Buffer buffer = Command(Command::Confirm, Command::Android, "KEYROSTIK").toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());

	m_connectedToServer = true;
	emit connectedToServerChanged();
}

void ClientSocket::onDisconnected()
{
	qDebug() << "CLIENT DISCONNECTED";
	m_connectedToServer = false;
	emit connectedToServerChanged();
}

void ClientSocket::onReadyRead()
{
	QByteArray data = m_socket->readAll();
	onReadyReadCheck(data);
}

void ClientSocket::onReadyReadCheck(QByteArray data)
{
	static int alldatasize = 0;
	static bool writedata = false;
	static QByteArray allData = "";
	if(allData.isEmpty())
	{
		writedata = true;
		char* temp_size = new char[4];
		for (unsigned int i = 0; i < 4; i++)
		{
			temp_size[i] = data[i];
		}


		alldatasize = *reinterpret_cast<unsigned int*>(temp_size) + 4;

	}

	if(!writedata)
		return;
	allData += data;
	qDebug() << "SUM" << allData.size();
	if(allData.size() == alldatasize)
	{
		writedata = false;
		onReadyReadProccess(allData);
		allData.clear();
	}else if(allData.size() > alldatasize)
	{
		QByteArray firstData = allData.left(alldatasize);
		onReadyReadProccess(firstData);
		QByteArray secondData = allData.right(allData.size() - firstData.size());
		allData.clear();
		onReadyReadCheck(secondData);
	}
}

void ClientSocket::onReadyReadProccess(QByteArray data)
{
	Buffer buffer = Buffer::fromBytes(data);
	Commands commands = Commands::fromBuffer(buffer);

	for(unsigned int i = 0; i < commands.commands.size(); i++)
	{
		Command c = commands.commands[i];
		switch (c.title())
		{
			case Command::ConfirmConnect2Controler:
			{
				QString adminStr = QString::fromStdString(c.buffer().split(' ')[0]);
				qDebug() << adminStr;
				Client::instance()->setIsAdmin(adminStr == "ADMIN");
				m_connectedToControler = true;
				emit connectedToControlerChanged();
				break;
			}
			case Command::Control:
			{
				switch (c.controlAction())
				{
					case Command::UpdateItems:
					{
						qDebug() << "UPDITS";
						Items items = Items::fromBuffer(c.buffer());
						qDebug() << items.m_items.size();
						qDebug() << "UPDITS2";
						break;
					}
					case Command::UpdateRooms:
					{
						qDebug() << "UPDRMS";
						Rooms rooms = Rooms::fromBuffer(c.buffer());
						Client::instance()->setRooms(rooms);
						qDebug() << "UPDRMS2";
						break;
					}
					case Command::TriggeredItem:
					{
						Item item = Item::fromBuffer(c.buffer());
						break;
					}
					default:
						break;
				}
				break;
			}
			case Command::Settings:
			{
				switch (c.settingsAction())
				{
					case Command::UsersInfo:
					{
						Buffer buffer = c.buffer();
						BufferStream stream (&buffer, BufferStream::ReadOnly);

						int size;
						stream >> size;

						QList<User> users;
						for(int i = 0; i < size; i++)
						{
							User u;
							stream >> u;
							users.append(u);
						}

						Client::instance()->usersObject()->setUsers(users);

						break;
					}
				}
				break;
			}
			default:
				break;
		}
	}
}

QString ClientSocket::key() const
{
	return m_key;
}

bool ClientSocket::connectedToControler() const
{
	return m_connectedToControler;
}

bool ClientSocket::connectedToServer() const
{
	return m_connectedToServer;
}
