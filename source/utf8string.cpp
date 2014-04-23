#include "utf8string.hpp"

namespace utf8rewind {

	Utf8String::Utf8String()
		: _length(0)
	{
		_buffer.push_back(0);
	}

	Utf8String::Utf8String(const Utf8String& other)
		: _length(other.length())
	{
		if (other.size() > 0)
		{
			_buffer.resize(other.size());
			memcpy(&_buffer[0], other.c_str(), other.size() - 1);
		}
		else
		{
			_buffer.push_back(0);
		}
	}

	Utf8String::Utf8String(const char* text)
		: _length(0)
	{
		int length = utf8len(text);
		if (length > 0)
		{
			_length = (size_t)length;

			_buffer.resize(strlen(text) + 1);
			memcpy(&_buffer[0], text, _buffer.size() - 1);
			_buffer.back() = 0;
		}
		else
		{
			_buffer.push_back(0);
		}
	}

	Utf8String::Utf8String(const wchar_t* text)
		: _length(0)
	{
		size_t length = wcslen(text);
		if (length > 0)
		{
			int size = wctoutf8(text, length + 2, nullptr, 0);
			if (size > 0)
			{
				_buffer.resize(size + 1);
				wctoutf8(text, length + 2, &_buffer[0], size + 1);
				_buffer.back() = 0;

				_length = length;
			}
		}

		if (_length == 0)
		{
			_buffer.push_back(0);
		}
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