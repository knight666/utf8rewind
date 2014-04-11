#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(SeekForward, Valid)
{
	const char* t = "\xCF\x84\xE1\xBD\xB4\xCE\xBD \xCE\xBA\xE1\xBD\xB9\xCF\x88\xCE\xB7";

	const char* r = utf8seek(t, t, 4, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xCE\xBA\xE1\xBD\xB9\xCF\x88\xCE\xB7", r);
}

TEST(SeekForward, InvalidCharacter)
{
	const char* t = "\xD0\xBC\xFF KKD";

	const char* r = utf8seek(t, t, 3, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("\xFF KKD", r);
}

TEST(SeekForward, PastEnd)
{
	const char* t = "\xCE\xBC\xCE\xB5\xCF\x84\xCF\x81\xE1\xBD\xB1\xCE\xB5\xCE\xB9";

	const char* r = utf8seek(t + 2, t, 18, SEEK_CUR);

	EXPECT_EQ(t + 15, r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, SwappedParameters)
{
	const char* t = "10-12 \xD0\xBC\xD0\xB0\xD1\x80\xD1\x82\xD0\xB0 1997";

	const char* r = utf8seek(t, t + strlen(t), 6, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xD0\xBC\xD0\xB0\xD1\x80\xD1\x82\xD0\xB0 1997", r);
}

TEST(SeekForward, Ascii)
{
	const char* t = "Bringing down the ship.";

	const char* r = utf8seek(t, t, 14, SEEK_CUR);

	EXPECT_EQ(t + 14, r);
	EXPECT_STREQ("the ship.", r);
}

TEST(SeekForward, AsciiFromMiddle)
{
	const char* t = "Armageddon";

	const char* r = utf8seek(t + 4, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("ddon", r);
}

TEST(SeekForward, AsciiPastEnd)
{
	const char* t = "Peasants";

	const char* r = utf8seek(t, t, 15, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, AsciiAtEnd)
{
	const char* t = "Shipping";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, AsciiSwappedParameters)
{
	const char* t = "Buoying";

	const char* r = utf8seek(t, t + strlen(t), 3, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("ying", r);
}

TEST(SeekForward, AsciiZeroOffset)
{
	const char* t = "Banana";

	const char* r = utf8seek(t + 2, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("nana", r);
}

TEST(SeekForward, TwoBytes)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 3, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);
}

TEST(SeekForward, TwoBytesFromMiddle)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + 6, t, 4, SEEK_CUR);

	EXPECT_EQ(t + 14, r);
	EXPECT_STREQ("\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);
}

TEST(SeekForward, TwoBytesPastEnd)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 50, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, TwoBytesAtEnd)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, TwoBytesSwappedParameters)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t + strlen(t), 2, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);
}

TEST(SeekForward, TwoBytesZeroOffset)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + 6, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);
}

TEST(SeekForward, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);
}

TEST(SeekForward, ThreeBytesFromMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 3, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE0\xA4\xBD", r);
}

TEST(SeekForward, ThreeBytesPastEnd)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 3, t, 4, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, ThreeBytesAtEnd)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, ThreeBytesSwappedParameters)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t + strlen(t), 1, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);
}

TEST(SeekForward, ThreeBytesZeroOffset)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 6, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);
}

TEST(SeekForward, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 2, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);
}

TEST(SeekForward, FourBytesFromMiddle)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + 4, t, 3, SEEK_CUR);

	EXPECT_EQ(t + 16, r);
	EXPECT_STREQ("\xF0\x90\x92\x80", r);
}

TEST(SeekForward, FourBytesPastEnd)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 25, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, FourBytesAtEnd)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + strlen(t), t, 2, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekForward, FourBytesSwappedParameters)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t + strlen(t), 2, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);
}

TEST(SeekForward, FourBytesZeroOffset)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + 8, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);
}

