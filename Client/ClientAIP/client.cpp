#include "client.h"

Client* Client::m_instance = nullptr;

Client::Client(QObject *parent) : QObject(parent), m_isAdmin(true)
{
	m_instance = this;
	m_clientSocket = new ClientSocket(this);

	m_rooms = new QtRooms(this);
	m_usersObject = new UsersObject(this);
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

UsersObject *Client::usersObject() const
{
	return m_usersObject;
}

bool Client::isAdmin() const
{
	return m_isAdmin;
}

void Client::setIsAdmin(bool isAdmin)
{
	m_isAdmin = isAdmin;
	emit isAdminChanged();
}

QString Client::login() const
{
	return m_login;
}

void Client::setLogin(const QString &login)
{
	m_login = login;
	emit loginChanged();
}

Client *Client::instance()
{
	return m_instance;
}
