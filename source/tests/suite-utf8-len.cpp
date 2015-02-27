#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(Utf8Len, Valid)
{
	const char* c = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";

	EXPECT_EQ(3, utf8len(c));
}

TEST(Utf8Len, EndsInMiddle)
{
	const char* c = "Forest\0dweller";

	EXPECT_EQ(6, utf8len(c));
}

TEST(Utf8Len, OneByteCodepoint)
{
	const char* c = "\x30";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, TwoByteCodepoint)
{
	const char* c = "\xC4\xB3";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, TwoByteCodepointLonelyStart)
{
	const char* c = "\xDA";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, TwoByteCodepointLonelyStartAll)
{
	const char* c =
		"\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF "\
		"\xD0 \xD1 \xD2 \xD3 \xD4 \xD5 \xD6 \xD7 \xD8 \xD9 \xDA \xDB \xDC \xDD \xDE \xDF ";

	EXPECT_EQ(64, utf8len(c));
}

TEST(Utf8Len, TwoByteCodepointNotEnoughData)
{
	const char* c = "\xCC";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, TwoByteCodepointOverlong)
{
	const char* c = "\xC0\xAF";

	EXPECT_EQ(1, utf8len(c));
}

TEST(Utf8Len, TwoByteCodepointOverlongNotEnoughData)
{
	const char* c = "\xC0";

	EXPECT_EQ(1, utf8len(c));
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

TEST(Utf8Len, ZeroUtf8Len)
{
	const char* c = "";

	EXPECT_EQ(0, utf8len(c));
}

TEST(Utf8Len, NoData)
{
	EXPECT_EQ(0, utf8len(nullptr));
}