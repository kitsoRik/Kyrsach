#include "item.h"
#include <sstream>

Item::Item()
{
	type = 0;
}

Buffer Item::toBuffer() const
{
	Buffer buffer;
	BufferStream stream (&buffer, BufferStream::WriteOnly);
	stream << *this;
	return buffer;
}

Item Item::fromBuffer(Buffer buffer)
{
	Item item;
	BufferStream stream (&buffer, BufferStream::WriteOnly);
	stream >> item;
	return item;
}

BufferStream &operator <<(BufferStream &stream, const Item &item)
{
	stream << item.roomIdentifier;
	stream << item.roomName;
	stream << item.identifier;
	stream << item.type;
	stream << item.pin;
	stream << item.angle;
	stream << item.on;
	stream << item.monitor;
	return stream;
}

BufferStream &operator >>(BufferStream &stream, Item &item)
{
	stream >> item.roomIdentifier;
	stream >> item.roomName;
	stream >> item.identifier;
	stream >> item.type;
	stream >> item.pin;
	stream >> item.angle;
	stream >> item.on;
	stream >> item.monitor;
	return stream;
}

Item* Items::addItem(const Item& it)
{
	static int n = 0;
	std::stringstream ss;
	ss << n++;

	Item *item = new Item(it);
	item->identifier = ss.str();
	m_items.push_front(item);

	return item;
}

Item* Items::itemFromIdentifier(const std::string &identifier)
{
	for(Item *item : m_items)
	{
		if(item->identifier == identifier)
			return item;
	}
	return nullptr;
}

Buffer Items::toBuffer()
{
	Buffer buffer;
	BufferStream stream (&buffer, BufferStream::WriteOnly);
	stream << *this;
	return buffer;
}

Items Items::fromBuffer(Buffer buffer)
{
	Items items;
	BufferStream stream (&buffer, BufferStream::WriteOnly);
	stream >> items;
	return items;
}

BufferStream &operator >>(BufferStream &parser, Items &items)
{
	unsigned int size;
	parser >> size;
	for(unsigned int i = 0; i < size; i++)
	{
		Item *it = new Item;
		parser >> *it;
		items.m_items.push_back(it);
	}
	return parser;
}

BufferStream &operator <<(BufferStream &parser, const Items &items)
{
	parser << static_cast<unsigned int>(items.m_items.size());
	for(Item *item : items.m_items)
	{
		parser << *item;
	}
	return parser;
}
