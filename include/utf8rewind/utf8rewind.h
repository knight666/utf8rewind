/*! 
	@file utf8rewind.h
	@brief Functions for working with UTF-8 encoded text.
*/

#ifndef _UTF8REWIND_H_
#define _UTF8REWIND_H_

/// @cond IGNORE
#include <stdio.h>
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

typedef unsigned int unicode_t; /*!< Unicode codepoint. */
typedef unsigned short ucs2_t; /*!< UCS-2 encoded codepoint. */
typedef unsigned short utf16_t; /*!< UTF-16 encoded codepoint. */

//! Check if a character is valid according to UTF-8 encoding.
/*!
	@param encodedCharacter Character to check.

	@return 1 on success or 0 on failure.
*/
int utf8charvalid(char encodedCharacter);

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

//! Encode a Unicode codepoint to UTF-8.
/*!
	Unicode codepoints must be in the range 0 - U+10FFFF,
	however the range U+D800 to U+DFFF is reserved for
	surrogate pairs and cannot be encoded.

	@param codePoint Unicode codepoint.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.

	@return Amount of bytes written or an error code.
		- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
*/
int utf8encode(unicode_t codePoint, char* target, size_t targetSize);

//! Convert a UCS-2 codepoint to UTF-8.
/*!
	UCS-2 encoding is similar to UTF-16 encoding, except that it
	does not use surrogate pairs to encode values beyond U+FFFF.

	This encoding was standard on Microsoft Windows XP. Newer
	versions of Windows use UTF-16 instead.

	@note Surrogate pairs cannot be converted using this function.
	Use utf8convertutf16 instead.

	@param codePoint UCS-2 encoded codepoint.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.

	@return Amount of bytes written or an error code.
		- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
		- #UTF8_ERR_UNHANDLED_SURROGATE_PAIR Codepoint is part of a surrogate pair.
*/
int utf8convertucs2(ucs2_t codePoint, char* target, size_t targetSize);

//! Convert a UTF-16 encoded string to UTF-8.
/*!
	UTF-16 encoded text consists of two up to four bytes per
	encoded codepoint. A codepoint may consist of a high and low 
	surrogate pair, which allows the encoding of the full range
	of Unicode characters that would otherwise not fit in a
	single 16-bit integer.

	Example:

	@code{.c}
		const wchar_t* input = "textures/\xD803\xDC11.png";
		size_t output_size = (wcslen(input) + 1) * sizeof(char);
		char* output = (wchar_t*)malloc(output_size);
		int result = 0;

		memset(output, 0, output_size);
		result = wctoutf8(input, wcslen(input), output, output_size);
		if (result > 0)
		{
			Texture_Load(output);
		}
	@endcode

	@param input UTF-16 encoded string.
	@param inputSize Size of the input in bytes.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.

	@return Amount of bytes written or an error code.
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for encoding.
	- #UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR High surrogate pair was not matched.
	- #UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR Low surrogate pair was not matched.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
	- #UTF8_ERR_INVALID_CHARACTER Codepoint could not be encoded.

	@sa utf8towc
*/
int wctoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize);

//! Decode a UTF-8 encoded codepoint to a Unicode codepoint.
/*!
	The result of this function can be used to offset the input
	string in order to decode all characters in a string.

	@param text Input string.
	@param result String to write the result to.

	@return Input offset in bytes or 0 on failure.
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

	Directions:
	- `SEEK_SET` Offset is from the start of the string.
	- `SEEK_CUR` Offset is from the current position of the string.
	- `SEEK_END` Offset is from the end of the string.

	@note `textStart` must come before `text` in memory when
	seeking from the current or end position.

	@param text Input string.
	@param textStart Start of input string.
	@param offset Requested offset in string.

	@param direction Offset string or no change on error.
*/
const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction);

#if defined(__cplusplus)
}
#endif

#endif