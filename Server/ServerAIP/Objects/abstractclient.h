#ifndef ABSTRACTCLIENT_H
#define ABSTRACTCLIENT_H

#include <QTcpSocket>
#include "Data/command.h"

class AbstractClient : public QObject
{
	Q_OBJECT
public:
	explicit AbstractClient(QTcpSocket *socket, QObject *parent = nullptr);

	QTcpSocket *m_socket;
signals:
	void confirmed();

	void readyCommand(const Command &command);

public slots:
	void confirmConnect2Controler();


private slots:
	void onConnected();
	void onDisconnected();
	void onReadyRead();

private:
};

#endif // ABSTRACTCLIENT_H
