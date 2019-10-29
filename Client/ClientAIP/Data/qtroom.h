#ifndef QTROOM_H
#define QTROOM_H

#include <QObject>
#include "room.h"
#include "qtitems.h"

class QtRoom : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QtItems* qtItems READ qtItems CONSTANT)
public:
	explicit QtRoom(QObject *parent = nullptr);

	void setName(const QString &name);
	QString name() const;

	QtItems *qtItems() const;

	void setRoom(const Room &room);

signals:
	void nameChanged();

public slots:

private:
	Room m_room;
	bool m_firstName;
	QString m_name;
	QtItems *m_qtItems;
};

#endif // QTROOM_H
