﻿#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(ToUtf8, Convert)
{
	const wchar_t c[] = {
		L'ज',
		L'ड',
		L'त'
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(9, widetoutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4", b);
}

TEST(ToUtf8, OneByte)
{
	wchar_t c = L'*';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, widetoutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("*", b);
}

TEST(ToUtf8, TwoBytes)
{
	wchar_t c = 0x2CC;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, widetoutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xCB\x8C", b);
}

TEST(ToUtf8, ThreeBytes)
{
	wchar_t c = 0x1280;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, widetoutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE1\x8A\x80", b);
}

#if UTF8_WCHAR_UTF32

TEST(ToUtf8, FourBytes)
{
	wchar_t c = 0x1F612;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, widetoutf8(&c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x9F\x98\x92", b);
}

#endif

TEST(ToUtf8, SurrogatePair)
{
	const wchar_t c[] = {
		0xD834,
		0xDD1E
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, widetoutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x9D\x84\x9E", b);
}

TEST(ToUtf8, SurrogatePairMinimum)
{
	const wchar_t c[] = {
		0xD800, 0xDC00
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, widetoutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x90\x80\x80", b);
}

TEST(ToUtf8, SurrogatePairMaximum)
{
	const wchar_t c[] = {
		0xDBFF, 0xDFFF
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, widetoutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF4\x8F\xBF\xBF", b);
}

TEST(ToUtf8, UnmatchedLowSurrogatePair)
{
	const wchar_t c[] = {
		0xD800, 0x1100
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, widetoutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, UnmatchedHighSurrogatePair)
{
	const wchar_t c[] = {
		0xDD1E, 0xD834
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, widetoutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, NoData)
{
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, widetoutf8(nullptr, sizeof(wchar_t), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, NotEnoughSpace)
{
	const wchar_t c[] = {
		L"ं"
	};
	const size_t s = 2;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, widetoutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, InvalidLength)
{
	const wchar_t c[] = {
		0x0D88
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, widetoutf8(c, 1, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, InvalidData)
{
	const wchar_t c[] = {
		0xD800
	};
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, widetoutf8(c, sizeof(c), b, s, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_STREQ("", b);
}

TEST(ToUtf8, OutputLengthOneByte)
{
	const wchar_t c[] = {
		0x0021
	};
	int32_t errors = 0;

	EXPECT_EQ(1, widetoutf8(c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUtf8, OutputLengthTwoBytes)
{
	const wchar_t c[] = {
		0x00DD
	};
	int32_t errors = 0;

	EXPECT_EQ(2, widetoutf8(c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUtf8, OutputLengthThreeBytes)
{
	const wchar_t c[] = {
		0x8812
	};
	int32_t errors = 0;

	EXPECT_EQ(3, widetoutf8(c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

#if UTF8_WCHAR_UTF32

TEST(ToUtf8, OutputLengthFourBytes)
{
	const wchar_t c[] = {
		0x1D245
	};
	int32_t errors = 0;

	EXPECT_EQ(4, widetoutf8(c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

#endif

TEST(ToUtf8, OutputLengthSurrogatePair)
{
	const wchar_t c[] = {
		0xD818, 0xDCDE
	};
	int32_t errors = 0;

	EXPECT_EQ(4, widetoutf8(c, sizeof(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}