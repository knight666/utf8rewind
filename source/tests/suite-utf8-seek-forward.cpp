#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-seeking.hpp"

TEST(Utf8SeekForward, OneByteSingle)
{
	const char* t = "D";
	
	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleFirst)
{
	const char* t = "\0";

	EXPECT_SEEKEQ2(t, 0, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleLast)
{
	const char* t = "\x7F";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleInvalidContinuationByteFirst)
{
	const char* t = "\x80";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleInvalidContinuationByteLast)
{
	const char* t = "\xBF";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleOverlongOneByte)
{
	const char* t = "\x75\x84";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleOverlongTwoBytes)
{
	const char* t = "\x4A\x9A\x9A";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleOverlongThreeBytes)
{
	const char* t = "\x5F\x84\x92\x9A";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleOverlongFourBytes)
{
	const char* t = "\x6F\x84\x9A\x99\xA9";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleOverlongFiveBytes)
{
	const char* t = "\x54\x85\x94\x91\x9A\x99";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 6, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteSingleOverlongSixBytes)
{
	const char* t = "\x62\x9A\x85\x82\x81\x9A\x9A";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 7, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteMultiple)
{
	const char* t = "education";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, 0, 0, 8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, 0, 0, 9, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteMultipleContinuationBytes)
{
	const char* t = "\x91\x45\x89\x82\xA2";
	
	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
}

TEST(Utf8SeekForward, OneByteMultipleOverlong)
{
	const char* t = "\x25\x88\x9A"
					"\x26\x9A\x99\x9B"
					"\x36\x83\x94";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, 0, 0, 8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, 0, 0, 9, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, 0, 0, 10, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingle)
{
	const char* t = "\xC2\x86";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleFirst)
{
	const char* t = "\xC0\x80";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleLast)
{
	const char* t = "\xDF\xBF";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xC6\x25";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xD4\xC5";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleMissingOneByte)
{
	const char* t = "\xCD";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleOverlongOneByte)
{
	const char* t = "\xC2\xB4\xB2";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleOverlongTwoBytes)
{
	const char* t = "\xCD\xA4\x82\xA9";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleOverlongThreeBytes)
{
	const char* t = "\xC1\x9A\xA8\xB2\x87";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleOverlongFourBytes)
{
	const char* t = "\xD4\xB8\xB9\x92\xA1\xB3";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 5, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesSingleOverlongFiveBytes)
{
	const char* t = "\xD2\xB5\x82\x81\x9A\x99\x82";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 6, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesMultiple)
{
	const char* t = "\xC1\xA7\xC0\xB2\xD1\x98";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesMultipleInvalid)
{
	const char* t = "\x23\xC4\xC5\x80\xD4";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, TwoBytesMultipleOverlong)
{
	const char* t = "\xC2\x85\x99\x9A"
					"\xD1\x8B\x86"
					"\xC4\xB0\xB1\x99";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, 0, 0, 6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 10, 0, 0, 7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 11, 0, 0, 8, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingle)
{
	const char* t = "\xE0\x92\x9A";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleFirst)
{
	const char* t = "\xE0\x80\x80";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleLast)
{
	const char* t = "\xEF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xE1\x3A\x87";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xE2\xC4\x81";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xE0\x82\x16";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xE1\xA3\xDF";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleMissingOneByte)
{
	const char* t = "\xE2\x87";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleMissingTwoBytes)
{
	const char* t = "\xEA";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleOverlongOneByte)
{
	const char* t = "\xE5\x81\x84\x99";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleOverlongTwoBytes)
{
	const char* t = "\xE6\x89\x92\x9A\xA6";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesSingleOverlongThreeBytes)
{
	const char* t = "\xE7\x92\x9A\x88\xA8\x92";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesMultiple)
{
	const char* t = "\xE1\xA5\x98"
					"\xE1\xB0\xB1"
					"\xEA\x8F\x9F";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesMultipleInvalid)
{
	const char* t = "\xE0\xE2\x82\xE3\x87\x17\xE3\xD2";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 5, SEEK_CUR);
}

TEST(Utf8SeekForward, ThreeBytesMultipleOverlong)
{
	const char* t = "\xE4\x82\x99\xB2"
					"\xEF\x87\x94\xB1\x92"
					"\xEA\x81\x82\x83\x84\x85";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 12, 0, 0, 6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 13, 0, 0, 7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 14, 0, 0, 8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 15, 0, 0, 9, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingle)
{
	const char* t = "\xF1\x9A\xB1\xA2";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleFirst)
{
	const char* t = "\xF0\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleLast)
{
	const char* t = "\xF4\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF2\x18\x82\x83";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xF1\xC4\xA5\xB1";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xF3\x81\x49\x81";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xF3\x92\xDF\x92";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xF2\x97\x9A\x34";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xF4\x90\x90\xD3";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleMissingOneByte)
{
	const char* t = "\xF2\x81\xA4";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleMissingTwoBytes)
{
	const char* t = "\xF3\xA5";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesSingleMissingThreeBytes)
{
	const char* t = "\xF0";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesMultiple)
{
	const char* t = "\xF1\x87\xA2\xA2\xF2\x8A\x95\x9A\xF3\x82\x82\x93\xF2\x86\xA3\x94";

	EXPECT_SEEKEQ2(t, 12, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, FourBytesMultipleInvalid)
{
	const char* t = "\xF1\xC3\x82\x25\xF4\x87\xD5\x99\xC3\x81";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingle)
{
	const char* t = "\xF8\x91\xBF\xB2\x93";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleFirst)
{
	const char* t = "\xF8\x80\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleLast)
{
	const char* t = "\xFB\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF8\x17\x81\x9A\x88";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xF9\xD5\xA5\x87\x99";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xFA\xA7\x43\xA6\x86";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xF9\x97\xD6\x81\xB1";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xF8\x92\x87\x51\x97";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xFA\xA7\x96\xC4\x81";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleInvalidContinuationFourthByteLower)
{
	const char* t = "\xF9\xA2\xB4\x85\x26";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleInvalidContinuationFourthByteUpper)
{
	const char* t = "\xFA\xA1\xA2\xA3\xF4";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleMissingOneByte)
{
	const char* t = "\xFA\x80\x81\x82";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleMissingTwoBytes)
{
	const char* t = "\xFB\xB2\xB3";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleMissingThreeBytes)
{
	const char* t = "\xF8\x91";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesSingleMissingFourBytes)
{
	const char* t = "\xF8";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesMultiple)
{
	const char* t = "\xF8\x96\xA2\x88\x92\xFA\x81\x92\xBF\xB2\xF9\xAC\xAE\x8E\x9F";

	EXPECT_SEEKEQ2(t, 10, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, FiveBytesMultipleInvalid)
{
	const char* t = "\xF8\xF9\x82\xFA\x99\x9B\x9C\x15\xF8\x92";

	EXPECT_SEEKEQ2(t, 8, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingle)
{
	const char* t = "\xFC\x88\x9A\x81\x92\x94";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleFirst)
{
	const char* t = "\xFC\x80\x80\x80\x80\x80";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleLast)
{
	const char* t = "\xFD\xBF\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ2(t, 6, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xFC\x37\x81\x92\x8B\xB2";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xFC\xD5\xB4\xB2\x92\x97";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xFC\x93\x5E\xA3\xB1\x86";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xFD\x86\xF5\x87\x89\x9C";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xFC\x95\xA8\x26\x97\xA3";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xFD\x94\x87\xF1\xB1\xAF";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationFourthByteLower)
{
	const char* t = "\xFD\x85\x92\xA2\x34\xB3";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationFourthByteUpper)
{
	const char* t = "\xFC\x84\x92\xA1\xC4\xA2";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationFifthByteLower)
{
	const char* t = "\xFC\x87\x86\x92\x84\x26";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleInvalidContinuationFifthByteUpper)
{
	const char* t = "\xFD\x97\x96\xA5\x88\xD5";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleMissingOneByte)
{
	const char* t = "\xFC\x89\x81\x80\xB2";

	EXPECT_SEEKEQ2(t, 5, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleMissingTwoBytes)
{
	const char* t = "\xFD\xA4\x98\xA2";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleMissingThreeBytes)
{
	const char* t = "\xFC\xAD\xAF";

	EXPECT_SEEKEQ2(t, 3, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleMissingFourBytes)
{
	const char* t = "\xFC\x8A";

	EXPECT_SEEKEQ2(t, 2, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesSingleMissingFiveBytes)
{
	const char* t = "\xFD";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesMultiple)
{
	const char* t = "\xFC\x87\x81\xA2\x92\x90\xFD\x81\x82\xA7\x87\x99"
					"\xFC\x86\x85\x80\x8A\xA7\xFC\x8A\x91\xAA\x82\xB2";

	EXPECT_SEEKEQ2(t, 18, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, SixBytesMultipleInvalid)
{
	const char* t = "\xFD\x81\xFD\x81\x96\x9C\xFC\x9F\x9C\xFD\x88\x81\x99\x15\xFC\x92";

	EXPECT_SEEKEQ2(t, 13, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteSingleFirst)
{
	const char* t = "\xFE";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteSingleLast)
{
	const char* t = "\xFF";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteSingleOverlongOneByte)
{
	const char* t = "\xFE\xA6";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteSingleOverlongTwoBytes)
{
	const char* t = "\xFF\x85\xA7";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteSingleOverlongThreeBytes)
{
	const char* t = "\xFE\x86\xA7\xB0";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteSingleOverlongFourBytes)
{
	const char* t = "\xFF\xA5\x91\xA9\xB3";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteSingleOverlongFiveBytes)
{
	const char* t = "\xFE\x85\x85\x92\xA9\x92";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 6, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteSingleOverlongSixBytes)
{
	const char* t = "\xFF\xA4\x82\x9A\x92\x99\xB2";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 7, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteMultiple)
{
	const char* t = "\xFE\xFF\xFF\xFE\xFE";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
}

TEST(Utf8SeekForward, IllegalByteMultipleOverlong)
{
	const char* t = "\xFE\x82\x92\x9A"
					"\xFF\x85\x82"
					"\xFE\xB4";

	EXPECT_SEEKEQ2(t, 1, 0, 0, 1, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 2, 0, 0, 2, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 3, 0, 0, 3, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 5, 0, 0, 5, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 6, 0, 0, 6, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 7, 0, 0, 7, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 8, 0, 0, 8, SEEK_CUR);
	EXPECT_SEEKEQ2(t, 9, 0, 0, 9, SEEK_CUR);
}

TEST(Utf8SeekForward, StringPastEnd)
{
	const char* t = "\xCE\xBC\xCE\xB5\xCF\x84\xCF\x81\xE1\xBD\xB1\xCE\xB5\xCE\xB9";

	EXPECT_SEEKEQ2(t, 15, 0, 0, 18, SEEK_CUR);
}

TEST(Utf8SeekForward, StringAtEnd)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	EXPECT_SEEKEQ2(t, strlen(t), strlen(t), 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, StringFromMiddle)
{
	const char* t = "Armageddon";

	EXPECT_SEEKEQ2(t, 6, 4, 0, 2, SEEK_CUR);
}

TEST(Utf8SeekForward, StringEndsInMiddle)
{
	const char* t = "\xD0\xBE\xD0\xBA\0\xD0\xB0\xD0\xBB";

	EXPECT_SEEKEQ2(t, 4, 0, 0, 4, SEEK_CUR);
}

TEST(Utf8SeekForward, StringSwappedParameters)
{
	const char* t = "10-12 \xD0\xBC\xD0\xB0\xD1\x80\xD1\x82\xD0\xB0 1997";

	EXPECT_SEEKEQ2(t, 0, 0, strlen(t), 6, SEEK_CUR);
}