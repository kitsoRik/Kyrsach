#include "buffer.h"

Buffer::Buffer() : size(0), array(nullptr)
{
	
}

Buffer::Buffer(const Buffer &buffer)
{
	size = buffer.size;
	if(size != 0)
		this->array = new char[size];
	else
		this->array = nullptr;
	for (int i = 0; i < size; i++)
		this->array[i] = buffer.array[i];

}

Buffer::Buffer(char* s)
{
	*this = fromUtf8(static_cast<const char*>(s));
}

Buffer::Buffer(const char* s)
{
	*this = fromUtf8(s);
}

Buffer::Buffer(const std::string& str)
{
	*this = fromUtf8(str);
}

void Buffer::append(const char& c)
{
	resize(size + 1);
	array[size - 1] = c;
}

void Buffer::append(const char* c, const unsigned int& size)
{
	unsigned int oldSize = this->size;
	resize(this->size + size);
	for (unsigned int i = oldSize; i < this->size; i++)
	{
		array[i] = c[i];
	}
}

void Buffer::resize(const unsigned int& size)
{
	if (this->size == size)
		return;
	char* s = new char[size];

	unsigned int i;
	for (i = 0; i < size && i < this->size; i++)
	{
		s[i] = array[i];
	}
	for (; i < size; i++)
		s[i] = 0;

	this->size = size;

	if(array)
	{
		delete array;
		array = nullptr;
	}
	array = s;
}

char* Buffer::toBytes()
{
	unsigned int fullSize = size + 4;
	char* data = new char[fullSize];
	for (unsigned int i = 0; i < 4; i++)
	{
		data[i] = static_cast<char>(size >> i * 8);
	}
	for (unsigned int i = 4; i < fullSize; i++)
	{
		data[i] = array[i - 4];
	}

	return data;
}

Buffer Buffer::fromBytes(const char* s)
{
	Buffer buffer;

	char* temp_size = new char[4];
	for (unsigned int i = 0; i < 4; i++)
	{
		temp_size[i] = s[i];
	}


	buffer.size = *reinterpret_cast<unsigned int*>(temp_size);

	buffer.array = new char[buffer.size];
	for (unsigned int i = 0; i < buffer.size; i++)
	{
		buffer.array[i] = s[i + 4];
	}

	return buffer;
}

std::vector<std::string> Buffer::split(const char sep)
{
	std::vector<std::string> v;
	unsigned int lastSepN = 0;
	for (unsigned int i = 0; i < this->size; i++)
	{
		if (this->array[i] == sep)
		{
			std::string temp;
			for (unsigned int j = lastSepN; j < i; j++)
			{
				temp += this->array[j];
			}
			v.push_back(temp);
			lastSepN = i + 1;
		}
	}
	std::string temp;
	for (unsigned int j = lastSepN; j < this->size; j++)
	{
		temp += this->array[j];
	}
	v.push_back(temp);
	return v;
}

Buffer Buffer::fromUtf8(const std::string & str)
{
	return fromUtf8(str.c_str(), static_cast<unsigned int>(str.size()));
}

Buffer Buffer::fromUtf8(const char* s)
{
	unsigned int size = 0;
	while (s[size++]);
	return fromUtf8(s, size-1);
}

Buffer Buffer::fromUtf8(const char* s, const int size)
{
	return Buffer::fromUtf8(s, static_cast<unsigned int>(size));
}

Buffer Buffer::fromUtf8(const char* s, const unsigned int size)
{
	Buffer buffer;
	buffer.resize(size);
	for (unsigned int i = 0; i < size; i++)
	{
		buffer.array[i] = s[i];
	}
	return buffer;
}

BufferStream& operator <<(BufferStream & stream, const Buffer & buffer)
{
	unsigned int size = buffer.size;
	stream.writeData(buffer.array, size);

	return stream;
}

BufferStream& operator >>(BufferStream & stream, Buffer & buffer)
{
	unsigned int size;
	stream >> size;

	buffer.resize(size);

	for (unsigned int i = 0; i < size; i++)
	{
		stream >> buffer.array[i];
	}

	return stream;
}

char& Buffer::operator[](const unsigned int& index)
{
	return array[index];
}

Buffer& Buffer::operator=(const Buffer &buffer)
{
	if(this->array)
	{
		Serial.print("SIZE WHEN DEL: ");
		Serial.println(this->size);
		delete[] this->array;
		this->array = nullptr;
	}
	this->size = buffer.size;
	array = new char[size];

	for(int i = 0; i < size; i++)
		array[i] = buffer.array[i];
}

BufferStream::BufferStream(Buffer * buffer, const BufferStream::TypeStream & typeStream)
	: m_readIndex(0), m_writeIndex(0)
{
	m_buffer = buffer;
	m_typeStream = typeStream;
}

BufferStream& BufferStream::writeString(const char* s)
{
	unsigned int size = 0;
	while(s[size++]);
	*this << size;
	for (unsigned int i = 0; i < size; i++)
		* this << s[i];
	return *this;
}

BufferStream& BufferStream::readString(char*& s)
{
	unsigned int size;
	*this >> size;
	s = new char[size + 1];
	for (unsigned int i = 0; i < size; i++)
	{
		char c;
		*this >> c;
		s[i] = c;
	}
	s[size] = 0;
	return *this;
}

