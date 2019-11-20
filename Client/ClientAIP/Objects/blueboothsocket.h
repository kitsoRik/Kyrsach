#ifndef BLUEBOOTHSOCKET_H
#define BLUEBOOTHSOCKET_H

#include <QtBluetooth>

class BlueboothSocket : public QObject
{
	Q_OBJECT
public:
	explicit BlueboothSocket(QObject *parent = nullptr);

signals:

public slots:

private:
	QBluetoothSocket *socket;
	QBluetoothDeviceDiscoveryAgent *agent;
};

#endif // BLUEBOOTHSOCKET_H
