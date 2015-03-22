#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(Utf8ToUtf32, Character)
{
	const char* i = "\xF0\x9F\x98\xA4";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0001F624, o[0]);
}

TEST(Utf8ToUtf32, String)
{
	const char* i = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000091C, o[0]);
	EXPECT_EQ(0x00000921, o[1]);
	EXPECT_EQ(0x00000924, o[2]);
}

TEST(Utf8ToUtf32, StringEndsInMiddle)
{
	const char* i = "How un\0for";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(40, utf8toutf32(i, 10, o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('H', o[0]);
	EXPECT_EQ('o', o[1]);
	EXPECT_EQ('w', o[2]);
	EXPECT_EQ(' ', o[3]);
	EXPECT_EQ('u', o[4]);
	EXPECT_EQ('n', o[5]);
	EXPECT_EQ(0, o[6]);
	EXPECT_EQ('f', o[7]);
	EXPECT_EQ('o', o[8]);
	EXPECT_EQ('r', o[9]);
}

TEST(Utf8ToUtf32, StringDataSizeUnder)
{
	const char* i = "Tree";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, 3, o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('T', o[0]);
	EXPECT_EQ('r', o[1]);
	EXPECT_EQ('e', o[2]);
}

TEST(Utf8ToUtf32, StringDataSizeOver)
{
	const char* i = "Bark";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(32, utf8toutf32(i, 8, o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('B', o[0]);
	EXPECT_EQ('a', o[1]);
	EXPECT_EQ('r', o[2]);
	EXPECT_EQ('k', o[3]);
	EXPECT_EQ(0, o[4]);
}

TEST(Utf8ToUtf32, StringBufferTooSmall)
{
	const char* i = "Ba\xC2\xA2\xC2\xA2y";
	const size_t s = 4;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_EQ('B', o[0]);
	EXPECT_EQ('a', o[1]);
	EXPECT_EQ(0x000000A2, o[2]);
	EXPECT_EQ(0x000000A2, o[3]);
}

TEST(Utf8ToUtf32, Ascii)
{
	const char* i = "\x5F";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000005F, o[0]);
}

TEST(Utf8ToUtf32, AsciiFirst)
{
	const char* i = "\0";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(Utf8ToUtf32, AsciiLast)
{
	const char* i = "\x7F";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000007F, o[0]);
}

TEST(Utf8ToUtf32, AsciiString)
{
	const char* i = "Boat";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('B', o[0]);
	EXPECT_EQ('o', o[1]);
	EXPECT_EQ('a', o[2]);
	EXPECT_EQ('t', o[3]);
}

TEST(Utf8ToUtf32, AsciiInvalid)
{
	const char* i = "\x88";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, AsciiMalformedContinuationByteFirst)
{
	const char* i = "\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, AsciiMalformedContinuationByteLast)
{
	const char* i = "\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, AsciiMalformedContinuationByteCombined)
{
	const char* i =
		"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F" \
		"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F" \
		"\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF" \
		"\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(256, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	for (size_t i = 0; i < 64; ++i)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
	}
}

TEST(Utf8ToUtf32, AsciiIllegalByteFE)
{
	const char* i = "\xFE";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, AsciiIllegalByteFEInString)
{
	const char* i = "gr\xC3\xB6\xC3\x9F" "er\xFE" "en";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(36, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('g', o[0]);
	EXPECT_EQ('r', o[1]);
	EXPECT_EQ(0x000000F6, o[2]);
	EXPECT_EQ(0x000000DF, o[3]);
	EXPECT_EQ('e', o[4]);
	EXPECT_EQ('r', o[5]);
	EXPECT_EQ(0x0000FFFD, o[6]);
	EXPECT_EQ('e', o[7]);
	EXPECT_EQ('n', o[8]);
}

TEST(Utf8ToUtf32, AsciiIllegalByteFF)
{
	const char* i = "\xFF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, AsciiIllegalByteFFInString)
{
	const char* i = "Zw\xC3\xB6l\xFF" "f";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ('Z', o[0]);
	EXPECT_EQ('w', o[1]);
	EXPECT_EQ(0x000000F6, o[2]);
	EXPECT_EQ('l', o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
	EXPECT_EQ('f', o[5]);
}

TEST(Utf8ToUtf32, TwoBytes)
{
	const char* i = "\xC2\xA2";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x000000A2, o[0]);
}

TEST(Utf8ToUtf32, TwoBytesFirst)
{
	const char* i = "\xC2\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x00000080, o[0]);
}

TEST(Utf8ToUtf32, TwoBytesLast)
{
	const char* i = "\xDF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x000007FF, o[0]);
}

TEST(Utf8ToUtf32, TwoBytesOverlong)
{
	const char* i = "\xC0\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, TwoBytesString)
{
	const char* i = "\xDD\xAE\xDE\x8A\xDF\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000076E, o[0]);
	EXPECT_EQ(0x0000078A, o[1]);
	EXPECT_EQ(0x000007C0, o[2]);
}

TEST(Utf8ToUtf32, TwoBytesOverlongNull)
{
	const char* i = "\xC0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, TwoBytesOverlongMaximum)
{
	const char* i = "\xC1\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, TwoBytesLonelyStartFirst)
{
	const char* i = "\xC0 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, TwoBytesLonelyStartLast)
{
	const char* i = "\xDF ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, TwoBytesLonelyStartCombined)
{
	const char* i =
		"\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF "\
		"\xD0 \xD1 \xD2 \xD3 \xD4 \xD5 \xD6 \xD7 \xD8 \xD9 \xDA \xDB \xDC \xDD \xDE \xDF ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(256, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 32; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, TwoBytesNotEnoughData)
{
	const char* i = "\xC2";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytes)
{
	const char* i = "\xE1\x8C\x8A";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000130A, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x00000800, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFF, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesString)
{
	const char* i = "\xE3\x81\x8A\xE3\x81\x8D\xE3\x81\x99\xE3\x81\x88";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000304A, o[0]);
	EXPECT_EQ(0x0000304D, o[1]);
	EXPECT_EQ(0x00003059, o[2]);
	EXPECT_EQ(0x00003048, o[3]);
}

TEST(Utf8ToUtf32, ThreeBytesOverlong)
{
	const char* i = "\xE0\x80\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesOverlongNull)
{
	const char* i = "\xE0\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesOverlongMaximum)
{
	const char* i = "\xE0\x9F\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, ThreeBytesLonelyStartFirst)
{
	const char* i = "\xE0 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, ThreeBytesLonelyStartLast)
{
	const char* i = "\xEF ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, ThreeBytesLonelyStartCombined)
{
	const char* i =
		"\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(128, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 16; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, ThreeBytesNotEnoughData)
{
	const char* i = "\xEF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytes)
{
	const char* i = "\xF0\x9F\x98\x8E";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0001F60E, o[0]);
}

TEST(Utf8ToUtf32, FourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x00010000, o[0]);
}

TEST(Utf8ToUtf32, FourBytesLast)
{
	const char* i = "\xF4\x80\x83\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x001000FF, o[0]);
}

TEST(Utf8ToUtf32, FourBytesString)
{
	const char* i = "\xF0\x90\xB0\x97\xF0\x90\xB0\x8C\xF0\x90\xA8\x95";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x00010C17, o[0]);
	EXPECT_EQ(0x00010C0C, o[1]);
	EXPECT_EQ(0x00010A15, o[2]);
}

TEST(Utf8ToUtf32, FourBytesOverlong)
{
	const char* i = "\xF0\x80\x80\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesOverlongNull)
{
	const char* i = "\xF0\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesOverlongFirst)
{
	const char* i = "\xF4\x90\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesOverlongLast)
{
	const char* i = "\xF7\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesOverlongMaximum)
{
	const char* i = "\xF0\x8F\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FourBytesLonelyStartFirst)
{
	const char* i = "\xF0 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, FourBytesLonelyStartLast)
{
	const char* i = "\xF7 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, FourBytesLonelyStartCombined)
{
	const char* i =
		"\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(64, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 8; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, FourBytesNotEnoughData)
{
	const char* i = "\xF0\x90\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlong)
{
	const char* i = "\xF8\x80\x80\x80\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlongFirst)
{
	const char* i = "\xF8\x88\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlongLast)
{
	const char* i = "\xFB\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlongNull)
{
	const char* i = "\xF8\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesOverlongMaximum)
{
	const char* i = "\xF8\x87\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, FiveBytesLonelyStartFirst)
{
	const char* i = "\xF8 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, FiveBytesLonelyStartLast)
{
	const char* i = "\xFB ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, FiveBytesLonelyStartCombined)
{
	const char* i =
		"\xF8 \xF9 \xFA \xFB ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(32, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 4; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, FiveBytesNotEnoughData)
{
	const char* i = "\xF8\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlong)
{
	const char* i = "\xFC\x80\x80\x80\x80\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlongFirst)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlongLast)
{
	const char* i = "\xFD\xBF\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlongNull)
{
	const char* i = "\xFC\x80\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesOverlongMaximum)
{
	const char* i = "\xFC\x83\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesLonelyStartFirst)
{
	const char* i = "\xFC ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, SixBytesLonelyStartLast)
{
	const char* i = "\xFD ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(Utf8ToUtf32, SixBytesLonelyStartCombined)
{
	const char* i =
		"\xFC \xFD ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);

	for (size_t i = 0; i < 2; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(Utf8ToUtf32, SixBytesNotEnoughData)
{
	const char* i = "\xFC\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SixBytesNotEnoughSpace)
{
	const char* i = "Der\xFD\xBF\xBF\xBF\xBF\xBFp";
	const size_t s = 4;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_EQ('D', o[0]);
	EXPECT_EQ('e', o[1]);
	EXPECT_EQ('r', o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(Utf8ToUtf32, SurrogatePair)
{
	const char* i = "\xED\xAD\x80\xED\xBE\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
}

TEST(Utf8ToUtf32, SurrogatePairCombined)
{
	const char* i = "\
\xED\xA0\x80\xED\xB0\x80\
\xED\xA0\x80\xED\xBF\xBF\
\xED\xAD\xBF\xED\xB0\x80\
\xED\xAD\xBF\xED\xBF\xBF\
\xED\xAE\x80\xED\xB0\x80\
\xED\xAE\x80\xED\xBF\xBF\
\xED\xAF\xBF\xED\xB0\x80\
\xED\xAF\xBF\xED\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(64, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
	EXPECT_EQ(0x0000FFFD, o[5]);
	EXPECT_EQ(0x0000FFFD, o[6]);
	EXPECT_EQ(0x0000FFFD, o[7]);
	EXPECT_EQ(0x0000FFFD, o[8]);
	EXPECT_EQ(0x0000FFFD, o[9]);
	EXPECT_EQ(0x0000FFFD, o[10]);
	EXPECT_EQ(0x0000FFFD, o[11]);
	EXPECT_EQ(0x0000FFFD, o[12]);
	EXPECT_EQ(0x0000FFFD, o[13]);
	EXPECT_EQ(0x0000FFFD, o[14]);
	EXPECT_EQ(0x0000FFFD, o[15]);
}

TEST(Utf8ToUtf32, SurrogatePairHigh)
{
	const char* i = "\xED\xAD\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairHighStart)
{
	const char* i = "\xED\xA0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairHighEnd)
{
	const char* i = "\xED\xAF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairLow)
{
	const char* i = "\xED\xBE\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairLowStart)
{
	const char* i = "\xED\xB0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairLowEnd)
{
	const char* i = "\xED\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairOverlongHighStart)
{
	const char* i = "\xF0\x8D\xA0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairOverlongHighEnd)
{
	const char* i = "\xF0\x8D\xAF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairOverlongLowStart)
{
	const char* i = "\xF0\x8D\xAD\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairOverlongLowEnd)
{
	const char* i = "\xF0\x8D\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, SurrogatePairNotEnoughData)
{
	const char* i = "\xED\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(Utf8ToUtf32, AmountOfBytes)
{
	const char* i = "\xF0\x90\xB0\xAC";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, AmountOfBytesString)
{
	const char* i = "Pchn\xC4\x85\xC4\x87 w t\xC4\x99";
	int32_t errors = 0;

	EXPECT_EQ(44, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, AmountOfBytesNotEnoughData)
{
	const char* i = "\xC8";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, AmountOfBytesOverlong)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, AmountOfBytesNoData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(nullptr, 1, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8ToUtf32, ErrorsIsReset)
{
	const char* i = "\xE0\xA2\xA8";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 8711;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
	EXPECT_EQ(0x000008A8, o[0]);
}

TEST(Utf8ToUtf32, OverlappingParametersFits)
{
	int32_t errors = 0;

	char data[128] = { 0 };
	strcpy(data, "silence");

	const char* i = data;
	size_t is = 7;
	unicode_t* o = (unicode_t*)(data + 7);
	size_t os = 28;

	EXPECT_EQ(28, utf8toutf32(i, is, o, os, &errors));
	EXPECT_MEMEQ("silences\0\0\0i\0\0\0l\0\0\0e\0\0\0n\0\0\0c\0\0\0e\0\0\0", data, 33);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersStartsEqual)
{
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data + 16;
	size_t is = 24;
	unicode_t* o = (unicode_t*)(data + 16);
	size_t os = 18;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersEndsEqual)
{
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data + 40;
	size_t is = 20;
	unicode_t* o = (unicode_t*)(data + 19);
	size_t os = 41;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersInputStartsInTarget)
{
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data + 17;
	size_t is = 51;
	unicode_t* o = (unicode_t*)(data + 2);
	size_t os = 27;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersInputEndsInTarget)
{
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data + 21;
	size_t is = 45;
	unicode_t* o = (unicode_t*)(data + 2);
	size_t os = 100;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersInputInsideTarget)
{
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data + 50;
	size_t is = 11;
	unicode_t* o = (unicode_t*)(data + 41);
	size_t os = 38;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersTargetStartsInInput)
{
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data + 14;
	size_t is = 34;
	unicode_t* o = (unicode_t*)(data + 21);
	size_t os = 34;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersTargetEndsInInput)
{
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data + 60;
	size_t is = 31;
	unicode_t* o = (unicode_t*)(data + 14);
	size_t os = 71;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST(Utf8ToUtf32, OverlappingParametersTargetInsideInput)
{
	int32_t errors = 0;

	char data[128] = { 0 };

	const char* i = data + 2;
	size_t is = 100;
	unicode_t* o = (unicode_t*)(data + 41);
	size_t os = 12;

	EXPECT_EQ(0, utf8toutf32(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}