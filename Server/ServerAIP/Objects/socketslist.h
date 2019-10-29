#ifndef SOCKETSLIST_H
#define SOCKETSLIST_H

#include "Data/command.h"
#include <QTcpSocket>

class SocketsList : public QObject
{
	Q_OBJECT
public:
	explicit SocketsList(QObject *parent = nullptr);

signals:
	void confirmOS(const Command &command, QTcpSocket *socket);

public slots:
	void add(QTcpSocket *socket);

	bool remove(QTcpSocket *socket);

private slots:
	void onReadyRead();

private:
	QList<QTcpSocket *> m_sockets;
};

#endif // SOCKETSLIST_H
