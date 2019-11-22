#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#if defined(Q_OS_WIN)
#include <QApplication>
#include <QSystemTrayIcon>
#endif

class Message
{
public:
	Message();

	void send();

	QString title;
	QString text;

#if defined(Q_OS_WIN)
	QSystemTrayIcon tray;
#endif

	static void sendMessage(const QString &title, const QString &text);
};

#endif // MESSAGE_H
