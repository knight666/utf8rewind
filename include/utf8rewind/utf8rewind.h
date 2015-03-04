/*
	Copyright (C) 2014-2015 Quinten Lansu

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
	\file utf8rewind.h
	\brief Functions for working with UTF-8 encoded text.
*/

#ifndef _UTF8REWIND_H_
#define _UTF8REWIND_H_

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

/*!
	\defgroup errors Error codes
	\{
*/

/*!
	\def UTF8_ERR_INVALID_DATA
	\brief Input data is invalid.
*/
#define UTF8_ERR_INVALID_DATA                   (-1)

/*!
	\def UTF8_ERR_INVALID_FLAG
	\brief Input flag is invalid.
*/
#define UTF8_ERR_INVALID_FLAG                   (-2)

/*!
	\def UTF8_ERR_NOT_ENOUGH_SPACE
	\brief Buffer does not have enough space for result.
*/
#define UTF8_ERR_NOT_ENOUGH_SPACE               (-3)

/*!
	\def UTF8_ERR_OVERLAPPING_PARAMETERS
	\brief Parameters overlap in memory.
*/
#define UTF8_ERR_OVERLAPPING_PARAMETERS         (-4)

/*!
	\def UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR
	\brief Unmatched high codepoint in surrogate pair.
*/
#define UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR  (-5)

/*!
	\def UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR
	\brief Unmatched low codepoint in surrogate pair.
*/
#define UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR   (-6)

/*!
	\}
*/

#define UTF8_NORMALIZATION_RESULT_YES           (0)
#define UTF8_NORMALIZATION_RESULT_MAYBE         (1)
#define UTF8_NORMALIZATION_RESULT_NO            (2)

/*!
	\defgroup flags Flags
	\{
*/

/*!
	\def UTF8_NORMALIZE_COMPOSE
	\brief Normalize input to NFC.
*/
#define UTF8_NORMALIZE_COMPOSE                  0x00000001

/*!
	\def UTF8_NORMALIZE_DECOMPOSE
	\brief Normalize input to NFD.
*/
#define UTF8_NORMALIZE_DECOMPOSE                0x00000002

/*!
	\def UTF8_NORMALIZE_COMPATIBILITY
	\brief Changes composed normalization to NFKC or decomposed normalization to NFKD.
*/
#define UTF8_NORMALIZE_COMPATIBILITY            0x00000004

/*!
	\}
*/

/*!
	\defgroup configuration Global configuration
	\{
*/

/*!
	\def UTF8_WCHAR_SIZE
	\brief Specifies the size of the `wchar_t` type. On Windows this
	is 2, on POSIX systems it is 4. If not specified on the command
	line, the compiler tries to automatically determine the value.
*/

#ifndef UTF8_WCHAR_SIZE
	#if (__SIZEOF_WCHAR_T__ == 4) || (WCHAR_MAX > UINT16_MAX) || (__WCHAR_MAX__ > UINT16_MAX)
		#define UTF8_WCHAR_SIZE (4)
	#else
		#define UTF8_WCHAR_SIZE (2)
	#endif
#endif

#if (UTF8_WCHAR_SIZE == 4)
	/*!
		\def UTF8_WCHAR_UTF32
		\brief The `wchar_t` type is treated as UTF-32 (4 bytes).
	*/
	#define UTF8_WCHAR_UTF32 (1)
#elif (UTF8_WCHAR_SIZE == 2)
	/*!
		\def UTF8_WCHAR_UTF16
		\brief The `wchar_t` type is treated as UTF-16 (2 bytes).
	*/
	#define UTF8_WCHAR_UTF16 (1)
#else
	#error Invalid size for wchar_t type.
#endif

/*!
	\}
*/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
	\var utf16_t
	\brief UTF-16 encoded codepoint.
*/
typedef uint16_t utf16_t;

/*!
	\var unicode_t
	\brief Unicode codepoint.
*/
typedef uint32_t unicode_t;

