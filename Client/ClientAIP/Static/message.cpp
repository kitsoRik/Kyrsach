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
#if defined(Q_OS_WIN)
	tray.showMessage(title, text);
#elif defined(Q_OS_ANDROID)
	QAndroidJniObject::callStaticMethod<void>("org/MyNot",
										   "notify1");
#endif
}

void Message::sendMessage(const QString &title, const QString &text)
{
	Message message;
	message.title = title;
	message.text = text;
	message.send();
}

