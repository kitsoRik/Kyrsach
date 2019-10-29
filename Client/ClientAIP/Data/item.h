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
		MagSig
	};

	std::string roomIdentifier;
	std::string roomName;
	std::string identifier;
	int pin;
	int type;
	int angle;
	bool on;
	bool monitor;

	Item();


	Buffer toBuffer() const;
	static Item fromBuffer(Buffer buffer);

	friend BufferStream& operator <<(BufferStream &parser, const Item &item);
	friend BufferStream& operator >>(BufferStream &parser, Item &item);
};

class Items
{
public:
	std::list<Item *> m_items;

	~Items(){
		for(Item *item :m_items)
		{
			delete item;
		}
	}

	Item* addItem(const Item& item);
	Item* itemFromIdentifier(const std::string& identifier);

	Buffer toBuffer();
	static Items fromBuffer(Buffer buffer);

	friend BufferStream& operator <<(BufferStream &parser, const Items &items);
	friend BufferStream& operator >>(BufferStream &parser, Items &items);
};

#endif // ITEM_H
