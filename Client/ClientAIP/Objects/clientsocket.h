#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "Data/command.h"

#if defined(Q_OS_WIN)
const QString HOST = "31.131.28.232";
#elif defined(Q_OS_ANDROID)
const QString HOST = "31.131.28.232";
#endif
const quint16 PORT = 3000;

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
	void connectToServer(const QString &host = HOST, const quint16 &port = PORT);
	void connectToControler(const QString &key, const QString &login, const QString &password);

	void addRoom(const QString &name);
	void addItem(const QList<int> &pins, const QString &type, const QString &roomName);
	void turnItem(const Item &item);
	void updateItems();
	void updateRooms();

	void addUser(const QString &login, const QString &password);

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
