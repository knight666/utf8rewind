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
#include "normalization.h"

#if defined(__GNUC__) && !defined(COMPILER_ICC)
	#define UTF8_UNUSED(_parameter) _parameter __attribute__ ((unused))
#else
	#define UTF8_UNUSED(_parameter) _parameter
#endif

char* safe_strcpy(char* target, size_t targetSize, const char* input, size_t inputSize)
{
#if WIN32 || _WINDOWS
	strncpy_s(target, targetSize, input, inputSize);
	return target;
#else
	size_t copy_size = (targetSize < inputSize) ? targetSize : inputSize;
	return strncpy(target, input, copy_size);
#endif
}

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

const char* seekforward(const char* src, const char* srcEnd, size_t srcLength, off_t offset)
{
	size_t i;

	if (srcEnd <= src || offset <= 0 || srcLength == 0)
	{
		return src;
	}
	if (offset >= (off_t)srcLength)
	{
		return srcEnd;
	}

	do
	{
		size_t codepoint_length = codepoint_decoded_length[(uint8_t)*src];
		if (codepoint_length == 0)
		{
			codepoint_length = 1;
		}

		src++;
		srcLength--;

		for (i = 1; i < codepoint_length && srcLength > 0; ++i)
		{
			if (
				(*src & 0x80) == 0    || /* Not a continuation byte */
				(*src & 0xC0) == 0xC0    /* Start of a new sequence */
			)
			{
				break;
			}

			src++;
			srcLength--;
		}
	}
	while (--offset > 0 && srcLength > 0);

	return src;
}

const char* seekrewind(const char* srcStart, const char* src, size_t srcLength, off_t offset)
{
	if (srcStart >= src || offset >= 0 || srcLength == 0)
	{
		return src;
	}
	if (-offset >= (off_t)srcLength)
	{
		return srcStart;
	}

	/* Ignore NUL codepoint */
	src--;

	while (src != srcStart)
	{
		if (
			(*src & 0x80) == 0 ||    /* ASCII */
			(*src & 0xC0) != 0x80 || /* Malformed continuation byte */
			(*src & 0xFE) == 0xFE    /* Illegal byte */
		)
		{
			++offset;
			if (offset == 0)
			{
				break;
			}
		}

		src--;
	}

	return src;
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
				return seekforward(text, textEnd, textLength, offset);
			}
			else
			{
				return seekrewind(textStart, text, textLength, offset);
			}

		} break;

	case SEEK_SET:
		{
			if (text < textStart)
			{
				return text;
			}

			return seekforward(textStart, textEnd, textLength, offset);

		} break;

	case SEEK_END:
		return seekrewind(textStart, textEnd, textLength, -offset);

	default:
		return text;

	}
}

