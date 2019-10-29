#ifndef ROOM_H
#define ROOM_H

#include "item.h"

class Room
{
public:
	Room();

	std::string identifier;
	std::string name;

	Items items;

	Item* addItem(const Item &item);

	Buffer toBuffer() const;
	static Room fromBuffer(Buffer buffer);

	friend BufferStream& operator <<(BufferStream &stream, const Room &room);
	friend BufferStream& operator >>(BufferStream &stream, Room &room);
};

class Rooms
{
public:
	std::list<Room *> rooms;

	Room* roomFromIdentifier(const std::string &identifier);

	Room* createRoom(const std::string &name);
	Room* addRoom(const Room &room);

	Buffer toBuffer() const;
	static Rooms fromBuffer(Buffer buffer);

	friend BufferStream& operator <<(BufferStream &stream, const Rooms &rooms);
	friend BufferStream& operator >>(BufferStream &stream, Rooms &rooms);
};

#endif // ROOM_H
