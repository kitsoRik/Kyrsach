#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "Data/command.h"

class Item;

class ClientSocket : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool connectedToServer READ connectedToServer NOTIFY connectedToServerChanged)
	Q_PROPERTY(bool connectedToControler READ connectedToControler NOTIFY connectedToControlerChanged)

	Q_PROPERTY(QString key READ key NOTIFY keyChanged)
public:
	explicit ClientSocket(QObject *parent = nullptr);

	bool connectedToServer() const;
	bool connectedToControler() const;

	QString key() const;

signals:
	void connectedToServerChanged();
	void connectedToControlerChanged();

	void keyChanged();

public slots:
	void connectToServer(const QString &host, const quint16 &port);
	void connectToControler(const QString &key, const QString &password);

	void addRoom();
	void addItem(const QList<int> &pins, const QString &type, const QString &roomName);
	void turnItem(const Item &item);
	void updateItems();
	void updateRooms();

private slots:
	void onConnected();
	void onDisconnected();
	void onReadyRead();
	void onReadyReadCheck(QByteArray data);
	void onReadyReadProccess(QByteArray data);
private:
	QTcpSocket *m_socket;

	bool m_connectedToServer;
	bool m_connectedToControler;

	QString m_key;
};

#endif // CLIENTSOCKET_H
