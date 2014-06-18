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

/*! 
	@file utf8rewind.h
	@brief Functions for working with UTF-8 encoded text.
*/

#ifndef _UTF8REWIND_H_
#define _UTF8REWIND_H_

/// @cond IGNORE
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>
/// @endcond

#define UTF8_ERR_INVALID_CHARACTER (-1)
#define UTF8_ERR_INVALID_DATA (-2)
#define UTF8_ERR_NOT_ENOUGH_SPACE (-3)
#define UTF8_ERR_OUT_OF_RANGE (-4)
#define UTF8_ERR_UNHANDLED_SURROGATE_PAIR (-5)
#define UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR (-6)
#define UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR (-7)

#if defined(__cplusplus)
extern "C" {
#endif

typedef uint32_t unicode_t; /*!< Unicode codepoint. */
typedef uint16_t ucs2_t; /*!< UCS-2 encoded codepoint. */
typedef uint16_t utf16_t; /*!< UTF-16 encoded codepoint. */

//! Check if a character is valid according to UTF-8 encoding.
/*!
	@param encodedCharacter Character to check.

	@return 1 on success or 0 on failure.
*/
int8_t utf8charvalid(char encodedCharacter);

//! Returns the length in bytes of the encoded character.
/*!
	A UTF-8 encoded codepoint must start with a special byte.
	This byte indicates how many bytes are used to encode the
	codepoint, up to a maximum of 6.

	This function can be used to determine the amount of bytes
	used to encode a codepoint.

	@param encodedCharacter Character to check.

	@return Amount of bytes written or an error code.
		- #UTF8_ERR_INVALID_CHARACTER Not a valid UTF-8 continuation byte.
*/
int utf8charlen(char encodedCharacter);

//! Get the length in codepoints of a UTF-8 encoded string.
/*!
	Example:

	@code{.c}
		int CheckPassword(const char* password)
		{
			size_t length = utf8len(password);
			return (length == utf8len("hunter2"));
		}
	@endcode

	@param text UTF-8 encoded string.

	@return Length in codepoints or an error code.
		- SIZE_MAX An invalid character was encountered.
*/
size_t utf8len(const char* text);

//! Encode a Unicode codepoint to UTF-8.
/*!
	Unicode codepoints must be in the range 0 - U+10FFFF,
	however the range U+D800 to U+DFFF is reserved for
	surrogate pairs and cannot be encoded.

	Example:

	@code{.c}
		char result[128];
		char* dst;
		int32_t errors = 0;

		memset(result, 0, 128);
		strcat(result, "STARG");
		dst = result + strlen(result);
		utf8encode(0x1402, dst, 128 - strlen(result), &errors);
		strcat(result, "TE");
	@endcode

	@param codepoint Unicode codepoint.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.
	@param errors Output for errors.

	@return Amount of bytes written or SIZE_MAX on error.

	Errors:
		- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.

	@sa wctoutf8
	@sa utf8convertucs2
*/
size_t utf8encode(unicode_t codepoint, char* target, size_t targetSize, int32_t* errors);

//! Convert a UCS-2 codepoint to UTF-8.
/*!
	UCS-2 encoding is similar to UTF-16 encoding, except that it
	does not use surrogate pairs to encode values beyond U+FFFF.

	This encoding was standard on Microsoft Windows XP. Newer
	versions of Windows use UTF-16 instead.

	If 0 is specified as the target buffer, this function
	returns the number of bytes needed to store the codepoint.

	@note Surrogate pairs cannot be converted using this function.
	Use wctoutf8() instead.

	Example:

	@code{.c}
		ucs2_t input[] = { 0x3041, 0x304B, 0x3060, 0x3074 };
		const size_t input_size = sizeof(input) / sizeof(ucs2_t);
		const size_t text_size = 128;
		char text[text_size] = { 0 };
		char* dst = text;
		size_t i;
		size_t offset;
		int32_t errors = 0;

		for (i = 0; i < input_size; ++i)
		{
			offset = utf8convertucs2(input[i], dst, text_size, &errors);
			if (offset == SIZE_MAX)
			{
				return errors;
			}

			dst += offset;
		}
	@endcode

	@param codepoint UCS-2 encoded codepoint.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.
	@param errors Output for errors.

	@return Amount of bytes written or SIZE_MAX on error.

	Errors:
		- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
		- #UTF8_ERR_UNHANDLED_SURROGATE_PAIR Codepoint is part of a surrogate pair.

	@sa wctoutf8
*/
size_t utf8convertucs2(ucs2_t codepoint, char* target, size_t targetSize, int32_t* errors);

//! Convert a UTF-16 encoded string to UTF-8.
/*!
	UTF-16 encoded text consists of two up to four bytes per
	encoded codepoint. A codepoint may consist of a high and low 
	surrogate pair, which allows the encoding of the full range
	of Unicode characters that would otherwise not fit in a
	single 16-bit integer.

	If 0 is specified as the target buffer, the function returns
	the number of bytes needed to store the string.

	Example:

	@code{.c}
		const wchar_t* input = L"textures/\xD803\xDC11.png";
		size_t output_size = 0;
		char* output = 0;
		size_t result = 0;
		int32_t errors = 0;

		result = wctoutf8(input, wcslen(input) * sizeof(wchar_t), 0, 0, &errors);
		if (result != (size_t)SIZE_MAX)
		{
			output_size = result + 1;

			output = (char*)malloc(output_size);
			memset(output, 0, output_size);

			result = wctoutf8(input, wcslen(input) * sizeof(wchar_t), output, output_size, &errors);
			if (result != (size_t)SIZE_MAX)
			{
				Texture_Load(output);
			}

			free(output);
		}
	@endcode

	@param input UTF-16 encoded string.
	@param inputSize Size of the input in bytes.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.
	@param errors Output for errors.

	@return Amount of bytes written or SIZE_MAX on error.
	
	Errors:
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for encoding.
	- #UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR High surrogate pair was not matched.
	- #UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR Low surrogate pair was not matched.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
	- #UTF8_ERR_INVALID_CHARACTER Codepoint could not be encoded.

	@sa utf8towc
*/
size_t wctoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

//! Decode a UTF-8 encoded codepoint to a Unicode codepoint.
/*!
	The result of this function can be used to offset the input
	string in order to decode all characters in a string.

	Example:

	@code{.c}
		const char* input = "Name: Bj\xC3\xB6rn Zonderland";
		const char* src = input;
		unicode_t codepoint;
		int offset;
		int i;

		FontBatch_Start();

		for (i = 0; i < utf8len(input); ++i)
		{
			offset = utf8decode(src, &codepoint);
			if (offset <= 0)
			{
				break;
			}

			Font_AddCharacter(codepoint);

			src += offset;
		}

		FontBatch_End();
		FontBatch_Draw(100, 100);
	@endcode

	@param text Input string.
	@param result String to write the result to.

	@return Input offset in bytes or an error code.
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for decoding.
	- #UTF8_ERR_INVALID_CHARACTER Input does not point to a valid UTF-8 encoded character.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Could not write result.

	@sa utf8encode
*/
int utf8decode(const char* text, unicode_t* result);

//! Convert a UTF-8 encoded string to UTF-16.
/*!
	Example:

	@code{.c}
		const char* input = "Bj\xC3\xB6rn Zonderland";
		size_t output_size = (strlen(input) + 1) * sizeof(wchar_t);
		wchar_t* output = (wchar_t*)malloc(output_size);
		int result = 0;

		memset(output, 0, output_size);
		result = utf8towc(input, strlen(input), output, output_size);
		if (result > 0)
		{
			Player_SetName(output);
		}
	@endcode

	@param input UTF-8 encoded string.
	@param inputSize Size of the input in bytes.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.

	@return Amount of bytes written or an error code.
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for decoding.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.

	@sa wctoutf8
	@sa utf8decode
*/
int utf8towc(const char* input, size_t inputSize, wchar_t* target, size_t targetSize);

//! Seek into a UTF-8 encoded string.
/*!
	Working with UTF-8 encoded strings can be tricky due to
	the nature of the variable-length encoding. Because one
	character no longer equals one byte, it can be difficult
	to skip around in a UTF-8 encoded string without
	decoding the codepoints.

	This function provides an interface similar to `fseek`
	in order to enable skipping to another part of the
	string.

	Example:

	@code{.c}
		const char* text = "Input: <LEFT ARROW>";
		const char* input = utf8seek(text, text, utf8len("Input: "), SEEK_SET);
	@endcode

	Directions:
	- `SEEK_SET` Offset is from the start of the string.
	- `SEEK_CUR` Offset is from the current position of the string.
	- `SEEK_END` Offset is from the end of the string.

	@note `textStart` must come before `text` in memory when
	seeking from the current or end position.

	@param text Input string.
	@param textStart Start of input string.
	@param offset Requested offset in codepoints.
	@param direction Direction to seek in.

	@return Changed string or no change on error.
*/
const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction);

#if defined(__cplusplus)
}
#endif

#endif
