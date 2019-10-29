#include "socketslist.h"

SocketsList::SocketsList(QObject *parent) : QObject(parent)
{

}

void SocketsList::add(QTcpSocket *socket)
{
	m_sockets.append(socket);

	connect(socket, &QTcpSocket::readyRead, this, &SocketsList::onReadyRead);
}

bool SocketsList::remove(QTcpSocket *socket)
{
	return m_sockets.removeOne(socket);
}

void SocketsList::onReadyRead()
{
	QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
	auto data = socket->readAll();
	Buffer buffer = Buffer::fromUtf8(data, data.size());
	Commands commands = Commands::fromBuffer(buffer);
	Command command = commands.commands.front();
	switch (command.title())
	{
		case Command::Confirm:
		{
			emit confirmOS(command, socket);
			break;
		}
	}

}
