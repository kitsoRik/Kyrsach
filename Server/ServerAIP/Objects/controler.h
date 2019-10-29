#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QTcpSocket>
#include "Objects/androidclient.h"
#include "Data/room.h"

class Controler : public QObject
{
	Q_OBJECT
public:
	explicit Controler(QTcpSocket *socket, QObject *parent = nullptr);

	void addClient(AbstractClient *client);

	QString key() const;
	void setKey(const QString &key);

	QString password() const;
	void setPassword(const QString &password);

	QList<AbstractClient *> clients() const;

	friend class ControlerList;

	bool confirmedConnection() const;
	void setConfirmedConnection(bool confirmedConnection);

signals:
	void disconnected();

public slots:
	void parseCommand(const Command &command);

	void updateItems(AbstractClient *client);
	void updateRooms(AbstractClient *client);
	void triggeredItem(Item item, AbstractClient *client);

private slots:
	void onConnected();
	void onDisconnected();
	void onReadyRead();

	void onReadyCommand(const Command &command);

private:
	QTcpSocket *m_socket;
	QString m_key;
	QString m_password;

	QList<AbstractClient *> m_clients;

	Rooms m_rooms;

	bool m_confirmedConnection;

};

#endif // CONTROLER_H
