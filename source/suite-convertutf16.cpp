#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ConvertUtf16, Ucs2OneByte)
{
	utf16_t c = L'*';
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(1, utf8convertutf16((const char*)&c, 2, b, s, &r));
	EXPECT_EQ(2, r);
	EXPECT_STREQ("*", b);
}

TEST(ConvertUtf16, Ucs2TwoBytes)
{
	utf16_t c = 0x02CC;
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(2, utf8convertutf16((const char*)&c, 2, b, s, &r));
	EXPECT_EQ(2, r);
	EXPECT_STREQ("\xCB\x8C", b);
}

TEST(ConvertUtf16, Ucs2ThreeBytes)
{
	utf16_t c = 0x1280;
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(3, utf8convertutf16((const char*)&c, 2, b, s, &r));
	EXPECT_EQ(2, r);
	EXPECT_STREQ("\xE1\x8A\x80", b);
}

TEST(ConvertUtf16, SurrogatePair)
{
	const char* c = "\x34\xD8\x1E\xDD";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(4, utf8convertutf16(c, 4, b, s, &r));
	EXPECT_EQ(4, r);
	EXPECT_STREQ("\xF0\x9D\x84\x9E", b);
}

TEST(ConvertUtf16, SurrogatePairMinimum)
{
	const char* c = "\x00\xD8\x00\xDC";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(4, utf8convertutf16(c, 4, b, s, &r));
	EXPECT_EQ(4, r);
	EXPECT_STREQ("\xF0\x90\x80\x80", b);
}

TEST(ConvertUtf16, SurrogatePairMaximum)
{
	const char* c = "\xFF\xDB\xFF\xDF";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(4, utf8convertutf16(c, 4, b, s, &r));
	EXPECT_EQ(4, r);
	EXPECT_STREQ("\xF4\x8F\xBF\xBF", b);
}

TEST(ConvertUtf16, UnmatchedLowSurrogatePair)
{
	const char* c = "\x00\xD8\x00\x11";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, utf8convertutf16(c, 4, b, s, &r));
	EXPECT_EQ(0, r);
	EXPECT_STREQ("", b);
}

TEST(ConvertUtf16, UnmatchedHighSurrogatePair)
{
	const char* c = "\x1E\xDD\x34\xD8";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR, utf8convertutf16(c, 4, b, s, &r));
	EXPECT_EQ(0, r);
	EXPECT_STREQ("", b);
}

TEST(ConvertUtf16, NoData)
{
	const char* c = nullptr;
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8convertutf16(c, 2, b, s, &r));
	EXPECT_EQ(0, r);
	EXPECT_STREQ("", b);
}

TEST(ConvertUtf16, InvalidLength)
{
	const char* c = "\x88\x0D";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8convertutf16(c, 1, b, s, &r));
	EXPECT_EQ(0, r);
	EXPECT_STREQ("", b);
}

TEST(ConvertUtf16, NotEnoughData)
{
	const char* c = "\x00\xD8";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8convertutf16(c, 2, b, s, &r));
	EXPECT_EQ(0, r);
	EXPECT_STREQ("", b);
}

TEST(ConvertUtf16, NotEnoughSpace)
{
	const char* c = "\x00\xD8\x10\xD8";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8convertutf16(c, 4, b, s, &r));
	EXPECT_EQ(0, r);
	EXPECT_STREQ("", b);
}

TEST(ConvertUtf16, Ucs2NotEnoughData)
{
	const char* c = " ";
	const size_t s = 256;
	char b[s] = { 0 };
	int r = 0;

	EXPECT_EQ(UTF8_ERR_INVALID_DATA, utf8convertutf16(c, 1, b, s, &r));
	EXPECT_EQ(0, r);
	EXPECT_STREQ("", b);
}