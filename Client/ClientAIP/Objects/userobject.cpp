#include "userobject.h"

UserObject::UserObject(QObject *parent) : QObject(parent)
{

}

void UserObject::setUser(const User &user)
{
	m_user = user;

	loginChanged();
	onlineChanged();
}

QString UserObject::login()
{
	return QString::fromStdString(m_user.login());
}

bool UserObject::online()
{
	return m_user.online();
}
