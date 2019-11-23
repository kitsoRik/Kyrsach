#ifndef CONTROLERSOCKET_H
#define CONTROLERSOCKET_H

#include <QtNetwork>
#include "Data/controlersettings.h"

class ControlerSocket : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

	Q_PROPERTY(QString ssid READ ssid WRITE setSsid NOTIFY ssidChanged)
	Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

	Q_PROPERTY(bool connectToWiFi READ connectToWiFi NOTIFY connectToWiFiChanged)
	Q_PROPERTY(bool connectToServer READ connectToServer NOTIFY connectToServerChanged)

public:
	explicit ControlerSocket(QObject *parent = nullptr);

	bool connected() const;

	QString ssid() const;
	void setSsid(const QString &ssid);

	QString password() const;
	void setPassword(const QString &pass);

	bool connectToWiFi() const;
	bool connectToServer() const;

signals:
	void connectedChanged();
	void ssidChanged();
	void passwordChanged();

	void connectToWiFiChanged();
	void connectToServerChanged();


public slots:
	void connectToHost(const QString &host, const quint16 &port);

	void writeSsidPassword(const QString &ssid, const QString &password);

	void saveSettings() const;

private:
	QTcpSocket *m_socket;

	ControlerSettings m_settings;
};

#endif // CONTROLERSOCKET_H
