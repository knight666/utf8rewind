#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(DecodeUtf32, Character)
{
	const char* i = "\xF0\x9F\x98\xA4";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0001F624, o[0]);
}

TEST(DecodeUtf32, String)
{
	const char* i = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000091C, o[0]);
	EXPECT_EQ(0x00000921, o[1]);
	EXPECT_EQ(0x00000924, o[2]);
}

TEST(DecodeUtf32, StringEndsInMiddle)
{
	const char* i = "How un\0fortunate";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, 16, o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('H', o[0]);
	EXPECT_EQ('o', o[1]);
	EXPECT_EQ('w', o[2]);
	EXPECT_EQ(' ', o[3]);
	EXPECT_EQ('u', o[4]);
	EXPECT_EQ('n', o[5]);
}

TEST(DecodeUtf32, StringDataSizeUnder)
{
	const char* i = "Tree";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, 3, o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('T', o[0]);
	EXPECT_EQ('r', o[1]);
	EXPECT_EQ('e', o[2]);
}

TEST(DecodeUtf32, StringDataSizeOver)
{
	const char* i = "Bark";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, 8, o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('B', o[0]);
	EXPECT_EQ('a', o[1]);
	EXPECT_EQ('r', o[2]);
	EXPECT_EQ('k', o[3]);
}

TEST(DecodeUtf32, Ascii)
{
	const char* i = "\x5F";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000005F, o[0]);
}

TEST(DecodeUtf32, AsciiFirst)
{
	const char* i = "\0";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x00000000, o[0]);
}

TEST(DecodeUtf32, AsciiLast)
{
	const char* i = "\x7F";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000007F, o[0]);
}

TEST(DecodeUtf32, AsciiInvalid)
{
	const char* i = "\x88";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(DecodeUtf32, TwoBytes)
{
	const char* i = "\xC2\xA2";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x000000A2, o[0]);
}

TEST(DecodeUtf32, TwoBytesFirst)
{
	const char* i = "\xC2\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00000080, o[0]);
}

TEST(DecodeUtf32, TwoBytesLast)
{
	const char* i = "\xDF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x000007FF, o[0]);
}

