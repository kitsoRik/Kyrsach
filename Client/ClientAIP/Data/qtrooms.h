#ifndef QTROOMS_H
#define QTROOMS_H

#include <QObject>
#include "qtroom.h"

class QtRooms : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int roomsSize READ roomsSize NOTIFY roomsSizeChanged)
public:
	explicit QtRooms(QObject *parent = nullptr);

	int roomsSize() const;

	Q_INVOKABLE QtRoom *roomAt(const int &index);

signals:
	void roomsSizeChanged();

public slots:
	void setRooms(const Rooms &rooms);

private:
	QList<QtRoom *> m_rooms;
};

#endif // QTROOMS_H
