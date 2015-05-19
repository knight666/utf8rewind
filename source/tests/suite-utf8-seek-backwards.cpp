#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-seeking.hpp"

TEST(Utf8SeekBackwards, OneByteSingle)
{
	const char* t = "*";

	EXPECT_SEEKEQ("*", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteSingleFirst)
{
	const char* t = "\0";

	EXPECT_SEEKEQ("", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteSingleLast)
{
	const char* t = "\x7F";

	EXPECT_SEEKEQ("\x7F", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteSingleInvalidContinuationByteFirst)
{
	const char* t = "\x80";

	EXPECT_SEEKEQ("\x80", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteSingleInvalidContinuationByteLast)
{
	const char* t = "\xBF";

	EXPECT_SEEKEQ("\xBF", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteSingleIllegalByteFirst)
{
	const char* t = "\xFE";

	EXPECT_SEEKEQ("\xFE", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteSingleIllegalByteLast)
{
	const char* t = "\xFF";

	EXPECT_SEEKEQ("\xFF", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteMultiple)
{
	const char* t = "Heartbreaker";

	EXPECT_SEEKEQ("breaker", 5, utf8seek(t + strlen(t), t, -7, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteMultipleInvalidContinuationByte)
{
	const char* t = "\x98\x92\xB4\x81\xA5\x88\x93";

	EXPECT_SEEKEQ("\x81\xA5\x88\x93", 3, utf8seek(t + strlen(t), t, -4, SEEK_CUR));
}

TEST(Utf8SeekBackwards, OneByteMultipleIllegal)
{
	const char* t = "\xFE\xFE\xFF\xFE\xFE\xFF";

	EXPECT_SEEKEQ("\xFE\xFF", 4, utf8seek(t + strlen(t), t, -2, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesSingle)
{
	const char* t = "\xC4\x82";

	EXPECT_SEEKEQ("\xC4\x82", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesSingleFirst)
{
	const char* t = "\xC0\x80";

	EXPECT_SEEKEQ("\xC0\x80", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesSingleLast)
{
	const char* t = "\xDF\xBF";

	EXPECT_SEEKEQ("\xDF\xBF", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xC8\x4A";

	EXPECT_SEEKEQ("\x4A", 1, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xD0\xC3";

	EXPECT_SEEKEQ("\xC3", 1, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesSingleMissingOneByte)
{
	const char* t = "\xCC";

	EXPECT_SEEKEQ("\xCC", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesSingleOverlong)
{
	const char* t = "\xC2\x86\x82";

	EXPECT_SEEKEQ("\x82", 2, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesMultiple)
{
	const char* t = "\xC4\x97\xD5\x99\xCA\xAA\xCD\xB2";

	EXPECT_SEEKEQ("\xD5\x99\xCA\xAA\xCD\xB2", 2, utf8seek(t + strlen(t), t, -3, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesMultipleInvalid)
{
	const char* t = "\xC3\xD4\xC9\x15\xD5\xD1";

	EXPECT_SEEKEQ("\x15\xD5\xD1", 3, utf8seek(t + strlen(t), t, -3, SEEK_CUR));
}

TEST(Utf8SeekBackwards, TwoBytesMultipleOverlong)
{
	const char* t = "\xC8\x9A\x99\xD2\xA4\xA1\x99\xD2\x92\xA9";

	EXPECT_SEEKEQ("\xD2\xA4\xA1\x99\xD2\x92\xA9", 3, utf8seek(t + strlen(t), t, -5, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingle)
{
	const char* t = "\xE2\x86\xB9";

	EXPECT_SEEKEQ("\xE2\x86\xB9", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleFirst)
{
	const char* t = "\xE0\x80\x80";

	EXPECT_SEEKEQ("\xE0\x80\x80", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleLast)
{
	const char* t = "\xEF\xBF\xBF";

	EXPECT_SEEKEQ("\xEF\xBF\xBF", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xE2\x5A\x9A";

	EXPECT_SEEKEQ("\x9A", 2, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xEA\xD1\x8B";

	EXPECT_SEEKEQ("\x8B", 2, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xED\x8B\x2F";

	EXPECT_SEEKEQ("\x2F", 2, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xE2\x91\xD5";

	EXPECT_SEEKEQ("\xD5", 2, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleMissingOneByte)
{
	const char* t = "\xEE\x8A";

	EXPECT_SEEKEQ("\xEE\x8A", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleMissingTwoBytes)
{
	const char* t = "\xE5";

	EXPECT_SEEKEQ("\xE5", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesSingleOverlong)
{
	const char* t = "\xE8\x98\x9A\x86";

	EXPECT_SEEKEQ("\x86", 3, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesMultiple)
{
	const char* t = "\xE2\x87\x99\xE4\x9A\xAB\xE3\xBD\xBD"
					"\xE5\x99\xA8\xE7\xB1\x8A";

	EXPECT_SEEKEQ("\xE3\xBD\xBD\xE5\x99\xA8\xE7\xB1\x8A", 6, utf8seek(t + strlen(t), t, -3, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesMultipleInvalid)
{
	const char* t = "\xE8\x92\xEF\x16\xE4\x92\xE0";

	EXPECT_SEEKEQ("\xEF\x16\xE4\x92\xE0", 2, utf8seek(t + strlen(t), t, -4, SEEK_CUR));
}

TEST(Utf8SeekBackwards, ThreeBytesMultipleOverlong)
{
	const char* t = "\xE3\x97\xA9\x88\x92\xE4\x81\x9A\x91\xE4\x88\xA4\xB4\x81"
					"\xE5\x8A\x8A\x81\xE2\x96\x99\x99";

	EXPECT_SEEKEQ("\xE3\x97\xA9\x88\x92\xE4\x81\x9A\x91\xE4\x88\xA4\xB4\x81\xE5\x8A\x8A", 17, utf8seek(t + strlen(t), t, -3, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingle)
{
	const char* t = "\xF1\xB2\x91\x81";

	EXPECT_SEEKEQ("\xF1\xB2\x91\x81", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleFirst)
{
	const char* t = "\xF0\x80\x80\x80";

	EXPECT_SEEKEQ("\xF0\x80\x80\x80", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleLast)
{
	const char* t = "\xF4\xBF\xBF\xBF";

	EXPECT_SEEKEQ("\xF4\xBF\xBF\xBF", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF3\x17\x99\xA8";

	EXPECT_SEEKEQ("\xA8", 3, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xF1\xD5\xB2\x8B";

	EXPECT_SEEKEQ("\x8B", 3, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xF1\x85\x26\xB2";

	EXPECT_SEEKEQ("\xB2", 3, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xF4\x87\xE1\x88";

	EXPECT_SEEKEQ("\x88", 3, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xF2\x9A\x81\x31";

	EXPECT_SEEKEQ("\x31", 3, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xF2\x82\x92\xF5";

	EXPECT_SEEKEQ("\xF5", 3, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleMissingOneByte)
{
	const char* t = "\xF4\x87\x92";

	EXPECT_SEEKEQ("\xF4\x87\x92", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleMissingTwoByte)
{
	const char* t = "\xF2\x8A";

	EXPECT_SEEKEQ("\xF2\x8A", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleMissingThreeBytes)
{
	const char* t = "\xF0";

	EXPECT_SEEKEQ("\xF0", 0, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesSingleOverlong)
{
	const char* t = "\xF4\xB1\xAF\xAF\x9A";

	EXPECT_SEEKEQ("\x9A", 4, utf8seek(t + strlen(t), t, -1, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesMultiple)
{
	const char* t = "\xF2\x8A\x91\x99\xF4\x8A\x92\x90\xF1\x92\xA9\x92"
					"\xF4\xB4\xA4\x85\xF2\x95\x85\x86";

	EXPECT_SEEKEQ("\xF4\x8A\x92\x90\xF1\x92\xA9\x92\xF4\xB4\xA4\x85\xF2\x95\x85\x86", 4, utf8seek(t + strlen(t), t, -4, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesMultipleInvalid)
{
	const char* t = "\xF2\x87\xF9\xF4\x87\xF1\xF2\x81\x12";

	EXPECT_SEEKEQ("\xF4\x87\xF1\xF2\x81\x12", 3, utf8seek(t + strlen(t), t, -4, SEEK_CUR));
}

TEST(Utf8SeekBackwards, FourBytesMultipleOverlong)
{
	const char* t = "\xF3\x81\x81\x84\x84\xF4\x91\x95\xA8\x99"
					"\xF2\x87\x86\x9A\xA2\xF1\x92\x90\xA9\xA9"
					"\xF2\x81\x92\x92\x92";

	EXPECT_SEEKEQ("\xA2\xF1\x92\x90\xA9\xA9\xF2\x81\x92\x92\x92", 14, utf8seek(t + strlen(t), t, -5, SEEK_CUR));
}

TEST(Utf8SeekBackwards, Valid)
{
	const char* t = "\xCF\x84\xE1\xBD\xB4\xCE\xBD \xCE\xBA\xE1\xBD\xB9\xCF\x88\xCE\xB7";

	const char* r = utf8seek(t + strlen(t), t, -5, SEEK_CUR);

	EXPECT_EQ(t + 7, r);
	EXPECT_STREQ(" \xCE\xBA\xE1\xBD\xB9\xCF\x88\xCE\xB7", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(' ', o);
}

TEST(Utf8SeekBackwards, EndsWithAscii)
{
	const char* t = "Tree\xD1\x86j";

	const char* r = utf8seek(t + strlen(t), t, -3, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("e\xD1\x86j", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('e', o);
}

TEST(Utf8SeekBackwards, PastStart)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + strlen(t), t, -26, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x901, o);
}

TEST(Utf8SeekBackwards, AtStart)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x901, o);
}

TEST(Utf8SeekBackwards, FromMiddle)
{
	const char* t = "Banananana";

	const char* r = utf8seek(t + 6, t, -4, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("nananana", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(Utf8SeekBackwards, EndsInMiddle)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\0\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 16, t, -2, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekBackwards, LonelyStartDouble)
{
	const char* t = "Rock\xEF\xFBsoup";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xEF\xFBsoup", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, SwappedParameters)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t + strlen(t), -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, Ascii)
{
	const char* t = "Paper boat";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("at", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('a', o);
}

TEST(Utf8SeekBackwards, AsciiIllegalByteFE)
{
	const char* t = "Stuff\xFE happens";

	const char* r = utf8seek(t + 14, t, -11, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("ff\xFE happens", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('f', o);
}

TEST(Utf8SeekBackwards, AsciiIllegalByteFF)
{
	const char* t = "Mag\xFF KKD";

	const char* r = utf8seek(t + 8, t, -7, SEEK_CUR);

	EXPECT_EQ(t + 1, r);
	EXPECT_STREQ("ag\xFF KKD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('a', o);
}

TEST(Utf8SeekBackwards, AsciiMalformedContinuationByte)
{
	const char* t = "Player: \x87" "bob";

	const char* r = utf8seek(t + 12, t, -8, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("yer: \x87" "bob", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('y', o);
}

TEST(Utf8SeekBackwards, TwoBytes)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + strlen(t), t, -3, SEEK_CUR);

	EXPECT_EQ(t + 16, r);
	EXPECT_STREQ("\xD1\x86\xD0\xB8\xD0\xB8", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x446, o);
}

TEST(Utf8SeekBackwards, TwoBytesOverlong)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t + 6, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, TwoBytesOverlongFromMiddle)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF\xC0\xAF";

	const char* r = utf8seek(t + 6, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC0\xAF\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, TwoBytesOverlongPastStart)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xC0\xAF\xC1\xBF\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, TwoBytesLonelyStart)
{
	const char* t = "Gone\xC4" "Fishin'";

	const char* r = utf8seek(t + strlen(t), t, -8, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xC4" "Fishin'", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, TwoBytesLonelyStartAtStart)
{
	const char* t = "\xDCMegalodon";

	const char* r = utf8seek(t + strlen(t), t, -10, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xDCMegalodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x934, o);
}

TEST(Utf8SeekBackwards, ThreeBytesOverlong)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF";

	const char* r = utf8seek(t + strlen(t), t, -1, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, ThreeBytesOverlongFromMiddle)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t + 9, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\x80\xAF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, ThreeBytesOverlongPastStart)
{
	const char* t = "\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t + strlen(t), t, -4, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, ThreeBytesLonelyStart)
{
	const char* t = "Rocker\xE4" "coaster";

	const char* r = utf8seek(t + strlen(t), t, -10, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("er\xE4" "coaster", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('e', o);
}

TEST(Utf8SeekBackwards, ThreeBytesLonelyStartAtStart)
{
	const char* t = "\xEDSubmarine";

	const char* r = utf8seek(t + strlen(t), t, -10, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xEDSubmarine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, ThreeBytesNotEnoughData)
{
	const char* t = "Compan\xEF\xBFions";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("n\xEF\xBFions", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(Utf8SeekBackwards, ThreeBytesNotEnoughDataAtStart)
{
	const char* t = "\xEF\xBF" "BBBark";

	const char* r = utf8seek(t + strlen(t), t, -7, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xEF\xBF" "BBBark", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(Utf8SeekBackwards, FourBytesOverlong)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF4\x90\x80\x80";

	const char* r = utf8seek(t + strlen(t), t, -1, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FourBytesOverlongFromMiddle)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t + 12, t, -2, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FourBytesOverlongPastStart)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t + strlen(t), t, -5, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FourBytesLonelyStart)
{
	const char* t = "Clam\xF4shellpower";

	const char* r = utf8seek(t + strlen(t), t, -12, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("m\xF4shellpower", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('m', o);
}

TEST(Utf8SeekBackwards, FourBytesLonelyStartAtStart)
{
	const char* t = "\xF6Magic";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF6Magic", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FourBytesNotEnoughData)
{
	const char* t = "Brilli\xF0\x90\x80" "ant";

	const char* r = utf8seek(t + strlen(t), t, -4, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xF0\x90\x80" "ant", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FourBytesNotEnoughDataAtStart)
{
	const char* t = "\xF0\x90\x80Night";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x90\x80Night", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FiveBytesOverlong)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FiveBytesOverlongFromMiddle)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 10, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FiveBytesOverlongPastStart)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + strlen(t), t, -13, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FiveBytesLonelyStart)
{
	const char* t = "Beyond\xFAMegalodon";

	const char* r = utf8seek(t + strlen(t), t, -12, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("nd\xFAMegalodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(Utf8SeekBackwards, FiveBytesLonelyStartAtStart)
{
	const char* t = "\xFA" "Brontos";

	const char* r = utf8seek(t + strlen(t), t, -8, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFA" "Brontos", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, FiveBytesNotEnoughData)
{
	const char* t = "Central\xF8\x80\x80\x80Plaza baker";

	const char* r = utf8seek(t + strlen(t), t, -13, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("l\xF8\x80\x80\x80Plaza baker", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('l', o);
}

TEST(Utf8SeekBackwards, FiveBytesNotEnoughDataAtStart)
{
	const char* t = "\xF8\x80\x80\x80" "Apple";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF8\x80\x80\x80" "Apple", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, SixBytesOverlong)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, SixBytesOverlongFromMiddle)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 12, t, -1, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, SixBytesOverlongPastStart)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + strlen(t), t, -8, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, SixBytesLonelyStart)
{
	const char* t = "FC\xFCKnudde festival";

	const char* r = utf8seek(t + strlen(t), t, -16, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("\xFCKnudde festival", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, SixBytesLonelyStartAtStart)
{
	const char* t = "\xFDStuff";

	const char* r = utf8seek(t + strlen(t), t, -6, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFDStuff", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekBackwards, SixBytesNotEnoughData)
{
	const char* t = "Hint\xFC\x80\x80\x80\x80machine";

	const char* r = utf8seek(t + strlen(t), t, -9, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("t\xFC\x80\x80\x80\x80machine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('t', o);
}

TEST(Utf8SeekBackwards, SixBytesNotEnoughDataAtStart)
{
	const char* t = "\xFC\x80\x80\x80\x80" "Dull";

	const char* r = utf8seek(t + strlen(t), t, -5, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xFC\x80\x80\x80\x80" "Dull", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}