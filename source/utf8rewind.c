#include "utf8rewind.h"

#define MAX_BASIC_MULTILINGUAR_PLANE  0xFFFF
#define MAX_LEGAL_UTF32               0x10FFFF
#define REPLACEMENT_CHARACTER         0xFFFD
#define SURROGATE_HIGH_START          0xD800
#define SURROGATE_HIGH_END            0xDBFF
#define SURROGATE_LOW_START           0xDC00
#define SURROGATE_LOW_END             0xDFFF

int utf8charvalid(char encodedCharacter)
{
	return ((((unsigned char)encodedCharacter & 0xFE) != 0xC0) && ((unsigned char)encodedCharacter < 0xF5));
}

int utf8charlen(char encodedCharacter)
{
	if (!utf8charvalid(encodedCharacter))
	{
		return UTF8_ERR_INVALID_CHARACTER;
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
		return UTF8_ERR_INVALID_CHARACTER;
	}
}

int utf8len(const char* text)
{
	int length = 0;
	unsigned char codepoint = 0;
	int codepoint_length = 0;
	int text_length = 0;

	if (text == 0)
	{
		return 0;
	}

	text_length = (int)strlen(text);
	if (text_length == 0)
	{
		return 0;
	}

	while (*text != 0 && text_length > 0)
	{
		if (!utf8charvalid(*text))
		{
			return UTF8_ERR_INVALID_CHARACTER;
		}

		codepoint = (unsigned char)*text;

		if (codepoint <= 0x7F)
		{
			codepoint_length = 1;
		}
		else if ((codepoint & 0xE0) == 0xC0)
		{
			if (text_length < 2)
			{
				return UTF8_ERR_INVALID_CHARACTER;
			}

			codepoint_length = 2;
		}
		else if ((codepoint & 0xF0) == 0xE0)
		{
			if (text_length < 3)
			{
				return UTF8_ERR_INVALID_CHARACTER;
			}

			codepoint_length = 3;
		}
		else if ((codepoint & 0xF8) == 0xF0)
		{
			if (text_length < 4)
			{
				return UTF8_ERR_INVALID_CHARACTER;
			}

			codepoint_length = 4;
		}
		else
		{
			return UTF8_ERR_INVALID_CHARACTER;
		}

		length++;
		text += codepoint_length;
		text_length -= codepoint_length;
	}

	return length;
}

int utf8encode(unicode_t codePoint, char* target, size_t targetSize)
{
	size_t length = 1;
	unicode_t mask = 0;
	size_t i;

	if (target == 0)
	{
		return UTF8_ERR_NOT_ENOUGH_SPACE;
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
		return UTF8_ERR_NOT_ENOUGH_SPACE;
	}

	for (i = length - 1; i >= 1; --i)
	{
		target[i] = (char)((codePoint & 0x3F) | 0x80);
		codePoint >>= 6;
	}

	target[0] = (char)(codePoint | mask);

	return length;
}

int utf8convertucs2(ucs2_t codePoint, char* target, size_t targetSize)
{
	if (target == 0 || targetSize < 1)
	{
		return UTF8_ERR_NOT_ENOUGH_SPACE;
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
			return UTF8_ERR_NOT_ENOUGH_SPACE;
		}

		target[1] = (char)((codePoint       & 0x3F) | 0x80);
		target[0] = (char)((codePoint >> 6)         | 0xC0);

		return 2;
	}
	else if (codePoint <= 0xFFFF)
	{
		if (codePoint >= SURROGATE_HIGH_START && codePoint <= SURROGATE_LOW_END)
		{
			/*
				The range between U+D800 and U+DFFF is reserved
				for lead and trail surrogate pairs.
			*/

			return UTF8_ERR_UNHANDLED_SURROGATE_PAIR;
		}
		
		if (targetSize < 3)
		{
			return UTF8_ERR_NOT_ENOUGH_SPACE;
		}

		target[2] = (char)(( codePoint        & 0x3F) | 0x80);
		target[1] = (char)(((codePoint >>  6) & 0x3F) | 0x80);
		target[0] = (char)( (codePoint >> 12)         | 0xE0);

		return 3;
	}

	return 0;
}

int wctoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize)
{
	int result = 0;
	utf16_t surrogate_high;
	utf16_t surrogate_low;
	utf16_t current;
	unicode_t codepoint;
	const char* src = (const char*)input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	int bytes_written = 0;

	if (input == 0 || inputSize < 2)
	{
		return UTF8_ERR_INVALID_DATA;
	}

	while (src_size > 0)
	{
		current = *(utf16_t*)src;

		if (current < SURROGATE_HIGH_START || current > SURROGATE_LOW_END)
		{
			result = utf8convertucs2(*(const ucs2_t*)src, dst, dst_size);
			if (result <= 0)
			{
				return result;
			}

			src += 2;
			src_size -= 2;

			dst += result;
			dst_size -= result;

			bytes_written += result;
		}
		else
		{
			if (src_size < 4)
			{
				return UTF8_ERR_INVALID_DATA;
			}

			surrogate_high = *(utf16_t*)src;

			if (surrogate_high < SURROGATE_HIGH_START || surrogate_high > SURROGATE_HIGH_END)
			{
				return UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR;
			}

			surrogate_low = *(utf16_t*)(src + 2);

			if (surrogate_low < SURROGATE_LOW_START || surrogate_low > SURROGATE_LOW_END)
			{
				return UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR;
			}

			if (dst_size < 4)
			{
				return UTF8_ERR_NOT_ENOUGH_SPACE;
			}

			codepoint =
				0x10000 +
				(surrogate_low - SURROGATE_LOW_START) +
				((surrogate_high - SURROGATE_HIGH_START) << 10);

			if (codepoint >= 0x110000)
			{
				/* Unicode characters must be encoded in a maximum of four bytes. */

				return UTF8_ERR_INVALID_CHARACTER;
			}

			src += 4;
			src_size -= 4;

			dst[3] = (char)(( codepoint        & 0x3F) | 0x80);
			dst[2] = (char)(((codepoint >>  6) & 0x3F) | 0x80);
			dst[1] = (char)(((codepoint >> 12) & 0x3F) | 0x80);
			dst[0] = (char)( (codepoint >> 18)         | 0xF0);

			dst += 4;
			dst_size -= 4;

			bytes_written += 4;
		}
	}

	return bytes_written;
}

