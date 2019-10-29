#ifndef QTITEMS_H
#define QTITEMS_H

#include <QObject>
#include "Data/qtitem.h"


class QtItems : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int itemsSize READ itemsSize NOTIFY itemsSizeChanged)
public:
	explicit QtItems(QtRoom *parent = nullptr);

	int itemsSize() const;

	void setItems(const Items &items);

	Q_INVOKABLE QtItem *itemAt(const int &index);
	QtItem *itemPin(const int& pin);

	QtRoom *room();

signals:
	void itemsSizeChanged();

public slots:

private:
	QList<QtItem *> m_items;
	QtRoom *m_room;
};

#endif // QTITEMS_H
