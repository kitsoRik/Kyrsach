#include "qtitem.h"
#include "client.h"
#include "qtitems.h"
#include "qtroom.h"
#include <QFile>
#include <QBuffer>

QtItem::QtItem(QtItems *items) : QObject(items), m_firstName(false), m_canPressCamera(false)
{
	m_items = items;
	m_itemActions = new ItemActions(this);
}

void QtItem::setItem(const Item &item)
{
	/* QFile file("Q:/123.txt");
	file.open(QIODevice::ReadWrite);
	file.resize(0);

	for(int i = 0; i < item.dataSize; i++)
	{
		int a = item.data[i];
		file.write((QString::number(a) + " ").toUtf8());
	}
	file.close();

	QImage img = QImage::fromData(item.data, item.dataSize);
	setImage(img); */
	bool changed = m_item.on != item.on;
	m_item = item;

	emit pinChanged();
	emit typeChanged();
	emit angleChanged();
	emit onChanged();

	if(m_item.type == Item::Camera)
	{
		QImage img = QImage::fromData(item.data, item.dataSize);
		setImage(img);
		m_canPressCamera = true;
		if(m_pressCamera)
			turnCamera();
	}

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
	return m_item.pins[0];
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
		case Item::Camera:
			return "CAMERA";
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

void QtItem::turnCamera()
{
	if(!m_canPressCamera)
		return;
	m_canPressCamera = false;
	Client *client = Client::instance();
	Item item = m_item;

	item.dataSize = 0;
	delete[] item.data;
	item.data = nullptr;
	item.on = true;
	item.type = Item::Camera;

	client->clientSocket()->turnItem(item);
}

void QtItem::addLed()
{
	Client *client = Client::instance();
}

bool QtItem::pressCamera() const
{
	return m_pressCamera;
}

void QtItem::setPressCamera(bool pressCamera)
{
	m_pressCamera = pressCamera;
	emit pressCameraChanged();
}

QString QtItem::image() const
{
	QByteArray bArray;
	QBuffer buffer(&bArray);
	buffer.open(QIODevice::WriteOnly);
	m_image.save(&buffer, "JPEG");

	QString imageStr("data:image/jpg;base64,");
	imageStr.append(QString::fromLatin1(bArray.toBase64().data()));

	return imageStr;
}

void QtItem::setImage(const QImage &image)
{

	m_image = image;
	emit imageChanged();
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
