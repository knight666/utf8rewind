#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-seeking.hpp"

TEST(Utf8SeekSet, OneByteSingle)
{
	const char* t = "v";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, OneByteSingleFirst)
{
	const char* t = "\0";

	EXPECT_SEEKEQ2(t, 0, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, OneByteSingleLast)
{
	const char* t = "\x7F";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, OneByteSingleInvalidContinuationByteFirst)
{
	const char* t = "\x80";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, OneByteSingleInvalidContinuationByteLast)
{
	const char* t = "\xBF";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, OneByteSingleIllegalByteFirst)
{
	const char* t = "\xFE";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, OneByteSingleIllegalByteLast)
{
	const char* t = "\xFF";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, OneByteMultiple)
{
	const char* t = "Miner";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_SET);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_SET);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_SET);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_SET);
}

TEST(Utf8SeekSet, OneByteMultipleInvalid)
{
	const char* t = "\x81\x99\xAD\x92\xBF\xFF\xB3";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_SET);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_SET);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_SET);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_SET);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 6, SEEK_SET);
}

TEST(Utf8SeekSet, TwoBytesSingle)
{
	const char* t = "\xC7\x88";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, TwoBytesSingleFirst)
{
	const char* t = "\xC0\x80";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, TwoBytesSingleLast)
{
	const char* t = "\xDF\xBF";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, TwoBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xD4\x35";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_SET);
}

TEST(Utf8SeekSet, TwoBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xD0\xC4";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_SET);
}

TEST(Utf8SeekSet, TwoBytesSingleMissingOneByte)
{
	const char* t = "\xC6";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, TwoBytesMultiple)
{
	const char* t = "\xC7\x81\xC4\xB0\xC2\xA5\xC4\x87";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 3, SEEK_SET);
}

TEST(Utf8SeekSet, TwoBytesMultipleInvalid)
{
	const char* t = "\xC6\xD4\xD1\x15";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_SET);
}

