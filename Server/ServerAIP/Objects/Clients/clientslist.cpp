#include "clientslist.h"

#include <QVector>

ClientsList::ClientsList(QObject *parent) : QObject(parent)
{

}

void ClientsList::addClient(AbstractClient *client)
{
	connect(client, &AbstractClient::readyCommand, this, &ClientsList::onReadyCommand);

	m_clients.append(client);
}

void ClientsList::addClients(QList<AbstractClient *> clients)
{
	for(auto client : clients)
	{
		connect(client, &AbstractClient::readyCommand, this, &ClientsList::onReadyCommand);

		m_clients.append(client);
	}
}

bool ClientsList::remove(AbstractClient *client)
{
	disconnect(client, &AbstractClient::readyCommand, this, &ClientsList::onReadyCommand);
	return m_clients.removeOne(client);
}

void ClientsList::onReadyCommand(const Command &command)
{
	AbstractClient *client = static_cast<AbstractClient *>(sender());
	Command::Title title = command.title();
	switch (title)
	{
		case Command::Connect2Controler:
		{
			std::vector<std::string> v = command.buffer().split('|');
			QStringList list;
			auto it = v.begin();
			while(it != v.end())
				list.append(QString::fromStdString(*it++));

			if(list.size() != 2)
			{
				qDebug() << "ERROR. LIST SIZE =" << list.size();
				return;
			}
			QString key = list[0],
					password = list[1];
			emit connect2Controler(key, password, client);
			break;
		}
		default: break;
	}
}
