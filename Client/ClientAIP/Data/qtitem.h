#ifndef QTITEM_H
#define QTITEM_H

#include <QObject>
#include "item.h"
#include <Static/message.h>
#include "itemactions.h"
#include <QImage>

class QtItems;
class QtRoom;
class ItemActions;

class QtItem : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int pin READ pin NOTIFY pinChanged)
	Q_PROPERTY(QString type READ typeString NOTIFY typeChanged)
	Q_PROPERTY(int angle READ angle NOTIFY angleChanged)
	Q_PROPERTY(bool on READ on NOTIFY onChanged)
	Q_PROPERTY(ItemActions* itemActions READ itemActions CONSTANT)
	Q_PROPERTY(QtRoom* room READ room CONSTANT)

	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString image READ image NOTIFY imageChanged)

	Q_PROPERTY(bool pressCamera READ pressCamera WRITE setPressCamera NOTIFY pressCameraChanged)

	friend class QtItems;
public:
	explicit QtItem(QtItems* items = nullptr);

	void setItem(const Item &item);

	int pin() const;
	int type() const;
	int angle() const;
	QString typeString() const;
	bool on() const;

	ItemActions *itemActions();
	QtRoom *room();

	QString name() const;
	void setName(const QString &name);

	QString image() const;
	void setImage(const QImage &image);

	bool pressCamera() const;
	void setPressCamera(bool pressCamera);

signals:
	void pinChanged();
	void typeChanged();
	void angleChanged();
	void onChanged();

	void nameChanged();
	void imageChanged();

	void pressCameraChanged();

public slots:
	void turn();
	void turn(const bool &value);
	void turnLed();
	void turnServo(const int &angle);
	void turnMagSig();
	void turnCamera();

	static void addLed();

private:
	QtItems *m_items;
	Item m_item;
	ItemActions *m_itemActions;
	QImage m_image;

	bool m_firstName;
	QString m_name;

	bool m_pressCamera;
	bool m_canPressCamera;
};

#endif // QTITEM_H
