#include "room.h"

#include <sstream>

Room::Room()
{
	name = "MY ROOM";
}

Item* Room::addItem(const Item& it)
{
	Item *item = items.addItem(it);
	item->roomIdentifier = this->identifier;
	item->roomName = this->name;
	return item;
}

Buffer Room::toBuffer() const
{
	Buffer buffer;
	BufferStream stream (&buffer, BufferStream::WriteOnly);

	stream << *this;
	return buffer;
}

Room Room::fromBuffer(Buffer buffer)
{
	Room room;
	BufferStream stream (&buffer, BufferStream::WriteOnly);

	stream >> room;
	return room;
}

BufferStream &operator <<(BufferStream &stream, const Room &room)
{
	stream << room.identifier;
	stream << room.name;
	stream << room.items;
	return stream;
}

BufferStream &operator >>(BufferStream &stream, Room &room)
{
	stream >> room.identifier;
	stream >> room.name;
	stream >> room.items;
	return stream;
}

Room * Rooms::roomFromIdentifier(const std::string &identifier)
{
	for(auto room : rooms)
	{
		if(room->identifier == identifier)
			return room;
	}
	return nullptr;
}

Room* Rooms::createRoom(const std::string &name)
{
	static int n = 0;

	std::stringstream ss;
	ss << n++;

	Room *room = new Room;
	room->identifier = ss.str();
	room->name = name;

	return room;
}

Room* Rooms::addRoom(const Room &r)
{
	Room *room = new Room(r);
	rooms.push_back(room);
	return room;
}

Buffer Rooms::toBuffer() const
{
	Buffer buffer;
	BufferStream stream (&buffer, BufferStream::WriteOnly);

	stream << *this;
	return buffer;
}

Rooms Rooms::fromBuffer(Buffer buffer)
{
	Rooms rooms;
	BufferStream stream (&buffer, BufferStream::WriteOnly);

	stream >> rooms;
	return rooms;
}

BufferStream &operator <<(BufferStream &stream, const Rooms &rooms)
{
	stream << static_cast<unsigned int>(rooms.rooms.size());

	for(Room *room : rooms.rooms)
	{
		stream << *room;
	}

	return stream;
}

BufferStream &operator >>(BufferStream &stream, Rooms &rooms)
{
	unsigned int size;
	stream >> size;

	for(unsigned int i = 0; i < size; i++)
	{
		Room *room = new Room;
		stream >> *room;
		rooms.rooms.push_back(room);
	}
	return stream;
}
