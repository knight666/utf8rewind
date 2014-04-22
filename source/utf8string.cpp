#include "utf8string.hpp"

namespace utf8rewind {

	Utf8String::Utf8String()
		: _length(0)
	{
		_buffer.push_back(0);
	}

	Utf8String::Utf8String(const char* text)
		: _length(strlen(text))
	{
		_buffer.resize(_length + 1);
		memcpy(&_buffer[0], text, _length);
		_buffer.back() = 0;
	}

	size_t Utf8String::size() const
	{
		return _buffer.size();
	}

	size_t Utf8String::length() const
	{
		return _length;
	}

	void Utf8String::clear()
	{
		_buffer.clear();
		_buffer.push_back(0);
		_length = 0;
	}

	bool Utf8String::empty() const
	{
		return (_length == 0);
	}

	const char* Utf8String::c_str() const
	{
		return (const char*)_buffer.data();
	}

};