/*!
	\brief Get the length in codepoints of a UTF-8 encoded string.

	Example:

	\code{.c}
		uint8_t CheckPassword(const char* password)
		{
			size_t length = utf8len(password);
			return (length == utf8len("hunter2"));
		}
	\endcode

	\param[in]  text  UTF-8 encoded string.

	\return Length in codepoints.
*/
size_t utf8len(const char* text);

/*!
	\brief Convert a UTF-16 encoded string to a UTF-8 encoded string.

	\note This function should only be called directly if you are positive
	that you're working with UTF-16 encoded text. If you're working
	with wide strings, take a look at widetoutf8() instead.

	Example:

	\code{.c}
		uint8_t Player_SetNameUtf16(const utf16_t* name, size_t nameSize)
		{
			char buffer[256];
			size_t buffer_size = 255;
			size_t converted_size;
			int32_t errors = 0;

			converted_size = utf16toutf8(name, nameSize, buffer, buffer_size, &errors);
			if (converted_size == 0 ||
				errors != 0)
			{
				return 0;
			}
			buffer[converted_size] = 0;

			return Player_SetName(converted_name);
		}
	\endcode

	\param[in]   input       UTF-16 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Bytes written or amount of bytes needed for output
	if target buffer is specified as NULL.

	\retval #UTF8_ERR_INVALID_DATA                   Input does not contain enough bytes for encoding.
	\retval #UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR  High surrogate pair was not matched.
	\retval #UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR   Low surrogate pair was not matched.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE               Target buffer could not contain result.

	\sa utf32toutf8
	\sa widetoutf8
*/
size_t utf16toutf8(const utf16_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a UTF-32 encoded string to a UTF-8 encoded string.

	\note This function should only be called directly if you are positive
	that you're working with UTF-32 encoded text. If you're working
	with wide strings, take a look at widetoutf8() instead.

	Example:

	\code{.c}
		uint8_t Database_ExecuteQuery_Unicode(const unicode_t* query, size_t querySize)
		{
			char* converted = NULL;
			size_t converted_size;
			int8_t result = 0;
			int32_t errors = 0;
			
			converted_size = utf32toutf8(query, querySize, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != 0)
			{
				goto cleanup;
			}

			converted = (char*)malloc(converted_size + 1);
			utf32toutf8(query, querySize, converted, converted_size, NULL);
			converted[converted_size] = 0;

			result = Database_ExecuteQuery(converted);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = 0;
			}

			return result;
		}
	\endcode

	\param[in]   input       UTF-32 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Bytes written or amount of bytes needed for output
	if target buffer is specified as NULL.

	\retval #UTF8_ERR_INVALID_DATA                   Input does not contain enough bytes for encoding.
	\retval #UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR  High surrogate pair was not matched.
	\retval #UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR   Low surrogate pair was not matched.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE               Target buffer could not contain result.

	\sa utf16toutf8
	\sa widetoutf8
*/
size_t utf32toutf8(const unicode_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a wide string to a UTF-8 encoded string.

	Depending on the platform, wide strings are either UTF-16
	or UTF-32 encoded. This function takes a wide string as
	input and automatically calls the correct conversion
	function.
	
	This allows for a cross-platform treatment of wide text and
	is preferable to using the UTF-16 or UTF-32 versions
	directly.

	Example:

	\code{.c}
		texture_t Texture_Load_Wide(const wchar_t* input)
		{
			char* converted = NULL;
			size_t converted_size;
			size_t input_size = wcslen(input) * sizeof(wchar_t);
			texture_t result = NULL;
			int32_t errors = 0;

			converted_size = widetoutf8(input, input_size, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != 0)
			{
				goto cleanup;
			}

			converted = (char*)malloc(converted_size + 1);
			widetoutf8(input, input_size, converted, converted_size, NULL);
			converted[converted_size / sizeof(wchar_t)] = 0;

			result = Texture_Load(converted);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}

			return result;
		}
	\endcode

	\param[in]   input       Wide-encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Bytes written or amount of bytes needed for output
	if target buffer is specified as NULL.

	\retval #UTF8_ERR_INVALID_DATA                   Input does not contain enough bytes for encoding.
	\retval #UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR  High surrogate pair was not matched.
	\retval #UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR   Low surrogate pair was not matched.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE               Target buffer could not contain result.

	\sa utf8towide
	\sa utf16toutf8
	\sa utf32toutf8
