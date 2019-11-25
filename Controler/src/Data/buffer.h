#ifndef BUFFER_H
#define BUFFER_H

#define USE_STL

#ifdef USE_STL
#include <string>
#include <vector>
#endif

class BufferStream;

class Buffer
{
public:
	unsigned int size {0};
	char* array {nullptr};

	Buffer();
	Buffer(const Buffer &buffer);
	Buffer(char* s);
	Buffer(const char* s);
	Buffer(const std::string& str);
	Buffer(const char* s, const unsigned int& size);
	~Buffer() { if(array != nullptr) delete[] array;}

	void append(const char& c);
	void append(const char* c, const unsigned int& size);
	void resize(const unsigned int& size);

	unsigned int fullSize() { return size + sizeof(int); }

	char* toBytes();
	static Buffer fromBytes(const char *s);

	std::vector<std::string> split(const char sep);

	static Buffer fromUtf8(const std::string& str);
	static Buffer fromUtf8(const char* s);
	static Buffer fromUtf8(const char* s, const int size);
	static Buffer fromUtf8(const char* s, const unsigned int size);

	friend BufferStream& operator <<(BufferStream& stream, const Buffer& buffer);
	friend BufferStream& operator >>(BufferStream& stream, Buffer& buffer);

	char& operator[](const unsigned int& index);

	Buffer& operator=(const Buffer &buffer);
};

class BufferStream
{
public:
	enum TypeStream
	{
		ReadOnly,
		WriteOnly,
		ReadWrite
	};

	BufferStream(Buffer* buffer, const TypeStream& typeStream);

	BufferStream& writeString(const char* s);
	BufferStream& readString(char*& s);

	BufferStream& writeData(const unsigned char* s, const unsigned int& size);
	BufferStream& writeData(const char* s, const unsigned int& size);
	BufferStream& readData(unsigned char*& s, unsigned int&size);
	BufferStream& readData(char*& s, unsigned int& size);

	BufferStream& operator <<(const char& c);
	BufferStream& operator <<(const unsigned char& c) { return *this << char(c); }
	BufferStream& operator <<(const signed char& c) { return *this << char(c); }
	BufferStream& operator <<(const bool& b);
	BufferStream& operator <<(const short& n);
	BufferStream& operator <<(const int& n);
	BufferStream& operator <<(const long& n);
	BufferStream& operator <<(const long long& n);
	BufferStream& operator <<(const unsigned short& n) { return *this << short(n); }
	BufferStream& operator <<(const unsigned int& n) { return *this << int(n); }
	BufferStream& operator <<(const unsigned long& n) { return *this << long(n); }
	BufferStream& operator <<(const unsigned long long& n) { return *this << static_cast<long long>(n); }
#ifdef USE_STL
	BufferStream& operator <<(const std::string& s);
#endif
	BufferStream& operator >> (char& c);
	BufferStream& operator >> (unsigned char& c) { return *this >> reinterpret_cast<char&>(c); }
	BufferStream& operator >> (signed char& c) { return *this >> reinterpret_cast<char&>(c); }
	BufferStream& operator >> (bool& b);
	BufferStream& operator >> (short& n);
	BufferStream& operator >> (int& n);
	BufferStream& operator >> (long& n);
	BufferStream& operator >> (long long& n);
	BufferStream& operator >> (unsigned short& n) { return *this >> reinterpret_cast<short&>(n); }
	BufferStream& operator >> (unsigned int& n) { return *this >> reinterpret_cast<int&>(n); }
	BufferStream& operator >> (unsigned long& n) { return *this >> reinterpret_cast<long&>(n); }
	BufferStream& operator >> (unsigned long long& n) { return *this >> reinterpret_cast<long long&>(n); }
#ifdef USE_STL
	BufferStream& operator >>(std::string& s);
#endif

private:
	unsigned int m_readIndex;
	unsigned int m_writeIndex;

	Buffer* m_buffer;

	TypeStream m_typeStream;
};

#endif // BUFFER_H
