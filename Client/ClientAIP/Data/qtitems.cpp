#include "qtitems.h"
#include <QtDebug>
#include "qtroom.h"

QtItems::QtItems(QtRoom *parent) : QObject(parent)
{
	m_room = parent;
}

int QtItems::itemsSize() const
{
	return m_items.size();
}

void QtItems::setItems(const Items &items)
{
	QList<QtItem *> its;
	if(m_items.size() == items.m_items.size())
	{
		int i = 0;
		for(auto item : items.m_items)
		{
			m_items[i++]->setItem(*item);
		}
		return;
	}
	qDeleteAll(m_items);
	m_items.clear();

	for(auto item : items.m_items)
	{
		QtItem *qtItem = new QtItem(this);
		qtItem->setItem(*item);
		m_items.append(qtItem);
	}

	emit itemsSizeChanged();
}

QtItem *QtItems::itemAt(const int &index)
{
	return m_items.at(index);
}

QtItem *QtItems::itemPin(const int &pin)
{
	for(auto item : m_items)
	{
		if(item->pin() == pin)
			return item;
	}
	return nullptr;
}

QtRoom *QtItems::room()
{
	return m_room;
}