size_t transform_decomposition(const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t propertyType, uint8_t transformType, int32_t* errors)
{
	size_t bytes_written = 0;
	const char* src = input;
	size_t src_size = inputSize;
	char* dst = target;
	size_t dst_size = targetSize;

	if (src == 0 ||
		src_size == 0)
	{
		goto invaliddata;
	}

	while (src_size > 0)
	{
		if ((*src & 0x80) == 0)
		{
			/* Basic Latin codepoints are already decomposed */

			if (dst != 0)
			{
				if (dst_size < 1)
				{
					goto outofspace;
				}

				*dst = *src;

				dst++;
				dst_size--;
			}

			bytes_written++;

			src++;
			src_size--;
		}
		else
		{
			size_t resolved_size = 0;
			unicode_t codepoint;
			size_t codepoint_length = codepoint_read(&codepoint, src, src_size);

			if (queryproperty(codepoint, propertyType) == QuickCheckResult_No)
			{
				if (codepoint >= HANGUL_S_FIRST &&
					codepoint <= HANGUL_S_LAST)
				{
					/*
						Hangul decomposition
			
						Algorithm adapted from Unicode Technical Report #15:
						http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
					*/

					unicode_t s_index = codepoint - HANGUL_S_FIRST;
					unicode_t l = HANGUL_L_FIRST + (s_index / HANGUL_N_COUNT);
					unicode_t v = HANGUL_V_FIRST + (s_index % HANGUL_N_COUNT) / HANGUL_T_COUNT;
					unicode_t t = HANGUL_T_FIRST + (s_index % HANGUL_T_COUNT);

					/* Hangul syllables are always three bytes */
					resolved_size = (t != HANGUL_T_FIRST) ? 9 : 6;

					if (dst != 0 &&
						dst_size < resolved_size)
					{
						goto outofspace;
					}

					codepoint_write(l, &dst, &dst_size, errors);
					codepoint_write(v, &dst, &dst_size, errors);
					if (t != HANGUL_T_FIRST)
					{
						codepoint_write(t, &dst, &dst_size, errors);
					}
				}
				else
				{
					int32_t find_result;
					const char* resolved = finddecomposition(codepoint, transformType, &find_result);

					if (find_result == FindResult_Found)
					{
						resolved_size = strlen(resolved);

						if (dst != 0 &&
							resolved_size > 0)
						{
							if (dst_size < resolved_size)
							{
								goto outofspace;
							}

							memcpy(dst, resolved, resolved_size);

							dst += resolved_size;
							dst_size -= resolved_size;
						}
					}
					else
					{
						resolved_size = codepoint_write(codepoint, &dst, &dst_size, errors);
					}
				}
			}
			else
			{
				resolved_size = codepoint_write(codepoint, &dst, &dst_size, errors);
			}

			if (resolved_size == 0)
			{
				break;
			}
			bytes_written += resolved_size;

			src += codepoint_length;
			src_size -= codepoint_length;
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

size_t transform_composition(const char* input, size_t inputSize, char* target, size_t targetSize, uint8_t transformType, int32_t* errors)
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

	compose_initialize(&state, &src, &src_size, transformType);

	while (state.stage <= ComposeStage_OutOfInput)
	{
		uint8_t index;
		size_t written;

		index = compose_execute(&state);

		if (index != (uint8_t)-1)
		{
			if (dst != 0 &&
				dst_size < state.length[index])
			{
				goto outofspace;
			}

			written = codepoint_write(state.codepoint[index], &dst, &dst_size, errors);
			if (written == 0)
			{
				break;
			}
			bytes_written += written;
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
		/* Normalize to NFC before attempting to uppercase */

		compose_initialize(&state, &src, &src_size, UnicodeProperty_Normalization_Compose);

		while (state.stage <= ComposeStage_OutOfInput)
		{
			uint8_t index = compose_execute(&state);

			if (index != (uint8_t)-1)
			{
				size_t written = casemapping_execute(state.codepoint[index], &dst, &dst_size, UnicodeProperty_Uppercase, errors);

				if (written == 0)
				{
					break;
				}

				bytes_written += written;
			}
		}
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

				size_t written = casemapping_execute(codepoint, &dst, &dst_size, UnicodeProperty_Uppercase, errors);

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
		/* Normalize to NFC before attempting to lowercase */

		compose_initialize(&state, &src, &src_size, UnicodeProperty_Normalization_Compose);

		while (state.stage <= ComposeStage_OutOfInput)
		{
			uint8_t index = compose_execute(&state);

			if (index != (uint8_t)-1)
			{
				size_t written = casemapping_execute(state.codepoint[index], &dst, &dst_size, UnicodeProperty_Lowercase, errors);

				if (written == 0)
				{
					break;
				}

				bytes_written += written;
			}
		}
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

				size_t written = casemapping_execute(codepoint, &dst, &dst_size, UnicodeProperty_Lowercase, errors);

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

size_t utf8transform(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors)
{
	if (
		(flags & UTF8_TRANSFORM_DECOMPOSED) != 0)
	{
		return transform_decomposition(input, inputSize, target, targetSize, UnicodeProperty_Normalization_Decompose, UnicodeProperty_Normalization_Decompose, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPATIBILITY_DECOMPOSED) != 0)
	{
		return transform_decomposition(input, inputSize, target, targetSize, UnicodeProperty_Normalization_Compatibility_Decompose, UnicodeProperty_Normalization_Compatibility_Decompose, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPOSED) != 0)
	{
		return transform_composition(input, inputSize, target, targetSize, UnicodeProperty_Normalization_Compose, errors);
	}
	else if (
		(flags & UTF8_TRANSFORM_COMPATIBILITY_COMPOSED) != 0)
	{
		return transform_composition(input, inputSize, target, targetSize, UnicodeProperty_Normalization_Compatibility_Compose, errors);
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