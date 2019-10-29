#include "qtitem.h"
#include "client.h"
#include "qtitems.h"
#include "qtroom.h"

QtItem::QtItem(QtItems *items) : QObject(items), m_firstName(false)
{
	m_items = items;
	m_itemActions = new ItemActions(this);
}

void QtItem::setItem(const Item &item)
{
	bool changed = m_item.on != item.on;
	m_item = item;

	emit pinChanged();
	emit typeChanged();
	emit angleChanged();
	emit onChanged();

	if(!m_firstName)
	{
		m_name = typeString();
		emit nameChanged();
		m_firstName = true;
	}

	if(changed)
	{
		for(ItemAction *itemAction : m_itemActions->listItemAction())
		{
			if(m_item.on != itemAction->needValue())
				continue;
			itemAction->trigger();
		}
	}
}

int QtItem::pin() const
{
	return m_item.pin;
}

int QtItem::type() const
{
	return m_item.type;
}

int QtItem::angle() const
{
	return m_item.angle;
}

QString QtItem::typeString() const
{
	switch (m_item.type)
	{
		case Item::Led:
			return "LED";
		case Item::Servo:
			return "SERVO";
		case Item::MagSig:
			return "MAGSIG";
		default:
			return "UNKNOWN";
	}
}

bool QtItem::on() const
{
	return m_item.on;
}

ItemActions *QtItem::itemActions()
{
	return m_itemActions;
}

void QtItem::turn()
{
	switch (m_item.type)
	{
		case Item::Led:
		{
			turnLed();
			break;
		}
		case Item::Servo:
		{
			turnServo(300);
			break;
		}
	}
}

void QtItem::turn(const bool &value)
{
	Client *client = Client::instance();
	Item item = m_item;
	item.on = value;
	client->clientSocket()->turnItem(item);
}

void QtItem::turnLed()
{
	Client *client = Client::instance();
	Item item = m_item;

	item.type = Item::Led;
	item.on = !item.on;
	client->clientSocket()->turnItem(item);
}

void QtItem::turnServo(const int &angle)
{
	Client *client = Client::instance();
	Item item = m_item;

	item.type = Item::Servo;
	item.angle = angle;

	client->clientSocket()->turnItem(item);
}

void QtItem::turnMagSig()
{
	Client *client = Client::instance();
	Item item = m_item;

	item.type = Item::MagSig;

	client->clientSocket()->turnItem(item);
}

void QtItem::addLed()
{
	Client *client = Client::instance();
}

QString QtItem::name() const
{
	return m_name;
}

void QtItem::setName(const QString &name)
{
	m_name = name;
	nameChanged();
}

QtRoom *QtItem::room()
{
	return m_items->room();
}
