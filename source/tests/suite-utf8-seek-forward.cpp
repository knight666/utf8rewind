#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-seeking.hpp"

TEST(Utf8SeekForward, OneByteSingle)
{
	const char* t = "D";
	
	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteSingleFirst)
{
	const char* t = "\0";

	EXPECT_SEEKEQ("", 0, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteSingleLast)
{
	const char* t = "\x7F";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteSingleInvalidContinuationByteFirst)
{
	const char* t = "\x80";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteSingleInvalidContinuationByteLast)
{
	const char* t = "\xBF";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteSingleIllegalByteFirst)
{
	const char* t = "\xFE";
	
	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteSingleIllegalByteLast)
{
	const char* t = "\xFF";
	
	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteSinglePastLength)
{
	const char* t = "u";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 15, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultiple)
{
	const char* t = "education";

	EXPECT_SEEKEQ("ion", 6, utf8seek(t, t, 6, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultipleEndsInMiddle)
{
	const char* t = "good\0food";

	EXPECT_SEEKEQ("", 4, utf8seek(t, t, 6, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultipleInvalidContinuationByteAndValid)
{
	const char* t = "\x91" "o";
	
	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 2, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultipleValidAndInvalidContinuationByte)
{
	const char* t = "4" "\xA2";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 2, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultipleIllegalAndValid)
{
	const char* t = "\xFE" "d";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 2, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultipleValidAndIllegal)
{
	const char* t = "b" "\xFF";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 2, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultiplePastLength)
{
	const char* t = "phantom";

	EXPECT_SEEKEQ("", 7, utf8seek(t, t, 33, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingle)
{
	const char* t = "\xC2\x86";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleFirst)
{
	const char* t = "\xC2\x80";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleLast)
{
	const char* t = "\xDF\xBF";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSinglePastEnd)
{
	const char* t = "\xC8\x82";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 4, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xC6\x25";

	EXPECT_SEEKEQ("\x25", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xD4\xC5";

	EXPECT_SEEKEQ("\xC5", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleInvalidContinuationPastEnd)
{
	const char* t = "\xC3\x17";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 16, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleMissingOneByte)
{
	const char* t = "\xCD";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleOverlongOneByteFirst)
{
	const char* t = "\xC0\x80";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleOverlongOneByteLast)
{
	const char* t = "\xC1\xBF";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesSingleOverlongPastEnd)
{
	const char* t = "\xC0\x8A";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 13, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesMultiple)
{
	const char* t = "\xC1\xA7\xC0\xB2\xD1\x98";

	EXPECT_SEEKEQ("\xD1\x98", 4, utf8seek(t, t, 2, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesMultipleFromMiddle)
{
	const char* t = "\xC2\x87\xC7\x92\xC8\x88\xD1\xA6";

	EXPECT_SEEKEQ("\xD1\xA6", 6, utf8seek(t + 4, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesMultiplePastEnd)
{
	const char* t = "\xC8\x88\xC9\xA2\xDA\x87\xC2\x87";

	EXPECT_SEEKEQ("", 8, utf8seek(t, t, 8, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesMultipleOverlong)
{
	const char* t = "\xC0\x85\xC1\xA2\xC0\x85\xC0\xA4";

	EXPECT_SEEKEQ("\xC0\x85\xC0\xA4", 4, utf8seek(t, t, 2, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesMultipleOverlongFromMiddle)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF\xC0\xAF";

	EXPECT_SEEKEQ("\xC0\xAF", 6, utf8seek(t + 4, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesMultipleOverlongPastEnd)
{
	const char* t = "\xC1\x97\xC0\xB3\xC0\x91";

	EXPECT_SEEKEQ("", 6, utf8seek(t, t, 26, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingle)
{
	const char* t = "\xE0\x92\x9A";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleFirst)
{
	const char* t = "\xE0\xA0\x80";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleLast)
{
	const char* t = "\xEF\xBF\xBF";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSinglePastEnd)
{
	const char* t = "\xE2\x8A\x94";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 14, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xE1\x3A\x87";

	EXPECT_SEEKEQ("\x3A\x87", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xE2\xC4\x81";

	EXPECT_SEEKEQ("\xC4\x81", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xE0\x82\x16";

	EXPECT_SEEKEQ("\x16", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xE1\xA3\xDF";

	EXPECT_SEEKEQ("\xDF", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationPastEnd)
{
	const char* t = "\xE2\x14\x99";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 4, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleMissingOneByte)
{
	const char* t = "\xE2\x87";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleMissingTwoBytes)
{
	const char* t = "\xEA";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleOverlongOneByteFirst)
{
	const char* t = "\xE0\x80\x80";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleOverlongOneByteLast)
{
	const char* t = "\xE0\x81\xBF";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleOverlongTwoBytesFirst)
{
	const char* t = "\xE0\x82\x80";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleOverlongTwoBytesLast)
{
	const char* t = "\xE0\x9F\xBF";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesSingleOverlongPastEnd)
{
	const char* t = "\xE0\x83\x87";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 21, SEEK_CUR));
}

TEST(Utf8SeekForward, Valid)
{
	const char* t = "\xCF\x84\xE1\xBD\xB4\xCE\xBD \xCE\xBA\xE1\xBD\xB9\xCF\x88\xCE\xB7";

	const char* r = utf8seek(t, t, 4, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xCE\xBA\xE1\xBD\xB9\xCF\x88\xCE\xB7", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x3BA, o);
}

TEST(Utf8SeekForward, StringPastEnd)
{
	const char* t = "\xCE\xBC\xCE\xB5\xCF\x84\xCF\x81\xE1\xBD\xB1\xCE\xB5\xCE\xB9";

	EXPECT_SEEKEQ("", 0, utf8seek(t + 2, t, 18, SEEK_CUR));
}

TEST(Utf8SeekForward, StringAtEnd)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);
	EXPECT_STREQ("", r);
	EXPECT_EQ(t + strlen(t), r);
}

TEST(Utf8SeekForward, StringFromMiddle)
{
	const char* t = "Armageddon";

	const char* r = utf8seek(t + 4, t, 2, SEEK_CUR);
	EXPECT_STREQ("ddon", r);
	EXPECT_EQ(t + 6, r);
}

TEST(Utf8SeekForward, StringEndsInMiddle)
{
	const char* t = "\xD0\xBE\xD0\xBA\0\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t, t, 4, SEEK_CUR);
	EXPECT_STREQ("", r);
	EXPECT_EQ(t + 4, r);
}

TEST(Utf8SeekForward, StringSwappedParameters)
{
	const char* t = "10-12 \xD0\xBC\xD0\xB0\xD1\x80\xD1\x82\xD0\xB0 1997";

	EXPECT_SEEKEQ("10-12 \xD0\xBC\xD0\xB0\xD1\x80\xD1\x82\xD0\xB0 1997", 0, utf8seek(t, t + strlen(t), 6, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x934, o);
}

TEST(Utf8SeekForward, ThreeBytesOverlong)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("\xE0\x9F\xBF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekForward, ThreeBytesOverlongFromMiddle)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t + 6, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekForward, ThreeBytesOverlongPastEnd)
{
	const char* t = "\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 4, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, ThreeBytesLonelyStart)
{
	const char* t = "Rocker\xE4" "coaster";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("oaster", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('o', o);
}

TEST(Utf8SeekForward, ThreeBytesLonelyStartAtEnd)
{
	const char* t = "Submarine\xED";

	const char* r = utf8seek(t, t, 10, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, ThreeBytesNotEnoughData)
{
	const char* t = "Compan\xEF\xBFions";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("ons", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('o', o);
}

TEST(Utf8SeekForward, ThreeBytesNotEnoughDataAtEnd)
{
	const char* t = "BBBark\xEF\xBF";

	const char* r = utf8seek(t, t, 7, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(Utf8SeekForward, FourBytesOverlong)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF4\x90\x80\x80";

	const char* r = utf8seek(t, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x80\x80\xAF\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekForward, FourBytesOverlongFromMiddle)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t + 8, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekForward, FourBytesOverlongPastEnd)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, FourBytesLonelyStart)
{
	const char* t = "Clam\xF4shellpower";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("llpower", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('l', o);
}

TEST(Utf8SeekForward, FourBytesLonelyStartAtEnd)
{
	const char* t = "Magic\xF6";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, FourBytesNotEnoughData)
{
	const char* t = "Brilli\xF0\x90\x80" "ant";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("nt", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(Utf8SeekForward, FourBytesNotEnoughDataAtEnd)
{
	const char* t = "Night\xF0\x90\x80";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, FiveBytesOverlong)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 5, r);
	EXPECT_STREQ("\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekForward, FiveBytesOverlongFromMiddle)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 5, t, 1, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekForward, FiveBytesOverlongPastEnd)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 13, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, FiveBytesLonelyStart)
{
	const char* t = "Beyond\xFAMegalodon";

	const char* r = utf8seek(t, t, 7, SEEK_CUR);

	EXPECT_EQ(t + 7, r);
	EXPECT_STREQ("Megalodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('M', o);
}

TEST(Utf8SeekForward, FiveBytesLonelyStartAtEnd)
{
	const char* t = "Dinos\xFA";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, FiveBytesNotEnoughData)
{
	const char* t = "Central\xF8\x80\x80\x80Plaza baker";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 11, r);
	EXPECT_STREQ("Plaza baker", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('P', o);
}

TEST(Utf8SeekForward, FiveBytesNotEnoughDataAtEnd)
{
	const char* t = "Apple\xF8\x80\x80\x80";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, SixBytesOverlong)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekForward, SixBytesOverlongFromMiddle)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t + 6, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 18, r);
	EXPECT_STREQ("\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekForward, SixBytesOverlongPastEnd)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 81, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, SixBytesLonelyStart)
{
	const char* t = "FC\xFCKnudde festival";

	const char* r = utf8seek(t, t, 8, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("e festival", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('e', o);
}

TEST(Utf8SeekForward, SixBytesLonelyStartAtEnd)
{
	const char* t = "Stuff\xFD";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekForward, SixBytesNotEnoughData)
{
	const char* t = "Hint\xFC\x80\x80\x80\x80machine";

	const char* r = utf8seek(t, t, 6, SEEK_CUR);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("achine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('a', o);
}

TEST(Utf8SeekForward, SixBytesNotEnoughDataAtEnd)
{
	const char* t = "Dull\xFC\x80\x80\x80\x80";

	const char* r = utf8seek(t, t, 5, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}