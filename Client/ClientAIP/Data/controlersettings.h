#ifndef CONTROLERSETTINGS_H
#define CONTROLERSETTINGS_H

#include "buffer.h"
#include <string.h>

class ControlerSettings
{
public:
	ControlerSettings() = default;

	std::string ssid;
	std::string password;
	bool connectedToWiFi;
	bool connectedToServer;

	friend BufferStream & operator <<(BufferStream &stream, const ControlerSettings &settings)
	{
		stream << settings.ssid;
		stream << settings.password;
		stream << settings.connectedToWiFi;
		stream << settings.connectedToServer;

		return stream;
	}

	friend BufferStream & operator >>(BufferStream &stream, ControlerSettings &settings)
	{
		stream >> settings.ssid;
		stream >> settings.password;
		stream >> settings.connectedToWiFi;
		stream >> settings.connectedToServer;

		return stream;
	}

	Buffer toBuffer() const
	{
		Buffer buffer;
		BufferStream stream (&buffer, BufferStream::WriteOnly);

		stream << *this;
		return buffer;
	}

	static ControlerSettings fromBuffer()
	{
		ControlerSettings settings;
		Buffer buffer;
		BufferStream stream (&buffer, BufferStream::ReadOnly);

		stream >> settings;
		return settings;
	}

};

#endif // CONTROLERSETTINGS_H
