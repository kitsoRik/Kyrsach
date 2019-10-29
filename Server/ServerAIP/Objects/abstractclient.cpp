#include "abstractclient.h"

AbstractClient::AbstractClient(QTcpSocket *socket, QObject *parent) : QObject(parent)
{
	m_socket = socket;

	if(m_socket == nullptr)
		throw "Nullptr socket";

	connect(m_socket, &QTcpSocket::connected, this, &AbstractClient::onConnected);
	connect(m_socket, &QTcpSocket::disconnected, this, &AbstractClient::onDisconnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &AbstractClient::onReadyRead);
}

void AbstractClient::confirmConnect2Controler()
{
	Command command(Command::ConfirmConnect2Controler);
	Buffer buffer = command.toBuffer();
	m_socket->write(buffer.array, buffer.fullSize());
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
	Buffer buffer = Buffer::fromUtf8(data, data.size());
	Commands commands = Commands::fromBuffer(buffer);
	for(unsigned int i = 0; i < commands.commands.size(); i++)
		emit readyCommand(commands.commands.at(i));
}
