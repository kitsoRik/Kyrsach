#include "itemaction.h"

#include "itemactions.h"
#include "Static/message.h"
#include <QDebug>
#include "qtitem.h"

ItemAction::ItemAction(ItemActions *parent) : QObject(parent)
{
	m_itemActions = parent;
}

void ItemAction::trigger()
{
	if(!m_receiver)
	{
		Message::sendMessage("Title", "Triggered");
		return;
	}
	m_receiver->turn(m_setValue);
}

QtItem *ItemAction::source()
{
	return m_source;
}

void ItemAction::setSource(QtItem *source)
{
	m_source = source;
}

QtItem *ItemAction::receiver()
{
	return m_receiver;
}

void ItemAction::setReceiver(QtItem *receiver)
{
	m_receiver = receiver;
	emit receiverChanged();
}

int ItemAction::needValue() const
{
	return m_needValue;
}

void ItemAction::setNeedValue(int needValue)
{
	m_needValue = needValue;
	emit needValueChanged();
}

int ItemAction::setValue() const
{
	return m_setValue;
}

void ItemAction::setSetValue(int setValue)
{
	m_setValue = setValue;
	emit setValueChanged();
}

void ItemAction::deleteAction()
{
	m_itemActions->deleteItem(this);
}
