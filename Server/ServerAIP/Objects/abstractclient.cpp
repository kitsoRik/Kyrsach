#include "abstractclient.h"

AbstractClient::AbstractClient(QTcpSocket *socket, QObject *parent)
	: QObject(parent), m_isAdmin(false)
{
	m_socket = socket;

	if(m_socket == nullptr)
		throw "Nullptr socket";

	connect(m_socket, &QTcpSocket::connected, this, &AbstractClient::onConnected);
	connect(m_socket, &QTcpSocket::disconnected, this, &AbstractClient::onDisconnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &AbstractClient::onReadyRead);
}

void AbstractClient::confirmConnect2Controler(const bool &isAdmin)
{
	Command command(Command::ConfirmConnect2Controler);
	command.setBuffer(isAdmin ? "ADMIN" : "USER");
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
}

void AbstractClient::sendUsersInfoToAdmin(const QList<User> &users)
{
	Buffer buffer;
	BufferStream stream (&buffer, BufferStream::WriteOnly);

	stream << users.size();
	for(int i = 0; i < users.size(); i++)
	{
		stream << users.at(i);
	}

	Command command (Command::Settings, Command::UsersInfo, buffer);
	buffer = command.toBuffer();
	m_socket->write(buffer.toBytes(), buffer.fullSize());
}

void AbstractClient::onConnected()
{
	qDebug() << "CLIENT CONNECTED";
}

void AbstractClient::onDisconnected()
{
	qDebug() << "CLIENT DISCONNECTED";
}

void AbstractClient::onReadyRead()
{
	auto data = m_socket->readAll();
	Buffer buffer = Buffer::fromBytes(data);
	Commands commands = Commands::fromBuffer(buffer);
	for(unsigned int i = 0; i < commands.commands.size(); i++)
		emit readyCommand(commands.commands.at(i));
}

bool AbstractClient::isAdmin() const
{
	return m_isAdmin;
}

void AbstractClient::setIsAdmin(bool isAdmin)
{
	m_isAdmin = isAdmin;
}
