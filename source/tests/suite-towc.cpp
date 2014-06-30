#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ToWc, String)
{
	const char* c = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3 * UTF8_WCHAR_SIZE, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ(L"\x91C\x921\x924", b);
}

TEST(ToWc, Ascii)
{
	const char* c = "7";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1 * UTF8_WCHAR_SIZE, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ(L"7", b);
}

TEST(ToWc, AsciiNotEnoughData)
{
	const char* c = "%";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8towc(c, 0, b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, AsciiNotEnoughSpace)
{
	const char* c = "D";
	const size_t s = 0;
	wchar_t b[1] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, TwoBytes)
{
	const char* c = "\xCB\x8C";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1 * UTF8_WCHAR_SIZE, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ(L"\x02CC", b);
}

TEST(ToWc, TwoBytesNotEnoughData)
{
	const char* c = "\xDC";
	const size_t s = 1;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, TwoBytesNotEnoughSpace)
{
	const char* c = "\xDC\x80";
	const size_t s = 1;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8towc(c, strlen(c), (wchar_t*)b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(ToWc, ThreeBytes)
{
	const char* c = "\xEA\xAC\x81";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1 * UTF8_WCHAR_SIZE, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ(L"\xAB01", b);
}

TEST(ToWc, ThreeBytesNotEnoughData)
{
	const char* c = "\xEA\xAC";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, AboveBasicMultilingualPlane)
{
	const char* c = "\xF0\x90\xB0\x91";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

#if UTF8_WCHAR_UTF16
	EXPECT_EQ(4, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\xD803\xDC11", b);
#elif UTF8_WCHAR_UTF32
	EXPECT_EQ(4, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\x10C11", b);
#endif
	EXPECT_EQ(0, errors);
}

TEST(ToWc, AboveBasicMultilingualPlaneFirst)
{
	const char* c = "\xF0\x90\x80\x80";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

#if UTF8_WCHAR_UTF16
	EXPECT_EQ(4, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\xD800\xDC00", b);
#elif UTF8_WCHAR_UTF32
	EXPECT_EQ(4, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\x10000", b);
#endif
	EXPECT_EQ(0, errors);
}

TEST(ToWc, AboveBasicMultilingualPlaneLast)
{
	const char* c = "\xF4\x8F\xBF\xBF";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

#if UTF8_WCHAR_UTF16
	EXPECT_EQ(4, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\xDBFF\xDFFF", b);
#elif UTF8_WCHAR_UTF32
	EXPECT_EQ(4, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_STREQ(L"\x10FFFF", b);
#endif
	EXPECT_EQ(0, errors);
}

TEST(ToWc, AboveBasicMultilingualPlaneNotEnoughData)
{
	const char* c = "\xF0\x90\x8C";
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8towc(c, strlen(c), b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, AboveBasicMultilingualPlaneNotEnoughSpace)
{
	const char* c = "\xF0\x90\x82\xB0";
	const size_t s = 1;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8towc(c, strlen(c), (wchar_t*)b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(ToWc, NoData)
{
	const char* c = nullptr;
	const size_t s = 256;
	wchar_t b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8towc(c, 2, b, s * sizeof(wchar_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ(L"", b);
}