BufferStream &BufferStream::writeData(const unsigned char *s, const unsigned int &size)
{
	unsigned int t_size = size;
	*this << t_size;
	m_buffer->resize(m_buffer->size + size);
	for (unsigned int i = 0; i < size; i++)
	{
		char c = s[i];
		(*m_buffer)[m_writeIndex++] = static_cast<char>(c);
	}
	return *this;
}

BufferStream& BufferStream::writeData(const char* s, const unsigned int& size)
{
	unsigned int t_size = size;
	*this << t_size;
	m_buffer->resize(m_buffer->size + size);
	for (unsigned int i = 0; i < size; i++)
	{
		char c = s[i];
		(*m_buffer)[m_writeIndex++] = c;
	}
	return *this;
}

BufferStream &BufferStream::readData(unsigned char *&s, unsigned int &size)
{
	*this >> size;
	s = new unsigned char[size];
	for (unsigned int i = 0; i < size; i++)
	{
		char c = (*m_buffer)[m_readIndex++];
		s[i] = c;
	}
	return *this;
}

BufferStream& BufferStream::readData(char*& s, unsigned int& size)
{
	*this >> size;
	s = new char[size];
	for (unsigned int i = 0; i < size; i++)
	{
		unsigned char c = (*m_buffer)[m_readIndex++];
		s[i] = c;
	}
	return *this;
}

BufferStream& BufferStream::operator <<(const char& c)
{
	m_buffer->resize(m_buffer->size + sizeof(c));
	(*m_buffer)[m_writeIndex++] = static_cast<char>(c);
	return *this;
}

BufferStream& BufferStream::operator <<(const bool& b)
{
	m_buffer->resize(m_buffer->size + sizeof(b));
	(*m_buffer)[m_writeIndex++] = static_cast<char>(b);
	return *this;
}

BufferStream& BufferStream::operator <<(const short& n)
{
	m_buffer->resize(m_buffer->size + sizeof(n));
	for (unsigned int i = 0; i < sizeof(n); i++)
	{
		char c = static_cast<char>(n >> (i * 8));
		(*m_buffer)[m_writeIndex++] = c;
	}
	return *this;
}

BufferStream& BufferStream::operator <<(const int& n)
{
	m_buffer->resize(m_buffer->size + sizeof(n));
	for (unsigned int i = 0; i < sizeof(n); i++)
	{
		char c = static_cast<char>(n >> (i * 8));
		(*m_buffer)[m_writeIndex++] = c;
	}
	return *this;
}

BufferStream& BufferStream::operator <<(const long& n)
{
	m_buffer->resize(m_buffer->size + sizeof(n));
	for (unsigned int i = 0; i < sizeof(n); i++)
	{
		char c = static_cast<char>(n >> (i * 8));
		(*m_buffer)[m_writeIndex++] = c;
	}
	return *this;
}

BufferStream& BufferStream::operator <<(const long long& n)
{
	m_buffer->resize(m_buffer->size + sizeof(n));
	for (unsigned int i = 0; i < sizeof(n); i++)
	{
		char c = static_cast<char>(n >> (i * 8));
		(*m_buffer)[m_writeIndex++] = c;
	}
	return *this;
}

#ifdef USE_STL
BufferStream& BufferStream::operator<<(const std::string & s)
{
	unsigned int size = static_cast<unsigned int>(s.size());
	*this << size;
	for (unsigned int i = 0; i < size; i++)
	{
		*this << s[i];
	}

	return *this;
}
#endif

BufferStream& BufferStream::operator >> (char& c)
{
	c = (*m_buffer)[m_readIndex++];
	return *this;
}

BufferStream& BufferStream::operator >> (bool& b)
{
	b = static_cast<bool>((*m_buffer)[m_readIndex++]);
	return *this;
}

BufferStream& BufferStream::operator >> (short& n)
{
	char* c = new char[sizeof(n)];
	for (unsigned int i = 0; i < sizeof(n); i++)
	{
		c[i] = (*m_buffer)[m_readIndex++];
	}
	n = *reinterpret_cast<short*>(c);
	delete[] c;
	return *this;
}

BufferStream& BufferStream::operator >> (int& n)
{
	char* c = new char[sizeof(n)];
	for (unsigned int i = 0; i < sizeof(n); i++)
	{
		c[i] = (*m_buffer)[m_readIndex++];
	}
	n = *reinterpret_cast<int*>(c);
	delete[] c;
	return *this;
}

BufferStream& BufferStream::operator >> (long& n)
{
	char* c = new char[sizeof(n)];
	for (unsigned int i = 0; i < sizeof(n); i++)
	{
		c[i] = (*m_buffer)[m_readIndex++];
	}
	n = *reinterpret_cast<long*>(c);
	delete[] c;
	return *this;
}

BufferStream& BufferStream::operator >> (long long& n)
{
	char* c = new char[sizeof(n)];
	for (unsigned int i = 0; i < sizeof(n); i++)
	{
		c[i] = (*m_buffer)[m_readIndex++];
	}
	n = *reinterpret_cast<long long*>(c);
	delete[] c;
	return *this;
}

BufferStream& BufferStream::operator>>(std::string & s)
{
	unsigned int size;
	*this >> size;
	s.resize(size);
	for (unsigned int i = 0; i < size; i++)
	{
		char c;
		*this >> c;
		s[i] = c;
	}
	return *this;
}
