#include "client.h"

Client* Client::m_instance = nullptr;

Client::Client(QObject *parent) : QObject(parent)
{
	m_instance = this;
	m_clientSocket = new ClientSocket(this);

	m_rooms = new QtRooms(this);
}

Command::OS Client::currentOS()
{
	return Command::OS::Android; // FIXIT
#if defined (Q_OS_WIN)
	return Command::OS::Windows;
#elif defined (Q_OS_ANDROID)
	return Command::OS::Android;
#elif defined (Q_OS_IOS)
	return Command::OS::Android;
#endif
	return Command::OS::UnknownOS;
}

ClientSocket *Client::clientSocket() const
{
	return m_clientSocket;
}

QtRooms *Client::rooms() const
{
	return m_rooms;
}

void Client::setRooms(const Rooms &rooms)
{
	m_rooms->setRooms(rooms);
}
Client *Client::instance()
{
	return m_instance;
}
