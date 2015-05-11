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

TEST(Utf8SeekForward, OneByteMultiple)
{
	const char* t = "education";

	EXPECT_SEEKEQ("ion", 6, utf8seek(t, t, 6, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultipleInvalidContinuationByte)
{
	const char* t = "\x91\x45\x89\x82\xA2";
	
	EXPECT_SEEKEQ("\x82\xA2", 3, utf8seek(t, t, 3, SEEK_CUR));
}

TEST(Utf8SeekForward, OneByteMultipleIllegal)
{
	const char* t = "\xFE\x23\xFF\x60\x55";

	EXPECT_SEEKEQ("\x60\x55", 3, utf8seek(t, t, 3, SEEK_CUR));
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

TEST(Utf8SeekForward, TwoBytesSingleMissingOneByte)
{
	const char* t = "\xCD";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesMultiple)
{
	const char* t = "\xC1\xA7\xC0\xB2\xD1\x98";

	EXPECT_SEEKEQ("\xD1\x98", 4, utf8seek(t, t, 2, SEEK_CUR));
}

TEST(Utf8SeekForward, TwoBytesMultipleInvalid)
{
	const char* t = "\x23\xC4\xC5\x80\xD4";

	EXPECT_SEEKEQ("\xD4", 4, utf8seek(t, t, 3, SEEK_CUR));
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

TEST(Utf8SeekForward, ThreeBytesMultiple)
{
	const char* t = "\xE1\xA5\x98\xE1\xB0\xB1\xEA\x8F\x9F";

	EXPECT_SEEKEQ("\xEA\x8F\x9F", 6, utf8seek(t, t, 2, SEEK_CUR));
}

TEST(Utf8SeekForward, ThreeBytesMultipleInvalid)
{
	const char* t = "\xE0\xE2\x82\xE3\x87\x17\xE3\xD2";

	EXPECT_SEEKEQ("\x17\xE3\xD2", 5, utf8seek(t, t, 3, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingle)
{
	const char* t = "\xF1\x9A\xB1\xA2";

	EXPECT_SEEKEQ("", 4, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleFirst)
{
	const char* t = "\xF0\x90\x80\x80";

	EXPECT_SEEKEQ("", 4, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleLast)
{
	const char* t = "\xF4\x80\x83\xBF";

	EXPECT_SEEKEQ("", 4, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationByteFirstByteLower)
{
	const char* t = "\xF2\x18\x82\x83";

	EXPECT_SEEKEQ("\x18\x82\x83", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationByteFirstByteUpper)
{
	const char* t = "\xF1\xC4\xA5\xB1";

	EXPECT_SEEKEQ("\xC4\xA5\xB1", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationByteSecondByteLower)
{
	const char* t = "\xF3\x81\x49\x81";

	EXPECT_SEEKEQ("\x49\x81", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationByteSecondByteUpper)
{
	const char* t = "\xF3\x92\xDF\x92";

	EXPECT_SEEKEQ("\xDF\x92", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationByteThirdByteLower)
{
	const char* t = "\xF2\x97\x9A\x34";

	EXPECT_SEEKEQ("\x34", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationByteThirdByteUpper)
{
	const char* t = "\xF4\x90\x90\xD3";

	EXPECT_SEEKEQ("\xD3", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleMissingOneByte)
{
	const char* t = "\xF2\x81\xA4";

	EXPECT_SEEKEQ("", 3, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleMissingTwoBytes)
{
	const char* t = "\xF3\xA5";

	EXPECT_SEEKEQ("", 2, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesSingleMissingThreeBytes)
{
	const char* t = "\xF0";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesMultiple)
{
	const char* t = "\xF1\x87\xA2\xA2\xF2\x8A\x95\x9A\xF3\x82\x82\x93\xF2\x86\xA3\x94";

	EXPECT_SEEKEQ("\xF2\x86\xA3\x94", 12, utf8seek(t, t, 3, SEEK_CUR));
}

TEST(Utf8SeekForward, FourBytesMultipleInvalid)
{
	const char* t = "\xF1\xC3\x82\x25\xF4\x87\xD5\x99\xC3\x81";

	EXPECT_SEEKEQ("\xD5\x99\xC3\x81", 6, utf8seek(t, t, 4, SEEK_CUR));
}

TEST(Utf8SeekForward, StringPastEnd)
{
	const char* t = "\xCE\xBC\xCE\xB5\xCF\x84\xCF\x81\xE1\xBD\xB1\xCE\xB5\xCE\xB9";

	EXPECT_SEEKEQ("", 15, utf8seek(t, t, 18, SEEK_CUR));
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