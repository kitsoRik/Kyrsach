#include "androidclient.h"

AndroidClient::AndroidClient(QTcpSocket *socket, QObject *parent)
	: AbstractClient(socket, parent)
{
	qDebug() << "ANDROID CLIENT CREATED";
}
