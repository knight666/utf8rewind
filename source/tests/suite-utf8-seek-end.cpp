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

TEST(Utf8SeekEnd, ThreeBytesSingle)
{
	const char* t = "\xE4\x9A\x85";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleFirst)
{
	const char* t = "\xE0\x80\x80";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleLast)
{
	const char* t = "\xEF\xBF\xBF";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xE8\x2A\x82";

	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xE5\xC4\x8A";

	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xE2\x56\x95";

	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xEA\xC6\x92";

	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleMissingOneByte)
{
	const char* t = "\xE1\x96";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleMissingTwoBytes)
{
	const char* t = "\xEA";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleOverlongOneByte)
{
	const char* t = "\xE9\x9A\x92\x9C";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleOverlongTwoBytes)
{
	const char* t = "\xED\x81\x93\x90\x89";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleOverlongThreeBytes)
{
	const char* t = "\xE7\x9A\x99\x91\x97\x82";

	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesSingleOverlongFourBytes)
{
	const char* t = "\xEE\x97\x9A\x82\x91\xA5\x9A";

	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 5, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesMultiple)
{
	const char* t = "\xE9\x81\x99\xEA\xB7\x92"
					"\xE5\x81\x92";

	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesMultipleInvalid)
{
	const char* t = "\xE2\x91\xEA\xE2\x99\xC5\x9A";

	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, ThreeBytesMultipleOverlong)
{
	const char* t = "\xE4\x9A\x8A\x99\x9A"
					"\xE2\x89\x90\x92"
					"\xEB\x8A\x9B\x99";

	EXPECT_SEEKEQ(t, 12, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 9, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 8, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 6, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 7, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingle)
{
	const char* t = "\xF2\x92\x9A\x99";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleFirst)
{
	const char* t = "\xF0\x80\x80\x80";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleLast)
{
	const char* t = "\xF4\xBF\xBF\xBF";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF3\x24\xA9\xA4";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xF1\xC4\x92\x8A";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xF2\x11\xB3\xB1";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xF3\xC4\x8A\x90";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xF2\x9A\xB1\x41";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xF1\x82\x99\xD2";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleMissingOneByte)
{
	const char* t = "\xF2\x8A\x9B";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleMissingTwoBytes)
{
	const char* t = "\xF3\x8A";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleMissingThreeBytes)
{
	const char* t = "\xF0";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleOverlongOneByte)
{
	const char* t = "\xF2\x8B\x9A\xA2\x9A";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleOverlongTwoBytes)
{
	const char* t = "\xF3\x8A\x9B\xB2\x99\x9B";

	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesSingleOverlongThreeBytes)
{
	const char* t = "\xF0\x8A\x90\x9A\x9B\x99\xB1";

	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesMultiple)
{
	const char* t = "\xF3\x89\x9A\x99\xF2\x89\x9B\xB2"
					"\xF4\x85\x98\x9A";

	EXPECT_SEEKEQ(t, 8, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesMultipleInvalid)
{
	const char* t = "\xF3\x82\xF4\x99\x9A\xF4\x89\x91\xF3\x9A\xB2";

	EXPECT_SEEKEQ(t, 8, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, FourBytesMultipleOverlong)
{
	const char* t = "\xF4\x9A\x99\xA2\x99\x92"
					"\xF2\x9A\x88\x9A\xB2\xB4"
					"\xF0\xB2\xB1\x9C\x95"
					"\xF1\x88\x8A\x9A\xB2\xB0";

	EXPECT_SEEKEQ(t, 22, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 21, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 17, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 16, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 12, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 11, strlen(t), 0, 6, SEEK_END);
	EXPECT_SEEKEQ(t, 10, strlen(t), 0, 7, SEEK_END);
	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 8, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 9, SEEK_END);
	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 10, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 11, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingle)
{
	const char* t = "\xF9\x82\x99\x9A\xA2";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleFirst)
{
	const char* t = "\xF8\x80\x80\x80\x80";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleLast)
{
	const char* t = "\xFB\xBF\xBF\xBF\xBF";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleInvalidContinuationFirstByteLower)
{
	const char* t = "\xF9\x4C\x9A\x9B\x9C";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 5, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleInvalidContinuationFirstByteUpper)
{
	const char* t = "\xFA\xD2\x89\x9B\x90";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 1, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleInvalidContinuationSecondByteLower)
{
	const char* t = "\xFB\x92\x2B\x9A\xB2";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleInvalidContinuationSecondByteUpper)
{
	const char* t = "\xF9\x92\xE5\x99\xA2";

	EXPECT_SEEKEQ(t, 2, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleInvalidContinuationThirdByteLower)
{
	const char* t = "\xFA\x98\x92\x2A\x99";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleInvalidContinuationThirdByteUpper)
{
	const char* t = "\xFB\x86\x88\xC3\x92";

	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleInvalidContinuationFourthByteLower)
{
	const char* t = "\xF9\x92\x87\x96\x3E";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleInvalidContinuationFourthByteUpper)
{
	const char* t = "\xFA\x87\x92\x88\xD4";

	EXPECT_SEEKEQ(t, 4, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleMissingOneByte)
{
	const char* t = "\xFB\x95\x92\x9A";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleMissingTwoBytes)
{
	const char* t = "\xFA\x86\x9B";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleMissingThreeBytes)
{
	const char* t = "\xFB\x95";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleMissingFourBytes)
{
	const char* t = "\xF8";

	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 1, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleOverlongOneByte)
{
	const char* t = "\xF9\x85\x9A\x9B\x9B\xA4";

	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 2, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesSingleOverlongTwoBytes)
{
	const char* t = "\xFA\x95\x96\xA2\x8B\x9A\xB2";

	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 3, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesMultiple)
{
	const char* t = "\xFA\x92\x8B\x9A\x99\xFB\x99\x8A\x9B\x99"
					"\xF9\x9A\x8B\x9A\xAA\xFB\xA5\x9A\xBE\xBA";

	EXPECT_SEEKEQ(t, 15, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 10, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 4, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesMultipleInvalid)
{
	const char* t = "\xFB\x92\x99\xFA\x9B\x9B\xFB\x9A\x9A\x8B\xFA\xF8";

	EXPECT_SEEKEQ(t, 11, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 10, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 3, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 0, strlen(t), 0, 5, SEEK_END);
}

TEST(Utf8SeekEnd, FiveBytesMultipleOverlong)
{
	const char* t = "\xFA\x89\x9A\xB8\x92\x92\x94"
					"\xF8\x9A\x8B\xB7\x88\x85"
					"\xFA\x82\x8B\x99\x98\x9A\x88";

	EXPECT_SEEKEQ(t, 19, strlen(t), 0, 1, SEEK_END);
	EXPECT_SEEKEQ(t, 18, strlen(t), 0, 2, SEEK_END);
	EXPECT_SEEKEQ(t, 13, strlen(t), 0, 3, SEEK_END);
	EXPECT_SEEKEQ(t, 12, strlen(t), 0, 4, SEEK_END);
	EXPECT_SEEKEQ(t, 7, strlen(t), 0, 5, SEEK_END);
	EXPECT_SEEKEQ(t, 6, strlen(t), 0, 6, SEEK_END);
	EXPECT_SEEKEQ(t, 5, strlen(t), 0, 7, SEEK_END);
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