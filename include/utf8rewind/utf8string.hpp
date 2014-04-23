#pragma once

#include <vector>

#include <utf8rewind.h>

namespace utf8rewind {

	class Utf8String
	{

	public:

		Utf8String();
		Utf8String(const char* text);
		Utf8String(const wchar_t* text);

		// STL interface

		size_t size() const;
		size_t length() const;

		void clear();
		bool empty() const;

		const char* c_str() const;

	private:

		std::vector<char> _buffer;
		size_t _length;

	};

};