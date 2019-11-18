#ifndef USEROBJECT_H
#define USEROBJECT_H

#include "Data/user.h"
#include <QObject>

class UserObject : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString login READ login NOTIFY loginChanged)
	Q_PROPERTY(bool online READ online NOTIFY onlineChanged)
public:
	explicit UserObject(QObject *parent = nullptr);

	void setUser(const User &user);

	QString login();
	bool online();

signals:
	void loginChanged();
	void onlineChanged();

public slots:

private:
	User m_user;
};

#endif // USEROBJECT_H
