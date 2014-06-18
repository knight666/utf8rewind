#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ToUtf8, Convert)
{
	const wchar_t* c = L"जडत";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(9, wctoutf8(c, 6, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4", b);
}

TEST(ToUtf8, Ucs2OneByte)
{
	utf16_t c = L'*';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, wctoutf8((const wchar_t*)&c, 2, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("*", b);
}

TEST(ToUtf8, Ucs2TwoBytes)
{
	utf16_t c = 0x02CC;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, wctoutf8((const wchar_t*)&c, 2, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xCB\x8C", b);
}

TEST(ToUtf8, Ucs2ThreeBytes)
{
	utf16_t c = 0x1280;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, wctoutf8((const wchar_t*)&c, 2, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE1\x8A\x80", b);
}

TEST(ToUtf8, SurrogatePair)
{
	const char* c = "\x34\xD8\x1E\xDD";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, wctoutf8((const wchar_t*)c, 4, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x9D\x84\x9E", b);
}

TEST(ToUtf8, SurrogatePairMinimum)
{
	const char* c = "\x00\xD8\x00\xDC";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, wctoutf8((const wchar_t*)c, 4, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x90\x80\x80", b);
}

TEST(ToUtf8, SurrogatePairMaximum)
{
	const char* c = "\xFF\xDB\xFF\xDF";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, wctoutf8((const wchar_t*)c, 4, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF4\x8F\xBF\xBF", b);
}

TEST(ToUtf8, UnmatchedLowSurrogatePair)
{
	const char* c = "\x00\xD8\x00\x11";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, wctoutf8((const wchar_t*)c, 4, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, UnmatchedHighSurrogatePair)
{
	const char* c = "\x1E\xDD\x34\xD8";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, wctoutf8((const wchar_t*)c, 4, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, NoData)
{
	const char* c = nullptr;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, wctoutf8((const wchar_t*)c, 2, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, NotEnoughSpace)
{
	const wchar_t* c = L"ं";
	const size_t s = 2;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, wctoutf8(c, 2, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, InvalidLength)
{
	const char* c = "\x88\x0D";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, wctoutf8((const wchar_t*)c, 1, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, InvalidData)
{
	const char* c = "\x00\xD8";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, wctoutf8((const wchar_t*)c, 2, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, OutputLengthOneByte)
{
	utf16_t c = 0x0021;
	int32_t errors = 0;

	EXPECT_EQ(1, wctoutf8((const wchar_t*)&c, 2, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUtf8, OutputLengthTwoBytes)
{
	utf16_t c = 0x00DD;
	int32_t errors = 0;

	EXPECT_EQ(2, wctoutf8((const wchar_t*)&c, 2, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUtf8, OutputLengthThreeBytes)
{
	utf16_t c = 0x8812;
	int32_t errors = 0;

	EXPECT_EQ(3, wctoutf8((const wchar_t*)&c, 2, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUtf8, OutputLengthSurrogatePair)
{
	const char* c = "\x18\xD8\xDE\xDC";
	int32_t errors = 0;

	EXPECT_EQ(4, wctoutf8((const wchar_t*)c, 4, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}