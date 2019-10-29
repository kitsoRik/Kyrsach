#ifndef COMMAND_H
#define COMMAND_H

#include "Data/buffer.h"
#include <string>

class Commands;

class Command
{
public:
	enum Title
	{
		UnknownTitle = -1,
		Confirm,
		Control,
		Connect2Controler,
		ConfirmConnect2Controler,
		ConfirmConnection
	};

	enum OS
	{
		UnknownOS = -1,
		Controler,
		Android,
		Windows
	};

	enum ControlAction
	{
		UnknownControlAction = -1,
		TurnItem,
		AddItem,
		AddRoom,
		UpdateItems,
		UpdateRooms,
		TriggeredItem
	};

	Command();
	Command(const Title& title, const Buffer &buffer = "");
	Command(const Title& title, const OS& os, const Buffer &buffer = "");
	Command(const Title& title, const ControlAction& controlAction, const Buffer &buffer = "");



	Title title() const;
	void setTitle(const Title& title);

	OS os() const;
	void setOs(const OS& os);

	ControlAction controlAction() const;
	void setControlAction(const ControlAction &controlAction);

	Buffer buffer() const;
	void setBuffer(const Buffer &buffer);

	Buffer toBuffer() const;
	static Command fromBuffer(Buffer &buffer);

	friend BufferStream& operator <<(BufferStream& parser, const Command& command);
	friend BufferStream& operator >>(BufferStream& parser, Command& command);

private:
	Title m_title;
	OS m_os;
	ControlAction m_controlAction;

	Buffer m_buffer;
};

class Commands
{
public:
	std::vector<Command> commands;

	static Commands fromBuffer(Buffer buffer);
};

#endif // COMMAND_H
