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
	while(m_items.size() < items.m_items.size())
		m_items.append(new QtItem(this));
	while(m_items.size() > items.m_items.size())
	{
		auto item = m_items.last();
		m_items.pop_back();
		item->deleteLater();
	}
	int i = 0;
	for(auto item : items.m_items)
	{
		QtItem *qtItem = m_items.at(i++);
		qtItem->setItem(*item);
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
