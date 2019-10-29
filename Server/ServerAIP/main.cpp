#include <QCoreApplication>

#include "server.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	Server *serverControl = new Server;

	Q_UNUSED(serverControl)

	int exec = a.exec();
	return exec;
}
