#ifndef ABSTRACTCLIENT_H
#define ABSTRACTCLIENT_H

#include <QTcpSocket>
#include "Data/command.h"
#include "Data/user.h"

class AbstractClient : public QObject
{
	Q_OBJECT
public:
	explicit AbstractClient(QTcpSocket *socket, QObject *parent = nullptr);

	QTcpSocket *m_socket;

	bool isAdmin() const;
	void setIsAdmin(bool isAdmin);

signals:
	void confirmed();

	void readyCommand(const Command &command);

public slots:
	void confirmConnect2Controler(const bool &isAdmin);

	void sendUsersInfoToAdmin(const QList<User> &users);


private slots:
	void onConnected();
	void onDisconnected();
	void onReadyRead();

private:
	bool m_isAdmin;
};

#endif // ABSTRACTCLIENT_H