TEST(Utf8SeekSet, ThreeBytesSingle)
{
	const char* t = "\xE5\xB8\x84";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesSingleFirst)
{
	const char* t = "\xE0\x80\x80";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesSingleLast)
{
	const char* t = "\xEF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xE5\x3A\x9A";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xEA\xC4\x81";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xE8\x81\x24";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xE1\x94\xD4";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesSingleMissingOneByte)
{
	const char* t = "\xE2\x90";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesSingleMissingTwoBytes)
{
	const char* t = "\xEB";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesMultiple)
{
	const char* t = "\xE5\x8A\x99\xE7\xBA\x8A\xE9\x9A\x9A\xE2\xB8\x87";

	EXPECT_SEEKEQ2(t, 9, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekSet, ThreeBytesMultipleInvalid)
{
	const char* t = "\xE6\x82\xE3\xA2\xE4\xB2\x15\xE2\xE2";

	EXPECT_SEEKEQ2(t, 8, 0, 0, 5, SEEK_CUR);
}

TEST(Utf8SeekSet, FourBytesSingle)
{
	const char* t = "\xF2\x87\x92\xA2";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleFirst)
{
	const char* t = "\xF0\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleLast)
{
	const char* t = "\xF4\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF3\x25\xA3\x87";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xF2\xC4\xB7\x81";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xF2\x86\x4A\x92";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xF4\x97\xF3\x8A";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xF3\xA3\x94\x1A";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xF2\x97\x87\xD4";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleMissingOneByte)
{
	const char* t = "\xF3\x81\x92";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleMissingTwoBytes)
{
	const char* t = "\xF2\xB1";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesSingleMissingThreeBytes)
{
	const char* t = "\xF4";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesMultiple)
{
	const char* t = "\xF3\x82\x99\xA2\xF4\x82\x92\xA1"
					"\xF2\x99\x82\xAA\xF1\x89\x90\x91"
					"\xF4\xAC\x8C\x9A";

	EXPECT_SEEKEQ2(t, 12, 0, 0, 3, SEEK_SET);
}

TEST(Utf8SeekSet, FourBytesMultipleInvalid)
{
	const char* t = "\xF2\xA8\xA9\xF3\xF4\xA2\x5C\x92\xF1";

	EXPECT_SEEKEQ2(t, 7, 0, 0, 4, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingle)
{
	const char* t = "\xF8\x92\xA8\x82\xA6";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleFirst)
{
	const char* t = "\xF8\x80\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleLast)
{
	const char* t = "\xFB\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF9\x26\xA2\x82\x92";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xFA\xD6\x82\x99\xA2";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xFB\x86\x36\x8A\x9A";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xFC\x87\xD5\x99\xA2";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xFD\x9A\x99\x41\xA8";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xFB\x88\x92\xF5\x88";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleInvalidContinuationFourthByteLower)
{
	const char* t = "\xFC\xA6\x9A\xA2\x16";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleInvalidContinuationFourthByteUpper)
{
	const char* t = "\xFA\x9A\x95\x94\xC3";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleMissingOneByte)
{
	const char* t = "\xF8\xA8\x92\x99";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleMissingTwoBytes)
{
	const char* t = "\xF9\x9A\x92";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleMissingThreeBytes)
{
	const char* t = "\xFA\xA6";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesSingleMissingFourBytes)
{
	const char* t = "\xF9";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesMultiple)
{
	const char* t = "\xF8\xA4\x92\xB2\x99\xFA\x84\x92\xA9\x94"
					"\xF9\xA7\xA5\x92\xB2\xFC\x8D\xA2\x92\xA4"
					"\xF8\xB5\x88\x88\x88";

	EXPECT_SEEKEQ2(t, 20, 0, 0, 4, SEEK_SET);
}

TEST(Utf8SeekSet, FiveBytesMultipleInvalid)
{
	const char* t = "\xF8\x9A\x92\xF8\xA8\x51\xFB\x88\x88\xF2";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 3, SEEK_SET);
}

TEST(Utf8SeekSet, SixBytesSingle)
{
	const char* t = "\xFD\x87\x99\x81\x99\x92";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleFirst)
{
	const char* t = "\xFC\x80\x80\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleLast)
{
	const char* t = "\xFD\xBF\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xFC\x12\x9A\x82\x8A\x99";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xFD\xD4\x85\xA5\x9A\xA9";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xFD\x87\x4A\x92\x92\xA9";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xFC\x80\xD6\x89\x91\x92";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xFC\x87\x82\x54\x82\x9B";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xFD\x97\x96\xF2\x81\x92";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationFourthByteLower)
{
	const char* t = "\xFC\x95\x90\x85\x53\x82";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationFourthByteUpper)
{
	const char* t = "\xFD\x84\x83\x84\xC4\x88";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationFifthByteLower)
{
	const char* t = "\xFC\xA5\xA2\x88\x92\x50";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleInvalidContinuationFifthByteUpper)
{
	const char* t = "\xFD\x85\x81\x80\x80\xD4";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleMissingOneByte)
{
	const char* t = "\xFC\x82\x89\x9A\x84";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleMissingTwoBytes)
{
	const char* t = "\xFD\xA0\xA1\xA2";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleMissingThreeBytes)
{
	const char* t = "\xFC\x95\x82";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleMissingFourBytes)
{
	const char* t = "\xFD\x88";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesSingleMissingFiveBytes)
{
	const char* t = "\xFC";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesMultiple)
{
	const char* t = "\xFC\x87\x97\xA2\x9A\x82\xFD\x88\x81\x99\x92\x94"
					"\xFD\xA6\xA5\x88\x92\x9A\xFC\x99\x9A\x9B\x9C\x9D"
					"\xFC\xA7\xA8\x95\x82\x99";

	EXPECT_SEEKEQ2(t, 18, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekSet, SixBytesMultipleInvalid)
{
	const char* t = "\xFD\x88\x88\x15\xFD\x9A\x91\x88\xFD\xFC\x15";

	EXPECT_SEEKEQ2(t, 10, 0, 0, 5, SEEK_CUR);
}

TEST(Utf8SeekSet, StringPastEnd)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	EXPECT_SEEKEQ2(t, 22, 0, 0, 33, SEEK_SET);
}

TEST(Utf8SeekSet, StringAtEnd)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 2, SEEK_SET);
}

TEST(Utf8SeekSet, StringFromMiddle)
{
	const char* t = "The Doctor";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_SET);
}

TEST(Utf8SeekSet, StringEndsInMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\0\xE0\xA4\xB4\xE0\xA4\xBD";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 5, SEEK_SET);
}

TEST(Utf8SeekSet, StringZeroOffset)
{
	const char* t = "Magic powered";

	EXPECT_SEEKEQ2(t, 0, 0, 0, 0, SEEK_SET);
}

TEST(Utf8SeekSet, StringNegativeOffset)
{
	const char* t = "Dreaming";

	EXPECT_SEEKEQ2(t, 0, 0, 0, -12, SEEK_SET);
}

TEST(Utf8SeekSet, StringSwappedParameters)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	EXPECT_SEEKEQ2(t, 0, 0, strlen(t), 3, SEEK_SET);
}

TEST(Utf8SeekSet, StringEmpty)
{
	const char* t = "";

	EXPECT_SEEKEQ2(t, 0, 0, 0, 3, SEEK_SET);
}