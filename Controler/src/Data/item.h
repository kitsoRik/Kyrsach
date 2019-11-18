#ifndef ITEM_H
#define ITEM_H

#include "buffer.h"
#include <list>

class Item;
class Items;

class Item
{
public:
	enum ItemType{
		Led,
		Servo,
		MagSig,
		Camera
	};

	std::string roomIdentifier;
	std::string roomName;
	std::string identifier;
	std::vector<int> pins;
	int type;
	int angle;
	bool on;
	bool monitor;
	unsigned char *data;
	int dataSize;

	Item();
	~Item();


	Buffer toBuffer() const;
	static Item fromBuffer(Buffer buffer);

	friend BufferStream& operator <<(BufferStream &parser, const Item &item);
	friend BufferStream& operator >>(BufferStream &parser, Item &item);
};

class Items
{
public:
	std::list<Item *> m_items;

	Item* addItem(const Item& item);
	Item* itemFromIdentifier(const std::string& identifier);

	Buffer toBuffer();
	static Items fromBuffer(Buffer buffer);

	friend BufferStream& operator <<(BufferStream &parser, const Items &items);
	friend BufferStream& operator >>(BufferStream &parser, Items &items);
};

#endif // ITEM_H
