#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-seeking.hpp"

TEST(Utf8SeekEnd, OneByteSingle)
{
	const char* t = "Z";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleFirst)
{
	const char* t = "\0";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleLast)
{
	const char* t = "\x7F";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleInvalidContinuationByteFirst)
{
	const char* t = "\x80";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleInvalidContinuationByteLast)
{
	const char* t = "\xBF";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleOverlongOneByte)
{
	const char* t = "\x26\x9C";

	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleOverlongTwoBytes)
{
	const char* t = "\x51\xA1\xB0";

	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleOverlongThreeBytes)
{
	const char* t = "\x24\x99\xA2\x81";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleOverlongFourBytes)
{
	const char* t = "\x3B\x9A\x80\x92\x86";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 5, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleOverlongFiveBytes)
{
	const char* t = "\x45\x86\x91\xA2\x85\x92";

	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 6, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteSingleOverlongSixBytes)
{
	const char* t = "\x5F\x92\xA9\x92\xA5\x82\x99";

	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 6, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 7, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteMultiple)
{
	const char* t = "Farmer";

	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 6, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteMultipleContinuationBytes)
{
	const char* t = "\x95\xA5\x92\xB2\x90";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 5, SEEK_END);
}

TEST(Utf8SeekEnd, OneByteMultipleOverlong)
{
	const char* t = "\x86\xB4\xA2"
					"\x72\xB1\x8D\x87\x95"
					"\x41\x90\x84";

	EXPECT_SEEKEQ(t, 10, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 9, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 8, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 7, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 6, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 7, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 8, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 9, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 10, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 11, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingle)
{
	const char* t = "\xC9\x86";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleFirst)
{
	const char* t = "\xC0\x80";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleLast)
{
	const char* t = "\xDF\xBF";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xCD\x24";

	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xD8\xD5";

	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleMissingOneByte)
{
	const char* t = "\xCE";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleOverlongOneByte)
{
	const char* t = "\xC5\x92\xB2";

	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleOverlongTwoBytes)
{
	const char* t = "\xD4\x85\x92\x9A";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleOverlongThreeBytes)
{
	const char* t = "\xDD\x81\x9A\x9C\xBC";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleOverlongFourBytes)
{
	const char* t = "\xD1\x96\x92\xAC\x84\xB2";

	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 5, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesSingleOverlongFiveBytes)
{
	const char* t = "\xDC\x92\x84\x92\x99\x9A\x83";

	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 6, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesMultiple)
{
	const char* t = "\xD4\x9A\xD4\x9C\xD4\x86\xD4\x8A\xD4\xB6";

	EXPECT_SEEKEQ(t, 8, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 5, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesMultipleInvalid)
{
	const char* t = "\xC8\xD8\xC4\xC7\xDE";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 5, SEEK_END);
}

TEST(Utf8SeekEnd, TwoBytesMultipleOverlong)
{
	const char* t = "\xC9\x82\x99\x9A"
					"\xD1\x82\x92"
					"\xD4\x9A\x9A\xB2";

	EXPECT_SEEKEQ(t, 10, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 9, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 7, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 6, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 7, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 8, SEEK_END);
}

TEST(Utf8SeekEnd, Valid)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 4, SEEK_END);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(Utf8SeekEnd, Offset)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + 12, t, 2, SEEK_END);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(Utf8SeekEnd, ZeroOffset)
{
	const char* t = "\xE2\xB7\xB0\xE2\xB8\x97\xE2\xB8\xBA\xE2\xB8\xAF\xE2\xB9\x8F";

	const char* r = utf8seek(t, t, 0, SEEK_END);

	EXPECT_EQ(t + 15, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekEnd, NegativeOffset)
{
	const char* t = "Alternative";

	const char* r = utf8seek(t, t, -2, SEEK_END);

	EXPECT_EQ(t + 11, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekEnd, PastStart)
{
	const char* t = "Moonshine";

	const char* r = utf8seek(t, t, 16, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Moonshine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('M', o);
}

TEST(Utf8SeekEnd, EndsInMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\0\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 5, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x901, o);
}

TEST(Utf8SeekEnd, LonelyStartDouble)
{
	const char* t = "Dino\xE8\xD5magic";

	const char* r = utf8seek(t, t, 8, SEEK_END);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("o\xE8\xD5magic", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('o', o);
}

TEST(Utf8SeekEnd, SwappedParameters)
{
	const char* t = "\xE2\xB7\xB0\xE2\xB8\x97\xE2\xB8\xBA\xE2\xB8\xAF\xE2\xB9\x8F";

	const char* r = utf8seek(t, t + strlen(t), 3, SEEK_END);

	EXPECT_EQ(t + 15, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekEnd, Empty)
{
	const char* t = "";

	const char* r = utf8seek(t, t, -5, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekEnd, AsciiIllegalByteFE)
{
	const char* t = "The time\xFE" "box";

	const char* r = utf8seek(t, t, 11, SEEK_END);

	EXPECT_EQ(t + 1, r);
	EXPECT_STREQ("he time\xFE" "box", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('h', o);
}

TEST(Utf8SeekEnd, AsciiIllegalByteFF)
{
	const char* t = "Mag\xFF KKD";

	const char* r = utf8seek(t, t, 7, SEEK_END);

	EXPECT_EQ(t + 1, r);
	EXPECT_STREQ("ag\xFF KKD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('a', o);
}

TEST(Utf8SeekEnd, AsciiMalformedContinuationByte)
{
	const char* t = "Player: \x87" "bob";

	const char* r = utf8seek(t, t, 8, SEEK_END);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("yer: \x87" "bob", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('y', o);
}

TEST(Utf8SeekEnd, TwoBytes)
{
	const char* t = "\xD4\x9A\xD4\x9C\xD4\x86\xD4\x8A\xD4\xB6";

	const char* r = utf8seek(t, t, 1, SEEK_END);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xD4\xB6", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x536, o);
}

TEST(Utf8SeekEnd, TwoBytesOverlong)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t, t, 1, SEEK_END);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, TwoBytesOverlongPastStart)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t, t, 6, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xC0\xAF\xC1\xBF\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, TwoBytesLonelyStart)
{
	const char* t = "Gone\xC4" "Fishin'";

	const char* r = utf8seek(t, t, 8, SEEK_END);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC4" "Fishin'", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, TwoBytesLonelyStartAtStart)
{
	const char* t = "\xDCMegalodon";

	const char* r = utf8seek(t, t, 10, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xDCMegalodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, ThreeBytes)
{
	const char* t = "\xE2\xB7\xB0\xE2\xB8\x97\xE2\xB8\xBA\xE2\xB8\xAF\xE2\xB9\x8F";

	const char* r = utf8seek(t, t, 2, SEEK_END);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE2\xB8\xAF\xE2\xB9\x8F", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x2E2F, o);
}

TEST(Utf8SeekEnd, ThreeBytesOverlong)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 1, SEEK_END);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, ThreeBytesOverlongPastStart)
{
	const char* t = "\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 4, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, ThreeBytesLonelyStart)
{
	const char* t = "Rocker\xE4" "coaster";

	const char* r = utf8seek(t, t, 10, SEEK_END);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("er\xE4" "coaster", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('e', o);
}

TEST(Utf8SeekEnd, ThreeBytesLonelyStartAtStart)
{
	const char* t = "\xEDSubmarine";

	const char* r = utf8seek(t, t, 10, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xEDSubmarine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, ThreeBytesNotEnoughData)
{
	const char* t = "Compan\xEF\xBFions";

	const char* r = utf8seek(t, t, 6, SEEK_END);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("n\xEF\xBFions", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(Utf8SeekEnd, ThreeBytesNotEnoughDataAtStart)
{
	const char* t = "\xEF\xBF" "BBBark";

	const char* r = utf8seek(t, t, 7, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xEF\xBF" "BBBark", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 4, SEEK_END);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(Utf8SeekEnd, FourBytesOverlong)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF4\x90\x80\x80";

	const char* r = utf8seek(t, t, 1, SEEK_END);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FourBytesOverlongPastStart)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t, t, 5, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FourBytesLonelyStart)
{
	const char* t = "Clam\xF4shellpower";

	const char* r = utf8seek(t, t, 12, SEEK_END);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("m\xF4shellpower", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('m', o);
}

TEST(Utf8SeekEnd, FourBytesLonelyStartAtStart)
{
	const char* t = "\xF6Magic";

	const char* r = utf8seek(t, t, 6, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF6Magic", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FourBytesNotEnoughData)
{
	const char* t = "Brilli\xF0\x90\x80" "ant";

	const char* r = utf8seek(t, t, 4, SEEK_END);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xF0\x90\x80" "ant", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FourBytesNotEnoughDataAtStart)
{
	const char* t = "\xF0\x90\x80Night";

	const char* r = utf8seek(t, t, 6, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x90\x80Night", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FiveBytesOverlong)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 2, SEEK_END);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FiveBytesOverlongPastStart)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 13, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FiveBytesLonelyStart)
{
	const char* t = "Beyond\xFAMegalodon";

	const char* r = utf8seek(t, t, 12, SEEK_END);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("nd\xFAMegalodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(Utf8SeekEnd, FiveBytesLonelyStartAtStart)
{
	const char* t = "\xFA" "Brontos";

	const char* r = utf8seek(t, t, 8, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFA" "Brontos", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, FiveBytesNotEnoughData)
{
	const char* t = "Central\xF8\x80\x80\x80Plaza baker";

	const char* r = utf8seek(t, t, 13, SEEK_END);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("l\xF8\x80\x80\x80Plaza baker", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('l', o);
}

TEST(Utf8SeekEnd, FiveBytesNotEnoughDataAtStart)
{
	const char* t = "\xF8\x80\x80\x80" "Apple";

	const char* r = utf8seek(t + strlen(t), t, 6, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF8\x80\x80\x80" "Apple", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, SixBytesOverlong)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 2, SEEK_END);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, SixBytesLonelyStart)
{
	const char* t = "FC\xFCKnudde festival";

	const char* r = utf8seek(t, t, 16, SEEK_END);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("\xFCKnudde festival", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, SixBytesLonelyStartAtStart)
{
	const char* t = "\xFDStuff";

	const char* r = utf8seek(t, t, 6, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFDStuff", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekEnd, SixBytesNotEnoughData)
{
	const char* t = "Hint\xFC\x80\x80\x80\x80machine";

	const char* r = utf8seek(t, t, 9, SEEK_END);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("t\xFC\x80\x80\x80\x80machine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('t', o);
}

TEST(Utf8SeekEnd, SixBytesNotEnoughDataAtStart)
{
	const char* t = "\xFC\x80\x80\x80\x80" "Dull";

	const char* r = utf8seek(t, t, 5, SEEK_END);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFC\x80\x80\x80\x80" "Dull", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}