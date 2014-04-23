#pragma once

#include <vector>
#include <utf8rewind.h>

namespace utf8rewind {

	//! Class for managing UTF-8 encoded text.
	class Utf8String
	{

	public:

		class iterator
		{

			friend class Utf8String;

		public:

			unicode_t operator * () const;

		private:

			iterator(const char* start, char* value);

		private:

			const char* _start;
			char* _value;

		};

	public:

		Utf8String();
		Utf8String(const Utf8String& other);
		Utf8String(const char* text);
		Utf8String(const wchar_t* text);

		iterator begin();
		iterator end();

		//! Get the length in codepoints.
		size_t length() const;

		//! Get the size in bytes of the container.
		size_t size() const;

		//! Clear the string.
		void clear();

		//! Check if the string is empty.
		bool empty() const;

		//! Get a pointer to the string's data.
		const char* c_str() const;

	private:

		std::vector<char> _buffer;
		size_t _length;

	};

};