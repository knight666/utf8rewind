#include "utf8rewind.h"

#define SURROGATE_START_LOW 0xD800
#define SURROGATE_END_HIGH 0xDFFF

int utf8charvalid(char encodedCharacter)
{
	return ((((unsigned char)encodedCharacter & 0xFE) != 0xC0) && ((unsigned char)encodedCharacter < 0xF5));
}

size_t utf8charlen(char encodedCharacter)
{
	if (!utf8charvalid(encodedCharacter))
	{
		return 0;
	}
	else if ((unsigned char)encodedCharacter <= 0x7F)
	{
		return 1;
	}
	else if (((unsigned char)encodedCharacter & 0xE0) == 0xC0)
	{
		return 2;
	}
	else if (((unsigned char)encodedCharacter & 0xF0) == 0xE0)
	{
		return 3;
	}
	else if (((unsigned char)encodedCharacter & 0xF8) == 0xF0)
	{
		return 4;
	}
	else
	{
		return 0;
	}
}

size_t utf8encode(unicode_t codePoint, char* target, size_t targetSize)
{
	size_t length = 1;
	unicode_t mask = 0;
	size_t i;

	if (target == 0)
	{
		return 0;
	}

	if (codePoint <= 0x7F)
	{
		target[0] = (char)codePoint;

		return 1;
	}

	if (codePoint <= 0x7FF)
	{
		length = 2;
		mask = 0xC0;
	}
	else if (codePoint <= 0xFFFF)
	{
		length = 3;
		mask = 0xE0;
	}
	else
	{
		length = 4;
		mask = 0xF0;
	}

	if (length >= targetSize)
	{
		return 0;
	}

	for (i = length - 1; i >= 1; --i)
	{
		target[i] = (char)((codePoint & 0x3F) | 0x80);
		codePoint >>= 6;
	}

	target[0] = (char)(codePoint | mask);

	return length;
}

size_t utf8convertucs2(ucs2_t codePoint, char* target, size_t targetSize)
{
	if (target == 0 || targetSize < 1)
	{
		return 0;
	}

	if (codePoint <= 0x7F)
	{
		target[0] = (char)codePoint;

		return 1;
	}
	else if (codePoint <= 0x7FF)
	{
		if (targetSize < 2)
		{
			return 0;
		}

		target[1] = (char)((codePoint       & 0x3F) | 0x80);
		target[0] = (char)((codePoint >> 6)         | 0xC0);

		return 2;
	}
	else if (codePoint <= 0xFFFF)
	{
		if (codePoint >= SURROGATE_START_LOW && codePoint <= SURROGATE_END_HIGH)
		{
			/*
				The range between U+D800 and U+DFFF is reserved
				for lead and trail surrogate pairs.
			*/

			return 0;
		}
		
		if (targetSize < 3)
		{
			return 0;
		}

		target[2] = (char)(( codePoint        & 0x3F) | 0x80);
		target[1] = (char)(((codePoint >>  6) & 0x3F) | 0x80);
		target[0] = (char)( (codePoint >> 12)         | 0xE0);

		return 3;
	}

	return 0;
}

size_t utf8decode(const char* text, unicode_t* result)
{
	size_t text_length;
	const unsigned char* src;
	size_t length;

	if (text == 0 || result == 0)
	{
		return 0;
	}

	text_length = strlen(text);
	if (text_length == 0 || !utf8charvalid(text[0]))
	{
		return 0;
	}

	src = (const unsigned char*)text;
	length = 0;

	if (src[0] <= 0x7F)
	{
		*result = (unicode_t)src[0];

		length = 1;
	}
	else if ((src[0] & 0xE0) == 0xC0)
	{
		if (text_length < 2)
		{
			*result = 0;

			return 0;
		}

		*result = +src[0] & 0x1F;
		*result = (*result << 6) | (src[1] & 0x3F);

		length = 2;
	}
	else if ((src[0] & 0xF0) == 0xE0)
	{
		if (text_length < 3)
		{
			*result = 0;

			return 0;
		}

		*result = src[0] & 0x0F;
		*result = (*result << 6) | (src[1] & 0x3F);
		*result = (*result << 6) | (src[2] & 0x3F);

		length = 3;
	}
	else if ((src[0] & 0xF8) == 0xF0)
	{
		if (text_length < 4)
		{
			result = 0;

			return 0;
		}

		*result = src[0] & 0x07;
		*result = (*result << 6) | (src[1] & 0x3F);
		*result = (*result << 6) | (src[2] & 0x3F);
		*result = (*result << 6) | (src[3] & 0x3F);

		length = 4;
	}
	else
	{
		length = 0;
	}

	return length;
}

const char* seekforward(const char* src, off_t offset)
{
	size_t length;
	const char* end;
	off_t i;

	length = strlen(src);
	if (length == 0)
	{
		return src;
	}

	end = src + length;

	for (i = 0; i < offset; ++i)
	{
		size_t char_length = utf8charlen(*src);
		if (char_length == 0)
		{
			break;
		}

		if (src + char_length >= end)
		{
			return end;
		}

		src += char_length;
	}

	return src;
}

const char* seekrewind(const char* src, const char* srcStart, off_t offset)
{
	int isAscii;
	int lastCheck;
	int i;

	if (srcStart >= src)
	{
		return src;
	}

	while (src != srcStart)
	{
		isAscii = (*src & 0x80) != 0x80;
		lastCheck = 0;

		if (isAscii)
		{
			src--;

			if (offset + 1 == 0)
			{
				lastCheck = ((*src & 0x80) == 0x80);
			}
		}

		if (!isAscii || lastCheck)
		{
			for (i = 0; i < 4; ++i)
			{
				if (src < srcStart)
				{
					src++;
					break;
				}

				src--;
				if ((*src & 0xC0) != 0x80)
				{
					break;
				}
			}
		}

		if (++offset == 0)
		{
			break;
		}
	}

	return src;
}

const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction)
{
	switch (direction)
	{

	case SEEK_CUR:
		{
			if (offset == 0)
			{
				return text;
			}
			else if (offset > 0)
			{
				return seekforward(text, offset);
			}
			else
			{
				return seekrewind(text, textStart, offset);
			}

		} break;

	case SEEK_SET:
		{
			return seekforward(textStart, offset);

		} break;

	case SEEK_END:
		{

		} break;
	}

	return text;
}