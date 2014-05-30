#pragma once

#include <vector>
#include <utf8rewind.h>

namespace utf8rewind {

	//! Class for managing UTF-8 encoded text.
	class Utf8String
	{

	public:

		//! Read-only class for stepping through a Utf8String instance.
		class iterator
		{

			friend class Utf8String;

		public:

			//! Get the codepoint the iterator is currently pointing at.
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

		Utf8String& operator = (const Utf8String& other);
		Utf8String& operator = (const char* text);
		Utf8String& operator = (const wchar_t* text);

		//! Get an iterator for the start of the string.
		/*!
			The begin iterator will always point to the
			first byte of the string.

			If the string is not valid or empty, the
			begin and end iterators will point to the
			same byte.
		*/
		iterator begin();

		//! Get an iterator for the end of the string.
		/*!
			The end iterator will always point to the last
			byte of the string and is guaranteed to be 0.

			If the string is not valid or empty, the
			begin and end iterators will point to the
			same byte.
		*/
		iterator end();

		//! Get the length in codepoints.
		/*!
			The length in codepoints of a UTF-8 string is
			not equal to the size in bytes minues one
			because UTF-8 encoded text uses a variable
			amount of bytes for each codepoint.s
		*/
		size_t length() const;

		//! Get the size in bytes of the container.
		size_t size() const;

		//! Clear the string.
		/*!
			When the string is cleared, the internal
			buffer is emptied, but the last byte is still
			an end-of-data character (0).
		*/
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