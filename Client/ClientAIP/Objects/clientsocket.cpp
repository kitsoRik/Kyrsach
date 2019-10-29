#include "clientsocket.h"
#include <QTimer>
#include "Data/buffer.h"
#include "Data/item.h"
#include "client.h"
#if defined(Q_OS_ANDROID)
#include <QtAndroidExtras>
#endif

#if defined(Q_OS_WIN)
const QString HOST = "31.131.31.228";
#elif defined(Q_OS_ANDROID)
const QString HOST = "31.131.31.228";
#endif
const int PORT = 3000;

ClientSocket::ClientSocket(QObject *parent)
	: QObject(parent), m_connectedToServer(false), m_connectedToControler(false)
{
	m_socket = new QTcpSocket(this);

	connect(m_socket, &QTcpSocket::connected, this, &ClientSocket::onConnected);
	connect(m_socket, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &ClientSocket::onReadyRead);

	connectToServer(HOST, PORT);
}

void ClientSocket::connectToServer(const QString &host, const quint16 &port)
{
	m_socket->connectToHost(host, port);
}

void ClientSocket::connectToControler(const QString &key, const QString &password)
{
	m_key = key;
	keyChanged();

	Command command(Command::Connect2Controler, (key + '|' + password).toStdString());

	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.array, buffer.fullSize());
}

void ClientSocket::addRoom()
{
	static int n = 0;
	Room room;
	room.name = ("Room " + QString::number(n++)).toStdString();


	Command command(Command::Control, Command::AddRoom, room.toBuffer());
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.array, buffer.fullSize());
}

void ClientSocket::addItem(const int &pin, const QString &type, const QString &roomName)
{
	Item item;
	item.pin = pin;
	item.roomName = roomName.toStdString();
	item.type = type == "LED" ? Item::Led : type == "SERVO" ? Item::Servo : Item::MagSig;
	item.monitor = type == "MAGSIG";

	Command command(Command::Control, Command::AddItem, item.toBuffer());
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.array, buffer.fullSize());
}

void ClientSocket::turnItem(const Item &item)
{
	Command command(Command::Control, Command::TurnItem, item.toBuffer());
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.array, buffer.fullSize());
}

void ClientSocket::updateItems()
{
	Command command(Command::Control, Command::UpdateItems);
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.array, buffer.fullSize());
}

void ClientSocket::updateRooms()
{
	Command command(Command::Control, Command::UpdateRooms);
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.array, buffer.fullSize());
}

void ClientSocket::onConnected()
{
	qDebug() << "CLIENT CONNECTED";
	Buffer buffer = Command(Command::Confirm, Command::Android, "KEYROSTIK").toBuffer();
	m_socket->write(buffer.array, buffer.fullSize());

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
	auto data = m_socket->readAll();
	Buffer buffer = Buffer::fromUtf8(data, data.size());
	Commands commands = Commands::fromBuffer(buffer);

	for(unsigned int i = 0; i < commands.commands.size(); i++)
	{
		Command c = commands.commands[i];
		switch (c.title())
		{
			case Command::ConfirmConnect2Controler:
			{
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
						qDebug() << "TRIGG" << item.pin;
						break;
					}
					default:
						break;
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
