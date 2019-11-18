#ifndef CLIENTSLIST_H
#define CLIENTSLIST_H

#include <QList>
#include "Objects/abstractclient.h"

class ClientsList : public QObject
{
	Q_OBJECT
public:
	explicit ClientsList(QObject *parent = nullptr);

signals:
	void connect2Controler(const QString &key,
						   const QString &login,
						   const QString &password,
						   AbstractClient *client);

public slots:
	void addClient(AbstractClient *client);
	void addClients(QList<AbstractClient *> clients);

	bool remove(AbstractClient *client);

private slots:
	void onReadyCommand(const Command& command);
private:
	QList<AbstractClient *> m_clients;
};

#endif // CLIENTSLIST_H
