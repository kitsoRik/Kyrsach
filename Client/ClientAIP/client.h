#ifndef CLIENT_H
#define CLIENT_H

#include "Objects/clientsocket.h"
#include "Data/qtrooms.h"

class Client : public QObject
{
	Q_OBJECT

	Q_PROPERTY(ClientSocket* clientSocket READ clientSocket CONSTANT)
	Q_PROPERTY(QtRooms* rooms READ rooms NOTIFY roomsChanged)
public:
	explicit Client(QObject *parent = nullptr);

	static Client *instance();

	static Command::OS currentOS();
	ClientSocket *clientSocket() const;

	QtRooms *rooms() const;
	void setRooms(const Rooms &rooms);

signals:
	void roomsChanged();

public slots:

private:
	ClientSocket *m_clientSocket;

	QtRooms *m_rooms;

	static Client *m_instance;
};

#endif // CLIENT_H
