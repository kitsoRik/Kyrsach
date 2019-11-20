#ifndef CONTROLERSOCKET_H
#define CONTROLERSOCKET_H

#include <QtNetwork>

class ControlerSocket : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
public:
	explicit ControlerSocket(QObject *parent = nullptr);

	bool connected() const;

signals:
	void connectedChanged();

public slots:
	void connectToHost(const QString &host, const quint16 &port);

	void writeSsidPassword(const QString &ssid, const QString &password);

private:
	QTcpSocket *m_socket;
};

#endif // CONTROLERSOCKET_H
