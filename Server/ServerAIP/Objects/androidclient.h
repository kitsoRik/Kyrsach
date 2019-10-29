#ifndef ANDROIDCLIENT_H
#define ANDROIDCLIENT_H

#include "abstractclient.h"

class AndroidClient : public AbstractClient
{
	Q_OBJECT
public:
	explicit AndroidClient(QTcpSocket *socket, QObject *parent = nullptr);

signals:

public slots:
};

#endif // ANDROIDCLIENT_H
