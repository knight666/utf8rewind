﻿#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(WideToUtf8, BasicLatin)
{
	wchar_t i[] = { 'c', 'o', 'n', 'n', 'e', 'c', 't' };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(7, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("connect", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, BasicLatinAmountOfBytes)
{
	wchar_t i[] = { '$', ' ', '4', '.', '3', '3' };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(6, widetoutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, BasicLatinNotEnoughSpace)
{
	wchar_t i[] = { 'r', 'o', 'c', 'k', 'e', 't' };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 4;
	int32_t errors;

	EXPECT_EQ(4, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("rock", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(WideToUtf8, TwoBytes)
{
	// MODIFIER LETTER LOW VERTICAL LINE
	// ARABIC LETTER NOON GHUNNA
	// GREEK CAPITAL LETTER BETA

	wchar_t i[] = { 0x02CC, 0x06BA, 0x0392 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(6, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xCB\x8C\xDA\xBA\xCE\x92", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, TwoBytesAmountOfBytes)
{
	// 03F7;GREEK CAPITAL LETTER SHO
	// 0572;ARMENIAN SMALL LETTER GHAD
	// 03CF;GREEK CAPITAL KAI SYMBOL

	wchar_t i[] = { 0x03F7, 0x0572, 0x03CF };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(6, widetoutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, TwoBytesNotEnoughSpace)
{
	// HEBREW POINT HATAF SEGOL
	// NKO LETTER A
	// ARABIC LETTER YEH WITH THREE DOTS BELOW

	wchar_t i[] = { 0x05B1, 0x07CA, 0x06D1 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 5;
	int32_t errors;

	EXPECT_EQ(4, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xD6\xB1\xDF\x8A", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(WideToUtf8, ThreeBytes)
{
	// DEVANAGARI LETTER JA
	// DEVANAGARI LETTER DDA
	// DEVANAGARI LETTER TA

	wchar_t i[] = { 0x091C, 0x0921, 0x0924 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(9, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, ThreeBytesAmountOfBytes)
{
	// ETHIOPIC SYLLABLE BU
	// VEDIC SIGN TIRYAK
	// NEW TAI LUE LETTER LOW KVA
	// ORIYA LETTER BHA

	wchar_t i[] = { 0x1261, 0x1CED, 0x19A8, 0x0B2D };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(12, widetoutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, ThreeBytesNotEnoughSpace)
{
	// MONGOLIAN LETTER MANCHU ALI GALI GHA
	// CANADIAN SYLLABICS MI
	// CANADIAN SYLLABICS TYO

	wchar_t i[] = { 0x189A, 0x14A5, 0x1572 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 7;
	int32_t errors;

	EXPECT_EQ(6, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xE1\xA2\x9A\xE1\x92\xA5", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

#if UTF8_WCHAR_UTF32

TEST(WideToUtf8, FourBytes)
{
	// UNAMUSED FACE
	// HEAVY NORTH WEST POINTING VINE LEAF
	// ALCHEMICAL SYMBOL FOR DISTILL

	wchar_t i[] = { 0x1F612, 0x1F760, 0x1F65C };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(12, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9F\x98\x92\xF0\x9F\x9D\xA0\xF0\x9F\x99\x9C", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, FourBytesAmountOfBytes)
{
	// TRAIN
	// RINGING BELL
	// FRAME WITH TILES
	// DOCUMENT WITH PICTURE

	wchar_t i[] = { 0x1F686, 0x1F56D, 0x1F5BD, 0x1F5BB };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(16, widetoutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, FourBytesNotEnoughSpace)
{
	// BABY SYMBOL
	// CJK COMPATIBILITY IDEOGRAPH-2F93A

	wchar_t i[] = { 0x1F6BC, 0x2F93A };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(4, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9F\x9A\xBC", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

#endif

TEST(WideToUtf8, SurrogatePair)
{
	const wchar_t i[] = { 0xD834, 0xDD1E, 0xDA88, 0xDC10 };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 255;
	int32_t errors;

	EXPECT_EQ(8, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("\xF0\x9D\x84\x9E\xF2\xB2\x80\x90", o);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, SurrogatePairAmountOfBytes)
{
	const wchar_t i[] = { 0xDAAD, 0xDC12, 0xDB01, 0xDC45 };
	size_t is = sizeof(i);
	int32_t errors;

	EXPECT_EQ(8, widetoutf8(i, is, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(WideToUtf8, SurrogatePairNotEnoughSpace)
{
	const wchar_t i[] = { 0xDBFE, 0xDFFE, 0xD818, 0xDCDE };
	size_t is = sizeof(i);
	char o[256] = { 0 };
	size_t os = 3;
	int32_t errors;

	EXPECT_EQ(0, widetoutf8(i, is, o, os, &errors));
	EXPECT_UTF8EQ("", o);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(WideToUtf8, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, widetoutf8(nullptr, sizeof(wchar_t), nullptr, 1, &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}