#include "controlersocket.h"

ControlerSocket::ControlerSocket(QObject *parent) : QObject(parent)
{
	m_socket = new QTcpSocket(this);

	connect(m_socket, &QTcpSocket::connected, [this]()
	{
		qDebug() << "CONNECTED";
		emit connectedChanged();
	});

	connect(m_socket, &QTcpSocket::stateChanged, [this](QAbstractSocket::SocketState state)
	{
	});


	connect(m_socket, &QTcpSocket::disconnected, [this]()
	{
		qDebug() << "DISCONNECTED";
		emit connectedChanged();
	});
	connect(m_socket, &QTcpSocket::readyRead, [this]()
	{
		auto data = m_socket->readAll();

		Buffer buffer = Buffer::fromBytes(data);
		BufferStream stream (&buffer, BufferStream::ReadOnly);

		stream >> m_settings;

		emit ssidChanged();
		emit passwordChanged();
		emit connectToWiFiChanged();
		emit connectToServerChanged();
	});

}

bool ControlerSocket::connected() const
{
	return m_socket->state() == QTcpSocket::ConnectedState;
}

QString ControlerSocket::ssid() const
{
	return QString::fromStdString(m_settings.ssid);
}

void ControlerSocket::setSsid(const QString &ssid)
{
	m_settings.ssid = ssid.toStdString();
	emit ssidChanged();
}

QString ControlerSocket::password() const
{
	return QString::fromStdString(m_settings.password);
}

void ControlerSocket::setPassword(const QString &pass)
{
	m_settings.password = pass.toStdString();
	passwordChanged();
}

bool ControlerSocket::connectToWiFi() const
{
	return m_settings.connectedToWiFi;
}

bool ControlerSocket::connectToServer() const
{
	return m_settings.connectedToServer;
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

void ControlerSocket::saveSettings() const
{
	Buffer buffer;
	BufferStream stream (&buffer, BufferStream::WriteOnly);

	stream << m_settings;

	char *s = buffer.toBytes();
	m_socket->write(s, buffer.fullSize());
	delete[] s;
}
