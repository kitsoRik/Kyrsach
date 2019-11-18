#ifndef USERSOBJECT_H
#define USERSOBJECT_H

#include "Data/user.h"
#include "userobject.h"

class UsersObject : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int usersSize READ usersSize NOTIFY usersSizeChanged)
public:
	explicit UsersObject(QObject *parent = nullptr);

	void setUsers(const QList<User> &users);

	int usersSize() const;

signals:
	void usersSizeChanged();

public slots:
	UserObject *userAt(const int &index);

private:
	QList<UserObject *> m_users;
};

#endif // USERSOBJECT_H
