#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include "Data/command.h"
#include "Objects/controler.h"
#include "Objects/androidclient.h"
#include "Objects/Controlers/controlerlist.h"
#include "Objects/Clients/clientslist.h"
#include "Objects/socketslist.h"

class Server : public QObject
{
	Q_OBJECT

	friend class ControlerList;
public:
	explicit Server(QObject *parent = nullptr);
	~Server() override;

	static Server *instance() { return m_instance; }

signals:

public slots:

private slots:
	void onNewConnection();

	void onConfirmOS(const Command &command, QTcpSocket *socket);

	void onConnect2Controler(const QString &key,
							 const QString &login,
							 const QString &password,
							 AbstractClient *client);

private:
	QTcpServer *m_server;

	SocketsList m_waitSockets;
	ClientsList m_waitClients;
	ControlerList m_controlers;

	static Server *m_instance;
};

#endif // SERVER_H
