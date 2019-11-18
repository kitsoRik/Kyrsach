#ifndef CLIENT_H
#define CLIENT_H

#include "Objects/clientsocket.h"
#include "Data/qtrooms.h"
#include "Objects/usersobject.h"

class Client : public QObject
{
	Q_OBJECT

	Q_PROPERTY(ClientSocket* clientSocket READ clientSocket CONSTANT)
	Q_PROPERTY(QtRooms* rooms READ rooms NOTIFY roomsChanged)
	Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY isAdminChanged)
	Q_PROPERTY(QString login READ login NOTIFY loginChanged)
	Q_PROPERTY(UsersObject* usersObject READ usersObject CONSTANT)

public:
	explicit Client(QObject *parent = nullptr);

	static Client *instance();

	static Command::OS currentOS();
	ClientSocket *clientSocket() const;

	QtRooms *rooms() const;
	void setRooms(const Rooms &rooms);

	UsersObject *usersObject() const;

	bool isAdmin() const;
	void setIsAdmin(bool isAdmin);

	QString login() const;
	void setLogin(const QString &login);

signals:
	void roomsChanged();
	void isAdminChanged();
	void loginChanged();

public slots:

private:
	bool m_isAdmin;
	QString m_login;
	ClientSocket *m_clientSocket;

	QtRooms *m_rooms;
	UsersObject *m_usersObject;

	static Client *m_instance;
};

#endif // CLIENT_H
