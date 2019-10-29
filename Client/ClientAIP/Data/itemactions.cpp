#include "itemactions.h"

ItemActions::ItemActions(QObject *parent) : QObject(parent)
{

}

ItemAction *ItemActions::itemActionAt(const int &index)
{
	ItemAction *item = m_itemActions.at(index);
	return item;
}

QList<ItemAction *> ItemActions::listItemAction()
{
	return m_itemActions;
}

int ItemActions::size() const
{
	return m_itemActions.size();
}

ItemAction *ItemActions::addItemAction(QtItem *source)
{
	ItemAction *itemAction = new ItemAction(this);

	itemAction->setSource(source);

	m_itemActions.append(itemAction);

	emit sizeChanged();

	return itemAction;
}

void ItemActions::deleteItemAt(const int &index)
{
	ItemAction *itemAction = m_itemActions.at(index);
	m_itemActions.removeAt(index);
	delete itemAction;
	emit sizeChanged();
}

void ItemActions::deleteItem(ItemAction *itemAction)
{
	deleteItemAt(m_itemActions.indexOf(itemAction));
}
