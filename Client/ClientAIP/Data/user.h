#ifndef USER_H
#define USER_H

#include "buffer.h"

class User
{
public:
	User();
	User(const std::string &login, const std::string &pass, const bool &online = false)
	{
		m_login = login;
		m_password = pass;
		m_online = online;
	}

	friend BufferStream& operator << (BufferStream &stream, const User &user)
	{
		stream << user.m_login;
		stream << user.m_password;
		stream << user.m_online;

		return stream;
	}

	friend BufferStream& operator >> (BufferStream &stream, User &user)
	{
		stream >> user.m_login;
		stream >> user.m_password;
		stream >> user.m_online;

		return stream;
	}

	std::string login() const;
	void setLogin(int login);

	int online() const;
	void setOnline(int online);

	std::string password() const;
	void setPassword(const std::string &password);

private:
	std::string m_login;
	std::string m_password;
	int m_online;
};

#endif // USER_H
