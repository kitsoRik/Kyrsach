#ifndef ITEMACTIONS_H
#define ITEMACTIONS_H

#include "itemaction.h"

class ItemActions : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int size READ size NOTIFY sizeChanged)
public:
	explicit ItemActions(QObject *parent = nullptr);

	Q_INVOKABLE ItemAction *itemActionAt(const int& index);
	QList<ItemAction *> listItemAction();

	int size() const;

signals:
	void sizeChanged();

public slots:
	ItemAction* addItemAction(QtItem *source);
	void deleteItemAt(const int &index);
	void deleteItem(ItemAction *itemAction);

private:
	QList<ItemAction *> m_itemActions;
};

#endif // ITEMACTIONS_H