TEST(DecodeUtf32, TwoBytesLonelyStartFirst)
{
	const char* i = "\xC0 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, TwoBytesLonelyStartLast)
{
	const char* i = "\xDF ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, TwoBytesLonelyStartCombined)
{
	const char* i =
		"\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF "\
		"\xD0 \xD1 \xD2 \xD3 \xD4 \xD5 \xD6 \xD7 \xD8 \xD9 \xDA \xDB \xDC \xDD \xDE \xDF ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(256, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);

	for (size_t i = 0; i < 32; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(DecodeUtf32, TwoBytesLonelyStartLength)
{
	const char* i = "\xD8 ";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, TwoBytesNotEnoughData)
{
	const char* i = "\xC2";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(DecodeUtf32, ThreeBytes)
{
	const char* i = "\xE1\x8C\x8A";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000130A, o[0]);
}

TEST(DecodeUtf32, ThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00000800, o[0]);
}

TEST(DecodeUtf32, ThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFF, o[0]);
}

TEST(DecodeUtf32, ThreeBytesLonelyStartFirst)
{
	const char* i = "\xE0 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, ThreeBytesLonelyStartLast)
{
	const char* i = "\xEF ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, ThreeBytesLonelyStartCombined)
{
	const char* i =
		"\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(128, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);

	for (size_t i = 0; i < 16; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(DecodeUtf32, ThreeBytesLonelyStartLength)
{
	const char* i = "\xE6 ";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, ThreeBytesNotEnoughData)
{
	const char* i = "\xEF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
}

TEST(DecodeUtf32, FourBytes)
{
	const char* i = "\xF0\x9F\x98\x8E";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0001F60E, o[0]);
}

TEST(DecodeUtf32, FourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x00010000, o[0]);
}

TEST(DecodeUtf32, FourBytesLast)
{
	const char* i = "\xF4\x80\x83\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x001000FF, o[0]);
}

TEST(DecodeUtf32, FourBytesLonelyStartFirst)
{
	const char* i = "\xF0 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, FourBytesLonelyStartLast)
{
	const char* i = "\xF7 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, FourBytesLonelyStartCombined)
{
	const char* i =
		"\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(64, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);

	for (size_t i = 0; i < 8; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(DecodeUtf32, FourBytesLonelyStartLength)
{
	const char* i = "\xF2 ";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, FourBytesNotEnoughData)
{
	const char* i = "\xF0\x90\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, FiveBytesLonelyStartFirst)
{
	const char* i = "\xF8 ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, FiveBytesLonelyStartLast)
{
	const char* i = "\xFB ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, FiveBytesLonelyStartCombined)
{
	const char* i =
		"\xF8 \xF9 \xFA \xFB ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(32, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);

	for (size_t i = 0; i < 4; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(DecodeUtf32, FiveBytesNotEnoughData)
{
	const char* i = "\xF8\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(DecodeUtf32, FiveBytesLonelyStartLength)
{
	const char* i = "\xFA ";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, SixBytesLonelyStartFirst)
{
	const char* i = "\xFC ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, SixBytesLonelyStartLast)
{
	const char* i = "\xFD ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(' ', o[1]);
}

TEST(DecodeUtf32, SixBytesLonelyStartCombined)
{
	const char* i =
		"\xFC \xFD ";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);

	for (size_t i = 0; i < 2; i += 2)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
		EXPECT_EQ(' ', o[i + 1]);
	}
}

TEST(DecodeUtf32, SixBytesLonelyStartLength)
{
	const char* i = "\xFA ";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, SixBytesNotEnoughData)
{
	const char* i = "\xFC\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(20, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
}

TEST(DecodeUtf32, OverlongSlashTwoBytes)
{
	const char* i = "\xC0\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
}

TEST(DecodeUtf32, OverlongSlashThreeBytes)
{
	const char* i = "\xE0\x80\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, OverlongSlashFourBytes)
{
	const char* i = "\xF0\x80\x80\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(DecodeUtf32, OverlongSlashFiveBytes)
{
	const char* i = "\xF8\x80\x80\x80\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(20, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
}

TEST(DecodeUtf32, OverlongSlashSixBytes)
{
	const char* i = "\xFC\x80\x80\x80\x80\xAF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
	EXPECT_EQ(0x0000FFFD, o[5]);
}

TEST(DecodeUtf32, OverlongNullTwoBytes)
{
	const char* i = "\xC0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
}

TEST(DecodeUtf32, OverlongNullThreeBytes)
{
	const char* i = "\xE0\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, OverlongNullFourBytes)
{
	const char* i = "\xF0\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(DecodeUtf32, OverlongNullFiveBytes)
{
	const char* i = "\xF8\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(20, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
}

TEST(DecodeUtf32, OverlongNullSixBytes)
{
	const char* i = "\xFC\x80\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
	EXPECT_EQ(0x0000FFFD, o[5]);
}

TEST(DecodeUtf32, OverlongMaximumTwoBytes)
{
	const char* i = "\xC1\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
}

TEST(DecodeUtf32, OverlongMaximumThreeBytes)
{
	const char* i = "\xE0\x9F\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, OverlongMaximumFourBytes)
{
	const char* i = "\xF0\x8F\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(DecodeUtf32, OverlongMaximumFiveBytes)
{
	const char* i = "\xF8\x87\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(20, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
}

TEST(DecodeUtf32, OverlongMaximumSixBytes)
{
	const char* i = "\xFC\x83\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
	EXPECT_EQ(0x0000FFFD, o[5]);
}

TEST(DecodeUtf32, OverlongFourBytesFirst)
{
	const char* i = "\xF4\x90\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(DecodeUtf32, OverlongFourBytesLast)
{
	const char* i = "\xF7\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(DecodeUtf32, OverlongFiveBytesFirst)
{
	const char* i = "\xF8\x88\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(20, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
}

TEST(DecodeUtf32, OverlongFiveBytesLast)
{
	const char* i = "\xFB\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(20, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
}

TEST(DecodeUtf32, OverlongSixBytesFirst)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
	EXPECT_EQ(0x0000FFFD, o[5]);
}

TEST(DecodeUtf32, OverlongSixBytesLast)
{
	const char* i = "\xFD\xBF\xBF\xBF\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
	EXPECT_EQ(0x0000FFFD, o[4]);
	EXPECT_EQ(0x0000FFFD, o[5]);
}

TEST(DecodeUtf32, OverlongNotEnoughSpace)
{
	const char* i = "\xFD\xBF\xBF\xBF\xBF\xBF";
	const size_t s = 4;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
	EXPECT_EQ(0x0000FFFD, o[3]);
}

TEST(DecodeUtf32, AmountOfBytes)
{
	const char* i = "\xF0\x90\xB0\xAC";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, AmountOfBytesOverlong)
{
	const char* i = "\xFC\x84\x80\x80\x80\x80";
	int32_t errors = 0;

	EXPECT_EQ(24, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, AmountOfBytesNotEnoughData)
{
	const char* i = "\xC8";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf32, AmountOfBytesNoData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf32(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(DecodeUtf32, MalformedContinuationByteFirst)
{
	const char* i = "\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(DecodeUtf32, MalformedContinuationByteLast)
{
	const char* i = "\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
}

TEST(DecodeUtf32, MalformedContinuationByteCombined)
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
	EXPECT_EQ(0, errors);
	for (size_t i = 0; i < 64; ++i)
	{
		EXPECT_EQ(0x0000FFFD, o[i]);
	}
}

TEST(DecodeUtf32, OverlongSurrogatePairHigh)
{
	const char* i = "\xED\xAD\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, OverlongSurrogatePairHighStart)
{
	const char* i = "\xED\xA0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, OverlongSurrogatePairHighEnd)
{
	const char* i = "\xED\xAF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, OverlongSurrogatePairLow)
{
	const char* i = "\xED\xBE\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, OverlongSurrogatePairLowStart)
{
	const char* i = "\xED\xB0\x80";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}

TEST(DecodeUtf32, OverlongSurrogatePairLowEnd)
{
	const char* i = "\xED\xBF\xBF";
	const size_t s = 256;
	unicode_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8toutf32(i, strlen(i), o, s * sizeof(unicode_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0000FFFD, o[0]);
	EXPECT_EQ(0x0000FFFD, o[1]);
	EXPECT_EQ(0x0000FFFD, o[2]);
}