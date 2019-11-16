#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#if defined(Q_OS_WIN)
#include <QApplication>
#endif

#include "client.h"
#include <iostream>

int main(int argc, char *argv[])
{

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#if defined(Q_OS_WIN)
	QApplication app(argc, argv);
#elif defined (Q_OS_ANDROID)
	QGuiApplication app(argc, argv);
#endif

	qmlRegisterType<ClientSocket>("RostikObjects", 1, 0, "ClientSocket");
	qmlRegisterType<QtItems>("RostikObjects", 1, 0, "QtItems");
	qmlRegisterType<QtItem>("RostikObjects", 1, 0, "QtItem");
	qmlRegisterType<QtRooms>("RostikObjects", 1, 0, "QtRooms");
	qmlRegisterType<QtRoom>("RostikObjects", 1, 0, "QtRoom");
	qmlRegisterType<ItemAction>("RostikObjects", 1, 0, "ItemAction");
	qmlRegisterType<ItemActions>("RostikObjects", 1, 0, "ItemActions");

	Client *client = new Client();

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);

	engine.rootContext()->setContextProperty("client", client);

	engine.load(url);

	return app.exec();
}

