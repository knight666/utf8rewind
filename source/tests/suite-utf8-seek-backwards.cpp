#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-seeking.hpp"

TEST(Utf8SeekBackwards, OneByteSingle)
{
	const char* t = "*";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleFirst)
{
	const char* t = "\0";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleLast)
{
	const char* t = "\x7F";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleInvalidContinuationByteFirst)
{
	const char* t = "\x80";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleInvalidContinuationByteLast)
{
	const char* t = "\xBF";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleOverlongOneByte)
{
	const char* t = "\x17\x87";

	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleOverlongTwoBytes)
{
	const char* t = "\x56\x92\x92";

	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleOverlongThreeBytes)
{
	const char* t = "\x6A\xA2\x95\x86";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleOverlongFourBytes)
{
	const char* t = "\x71\x8A\x92\xA9\xB1";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleOverlongFiveBytes)
{
	const char* t = "\x25\x82\x83\x9A\x9A\x8B";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleOverlongSixBytes)
{
	const char* t = "\x33\x84\x82\xA9\xB2\x9A\x86";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -7, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleIllegalByteFirst)
{
	const char* t = "\xFE";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteSingleIllegalByteLast)
{
	const char* t = "\xFF";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteMultiple)
{
	const char* t = "Heartbreaker";

	EXPECT_SEEKEQ2(t, 11, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -9, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -10, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -11, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -12, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteMultipleInvalid)
{
	const char* t = "\x98\x92\xB4\x81\xA5\x88\x93";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -7, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteMultipleOverlong)
{
	const char* t = "\x27\x99\xA9"
					"\x4A\x82\xA3\x92"
					"\x65\x94\x83"
					"\x24\xA4\x99\x9A\x82";

	EXPECT_SEEKEQ2(t, 14, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 13, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 12, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 11, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, strlen(t), 0, -6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, strlen(t), 0, -7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, strlen(t), 0, -8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -9, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -10, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -11, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -12, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -13, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -14, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -15, SEEK_CUR);
}

TEST(Utf8SeekBackwards, OneByteMultipleIllegal)
{
	const char* t = "\xFE\xFE\xFF\xFE\xFE\xFF";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingle)
{
	const char* t = "\xC4\x82";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleFirst)
{
	const char* t = "\xC0\x80";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleLast)
{
	const char* t = "\xDF\xBF";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xC8\x4A";

	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xD0\xC3";

	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleMissingOneByte)
{
	const char* t = "\xCC";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleOverlongOneByte)
{
	const char* t = "\xC2\x86\x82";

	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleOverlongTwoBytes)
{
	const char* t = "\xD1\x87\xA9\x92";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleOverlongThreeBytes)
{
	const char* t = "\xD8\x92\xA9\xB1\xA6";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleOverlongFourBytes)
{
	const char* t = "\xCA\x9A\x82\xB5\xA0\xA2";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesSingleOverlongFiveBytes)
{
	const char* t = "\xC9\xA1\xB1\x84\x92\xA1\xA0";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesMultiple)
{
	const char* t = "\xC4\x97\xD5\x99\xCA\xAA\xCD\xB2";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesMultipleInvalid)
{
	const char* t = "\xC3\xD4\xC9\x15\xD5\xD1";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, TwoBytesMultipleOverlong)
{
	const char* t = "\xC8\x9A\x99\xD2\xA4\xA1\xA9\x99\x99\xD2\x92\xA9";

	EXPECT_SEEKEQ2(t, 11, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -9, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingle)
{
	const char* t = "\xE2\x86\xB9";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleFirst)
{
	const char* t = "\xE0\x80\x80";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleLast)
{
	const char* t = "\xEF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xE2\x5A\x9A";

	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xEA\xD1\x8B";

	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xED\x8B\x2F";

	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xE2\x91\xD5";

	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleMissingOneByte)
{
	const char* t = "\xEE\x8A";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleMissingTwoBytes)
{
	const char* t = "\xE5";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleOverlongOneByte)
{
	const char* t = "\xE8\x98\x9A\x86";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleOverlongTwoBytes)
{
	const char* t = "\xEA\xA7\x91\x92\xA1";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleOverlongThreeBytes)
{
	const char* t = "\xE9\x81\x91\x91\x99\xA9";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesSingleOverlongFourBytes)
{
	const char* t = "\xE1\xA9\x82\x81\x80\x92\x99";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesMultiple)
{
	const char* t = "\xE2\x87\x99\xE4\x9A\xAB\xE3\xBD\xBD"
					"\xE5\x99\xA8\xE7\xB1\x8A";

	EXPECT_SEEKEQ2(t, 12, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesMultipleInvalid)
{
	const char* t = "\xE8\x92\xEF\x16\xE4\x92\xE0";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, ThreeBytesMultipleOverlong)
{
	const char* t = "\xE3\x97\xA9\x88\x81\x92"
					"\xE4\x81\x9A\x91\x99"
					"\xE4\x88\xA4\xB4\x81"
					"\xE5\x8A\x8A\x81\x99\x9A\x9B"
					"\xE2\x96\x99\x99";

	EXPECT_SEEKEQ2(t, 26, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 23, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 22, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 21, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 20, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 19, strlen(t), 0, -6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 16, strlen(t), 0, -7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 15, strlen(t), 0, -8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 14, strlen(t), 0, -9, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 11, strlen(t), 0, -10, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, strlen(t), 0, -11, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, strlen(t), 0, -12, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -13, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -14, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -15, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -16, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -17, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingle)
{
	const char* t = "\xF1\xB2\x91\x81";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleFirst)
{
	const char* t = "\xF0\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleLast)
{
	const char* t = "\xF4\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF3\x17\x99\xA8";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xF1\xD5\xB2\x8B";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xF1\x85\x26\xB2";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xF4\x87\xE1\x88";

	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xF2\x9A\x81\x31";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xF2\x82\x92\xF5";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleMissingOneByte)
{
	const char* t = "\xF4\x87\x92";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleMissingTwoByte)
{
	const char* t = "\xF2\x8A";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleMissingThreeBytes)
{
	const char* t = "\xF0";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleOverlongOneByte)
{
	const char* t = "\xF4\xB1\xAF\xAF\x9A";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleOverlongTwoBytes)
{
	const char* t = "\xF7\xA8\x91\x99\x92\xB2";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesSingleOverlongThreeBytes)
{
	const char* t = "\xF0\xB7\xA8\x81\x99\x9A\xA2";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesMultiple)
{
	const char* t = "\xF2\x8A\x91\x99\xF4\x8A\x92\x90\xF1\x92\xA9\x92"
					"\xF4\xB4\xA4\x85\xF2\x95\x85\x86";

	EXPECT_SEEKEQ2(t, 16, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 12, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesMultipleInvalid)
{
	const char* t = "\xF2\x87\xF9\xF4\x87\xF1\xF2\x81\x12";

	EXPECT_SEEKEQ2(t, 8, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FourBytesMultipleOverlong)
{
	const char* t = "\xF3\x81\x81\x84\x84\x87"
					"\xF4\x91\x95\xA8\x99\x99\xA1"
					"\xF2\x87\x86\x9A\xA2\xA0"
					"\xF1\x92\x90\xA9\xA9\x81\x92"
					"\xF2\x81\x92\x92\x92";

	EXPECT_SEEKEQ2(t, 30, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 26, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 25, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 24, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 23, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 19, strlen(t), 0, -6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 18, strlen(t), 0, -7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 17, strlen(t), 0, -8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 13, strlen(t), 0, -9, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 12, strlen(t), 0, -10, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 11, strlen(t), 0, -11, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, strlen(t), 0, -12, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -13, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -14, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -15, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -16, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingle)
{
	const char* t = "\xF9\x81\x92\x9A\x99";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleFirst)
{
	const char* t = "\xF8\x80\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleLast)
{
	const char* t = "\xFB\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF9\x25\x82\x92\xA2";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xFA\xD4\x97\x81\x83";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xFB\x98\x2A\xA9\xB1";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xF8\x84\xE3\x9A\x8A";

	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xFA\x88\x92\x17\x9A";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xF9\xA8\x8A\xC4\x8A";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleInvalidContinuationFourthByteLower)
{
	const char* t = "\xFA\xAB\xBB\x9A\x26";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleInvalidContinuationFourthByteUpper)
{
	const char* t = "\xF8\xA6\xA4\x8A\xE5";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleMissingOneByte)
{
	const char* t = "\xF9\xA5\x82\x99";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleMissingTwoBytes)
{
	const char* t = "\xFA\xB2\xA8";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleMissingThreeBytes)
{
	const char* t = "\xF8\xA2";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleMissingFourBytes)
{
	const char* t = "\xF8";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleOverlongOneByte)
{
	const char* t = "\xFA\x82\x9A\x99\x9A\x92";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesSingleOverlongTwoBytes)
{
	const char* t = "\xF8\x8A\x92\xA9\x99\xA2\x82";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesMultiple)
{
	const char* t = "\xF8\xA2\x88\x8A\x91\xFB\x92\xA9\xB9\x92"
					"\xFB\x92\xB4\xBA\x9A\xFA\xA4\xA1\x9A\x82";

	EXPECT_SEEKEQ2(t, 15, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesMultipleInvalid)
{
	const char* t = "\xF8\xA2\xFB\x9A\xA9\xFB\x84\x85\xFA\x99\xFB\xF8";

	EXPECT_SEEKEQ2(t, 11, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, FiveBytesMultipleOverlong)
{
	const char* t = "\xF8\x9A\x9A\x9A\x9A\x89\xFA\x8B\x8C\x8A\x82\x9A"
					"\xFB\x8A\x9A\x99\xB1\xB2";

	EXPECT_SEEKEQ2(t, 17, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 12, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 11, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingle)
{
	const char* t = "\xFC\x88\x9A\x81\x92\x94";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleFirst)
{
	const char* t = "\xFC\x80\x80\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleLast)
{
	const char* t = "\xFD\xBF\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xFC\x16\xA8\x92\xA9\x99";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xFD\xD4\x87\x9A\x92\xA8";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xFC\x85\x26\x91\xA9\x9A";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -5, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xFD\x81\xE2\x9A\x99\xAB";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xFC\xA8\x9B\x1F\x92\xA2";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -4, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xFD\x97\xA2\xF2\x9A\xB4";

	EXPECT_SEEKEQ2(t, 3, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationFourthByteLower)
{
	const char* t = "\xFD\x98\x9A\x91\x72\x81";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationFourthByteUpper)
{
	const char* t = "\xFC\xA1\xA4\xA3\xC5\x90";

	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationFifthByteLower)
{
	const char* t = "\xFC\x91\x82\x9A\x92\x22";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleInvalidContinuationFifthByteUpper)
{
	const char* t = "\xFC\x9A\x9F\x8A\x82\xDE";

	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleMissingOneByte)
{
	const char* t = "\xFD\x98\x92\x9A\x81";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleMissingTwoBytes)
{
	const char* t = "\xFC\x92\x92\x92";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleMissingThreeBytes)
{
	const char* t = "\xFD\x81\x9B";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleMissingFourBytes)
{
	const char* t = "\xFC\xB2";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleMissingFiveBytes)
{
	const char* t = "\xFD";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -1, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesSingleOverlongOneByte)
{
	const char* t = "\xFD\x81\x92\x99\xA9\xAB\x87";

	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesMultiple)
{
	const char* t = "\xFD\x87\x91\x81\xB2\xB1\xFC\xBF\xB2\xBA\xB0\xA2"
					"\xFC\x91\x81\x92\xA2\xA2";

	EXPECT_SEEKEQ2(t, 12, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -3, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesMultipleInvalid)
{
	const char* t = "\xFC\x98\xA2\x90\x12\xFD\x92\xFC\xFD\x81\x92\x15";

	EXPECT_SEEKEQ2(t, 11, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, SixBytesMultipleOverlong)
{
	const char* t = "\xFC\x82\x81\x99\xA9\x92\x92"
					"\xFD\x86\x92\x9A\xA9\x82\x81"
					"\xFC\x86\xA8\x92\x93\x94\xAB";

	EXPECT_SEEKEQ2(t, 20, strlen(t), 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 14, strlen(t), 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 13, strlen(t), 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, strlen(t), 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, strlen(t), 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, StringPastStart)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	EXPECT_SEEKEQ2(t, 0, strlen(t), 0, -26, SEEK_CUR);
}

TEST(Utf8SeekBackwards, StringAtStart)
{
	const char* t = "\xD7\x8A\x99" "Boo";

	EXPECT_SEEKEQ2(t, 0, 0, 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, StringFromMiddle)
{
	const char* t = "Banananana";

	EXPECT_SEEKEQ2(t, 5, 6, 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 6, 0, -2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 6, 0, -3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 6, 0, -4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 1, 6, 0, -5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 0, 6, 0, -6, SEEK_CUR);
}

TEST(Utf8SeekBackwards, StringEndsInMiddle)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\0\xFB\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 11, 16, 0, -1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, 16, 0, -2, SEEK_CUR);
}

TEST(Utf8SeekBackwards, StringSwappedParameters)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 0, 0, strlen(t), -2, SEEK_CUR);
}