TEST(SeekBackward, EndsWithAscii)
{
	const char* t = "Tree\xD1\x86j";

	const char* r = utf8seek(t + strlen(t), t, -3, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xD1\x86j", r);
}

TEST(SeekBackward, Ascii)
{
	const char* t = "Paper boat";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("at", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, AsciiFromMiddle)
{
	const char* t = "Banananana";

	const char* r = utf8seek(t + 6, t, -4, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("nananana", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, AsciiZeroOffset)
{
	const char* t = "Apple";

	const char* r = utf8seek(t + strlen(t) - 2, t, 0, SEEK_CUR);

	EXPECT_EQ(t + strlen(t) - 2, r);
	EXPECT_STREQ("le", r);
}

TEST(SeekBackward, AsciiPastStart)
{
	const char* t = "Beautiful day.";

	const char* r = utf8seek(t + strlen(t), t, -50, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Beautiful day.", r);
}

TEST(SeekBackward, AsciiAtStart)
{
	const char* t = "Blogspam";

	const char* r = utf8seek(t, t, -4, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("Blogspam", r);
}

TEST(SeekBackward, AsciiSwappedParameters)
{
	const char* t = "100 km/h";

	const char* r = utf8seek(t, t + strlen(t), -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("100 km/h", r);
}

TEST(SeekBackward, AsciiEndsInMiddle)
{
	const char* t = "Massive\0Pony";

	const char* r = utf8seek(t + 12, t, -8, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("ive", r);
}

TEST(SeekBackward, TwoBytes)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + strlen(t), t, -3, SEEK_CUR);

	EXPECT_EQ(t + 18, r);
	EXPECT_STREQ("\xD0\xB8\xD0\xB8", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, TwoBytesFromMiddle)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + 8, t, -3, SEEK_CUR);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, TwoBytesZeroOffset)
{
	const char* t = "\xD0\xB0\xD1\x86\xD0\xB8";

	const char* r = utf8seek(t + strlen(t), t, 0, SEEK_CUR);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
}

TEST(SeekBackward, TwoBytesPastStart)
{
	const char* t = "\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t + 6, t, -16, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, TwoBytesAtStart)
{
	const char* t = "\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t, t, -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, TwoBytesSwappedParameters)
{
	const char* t = "\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t, t + strlen(t), -3, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, TwoBytesEndsInMiddle)
{
	const char* t = "\xD0\xBE\xD0\xBA\0\xD0\xB0\xD0\xBB";

	const char* r = utf8seek(t + 9, t, -4, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("", r);
}

TEST(SeekBackward, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, ThreeBytesFromMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 9, t, -2, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, ThreeBytesZeroOffset)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 9, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, ThreeBytesPastStart)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + strlen(t), t, -26, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, ThreeBytesAtStart)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, ThreeBytesSwappedParameters)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t + strlen(t), -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, ThreeBytesEndsInMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\0\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 13, t, -5, SEEK_CUR);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("\xE0\xA4\x8B", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + strlen(t), t, -2, SEEK_CUR);

	EXPECT_EQ(t + 16, r);
	EXPECT_STREQ("\xF0\x90\x92\x80", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, FourBytesFromMiddle)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t + 16, t, -3, SEEK_CUR);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, FourBytesZeroOffset)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t + 16, t, 0, SEEK_CUR);

	EXPECT_EQ(t + 16, r);
	EXPECT_STREQ("\xF0\x9F\x89\x9E", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, FourBytesPastStart)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t + strlen(t), t, -12, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, FourBytesAtStart)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t, t, -2, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, FourBytesSwappedParameters)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t, t + strlen(t), -8, SEEK_CUR);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E", r);
	size_t o = 0;
	EXPECT_NE(0, utf8decode(r, &o));
}

TEST(SeekBackward, FourBytesEndsInMiddle)
{
	const char* t = "\xF0\x9F\x88\xB8\xF0\x9F\x89\x90\xF0\x9F\x89\x81\0\xF0\x9F\x89\x9A\xF0\x9F\x89\x9E";

	const char* r = utf8seek(t + 21, t, -4, SEEK_CUR);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("", r);
}

TEST(SeekSet, Empty)
{
	const char* t = "";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("", r);
	size_t o = 0;
	EXPECT_EQ(0, utf8decode(r, &o));
}

TEST(SeekSet, Ascii)
{
	const char* t = "Riverbed";

	const char* r = utf8seek(t, t, 4, SEEK_SET);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("rbed", r);
	size_t o = 0;
	EXPECT_EQ(1, utf8decode(r, &o));
}

TEST(SeekSet, AsciiOffset)
{
	const char* t = "The Doctor";

	const char* r = utf8seek(t + 4, t, 4, SEEK_SET);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("Doctor", r);
	size_t o = 0;
	EXPECT_EQ(1, utf8decode(r, &o));
}

TEST(SeekSet, AsciiPastEnd)
{
	const char* t = "Time-wimey";

	const char* r = utf8seek(t, t, 15, SEEK_SET);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
	size_t o = 0;
	EXPECT_EQ(0, utf8decode(r, &o));
}

TEST(SeekSet, AsciiNegative)
{
	const char* t = "TARDIS";

	const char* r = utf8seek(t, t, -2, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("TARDIS", r);
	size_t o = 0;
	EXPECT_EQ(1, utf8decode(r, &o));
}

TEST(SeekSet, TwoBytes)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);
	size_t o = 0;
	EXPECT_EQ(2, utf8decode(r, &o));
}

TEST(SeekSet, TwoBytesOffset)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t + 6, t, 5, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);
	size_t o = 0;
	EXPECT_EQ(2, utf8decode(r, &o));
}

TEST(SeekSet, TwoBytesPastEnd)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 33, SEEK_SET);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
	size_t o = 0;
	EXPECT_EQ(0, utf8decode(r, &o));
}

TEST(SeekSet, TwoBytesNegative)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, -3, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);
	size_t o = 0;
	EXPECT_EQ(2, utf8decode(r, &o));
}

TEST(SeekSet, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 2, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_EQ(3, utf8decode(r, &o));
}

TEST(SeekSet, ThreeBytesOffset)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t + 3, t, 2, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_EQ(3, utf8decode(r, &o));
}

TEST(SeekSet, ThreeBytesPastEnd)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 71, SEEK_SET);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
	size_t o = 0;
	EXPECT_EQ(0, utf8decode(r, &o));
}

TEST(SeekSet, ThreeBytesNegative)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, -2, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD", r);
	size_t o = 0;
	EXPECT_EQ(3, utf8decode(r, &o));
}

TEST(SeekSet, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80", r);
	size_t o = 0;
	EXPECT_EQ(4, utf8decode(r, &o));
}

TEST(SeekSet, FourBytesOffset)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t + 2, t, 1, SEEK_SET);

	EXPECT_EQ(t + 4, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);
	size_t o = 0;
	EXPECT_EQ(4, utf8decode(r, &o));
}

TEST(SeekSet, FourBytesPastEnd)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 28, SEEK_SET);

	EXPECT_EQ(t + strlen(t), r);
	EXPECT_STREQ("", r);
	size_t o = 0;
	EXPECT_EQ(0, utf8decode(r, &o));
}

TEST(SeekSet, FourBytesNegative)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, -1, SEEK_SET);

	EXPECT_EQ(t, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80", r);
	size_t o = 0;
	EXPECT_EQ(4, utf8decode(r, &o));
}