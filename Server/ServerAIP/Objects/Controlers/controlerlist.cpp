#include "controlerlist.h"
#include "server.h"

ControlerList::ControlerList(QObject *parent) : QObject(parent)
{

}

ControlerList::~ControlerList()
{

}

Controler *ControlerList::fromKey(const QString &key)
{
	for(auto controler : m_controlers)
	{
		if(controler->key() == key)
			return controler;
	}
	return nullptr;
}

void ControlerList::addControler(Controler *controler)
{
	m_controlers.append(controler);

	connect(controler, &Controler::disconnected, this, &ControlerList::onDisconnectedControler);

	const int INTERVAL = 5000;

	QTimer *timer = new QTimer(controler);
	timer->setInterval(INTERVAL);
	timer->start(INTERVAL);

	connect(timer, &QTimer::timeout, [timer, controler]()
	{
		return;
		if(!controler->m_confirmedConnection)
		{
			controler->m_socket->disconnectFromHost();
			timer->stop();
			timer->deleteLater();
			qDebug() << "DELETE";
			return;
		}
		controler->m_confirmedConnection = false;
		Command command(Command::ConfirmConnection);
		Buffer buffer = command.toBuffer();
		char *s = buffer.toBytes();
		controler->m_socket->write(s, buffer.fullSize());
		delete[] s;
	});
}

void ControlerList::removeControler(Controler *controler)
{
	m_controlers.removeOne(controler);
}

void ControlerList::onDisconnectedControler()
{
	Server *server = Server::instance();
	auto controler = static_cast<Controler *>(sender());

	removeControler(controler);


	server->m_waitClients.addClients(controler->m_clients);

	controler->deleteLater();
}
