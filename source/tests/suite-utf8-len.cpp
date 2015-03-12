#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(Utf8Len, Valid)
{
	EXPECT_EQ(3, utf8len("\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4"));
}

TEST(Utf8Len, StringEndsAtStart)
{
	EXPECT_EQ(0, utf8len("\0Spaceship"));
}

TEST(Utf8Len, StringEndsInMiddle)
{
	EXPECT_EQ(6, utf8len("Forest\0dweller"));
}

TEST(Utf8Len, OneByteSingle)
{
	EXPECT_EQ(1, utf8len("\x30"));
}

TEST(Utf8Len, OneByteSingleInvalidContinuation)
{
	EXPECT_EQ(1, utf8len("\x81"));
}

TEST(Utf8Len, OneByteSingleIllegal)
{
	EXPECT_EQ(1, utf8len("\xFE"));
}

TEST(Utf8Len, OneByteMultiple)
{
	EXPECT_EQ(5, utf8len("%@#!&"));
}

TEST(Utf8Len, OneByteMultipleInvalidContinuation)
{
	EXPECT_EQ(3, utf8len("\x84\x9A\xB8"));
}

TEST(Utf8Len, OneByteMultipleIllegal)
{
	EXPECT_EQ(4, utf8len("\xFF\xFE\xFF\xFE"));
}

TEST(Utf8Len, TwoBytesSingle)
{
	EXPECT_EQ(1, utf8len("\xC4\xB3"));
}

TEST(Utf8Len, TwoBytesSingleNotEnoughDataOneByte)
{
	EXPECT_EQ(1, utf8len("\xDA"));
}

TEST(Utf8Len, TwoBytesSingleInvalidContinuation)
{
	EXPECT_EQ(2, utf8len("\xC8\x7F"));
}

TEST(Utf8Len, TwoByteSingleOverlong)
{
	EXPECT_EQ(1, utf8len("\xC0\xAF"));
}

TEST(Utf8Len, TwoByteSingleOverlongNotEnoughData)
{
	EXPECT_EQ(1, utf8len("\xC1"));
}

TEST(Utf8Len, TwoBytesMultiple)
{
	EXPECT_EQ(3, utf8len("\xC2\xA7\xC5\xBC\xC2\xA9"));
}

TEST(Utf8Len, TwoBytesMultipleNotEnoughData)
{
	EXPECT_EQ(4, utf8len("\xC0\xDA\xCB\xDE"));
}

TEST(Utf8Len, TwoBytesMultipleInvalidContinuation)
{
	EXPECT_EQ(6, utf8len("\xDB\x6A\xDC\x12\xDE\x70"));
}

TEST(Utf8Len, TwoBytesMultipleOverlong)
{
	EXPECT_EQ(3, utf8len("\xC0\x9A\xC0\xA0\xC1\x80"));
}

TEST(Utf8Len, TwoBytesMultipleOverlongNotEnoughData)
{
	EXPECT_EQ(3, utf8len("\xC0\xC1\xC0"));
}

TEST(Utf8Len, ThreeByteCodepoint)
{
	const char* c = "\xE0\xAA\xBE";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, ThreeByteCodepointLonelyStart)
{
	const char* c = "\xE7";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, ThreeByteCodepointLonelyStartALl)
{
	const char* c =
		"\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF ";

	EXPECT_EQ(32, utf8len(c));
}

TEST(Utf8Len, ThreeByteCodepointNotEnoughData)
{
	const char* c = "\xE1\xB5";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, ThreeByteCodepointOverlong)
{
	const char* c = "\xE0\x80\xAF";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, ThreeByteCodepointOverlongNotEnoughData)
{
	const char* c = "\xE0\x80";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, FourByteCodepoint)
{
	const char* c = "\xF0\x90\x86\x84";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, FourByteCodepointLonelyStart)
{
	const char* c = "\xF2";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, FourByteCodepointLonelyStartAll)
{
	const char* c = "\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ";

	EXPECT_EQ(16, utf8len(c));
}

TEST(Utf8Len, FourByteCodepointNotEnoughData)
{
	const char* c = "\xF0\xDA";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, FourByteCodepointOverlong)
{
	const char* c = "\xF0\x8F\xBF\xBF";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, FourByteCodepointOverlongNotEnoughData)
{
	const char* c = "\xF0\x8F\xBF";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, FiveByteCodepointLonelyStart)
{
	const char* c = "\xF9";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, FiveByteCodepointLonelyStartAll)
{
	const char* c = "\xF8 \xF9 \xFA \xFB ";

	EXPECT_EQ(8, utf8len(c));
}

TEST(Utf8Len, FiveByteCodepointOverlong)
{
	const char* c = "\xF8\x87\xBF\xBF\xBF";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, FiveByteCodepointOverlongNotEnoughData)
{
	const char* c = "\xF8\x87\xBF\xBF";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, SixByteCodepointLonelyStart)
{
	const char* c = "\xFC";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, SixByteCodepointLonelyStartAll)
{
	const char* c = "\xFC \xFD ";

	EXPECT_EQ(4, utf8len(c));
}

TEST(Utf8Len, SixByteCodepointOverlong)
{
	const char* c = "\xFC\x83\xBF\xBF\xBF\xBF";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, SixByteCodepointOverlongNotEnoughData)
{
	const char* c = "\xFC\x83\xBF\xBF\xBF";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, IllegalByteFE)
{
	const char* c = "\xFE";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, IllegalByteFEInString)
{
	const char* c = "gr\xC3\xB6\xC3\x9F" "er\xFE" "en";

	EXPECT_EQ(9, utf8len(c));
}

TEST(Utf8Len, IllegalByteFF)
{
	const char* c = "\xFE";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, IllegalByteFFInString)
{
	const char* c = "Zw\xC3\xB6l\xFF" "f";

	EXPECT_EQ(6, utf8len(c));
}

TEST(Utf8Len, ZeroLength)
{
	const char* c = "";

	EXPECT_EQ(0, utf8len(c));
}

TEST(Utf8Len, NoData)
{
	EXPECT_EQ(0, utf8len(nullptr));
}