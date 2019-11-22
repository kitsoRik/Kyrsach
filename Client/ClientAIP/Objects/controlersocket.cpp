#include "controlersocket.h"

ControlerSocket::ControlerSocket(QObject *parent) : QObject(parent)
{
	m_socket = new QTcpSocket(this);

	connect(m_socket, &QTcpSocket::connected, [this]()
	{
		qDebug() << "CONNECTED";
		emit connectedChanged();
	});


	connect(m_socket, &QTcpSocket::disconnected, [this]()
	{
		qDebug() << "DISCONNECTED";
		emit connectedChanged();
	});
	connect(m_socket, &QTcpSocket::readyRead, [this]()
	{
		auto data = m_socket->readAll();
		qDebug() << data;
		if(data == "OK")
			m_socket->disconnectFromHost();
	});
}

bool ControlerSocket::connected() const
{
	return m_socket->state() == QTcpSocket::ConnectedState;
}

void ControlerSocket::connectToHost(const QString &host, const quint16 &port)
{
	m_socket->connectToHost(host, port);
}

void ControlerSocket::writeSsidPassword(const QString &ssid, const QString &password)
{
	QString data = ssid + '|' + password;
	m_socket->write(data.toUtf8());
}
