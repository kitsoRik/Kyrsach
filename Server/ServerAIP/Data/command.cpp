#include "command.h"

Command::Command()
	: m_title(UnknownTitle),
	  m_os(UnknownOS),
	  m_controlAction(UnknownControlAction)
{

}

Command::Command(const Command::Title &title,
				 const Buffer &buffer)
	: Command()
{
	m_title = title;
	m_buffer = buffer;
}

Command::Command(const Command::Title& title,
				 const Command::OS& os,
				 const Buffer &buffer)
	: Command()
{
	m_title = title;
	m_os = os;
	m_buffer = buffer;
}

Command::Command(const Command::Title &title,
				 const Command::ControlAction &controlAction,
				 const Buffer &buffer)
	: Command()
{
	m_title = title;
	m_controlAction = controlAction;
	m_buffer = buffer;
}

Command::Command(const Command::Title &title,
				 const Command::SettingsAction &settingsAction,
				 const Buffer &buffer)
	: Command()
{
	m_title = title;
	m_settingsAction = settingsAction;
	m_buffer = buffer;
}

Command::Title Command::title() const
{
	return m_title;
}

void Command::setTitle(const Command::Title &title)
{
	m_title = title;
}

Command::OS Command::os() const
{
	return m_os;
}

void Command::setOs(const Command::OS &os)
{
	m_os = os;
}

Command::ControlAction Command::controlAction() const
{
	return m_controlAction;
}

void Command::setControlAction(const Command::ControlAction &controlAction)
{
	m_controlAction = controlAction;
}

Buffer Command::buffer() const
{
	return m_buffer;
}

void Command::setBuffer(const Buffer &buffer)
{
	m_buffer = buffer;
}

Buffer Command::toBuffer() const
{
	Buffer buffer;
	BufferStream stream (&buffer, BufferStream::WriteOnly);

	stream << '|';
	stream << *this;
	return buffer;
}

Command Command::fromBuffer(Buffer &buffer)
{
	Command command;
	BufferStream stream (&buffer, BufferStream::WriteOnly);
	stream >> command;
	return command;
}

Command::SettingsAction Command::settingsAction() const
{
	return m_settingsAction;
}

void Command::setSettingsAction(const SettingsAction &settingsAction)
{
	m_settingsAction = settingsAction;
}

BufferStream& operator <<(BufferStream& stream, const Command& command)
{
	stream << command.m_title;
	stream << command.m_os;
	stream << command.m_controlAction;
	stream << command.m_settingsAction;
	stream << command.m_buffer;

	return stream;
}

BufferStream& operator >>(BufferStream& stream, Command& command)
{
	stream >> *reinterpret_cast<int *>(&command.m_title);
	stream >> *reinterpret_cast<int *>(&command.m_os);
	stream >> *reinterpret_cast<int *>(&command.m_controlAction);
	stream >> *reinterpret_cast<int *>(&command.m_settingsAction);
	stream >> command.m_buffer;

	return stream;
}

Commands Commands::fromBuffer(Buffer buffer)
{
	Commands commands;
	BufferStream stream (&buffer, BufferStream::ReadOnly);

	while(true)
	{
		char s;
		stream >> s;
		if(s != '|')
		{
			break;
		}
		Command command;
		stream >> command;
		commands.commands.push_back(command);

		for(int i = 0; i < 4; i++)
		{
			stream >> s;
		}
	}
	return commands;
}
