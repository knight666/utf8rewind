/*
	Copyright (C) 2014 Quinten Lansu

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#include "utf8rewind.h"

#include "internal/casemapping.h"
#include "internal/codepoint.h"
#include "internal/composition.h"
#include "internal/database.h"
#include "internal/normalization.h"
#include "internal/seeking.h"

#if defined(__GNUC__) && !defined(COMPILER_ICC)
	#define UTF8_UNUSED(_parameter) _parameter __attribute__ ((unused))
#else
	#define UTF8_UNUSED(_parameter) _parameter
#endif

size_t utf8len(const char* text)
{
	const char* src;
	size_t src_index;
	size_t length = 0;
	size_t codepoint_length = 0;
	size_t text_length = 0;
	uint8_t codepoint = 0;

	if (text == 0)
	{
		return length;
	}

	text_length = strlen(text);
	if (text_length == 0)
	{
		return length;
	}

	while (*text != 0)
	{
		codepoint = (uint8_t)*text;

		codepoint_length = codepoint_decoded_length[codepoint];
		if (codepoint_length != 0)
		{
			/* Check if encoding is valid */

			src = text + 1;

			for (src_index = 1; src_index < codepoint_length; ++src_index)
			{
				if ((*src & 0x80) == 0)
				{
					/* Not a continuation byte for a multi-byte sequence */

					break;
				}

				src++;
			}
		}
		else
		{
			src_index = 1;
		}

		length++;

		if (src_index > text_length)
		{
			break;
		}

		text += src_index;
		text_length -= src_index;
	}

	return length;
}

size_t utf16toutf8(const utf16_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	size_t encoded_length = 0;
	utf16_t surrogate_low;
	utf16_t current;
	unicode_t codepoint;
	const utf16_t* src = (const utf16_t*)input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 || inputSize < sizeof(utf16_t))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return bytes_written;
	}

	while (src_size > 0)
	{
		current = *src;

		if (current == 0)
		{
			break;
		}
		else if (current >= SURROGATE_HIGH_START && current <= SURROGATE_LOW_END)
		{
			if (current > SURROGATE_HIGH_END)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR;
				}
				return bytes_written;
			}

			if (src_size < sizeof(utf16_t))
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_INVALID_DATA;
				}
				return bytes_written;
			}

			src++;
			src_size -= sizeof(utf16_t);

			surrogate_low = *src;

			if (surrogate_low < SURROGATE_LOW_START || surrogate_low > SURROGATE_LOW_END)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR;
				}
				return bytes_written;
			}

			codepoint =
				0x10000 +
				(surrogate_low - SURROGATE_LOW_START) +
				((current - SURROGATE_HIGH_START) << 10);
		}
		else
		{
			codepoint = (unicode_t)current;
		}

		encoded_length = codepoint_write(codepoint, &dst, &dst_size, errors);
		if (encoded_length == 0)
		{
			return bytes_written;
		}

		src++;
		src_size -= sizeof(utf16_t);

		bytes_written += encoded_length;
	}

	return bytes_written;
}

size_t utf32toutf8(const unicode_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
	unicode_t codepoint;
	size_t encoded_length;
	utf16_t surrogate_low;
	const unicode_t* src = (const unicode_t*)input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	size_t bytes_written = 0;

	if (input == 0 || inputSize < sizeof(unicode_t))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return bytes_written;
	}

	while (src_size > 0)
	{
		codepoint = *src;

		if (codepoint == 0)
		{
			break;
		}
		else if (codepoint >= SURROGATE_HIGH_START && codepoint <= SURROGATE_LOW_END)
		{
			if (codepoint > SURROGATE_HIGH_END)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR;
				}
				return bytes_written;
			}

			if (src_size < sizeof(unicode_t))
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_INVALID_DATA;
				}
				return bytes_written;
			}

			src++;
			src_size -= sizeof(unicode_t);

			surrogate_low = *src;

			if (surrogate_low < SURROGATE_LOW_START || surrogate_low > SURROGATE_LOW_END)
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR;
				}
				return bytes_written;
			}

			codepoint =
				0x10000 +
				(surrogate_low - SURROGATE_LOW_START) +
				((codepoint - SURROGATE_HIGH_START) << 10);
		}

		encoded_length = codepoint_write(codepoint, &dst, &dst_size, errors);
		if (encoded_length == 0)
		{
			return bytes_written;
		}

		src++;
		src_size -= sizeof(unicode_t);

		bytes_written += encoded_length;
	}

	return bytes_written;
}

size_t widetoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors)
{
#if UTF8_WCHAR_UTF16
	return utf16toutf8((const utf16_t*)input, inputSize, target, targetSize, errors);
#elif UTF8_WCHAR_UTF32
	return utf32toutf8((const unicode_t*)input, inputSize, target, targetSize, errors);
#else
	return SIZE_MAX;
#endif
}

