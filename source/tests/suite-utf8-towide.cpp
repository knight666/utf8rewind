#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf8ToWide, String)
{
	const char* c = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3 * UTF8_WCHAR_SIZE, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_STREQ(L"\x91C\x921\x924", b);
}

TEST(Utf8ToWide, Ascii)
{
	const char* c = "7";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1 * UTF8_WCHAR_SIZE, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_STREQ(L"7", b);
}

TEST(Utf8ToWide, AsciiNotEnoughData)
{
	const char* c = "%";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8towide(c, 0, b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ(L"", b);
}

TEST(Utf8ToWide, AsciiNotEnoughSpace)
{
	const char* c = "D";
	const size_t s = 0;
	wchar_t b[1] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ(L"", b);
}

TEST(Utf8ToWide, TwoBytes)
{
	const char* c = "\xCB\x8C";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1 * UTF8_WCHAR_SIZE, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_STREQ(L"\x02CC", b);
}

TEST(Utf8ToWide, TwoBytesNotEnoughData)
{
	const char* c = "\xDC";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1 * UTF8_WCHAR_SIZE, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_STREQ(L"\xFFFD", b);
}

TEST(Utf8ToWide, TwoBytesNotEnoughSpace)
{
	const char* c = "\xDC\x80";
	const size_t s = 1;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8towide(c, strlen(c), (wchar_t*)b, s, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(Utf8ToWide, ThreeBytes)
{
	const char* c = "\xEA\xAC\x81";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1 * UTF8_WCHAR_SIZE, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_STREQ(L"\xAB01", b);
}

TEST(Utf8ToWide, ThreeBytesNotEnoughData)
{
	const char* c = "\xEA\xAC";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(UTF8_WCHAR_SIZE, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_STREQ(L"\xFFFD", b);
}

TEST(Utf8ToWide, AboveBasicMultilingualPlane)
{
	const char* c = "\xF0\x90\xB0\x91";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

#if UTF8_WCHAR_UTF16
	EXPECT_EQ(4, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\xD803\xDC11", b);
#elif UTF8_WCHAR_UTF32
	EXPECT_EQ(4, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\x10C11", b);
#endif
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToWide, AboveBasicMultilingualPlaneFirst)
{
	const char* c = "\xF0\x90\x80\x80";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

#if UTF8_WCHAR_UTF16
	EXPECT_EQ(4, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\xD800\xDC00", b);
#elif UTF8_WCHAR_UTF32
	EXPECT_EQ(4, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\x10000", b);
#endif
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToWide, AboveBasicMultilingualPlaneLast)
{
	const char* c = "\xF4\x8F\xBF\xBF";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

#if UTF8_WCHAR_UTF16
	EXPECT_EQ(4, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\xDBFF\xDFFF", b);
#elif UTF8_WCHAR_UTF32
	EXPECT_EQ(4, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\x10FFFF", b);
#endif
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToWide, AboveBasicMultilingualPlaneNotEnoughData)
{
	const char* c = "\xF0\x90\x8C";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(UTF8_WCHAR_SIZE, utf8towide(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_STREQ(L"\xFFFD", b);
}

TEST(Utf8ToWide, AboveBasicMultilingualPlaneNotEnoughSpace)
{
	const char* c = "\xF0\x90\x82\xB0";
	const size_t s = 1;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8towide(c, strlen(c), (wchar_t*)b, s, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(Utf8ToWide, ErrorsIsReset)
{
	const char* i = "Yeah";
	size_t is = sizeof(i);
	wchar_t o[256] = { 0 };
	size_t os = 255 * UTF8_WCHAR_SIZE;
	int32_t errors = 1989;

	EXPECT_EQ(4 * UTF8_WCHAR_SIZE, utf8towide(i, is, o, os, &errors));
	EXPECT_STREQ(L"Yeah", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToWide, InvalidData)
{
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8towide(nullptr, 0, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8ToWide, OverlappingParametersFits)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };
	strcpy((char*)data, "Fire!");

	const char* i = (const char*)data;
	size_t is = 5;
	wchar_t* o = (wchar_t*)(data + is);
	size_t os = 5 * sizeof(wchar_t);

	EXPECT_EQ(5 * UTF8_WCHAR_SIZE, utf8towide(i, is, o, os, &errors));
#if UTF8_WCHAR_UTF32
	EXPECT_MEMEQ("Fire!F\0\0\0i\0\0\0r\0\0\0e\0\0\0!\0\0\0", (const char*)data, 25);
#else
	EXPECT_MEMEQ("Fire!F\0i\0r\0e\0!\0", (const char*)data, 15);
#endif
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToWide, OverlappingParametersStartsEqual)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };

	const char* i = (const char*)data;
	size_t is = 7 * sizeof(wchar_t);
	wchar_t* o = (wchar_t*)data;
	size_t os = 16 * sizeof(wchar_t);

	EXPECT_EQ(0, utf8towide(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToWide, OverlappingParametersEndsEqual)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };

	const char* i = (const char*)(data + (1 * sizeof(wchar_t)));
	size_t is = 9 * sizeof(wchar_t);
	wchar_t* o = (wchar_t*)(data + (6 * sizeof(wchar_t)));
	size_t os = 4 * sizeof(wchar_t);

	EXPECT_EQ(0, utf8towide(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToWide, OverlappingParametersInputStartsInTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };

	const char* i = (const char*)(data + (23 * sizeof(wchar_t)));
	size_t is = 15 * sizeof(wchar_t);
	wchar_t* o = (wchar_t*)(data + (15 * sizeof(wchar_t)));
	size_t os = 25 * sizeof(wchar_t);

	EXPECT_EQ(0, utf8towide(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToWide, OverlappingParametersInputEndsInTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };

	const char* i = (const char*)(data + (4 * sizeof(wchar_t)));
	size_t is = 20 * sizeof(wchar_t);
	wchar_t* o = (wchar_t*)(data + (16 * sizeof(wchar_t)));
	size_t os = 16 * sizeof(wchar_t);

	EXPECT_EQ(0, utf8towide(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToWide, OverlappingParametersInputInsideTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };

	const char* i = (const char*)(data + (19 * sizeof(wchar_t)));
	size_t is = 5 * sizeof(wchar_t);
	wchar_t* o = (wchar_t*)(data + (14 * sizeof(wchar_t)));
	size_t os = 14 * sizeof(wchar_t);

	EXPECT_EQ(0, utf8towide(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToWide, OverlappingParametersTargetStartsInInput)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };

	const char* i = (const char*)(data + (2 * sizeof(wchar_t)));
	size_t is = 23 * sizeof(wchar_t);
	wchar_t* o = (wchar_t*)(data + (19 * sizeof(wchar_t)));
	size_t os = 13 * sizeof(wchar_t);

	EXPECT_EQ(0, utf8towide(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToWide, OverlappingParametersTargetEndsInInput)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };

	const char* i = (const char*)(data + (19 * sizeof(wchar_t)));
	size_t is = 14 * sizeof(wchar_t);
	wchar_t* o = (wchar_t*)(data + (3 * sizeof(wchar_t)));
	size_t os = 21 * sizeof(wchar_t);

	EXPECT_EQ(0, utf8towide(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToWide, OverlappingParametersTargetInsideInput)
{
	int32_t errors = UTF8_ERR_NONE;

	uint8_t data[128] = { 0 };

	const char* i = (const char*)(data + (11 * sizeof(wchar_t)));
	size_t is = 22 * sizeof(wchar_t);
	wchar_t* o = (wchar_t*)(data + (19 * sizeof(wchar_t)));
	size_t os = 4 * sizeof(wchar_t);

	EXPECT_EQ(0, utf8towide(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}