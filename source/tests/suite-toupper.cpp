#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(ToUpper, BasicLatin)
{
	const char* c = "Holiday Special!";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(16, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("HOLIDAY SPECIAL!", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinUppercase)
{
	const char* c = "B";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("B", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinLowercase)
{
	const char* c = "w";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("W", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, BasicLatinUnaffected)
{
	const char* c = ")";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ(")", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, TwoBytes)
{
	const char* c = "tiny \xC8\x9B";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(7, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("TINY \xC8\x9A", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, TwoBytesUppercase)
{
	const char* c = "\xC8\x8A";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xC8\x8A", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, TwoBytesLowercase)
{
	const char* c = "\xC6\x9A";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xC8\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, TwoBytesUnaffected)
{
	const char* c = "\xCD\xA6";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xCD\xA6", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, ThreeBytes)
{
	const char* c = "\xE2\xB4\x80" "agram";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE1\x82\xA0" "AGRAM", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, ThreeBytesUppercase)
{
	const char* c = "\xE1\x82\xBC";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE1\x82\xBC", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, ThreeBytesLowercase)
{
	const char* c = "\xE2\xB4\xA3";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE1\x83\x83", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, ThreeBytesUnaffected)
{
	const char* c = "\xE1\x83\xBF";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xE1\x83\xBF", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, FourBytes)
{
	const char* c = "mach\xF0\x90\x90\xAEne";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(10, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("MACH\xF0\x90\x90\x86NE", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, FourBytesUppercase)
{
	const char* c = "\xF0\x90\x90\x92";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\x92", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, FourBytesLowercase)
{
	const char* c = "\xF0\x90\x91\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\x9F", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, FourBytesUnaffected)
{
	const char* c = "\xF0\x91\xA3\xAE";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("\xF0\x91\xA3\xAE", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, Decomposed)
{
	const char* c = "O\xCC\x9Bu\xCC\x88\xCC\x84" "A\xCC\x87\xCC\x84";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(13, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("O\xCC\x9BU\xCC\x88\xCC\x84" "A\xCC\x87\xCC\x84", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, DecomposedUppercase)
{
	const char* c = "A\xCC\x80";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("A\xCC\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, DecomposedLowercase)
{
	const char* c = "i\xCC\x8C";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("I\xCC\x8C", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, AmountOfBytes)
{
	const char* c = "Magic";
	int32_t errors = 0;

	EXPECT_EQ(5, utf8toupper(c, strlen(c), nullptr, 0, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUpper, NotEnoughSpace)
{
	const char* c = "Merde";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, 0, &errors));
	EXPECT_UTF8EQ("MER", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToUpper, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toupper(nullptr, 1, nullptr, 0, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(ToUpperNormalize, BasicLatin)
{
	const char* c = "christmas SWEATer";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(17, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("CHRISTMAS SWEATER", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, BasicLatinUppercase)
{
	const char* c = "B";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("B", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, BasicLatinLowercase)
{
	const char* c = "f";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("F", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, BasicLatinUnaffected)
{
	const char* c = "\"";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\"", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, TwoBytesUppercase)
{
	const char* c = "\xD2\xA8";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xD2\xA8", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, TwoBytesLowercase)
{
	const char* c = "\xD3\x8F";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xD3\x80", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, TwoBytesUnaffected)
{
	const char* c = "\xD6\x8F";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xD6\x8F", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, ThreeBytesUppercase)
{
	const char* c = "\xE1\x82\xAC";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE1\x82\xAC", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, ThreeBytesLowercase)
{
	const char* c = "\xE1\xBA\xA1";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE1\xBA\xA0", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, ThreeBytesUnaffected)
{
	const char* c = "\xE1\xBF\xBD";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xE1\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, FourBytesUppercase)
{
	const char* c = "\xF0\x90\x90\x88";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\x88", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, FourBytesLowercase)
{
	const char* c = "\xF0\x90\x90\xBC";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x90\x90\x94", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, FourBytesUnaffected)
{
	const char* c = "\xF0\x91\xA3\xB2";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xF0\x91\xA3\xB2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, Decomposed)
{
	const char* c = "a\xCC\x91\xD0\xB5\xCC\x88\xC4\xB3";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC8\x82\xD0\x80\xC4\xB2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, DecomposedUppercase)
{
	const char* c = "N\xCC\xA7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC5\x85", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, DecomposedLowercase)
{
	const char* c = "t\xCC\xA7";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC5\xA2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, AmountOfBytes)
{
	const char* c = "U\xCC\x88\xCC\x84";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), nullptr, 0, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, NotEnoughSpace)
{
	const char* c = "a\xCC\x8A\xCC\x81i\xCC\x8F";
	const size_t s = 3;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xC7\xBA", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToUpperNormalize, InvalidCodepoint)
{
	const char* c = "\xF0\x94\x99";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8toupper(c, strlen(c), b, s - 1, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToUpperNormalize, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toupper(nullptr, 1, nullptr, 0, UTF8_TRANSFORM_NORMALIZED, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}