#include "qtroom.h"
#include "qtitems.h"
#include <QDebug>

QtRoom::QtRoom(QObject *parent) : QObject(parent), m_firstName(false)
{
	m_qtItems = new QtItems(this);
}

void QtRoom::setName(const QString &name)
{
	m_name = name;
	emit nameChanged();
}

QString QtRoom::name() const
{
	return m_name;
}

QtItems *QtRoom::qtItems() const
{
	return m_qtItems;
}

void QtRoom::setRoom(const Room &room)
{
	m_room = room;
	nameChanged();
	if(!m_firstName)
	{
		m_name = QString::fromStdString(m_room.name);
		emit nameChanged();
		m_firstName = true;
	}

	m_qtItems->setItems(room.items);
}
