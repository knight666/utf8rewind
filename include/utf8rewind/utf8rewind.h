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

#if defined(__cplusplus)
extern "C" {
#endif

typedef unsigned int unicode_t; /*!< Unicode codepoint. */
typedef unsigned short utf16_t; /*!< UTF-16 encoded codepoint. */

//! Check if a character is valid according to UTF-8 encoding.
/*!
	@param encodedCharacter The character to check.
	@return 1 on success, 0 on failure.
*/
int utf8charvalid(char encodedCharacter);

//! Gets the length in bytes of the encoded character.
/*!
	@param encodedCharacter The character to check.
	@return The length in bytes or 0 on failure.
*/
size_t utf8charlen(char encodedCharacter);

size_t utf8encode(unicode_t unicode, char* target, size_t targetSize);
size_t utf8encodeutf16(utf16_t character, char* target, size_t targetSize);
size_t utf8decode(const char* text, unicode_t* result);
const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction);
size_t wctoutf8(const utf16_t* text, char* target, size_t targetSize);
size_t utf8towc(const char* text, utf16_t* target, size_t targetSize);

#if defined(__cplusplus)
}
#endif

#endif