size_t utf8toutf16(const char* input, size_t inputSize, utf16_t* target, size_t targetSize, int32_t* errors)
{
	size_t bytes_written = 0;
	size_t decoded_length;
	unicode_t codepoint;
	const char* src = input;
	size_t src_length = inputSize;
	utf16_t* dst = target;
	size_t dst_size = targetSize;

	if (target != 0 && targetSize < sizeof(utf16_t))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
		}
		return bytes_written;
	}

	if (input == 0 || inputSize == 0)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return bytes_written;
	}

	while (src_length > 0)
	{
		decoded_length = codepoint_read(&codepoint, src, src_length);

		if (codepoint <= MAX_BASIC_MULTILINGUAR_PLANE)
		{
			if (dst != 0)
			{
				if (dst_size < sizeof(utf16_t))
				{
					if (errors != 0)
					{
						*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
					}
					return bytes_written;
				}

				if (codepoint >= SURROGATE_HIGH_START && codepoint <= SURROGATE_LOW_END)
				{
					*dst++ = REPLACEMENT_CHARACTER;
				}
				else
				{
					*dst++ = (utf16_t)codepoint;
				}

				dst_size -= sizeof(utf16_t);
			}

			bytes_written += sizeof(utf16_t);
		}
		else
		{
			if (dst != 0)
			{
				/* Codepoint must be converted to a surrogate pair. */

				if (dst_size < sizeof(unicode_t))
				{
					if (errors != 0)
					{
						*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
					}
					return bytes_written;
				}

				codepoint -= 0x10000;
				*dst++ = (codepoint >> 10) + SURROGATE_HIGH_START;
				*dst++ = (codepoint & 0x3FF) + SURROGATE_LOW_START;

				dst_size -= sizeof(unicode_t);
			}

			bytes_written += sizeof(unicode_t);
		}

		src += decoded_length;
		src_length -= decoded_length;
	}

	return bytes_written;
}

size_t utf8toutf32(const char* input, size_t inputSize, unicode_t* target, size_t targetSize, int32_t* errors)
{
	size_t bytes_written = 0;
	size_t decoded_length;
	unicode_t codepoint;
	const char* src = input;
	size_t src_length = inputSize;
	unicode_t* dst = target;
	size_t dst_size = targetSize;

	if (target != 0 && targetSize < sizeof(unicode_t))
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
		}
		return bytes_written;
	}

	if (input == 0 || inputSize == 0)
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_DATA;
		}
		return bytes_written;
	}

	while (src_length > 0)
	{
		decoded_length = codepoint_read(&codepoint, src, src_length);

		if (dst != 0)
		{
			if (dst_size < sizeof(unicode_t))
			{
				if (errors != 0)
				{
					*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
				}
				return bytes_written;
			}

			*dst++ = codepoint;
			dst_size -= sizeof(unicode_t);
		}

		bytes_written += sizeof(unicode_t);

		src += decoded_length;
		src_length -= decoded_length;
	}

	return bytes_written;
}

size_t utf8towide(const char* input, size_t inputSize, wchar_t* target, size_t targetSize, int32_t* errors)
{
#if UTF8_WCHAR_UTF16
	return utf8toutf16(input, inputSize, (utf16_t*)target, targetSize, errors);
#elif UTF8_WCHAR_UTF32
	return utf8toutf32(input, inputSize, (unicode_t*)target, targetSize, errors);
#else
	return SIZE_MAX;
#endif
}

const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction)
{
	size_t textLength = strlen(textStart);
	const char* textEnd = textStart + textLength;

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
				return seeking_forward(text, textEnd, textLength, offset);
			}
			else
			{
				return seeking_rewind(textStart, text, textLength, offset);
			}

		} break;

	case SEEK_SET:
		{
			if (text < textStart)
			{
				return text;
			}

			return seeking_forward(textStart, textEnd, textLength, offset);

		} break;

	case SEEK_END:
		return seeking_rewind(textStart, textEnd, textLength, -offset);

	default:
		return text;

	}
}

