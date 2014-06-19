#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(Encode, Encode)
{
	unicode_t c = 'U';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("U", b);
}

TEST(Encode, EncodeString)
{
	const size_t s = 256;
	char b[s] = { 0 };
	char* d = b;
	int32_t errors = 0;

	{
		unicode_t c = 'S';
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(1, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x1F12E;
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(4, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 'H';
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(1, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0xA840; // ꡀ
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(3, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x07D4; // ߔ
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(2, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	EXPECT_STREQ("S\xF0\x9F\x84\xAEH\xEA\xA1\x80\xDF\x94", b);
}

TEST(Encode, ZeroLength)
{
	unicode_t c = 0x8712;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8encode(c, b, 0, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(Encode, ZeroBuffer)
{
	unicode_t c = 'K';
	const size_t s = 256;
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8encode(c, nullptr, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(Encode, Ascii)
{
	unicode_t c = ')';
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ(")", b);
}

TEST(Encode, AsciiFirst)
{
	unicode_t c = 0;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\0", b);
}

TEST(Encode, AsciiLast)
{
	unicode_t c = 0x7F;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\x7F", b);
}

TEST(Encode, AsciiString)
{
	const size_t s = 256;
	char b[s] = { 0 };
	char* d = b;
	int32_t errors = 0;

	{
		unicode_t c = 'B';
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(1, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 'o';
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(1, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 'm';
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(1, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 'b';
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(1, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	EXPECT_STREQ("Bomb", b);
}

TEST(Encode, TwoBytes)
{
	unicode_t c = 0x03A9; // Ω
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xCE\xA9", b);
}

TEST(Encode, TwoBytesFirst)
{
	unicode_t c = 0x80; // Ω
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xC2\x80", b);
}

TEST(Encode, TwoBytesLast)
{
	unicode_t c = 0x07FF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xDF\xBF", b);
}

TEST(Encode, TwoBytesString)
{
	const size_t s = 256;
	char b[s] = { 0 };
	char* d = b;
	int32_t errors = 0;

	{
		unicode_t c = 0x0169; // ũ
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(2, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x014E; // Ŏ
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(2, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x0191; // Ƒ
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(2, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x014A; // Ŋ
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(2, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	EXPECT_STREQ("\xC5\xA9\xC5\x8E\xC6\x91\xC5\x8A", b);
}

TEST(Encode, TwoBytesBufferTooSmall)
{
	unicode_t c = 0x044B; // ы
	const size_t s = 1;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8encode(c, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(Encode, ThreeBytes)
{
	unicode_t c = 0x3DB1; // 㶱
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE3\xB6\xB1", b);
}

TEST(Encode, ThreeBytesFirst)
{
	unicode_t c = 0x800;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE0\xA0\x80", b);
}

TEST(Encode, ThreeBytesLast)
{
	unicode_t c = 0xFFFF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xEF\xBF\xBF", b);
}

TEST(Encode, ThreeBytesString)
{
	const size_t s = 256;
	char b[s] = { 0 };
	char* d = b;
	int32_t errors = 0;

	{
		unicode_t c = 0x2776; // ❶
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(3, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x2778; // ❸
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(3, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x2665; // ♥
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(3, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x277D; // ❽
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(3, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	EXPECT_STREQ("\xE2\x9D\xB6\xE2\x9D\xB8\xE2\x99\xA5\xE2\x9D\xBD", b);
}

TEST(Encode, ThreeBytesBufferTooSmall)
{
	unicode_t c = 0x1F61; // ὡ
	const size_t s = 2;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8encode(c, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(Encode, FourBytes)
{
	unicode_t c = 0x1D424; // 𝐤
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x9D\x90\xA4", b);
}

TEST(Encode, FourBytesFirst)
{
	unicode_t c = 0x10000;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\x90\x80\x80", b);
}

TEST(Encode, FourBytesLast)
{
	unicode_t c = 0x1000FF;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF4\x80\x83\xBF", b);
}

TEST(Encode, FourBytesString)
{
	const size_t s = 256;
	char b[s] = { 0 };
	char* d = b;
	int32_t errors = 0;

	{
		unicode_t c = 0x1F191; // 🆑
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(4, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x1F198; // 🆘
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(4, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	{
		unicode_t c = 0x1F19A; // 🆚
		size_t l = utf8encode(c, d, s, &errors);
		EXPECT_EQ(4, l);
		EXPECT_EQ(0, errors);
		d += l;
	}

	EXPECT_STREQ("\xF0\x9F\x86\x91\xF0\x9F\x86\x98\xF0\x9F\x86\x9A", b);
}

TEST(Encode, FourBytesBufferTooSmall)
{
	unicode_t c = 0x10840;
	const size_t s = 3;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(SIZE_MAX, utf8encode(c, b, s, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(Encode, AboveMaxLegalUtf32)
{
	unicode_t c = 0x110000;
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8encode(c, b, s, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xEF\xBF\xBD", b);
}