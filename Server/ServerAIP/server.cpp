#include "server.h"

const int PORT = 3000;
Server *Server::m_instance = nullptr;

Server::Server(QObject *parent) : QObject(parent)
{
	m_instance = this;

	m_server = new QTcpServer(this);

	connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);

	if(!m_server->listen(QHostAddress::Any, PORT))
	{
		qDebug() << "Error listen server: " <<  m_server->errorString();
		qApp->quit();
	}

	qDebug() << "Listening " << QString::number(PORT) + " port..";

	connect(&m_waitSockets, &SocketsList::confirmOS, this, &Server::onConfirmOS);

	connect(&m_waitClients, &ClientsList::connect2Controler, this, &Server::onConnect2Controler);
}

Server::~Server()
{

}

void Server::onNewConnection()
{
	qDebug() << "New connection";
	QTcpSocket *socket = m_server->nextPendingConnection();

	m_waitSockets.add(socket);
}

void Server::onConfirmOS(const Command &command, QTcpSocket *socket)
{
	socket->disconnect();
	m_waitSockets.remove(socket);

	Command::OS os = command.os();

	switch (os)
	{
		case Command::OS::Controler:
		{
			QString key = QString::fromUtf8(command.buffer().array, command.buffer().size);
			Controler *controler = m_controlers.fromKey(key);
			if(controler)
			{
				controler->changeSocket(socket);
			}else
			{
				controler = new Controler(socket, this);
				controler->setKey(key);

				m_controlers.addControler(controler);
			}
			break;
		}
		case Command::OS::Android:
		{
			auto client = new AndroidClient(socket, this);
			m_waitClients.addClient(client);
			break;
		}
		default:
		{
			throw "UNKNOWN CONFIRM OS";
		}
	}
}

void Server::onConnect2Controler(const QString &key,
								 const QString &login,
								 const QString &password,
								 AbstractClient *client)
{
	Controler *controler = m_controlers.fromKey(key);
	if(!controler)
	{
		qDebug() << "NULLABLE CLIENT CONTROLER";
		return;
	}
	auto checkConType = controler->checkConnectionData(login, password);
	if(checkConType == Controler::CheckConnectionResult::Unknown)
	{
		qDebug() << "INCORRECT DATA";
		return;
	}
	m_waitClients.remove(client);
	controler->addClient(checkConType, client);

}
