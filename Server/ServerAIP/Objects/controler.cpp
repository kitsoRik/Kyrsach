#include "controler.h"

#include <QTimer>

Controler::Controler(QTcpSocket *socket, QObject *parent)
	: QObject(parent), m_password("PASS"), m_confirmedConnection(true)
{
	m_socket = socket;
	if(socket == nullptr)
		throw "Null socket";

	connect(m_socket, &QTcpSocket::connected, this, &Controler::onConnected);
	connect(m_socket, &QTcpSocket::disconnected, this, &Controler::onDisconnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &Controler::onReadyRead);

	qDebug() << "CONTROLER CREATED";
}

void Controler::addClient(AbstractClient *client)
{
	client->setParent(this);

	connect(client, &AbstractClient::readyCommand, this, &Controler::onReadyCommand);

	client->confirmConnect2Controler();

	updateRooms(client);

	m_clients.append(client);
}

void Controler::onConnected()
{
	qDebug() << "CONTROLER CONNECTED";
}

void Controler::onDisconnected()
{
	qDebug() << "CONTROLER DISCONNECTED";
	emit disconnected();
}

void Controler::onReadyRead()
{
	QByteArray data = m_socket->readAll();
	Buffer buffer = Buffer::fromUtf8(data, data.size());
	Commands commands = Commands::fromBuffer(buffer);
	for(unsigned int i = 0; i < commands.commands.size(); i++)
	{
		Command command = commands.commands[i];
		parseCommand(command);
	}
}

void Controler::onReadyCommand(const Command &command)
{
	AbstractClient *client = static_cast<AbstractClient *>(sender());

	switch(command.title())
	{
		case Command::Control:
		{
			switch(command.controlAction())
			{
				case Command::AddRoom:
				{
					Buffer buffer = command.toBuffer();
					m_socket->write(buffer.array, buffer.size);
					break;
				}
				case Command::AddItem:
				{
					Buffer buffer = command.toBuffer();
					m_socket->write(buffer.array, buffer.size);
					break;
				}
				case Command::TurnItem:
				{
					Buffer buffer = command.toBuffer();
					m_socket->write(buffer.array, buffer.size);
					qDebug() << "TURN" << command.buffer();

					buffer = command.buffer();
					Item item = Item::fromBuffer(buffer);
					qDebug() << item.type;
					break;
				}
				case Command::UpdateItems:
				{
					qDebug() << "UPDITSF";
					Buffer buffer = command.toBuffer();
					m_socket->write(buffer.array, buffer.fullSize());
					break;
				}
				case Command::UpdateRooms:
				{
					qDebug() << "UPDRMSF";
					Buffer buffer = command.toBuffer();
					m_socket->write(buffer.array, buffer.fullSize());
					break;
				}
				default:
				{
					qDebug() << "ERROR COMMAND TITLE";
					return;
				}
			}
			break;
		}
		default:
			break;
	}
}

bool Controler::confirmedConnection() const
{
	return m_confirmedConnection;
}

void Controler::setConfirmedConnection(bool confirmedConnection)
{
	m_confirmedConnection = confirmedConnection;
}

void Controler::parseCommand(const Command &command)
{
	Command::Title title = command.title();
	switch (title)
	{
		case Command::Control:
		{
			Command::ControlAction controlAction = command.controlAction();
			switch (controlAction)
			{
				case Command::UpdateItems:
				{
					return;
					Buffer buffer = command.buffer();
					//m_items = Items::fromBuffer(buffer);

					for(auto client : m_clients)
					{
						updateItems(client);
					}

					break;
				}
				case Command::UpdateRooms:
				{
					Buffer buffer = command.buffer();
					m_rooms = Rooms::fromBuffer(buffer);
					for(auto client : m_clients)
					{
						updateRooms(client);
					}

					break;
				}
				case Command::TriggeredItem:
				{
					qDebug() << "TRIGG";
					Buffer buffer = command.buffer();
					Item item = Item::fromBuffer(buffer);
					for(auto client : m_clients)
					{
						triggeredItem(item, client);
					}
					break;
				}
				default:
					break;
			}
			break;
		}
		case Command::ConfirmConnection:
		{
			m_confirmedConnection = true;
			break;
		}
		default:
			break;
	}
}

void Controler::updateItems(AbstractClient *client)
{
	Buffer buffer = m_rooms.toBuffer();
	Command command(Command::Control, Command::UpdateItems, buffer);
	buffer = command.toBuffer();
	client->m_socket->write(buffer.array, buffer.fullSize());
}

void Controler::updateRooms(AbstractClient *client)
{
	Buffer buffer = m_rooms.toBuffer();
	Command command(Command::Control, Command::UpdateRooms, buffer);
	buffer = command.toBuffer();
	client->m_socket->write(buffer.array, buffer.fullSize());
}

void Controler::triggeredItem(Item item, AbstractClient *client)
{
	Buffer buffer = item.toBuffer();
	Command command(Command::Control, Command::TriggeredItem, buffer);
	buffer = command.toBuffer();
	client->m_socket->write(buffer.array, buffer.fullSize());
}

QString Controler::password() const
{
	return m_password;
}

void Controler::setPassword(const QString &password)
{
	m_password = password;
}

QString Controler::key() const
{
	return m_key;
}

void Controler::setKey(const QString &key)
{
	m_key = key;
}

QList<AbstractClient *> Controler::clients() const
{
	return m_clients;
}
