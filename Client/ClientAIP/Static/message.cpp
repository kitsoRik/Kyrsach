#include "message.h"
#if defined(Q_OS_WIN)
#include <QFileIconProvider>
#elif defined(Q_OS_ANDROID)
#include <QtAndroidExtras>
#endif

Message::Message()
{
#if defined(Q_OS_WIN)
	tray.setIcon(QFileIconProvider().icon(QFileIconProvider::Folder));
	tray.show();
#endif
}

void Message::send()
{
	QString title = titleFromType(type);
	QString text = this->text.replace("%pin%", QString::number(this->pin));
#if defined(Q_OS_WIN)
	tray.showMessage(title, text);
#elif defined(Q_OS_ANDROID)
	QAndroidJniObject::callStaticMethod<void>("org/MyNot",
										   "notify1");
#endif
}

void Message::sendChanged(const int &pin)
{
	Message message;
	message.pin = pin;
	message.type = Changed;
	message.text = "Pin %pin% changed";
	message.send();
}

void Message::sendMessage(const QString &title, const QString &text)
{
	Message message;
	message.type = Changed;
	message.text = text;
	message.send();
}

QString Message::titleFromType(const Message::MsgType msgType)
{
	switch (msgType)
	{
		case Changed: return "Changed";
	}
}
