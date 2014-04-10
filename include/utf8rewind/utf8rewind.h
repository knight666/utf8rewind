#ifndef _UTF8REWIND_H_
#define _UTF8REWIND_H_

#include <stdio.h>
#include <string.h>
#include <wchar.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef unsigned int unicode_t;
typedef unsigned short utf16_t;

int utf8charvalid(char encodedCharacter);
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