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

	enum MsgType
	{
		Changed
	};

	void send();

	int pin;
	MsgType type;
	QString text;

#if defined(Q_OS_WIN)
	QSystemTrayIcon tray;
#endif

	static void sendChanged(const int &pin);
	static void sendMessage(const QString &title, const QString &text);

private:
	QString titleFromType(const MsgType msgType);
};

#endif // MESSAGE_H