size_t utf8toupper(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	ComposeState state;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	if ((flags & UTF8_TRANSFORM_NORMALIZED) != 0)
	{
#if 0
		/* Normalize to NFC before attempting to uppercase */

		compose_initialize(&state, src, src_size, UnicodeProperty_Normalization_Compose);

		while (state.stage <= ComposeStage_OutOfInput)
		{
			uint8_t index = compose_execute(&state);

			if (index != (uint8_t)-1)
			{
				uint8_t generalCategory = database_queryproperty(state.codepoint[index], UnicodeProperty_GeneralCategory);

				size_t written = casemapping_execute(state.codepoint[index], &dst, &dst_size, generalCategory, UnicodeProperty_Uppercase, errors);
				if (written == 0)
				{
					break;
				}

				bytes_written += written;
			}
		}
#endif
	}
	else
	{
		/* Assume input is already NKC */

		while (src_size > 0)
		{
			if ((*src & 0x80) == 0)
			{
				/* Basic Latin */

				if (dst != 0)
				{
					if (dst_size < 1)
					{
						goto outofspace;
					}

					*dst = (*src >= 0x61 && *src <= 0x7A) ? *src - 0x20 : *src;

					dst++;
					dst_size--;
				}

				bytes_written++;

				src++;
				src_size--;
			}
			else
			{
				unicode_t codepoint;
				size_t codepoint_length = codepoint_read(&codepoint, src, src_size);
				uint8_t generalCategory = database_queryproperty(codepoint, UnicodeProperty_GeneralCategory);

				size_t written = casemapping_execute(codepoint, &dst, &dst_size, generalCategory, UnicodeProperty_Uppercase, errors);
				if (written == 0)
				{
					break;
				}

				bytes_written += written;

				src += codepoint_length;
				src_size -= codepoint_length;
			}
		}
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return bytes_written;
}

size_t utf8tolower(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	ComposeState state;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	if ((flags & UTF8_TRANSFORM_NORMALIZED) != 0)
	{
#if 0
		/* Normalize to NFC before attempting to lowercase */

		compose_initialize(&state, src, src_size, UnicodeProperty_Normalization_Compose);

		while (state.stage <= ComposeStage_OutOfInput)
		{
			uint8_t index = compose_execute(&state);

			if (index != (uint8_t)-1)
			{
				uint8_t generalCategory = database_queryproperty(state.codepoint[index], UnicodeProperty_GeneralCategory);

				size_t written = casemapping_execute(state.codepoint[index], &dst, &dst_size, generalCategory, UnicodeProperty_Lowercase, errors);
				if (written == 0)
				{
					break;
				}

				bytes_written += written;
			}
		}
#endif
	}
	else
	{
		/* Assume input is already NKC */

		while (src_size > 0)
		{
			if ((*src & 0x80) == 0)
			{
				/* Basic Latin */

				if (dst != 0)
				{
					if (dst_size < 1)
					{
						goto outofspace;
					}

					*dst = (*src >= 0x41 && *src <= 0x5A) ? *src + 0x20 : *src;

					dst++;
					dst_size--;
				}

				bytes_written++;

				src++;
				src_size--;
			}
			else
			{
				unicode_t codepoint;
				size_t codepoint_length = codepoint_read(&codepoint, src, src_size);
				uint8_t generalCategory = database_queryproperty(codepoint, UnicodeProperty_GeneralCategory);

				size_t written = casemapping_execute(codepoint, &dst, &dst_size, generalCategory, UnicodeProperty_Lowercase, errors);
				if (written == 0)
				{
					break;
				}

				bytes_written += written;

				src += codepoint_length;
				src_size -= codepoint_length;
			}
		}
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;

outofspace:
	if (errors != 0)
	{
		*errors = UTF8_ERR_NOT_ENOUGH_SPACE;
	}
	return bytes_written;
}

size_t utf8totitle(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;
	uint8_t property = UnicodeProperty_Titlecase;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		unicode_t codepoint;
		size_t codepoint_length = codepoint_read(&codepoint, src, src_size);
		uint8_t generalCategory = database_queryproperty(codepoint, UnicodeProperty_GeneralCategory);

		size_t written = casemapping_execute(codepoint, &dst, &dst_size, generalCategory, property, errors);
		if (written == 0)
		{
			break;
		}

		if (property == UnicodeProperty_Titlecase)
		{
			if ((generalCategory & GeneralCategory_Letter) != 0)
			{
				property = UnicodeProperty_Lowercase;
			}
		}
		else if ((generalCategory & GeneralCategory_Letter) == 0)
		{
			property = UnicodeProperty_Titlecase;
		}

		bytes_written += written;

		src += codepoint_length;
		src_size -= codepoint_length;
	}

	return bytes_written;

invaliddata:
	if (errors != 0)
	{
		*errors = UTF8_ERR_INVALID_DATA;
	}
	return bytes_written;
}

size_t utf8transform(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	if (
		(flags & UTF8_TRANSFORM_DECOMPOSED) != 0)
	{
		return normalize_decomposition(input, inputSize, target, targetSize, UnicodeProperty_Normalization_Decompose, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPATIBILITY_DECOMPOSED) != 0)
	{
		return normalize_decomposition(input, inputSize, target, targetSize, UnicodeProperty_Normalization_Compatibility_Decompose, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPOSED) != 0)
	{
		return normalize_composition(input, inputSize, target, targetSize, UnicodeProperty_Normalization_Compose, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPATIBILITY_COMPOSED) != 0)
	{
		return normalize_composition(input, inputSize, target, targetSize, UnicodeProperty_Normalization_Compatibility_Compose, errors);
	}
	else
	{
		if (errors != 0)
		{
			*errors = UTF8_ERR_INVALID_TRANSFORM;
		}
		return 0;
	}
}