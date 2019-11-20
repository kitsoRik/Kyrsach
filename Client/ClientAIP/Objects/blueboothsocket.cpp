#include "blueboothsocket.h"

BlueboothSocket::BlueboothSocket(QObject *parent) : QObject(parent)
{
	QBluetoothDeviceDiscoveryAgent *agent = new QBluetoothDeviceDiscoveryAgent;

	QObject::connect(agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
			[this](const QBluetoothDeviceInfo &info)
	{
		socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
		socket->connectToService(info.address(), QBluetoothUuid(QBluetoothUuid::SerialPort));
		qDebug() << "FOUND";
		QObject::connect(socket, &QBluetoothSocket::connected, [this]()
		{qDebug() << "WRITE";
			socket->write("Hello sukin syn");
		});
		QObject::connect(socket, &QBluetoothSocket::disconnected, []()
		{
		});
		QObject::connect(socket, &QBluetoothSocket::readyRead, [this]()
		{
		});
		QObject::connect(socket, &QBluetoothSocket::stateChanged, [this](QBluetoothSocket::SocketState state)
		{
		});
	});

	agent->start();
	qDebug() << "START";
}
