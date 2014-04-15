#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ToWc, Convert)
{
	const char* c = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(6, utf8towc(c, 9, b, s));
	EXPECT_STREQ(L"\x91C\x921\x924", b);
}

TEST(ToWc, OneByte)
{
	const char* c = "7";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(2, utf8towc(c, 1, b, s));
	EXPECT_STREQ(L"7", b);
}

TEST(ToWc, OneByteInvalidLength)
{
	const char* c = "%";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8towc(c, 0, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, OneByteNotEnoughSpace)
{
	const char* c = "D";
	const size_t s = 0;
	wchar_t b[1] = { 0 };

	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, utf8towc(c, 4, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, TwoBytes)
{
	const char* c = "\xCB\x8C";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(2, utf8towc(c, 2, b, s));
	EXPECT_STREQ(L"\x02CC", b);
}

TEST(ToWc, TwoBytesNotEnoughData)
{
	const char* c = "\xDC";
	const size_t s = 1;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8towc(c, 1, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, TwoBytesInvalidLength)
{
	const char* c = "\xDC\x80";
	const size_t s = 1;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, utf8towc(c, 2, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, TwoBytesNotEnoughSpace)
{
	const char* c = "\xDC\x80";
	const size_t s = 1;
	char b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, utf8towc(c, 2, (wchar_t*)b, s));
	EXPECT_STREQ("", b);
}

TEST(ToWc, ThreeBytes)
{
	const char* c = "\xEA\xAC\x81";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(2, utf8towc(c, 3, b, s));
	EXPECT_STREQ(L"\xAB01", b);
}

TEST(ToWc, ThreeBytesNotEnoughData)
{
	const char* c = "\xEA\xAC";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8towc(c, 2, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, ThreeBytesInvalidLength)
{
	const char* c = "\xE3\x83\xBB";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8towc(c, 2, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, ThreeBytesNotEnoughSpace)
{
	const char* c = "\xE1\x9F\x81";
	const size_t s = 0;
	wchar_t* b = nullptr;

	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, utf8towc(c, 2, b, s));
	EXPECT_STREQ(nullptr, b);
}

TEST(ToWc, SurrogatePairMinimum)
{
	const char* c = "\x00\xD8";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(2, utf8towc(c, 2, b, s));
	EXPECT_STREQ(L"\xFFFD", b);
}

TEST(ToWc, SurrogatePairMaximum)
{
	const char* c = "\xFF\xDF";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(2, utf8towc(c, 2, b, s));
	EXPECT_STREQ(L"\xFFFD", b);
}

TEST(ToWc, AboveBasicMultilingualPlane)
{
	const char* c = "\xF0\x90\xB0\x91";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(4, utf8towc(c, 4, b, s));
	EXPECT_STREQ(L"\xD803\xDC11", b);
}

TEST(ToWc, AboveBasicMultilingualPlaneNotEnoughData)
{
	const char* c = "\xF0\x90\x8C";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8towc(c, 3, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, AboveBasicMultilingualPlaneInvalidLength)
{
	const char* c = "\xF0\x90\x85\x82";
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8towc(c, 3, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, AboveBasicMultilingualPlaneNotEnoughSpace)
{
	const char* c = "\xF0\x90\x82\xB0";
	const size_t s = 1;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, utf8towc(c, 4, b, s));
	EXPECT_STREQ(L"", b);
}

TEST(ToWc, NoData)
{
	const char* c = nullptr;
	const size_t s = 256;
	wchar_t b[s] = { 0 };

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8towc(c, 2, b, s));
	EXPECT_STREQ(L"", b);
}