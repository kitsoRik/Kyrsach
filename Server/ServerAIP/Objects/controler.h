#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QTcpSocket>
#include "Objects/androidclient.h"
#include "Data/room.h"
#include "Data/user.h"

class Controler : public QObject
{
	Q_OBJECT
public:
	enum CheckConnectionResult
	{
		Unknown,
		Admin,
		User
	};

	explicit Controler(QTcpSocket *socket, QObject *parent = nullptr);

	void addClient(const CheckConnectionResult &connection, AbstractClient *client);
	void addUser(const QString &login, const QString &pass);

	QString key() const;
	void setKey(const QString &key);

	QList<AbstractClient *> clients() const;

	friend class ControlerList;

	bool confirmedConnection() const;
	void setConfirmedConnection(bool confirmedConnection);

	CheckConnectionResult checkConnectionData(const QString &login, const QString &password);

signals:
	void disconnected();

public slots:
	void parseCommand(const Command &command);

	void updateItems(AbstractClient *client);
	void updateRooms(AbstractClient *client);
	void triggeredItem(Item item, AbstractClient *client);

	void updateUsers();

private slots:
	void onConnected();
	void onDisconnected();
	void onReadyRead();
	void onReadyReadCheck(QByteArray data);
	void onReadyReadProccess(QByteArray data);

	void onReadyCommand(const Command &command);

private:
	QTcpSocket *m_socket;
	QString m_key;

	QList<AbstractClient *> m_clients;

	Rooms m_rooms;
	QList<::User> m_users;

	bool m_confirmedConnection;

};

#endif // CONTROLER_H
