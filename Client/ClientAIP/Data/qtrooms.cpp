#include "qtrooms.h"
#include <QtDebug>

QtRooms::QtRooms(QObject *parent) : QObject(parent)
{

}

int QtRooms::roomsSize() const
{
	return m_rooms.size();
}

QtRoom *QtRooms::roomAt(const int &index)
{
	return m_rooms.at(index);
}

void QtRooms::setRooms(const Rooms &rooms)
{
	unsigned int roomsSize = static_cast<unsigned int>(rooms.rooms.size()),
			size = static_cast<unsigned int>(m_rooms.size());
	if(roomsSize != size)
	{
		if(roomsSize > size)
		{
			while(roomsSize != size)
			{
				QtRoom *room = new QtRoom(this);
				m_rooms.append(room);
				size = static_cast<unsigned int>(m_rooms.size());
			}
		}else
		{
			while(roomsSize != size)
			{
				QtRoom *room = m_rooms.back();
				m_rooms.pop_back();
				delete room;
			}
		}
		roomsSizeChanged();
	}

	int i = 0;
	for(auto it = rooms.rooms.begin(); it != rooms.rooms.end(); it++)
	{
		Room *room = *it;
		QtRoom *qtRoom = m_rooms[i++];
		qtRoom->setRoom(*room);
	}
}