int utf8decode(const char* text, unicode_t* result)
{
	size_t text_length;
	const unsigned char* src;

	if (result == 0)
	{
		return UTF8_ERR_NOT_ENOUGH_SPACE;
	}

	if (text == 0)
	{
		return UTF8_ERR_INVALID_DATA;
	}

	text_length = strlen(text);
	if (text_length == 0)
	{
		return UTF8_ERR_INVALID_DATA;
	}

	if (!utf8charvalid(text[0]))
	{
		return UTF8_ERR_INVALID_CHARACTER;
	}

	src = (const unsigned char*)text;

	if (src[0] <= 0x7F)
	{
		*result = (unicode_t)src[0];

		return 1;
	}
	else if ((src[0] & 0xE0) == 0xC0)
	{
		if (text_length < 2)
		{
			*result = 0;

			return UTF8_ERR_INVALID_DATA;
		}

		*result = src[0] & 0x1F;
		*result = (*result << 6) | (src[1] & 0x3F);

		return 2;
	}
	else if ((src[0] & 0xF0) == 0xE0)
	{
		if (text_length < 3)
		{
			*result = 0;

			return UTF8_ERR_INVALID_DATA;
		}

		*result = src[0] & 0x0F;
		*result = (*result << 6) | (src[1] & 0x3F);
		*result = (*result << 6) | (src[2] & 0x3F);

		return 3;
	}
	else if ((src[0] & 0xF8) == 0xF0)
	{
		if (text_length < 4)
		{
			result = 0;

			return UTF8_ERR_INVALID_DATA;
		}

		*result = src[0] & 0x07;
		*result = (*result << 6) | (src[1] & 0x3F);
		*result = (*result << 6) | (src[2] & 0x3F);
		*result = (*result << 6) | (src[3] & 0x3F);

		return 4;
	}
	else
	{
		return UTF8_ERR_INVALID_CHARACTER;
	}
}

int utf8towc(const char* input, size_t inputSize, wchar_t* target, size_t targetSize)
{
	unicode_t codepoint;
	const char* src = input;
	int src_size = inputSize;
	wchar_t* dst = target;
	size_t dst_size = targetSize;
	int bytes_written = 0;

	if (target == 0)
	{
		return UTF8_ERR_NOT_ENOUGH_SPACE;
	}

	if (src == 0 || src_size == 0)
	{
		return UTF8_ERR_INVALID_DATA;
	}

	while (src_size > 0)
	{
		codepoint = *(utf16_t*)src;

		if (codepoint <= 0x7F)
		{
			src++;
			src_size--;
		}
		else if ((codepoint & 0xE0) == 0xC0)
		{
			if (src_size < 2)
			{
				return UTF8_ERR_INVALID_DATA;
			}

			codepoint = src[0] & 0x1F;
			codepoint = (codepoint << 6) | (src[1] & 0x3F);

			src += 2;
			src_size -= 2;
		}
		else if ((codepoint & 0xF0) == 0xE0)
		{
			if (src_size < 3)
			{
				return UTF8_ERR_INVALID_DATA;
			}

			codepoint = src[0] & 0x0F;
			codepoint = (codepoint << 6) | (src[1] & 0x3F);
			codepoint = (codepoint << 6) | (src[2] & 0x3F);

			src += 3;
			src_size -= 3;
		}
		else if ((codepoint & 0xF8) == 0xF0)
		{
			if (src_size < 4)
			{
				return UTF8_ERR_INVALID_DATA;
			}

			codepoint = src[0] & 0x07;
			codepoint = (codepoint << 6) | (src[1] & 0x3F);
			codepoint = (codepoint << 6) | (src[2] & 0x3F);
			codepoint = (codepoint << 6) | (src[3] & 0x3F);

			src += 4;
			src_size -= 4;
		}

		if (codepoint <= MAX_BASIC_MULTILINGUAR_PLANE)
		{
			if (dst_size < 2)
			{
				return UTF8_ERR_NOT_ENOUGH_SPACE;
			}

			if (codepoint >= SURROGATE_HIGH_START && codepoint <= SURROGATE_LOW_END)
			{
				*dst++ = REPLACEMENT_CHARACTER;

				src += 2;
				src_size -= 2;
			}
			else
			{
				*dst++ = (utf16_t)codepoint;
			}

			dst_size -= 2;

			bytes_written += 2;
		}
		else
		{
			/* Codepoint must be converted to a surrogate pair. */

			if (dst_size < 4)
			{
				return UTF8_ERR_NOT_ENOUGH_SPACE;
			}

			codepoint -= 0x10000;
			*dst++ = (codepoint >> 10) + SURROGATE_HIGH_START;
			*dst++ = (codepoint & 0x3FF) + SURROGATE_LOW_START;

			dst_size -= 4;

			bytes_written += 4;
		}
	}

	return bytes_written;
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
		int char_length = utf8charlen(*src);
		if (char_length <= 0)
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