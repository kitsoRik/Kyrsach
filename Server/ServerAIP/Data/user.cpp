#include "user.h"

User::User()
{

}

std::string User::login() const
{
	return m_login;
}

void User::setLogin(int login)
{
	m_login = login;
}

int User::online() const
{
	return m_online;
}

void User::setOnline(int online)
{
	m_online = online;
}

std::string User::password() const
{
	return m_password;
}

void User::setPassword(const std::string &password)
{
	m_password = password;
}