*/
size_t widetoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a UTF-8 encoded string to a UTF-16 encoded string.

	\note This function should only be called directly if you are positive
	that you *must* convert to UTF-16, independent of platform.
	If you're working with wide strings, take a look at utf8towide()
	instead.

	Erroneous byte sequences such as missing bytes, illegal bytes or
	overlong encodings of codepoints are converted to the
	replacement character U+FFFD.

	Example:

	\code{.c}
		void Font_DrawText(int x, int y, const char* text)
		{
			utf16_t buffer[256];
			size_t buffer_size = 255 * sizeof(utf16_t);
			int32_t errors = 0;
			
			size_t converted_size = utf8toutf16(text, strlen(text), buffer, buffer_size, &errors);
			if (converted_size > 0 &&
				errors == 0)
			{
				Legacy_DrawText(g_FontCurrent, x, y, (unsigned short*)buffer, converted_size / sizeof(utf16_t));
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Bytes written or amount of bytes needed for output
	if target buffer is specified as NULL.

	\retval #UTF8_ERR_INVALID_DATA      Input does not contain enough bytes for decoding.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE  Target buffer could not contain result.

	\sa utf8towide
	\sa utf8toutf32
*/
size_t utf8toutf16(const char* input, size_t inputSize, utf16_t* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a UTF-8 encoded string to a UTF-32 encoded string.

	\note This function should only be called directly if you are positive
	that you *must* convert to UTF-32, independent of platform.
	If you're working with wide strings, take a look at utf8towide()
	instead.

	Erroneous byte sequences such as missing bytes, illegal bytes or
	overlong encodings of codepoints are converted to the
	replacement character U+FFFD.

	Example:

	\code{.c}
		void TextField_AddCharacter(const char* encoded)
		{
			unicode_t codepoint = 0;
			int32_t errors = 0;

			utf8toutf32(encoded, strlen(encoded), &codepoint, sizeof(unicode_t), &errors);
			if (errors == 0)
			{
				TextField_AddCodepoint(codepoint);
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Bytes written or amount of bytes needed for output
	if target buffer is specified as NULL.

	\retval #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for decoding.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.

	\sa utf8towide
	\sa utf8toutf16
*/
size_t utf8toutf32(const char* input, size_t inputSize, unicode_t* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a UTF-8 encoded string to a wide string.

	Depending on the platform, wide strings are either UTF-16
	or UTF-32 encoded. This function takes a UTF-8 encoded
	string as input and automatically calls the correct
	conversion function.

	This allows for a cross-platform treatment of wide text and
	is preferable to using the UTF-16 or UTF-32 versions
	directly.

	Erroneous byte sequences such as missing bytes, illegal bytes or
	overlong encodings of codepoints are converted to the
	replacement character U+FFFD.

	\note Codepoints outside the Basic Multilingual Plane (BMP) are
	converted to surrogate pairs when using UTF-16. This means
	that strings containing characters outside the BMP
	converted on a platform with UTF-32 wide strings are *not*
	compatible with platforms with UTF-16 wide strings.

	\par Hence, it is preferable to keep all data as UTF-8 and only
	convert to wide strings when required by a third-party
	interface.

	Example:

	\code{.c}
		void Window_SetTitle(void* windowHandle, const char* text)
		{
			size_t input_size = strlen(text);
			wchar_t* converted = NULL;
			size_t converted_size;
			int32_t errors = 0;

			converted_size = utf8towide(text, input_size, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != 0)
			{
				goto cleanup;
			}

			converted = (wchar_t*)malloc(converted_size + sizeof(wchar_t));
			utf8towide(text, input_size, converted, converted_size, NULL);
			converted[converted_size / sizeof(wchar_t)] = 0;

			SetWindowTextW((HWND)windowHandle, converted);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Bytes written or amount of bytes needed for output
	if target buffer is specified as NULL.

	\retval #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for decoding.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.

	\sa widetoutf8
	\sa utf8toutf16
	\sa utf8toutf32
*/
size_t utf8towide(const char* input, size_t inputSize, wchar_t* target, size_t targetSize, int32_t* errors);

/*!
	\brief Seek into a UTF-8 encoded string.

	Working with UTF-8 encoded strings can be tricky due to
	the nature of the variable-length encoding. Because one
	character no longer equals one byte, it can be difficult
	to skip around in a UTF-8 encoded string without
	decoding the codepoints.

	This function provides an interface similar to `fseek`
	in order to enable skipping to another part of the
	string.

	\note `textStart` must come before `text` in memory when
	seeking from the current or end position.

	Example:

	\code{.c}
		const char* text = "Press \xE0\x80\x13 to continue.";
		const char fixed[1024];
		const char* commandStart;
		const char* commandEnd;

		memset(fixed, 0, 1024);

		commandStart = strstr(text, "\xE0\x80\x13");
		if (commandStart == 0)
		{
			return 0;
		}

		strncpy(fixed, text, commandStart - text);
		strcat(fixed, "ENTER");

		commandEnd = utf8seek(commandStart, text, 1, SEEK_CUR);
		if (commandEnd != commandStart)
		{
			strcat(fixed, commandEnd);
		}
	\endcode

	\param[in]  text       Input string.
	\param[in]  textStart  Start of input string.
	\param[in]  offset     Requested offset in codepoints.
	\param[in]  direction  Direction to seek in.
	\arg `SEEK_SET` Offset is from the start of the string.
	\arg `SEEK_CUR` Offset is from the current position of the string.
	\arg `SEEK_END` Offset is from the end of the string.

	\return Changed string or no change on error.
*/
const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction);

/*!
	\brief Convert UTF-8 encoded text to uppercase.

	This function allows users to convert UTF-8 encoded strings to
	uppercase without first changing the encoding to UTF-32.
	Conversion is fully compliant with the Unicode 7.0 standard.

	Although most codepoints can be converted in-place, there
	are notable exceptions. For example, U+00DF (LATIN SMALL LETTER
	SHARP S) maps to "U+0053 U+0053" (LATIN CAPITAL LETTER S) when
	uppercasing. Therefor, it is advised to first determine the size
	in bytes of the output by calling the function with a NULL output
	buffer.

	Only a handful of scripts make a distinction between upper- and
	lowercase. In addition to modern scripts, such as Latin, Greek,
	Armenian and Cyrillic, a few historic or archaic scripts have
	case. The vast majority of scripts do not have case distinctions.

	\note Case mapping is not reversible. That is, `toUpper(toLower(x))
	!= toLower(toUpper(x))`.

	Example:

	\code{.c}
		void Button_Draw(int32_t x, int32_t y, const char* text)
		{
			size_t input_size = strlen(text);
			char* converted = NULL;
			size_t converted_size;
			int32_t text_box_width, text_box_height;
			int32_t errors = 0;

			converted_size = utf8toupper(text, input_size, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != 0)
			{
				goto cleanup;
			}

			converted = (char*)malloc(converted_size + 1);
			utf8toupper(text, input_size, converted, converted_size, NULL);
			converted[converted_size] = 0;

			Font_GetTextDimensions(converted, &text_box_width, &text_box_height);

			Draw_BoxFilled(x - 4, y - 4, text_box_width + 8, text_box_height + 8, 0x088A08);
			Draw_BoxOutline(x - 4, y - 4, text_box_width + 8, text_box_height + 8, 0xA9F5A9);
			Font_DrawText(x + 2, y + 1, converted, 0x000000);
			Font_DrawText(x, y, converted, 0xFFFFFF);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Bytes written or amount of bytes needed for output if
	target buffer is specified as NULL.

	\retval  #UTF8_ERR_INVALID_DATA            Input does not contain enough bytes for decoding.
	\retval  #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval  #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer could not contain result.
*/
size_t utf8toupper(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

size_t utf8tolower(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

size_t utf8totitle(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

uint8_t utf8isnormalized(const char* input, size_t inputSize, size_t flags, size_t* offset);

size_t utf8normalize(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors);

#if defined(__cplusplus)
}
#endif

#endif
