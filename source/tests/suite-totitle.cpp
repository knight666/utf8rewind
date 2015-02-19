#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

TEST(ToTitle, BasicLatinSingleUppercase)
{
	const char* c = "J";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinSingleLowercase)
{
	const char* c = "z";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Z", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinSingleUnaffected)
{
	const char* c = "$";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("$", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinMultipleUppercase)
{
	const char* c = "BROWN";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Brown", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinMultipleLowercase)
{
	const char* c = "lady";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Lady", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinMultipleUnaffected)
{
	const char* c = "9711";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("9711", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinWord)
{
	const char* c = "ApplE";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Apple", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinSentence)
{
	const char* c = "PRINTER INK";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Printer Ink", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinSentencePunctuationInMiddle)
{
	const char* c = "RE/wind=cool";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Re/Wind=Cool", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinSentencePunctuationAtStart)
{
	const char* c = "/!\\alert imminent";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(17, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("/!\\Alert Imminent", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinSentencePunctuationAtEnd)
{
	const char* c = "you tell me ;)";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(14, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("You Tell Me ;)", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinAmountOfBytes)
{
	const char* c = "[Arrest]";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, BasicLatinNotEnoughSpace)
{
	const char* c = "assassin";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Ass", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedSingleUppercase)
{
	// LATIN CAPITAL LETTER DZ WITH CARON

	const char* c = "\xC7\x84";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC7\x85", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedSingleLowercase)
{
	// ARMENIAN SMALL LIGATURE MEN XEH

	const char* c = "\xEF\xAC\x97";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xD5\x84\xD5\xAD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedSingleTitlecase)
{
	// LATIN SMALL LETTER DZ

	const char* c = "\xC7\xB2";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC7\xB2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedSingleUnaffected)
{
	// ARABIC SMALL WAW

	const char* c = "\xDB\xA5";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xDB\xA5", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedMultipleUppercase)
{
	// LATIN CAPITAL LETTER C WITH HOOK
	// LATIN CAPITAL LETTER LJ
	// LATIN CAPITAL LETTER O WITH TILDE AND DIAERESIS

	const char* c = "\xC6\x87\xC7\x87\xE1\xB9\x8E";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC6\x87\xC7\x89\xE1\xB9\x8F", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedMultipleLowercase)
{
	// GREEK SMALL LETTER IOTA WITH DASIA AND OXIA
	// GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI AND YPOGEGRAMMENI
	// GREEK SMALL LETTER ETA WITH DASIA AND YPOGEGRAMMENI
	// GREEK SMALL LETTER ETA WITH PSILI AND OXIA

	const char* c = "\xE1\xBC\xB5\xE1\xBE\x87\xE1\xBE\x91\xE1\xBC\xA4";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xE1\xBC\xBD\xE1\xBE\x87\xE1\xBE\x91\xE1\xBC\xA4", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedMultipleTitlecase)
{
	// LATIN SMALL LETTER SHARP S
	// LATIN SMALL LETTER DZ
	// LATIN SMALL LIGATURE IJ

	const char* c = "\xC3\x9F\xC7\xB3\xC4\xB3";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Ss\xC7\xB3\xC4\xB3", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedMultipleUnaffected)
{
	// ARABIC PERCENT SIGN
	// COMBINING DOT ABOVE RIGHT
	// HEBREW POINT HATAF PATAH

	const char* c = "\xD9\xAA\xCD\x98\xD6\xB2";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xD9\xAA\xCD\x98\xD6\xB2", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedWord)
{
	const char* c = "\xCF\x84\xE1\xBD\xB4\xCE\xBD";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\xA4\xE1\xBD\xB4\xCE\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedSentence)
{
	const char* c = "\xC3\x81rv\xC3\xADzt\xC5\xB1r\xC5\x91 t\xC3\xBCk\xC3\xB6rf\xC3\xBAr\xC3\xB3g\xC3\xA9p";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(31, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC3\x81rv\xC3\xADzt\xC5\xB1r\xC5\x91 T\xC3\xBCk\xC3\xB6rf\xC3\xBAr\xC3\xB3g\xC3\xA9p", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedAmountOfBytes)
{
	// 00DF 0116 01D9 01F0
	// 0053 0073 0117 01DA 01F0

	const char* c = "\xC3\x9F\xC4\x96\xC7\x99\xC7\xB0";
	int32_t errors = 0;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, GeneralCategoryCaseMappedNotEnoughSpace)
{
	// 0149 00D8 03B0
	// 02BC 004E 00F8 03B0

	const char* c = "\xC5\x89\xC3\x98\xCE\xB0";
	const size_t s = 7;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCA\xBCN\xC3\xB8", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToTitle, InvalidCodepointSingle)
{
	const char* c = "\xF0\x92";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, InvalidCodepointMultiple)
{
	const char* c = "\xCB\xC0\xC0\xFA";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, InvalidCodepointAmountOfBytes)
{
	const char* c = "\xAB\xAC\xCA";
	int32_t errors = 0;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(ToTitle, InvalidCodepointNotEnoughSpace)
{
	const char* c = "\xDF\xDF\xDF";
	const size_t s = 8;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST(ToTitle, InvalidData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8totitle(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}