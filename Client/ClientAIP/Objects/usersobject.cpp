#include "usersobject.h"

UsersObject::UsersObject(QObject *parent) : QObject(parent)
{

}

void UsersObject::setUsers(const QList<User> &users)
{
	while(m_users.size() < users.size())
	{
		auto userObject = new UserObject(this);
		m_users.append(userObject);
	}

	while(m_users.size() > users.size())
	{
		auto userObject = m_users.last();
		m_users.pop_back();
		userObject->deleteLater();
	}

	int i = 0;
	for(auto user : users)
	{
		UserObject *userObject = m_users.at(i++);
		userObject->setUser(user);
	}

	emit usersSizeChanged();
}

int UsersObject::usersSize() const
{
	return m_users.size();
}

UserObject *UsersObject::userAt(const int &index)
{
	return m_users.at(index);
}
