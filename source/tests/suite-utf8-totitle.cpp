#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-locale.hpp"
#include "helpers-strings.hpp"

class Utf8ToTitle
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		SET_LOCALE_ENGLISH();
	}

	void TearDown()
	{
		RESET_LOCALE();
	}

};

TEST_F(Utf8ToTitle, BasicLatinSingleUppercase)
{
	const char* c = "J";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("J", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinSingleLowercase)
{
	const char* c = "z";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Z", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinSingleUnaffected)
{
	const char* c = "$";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("$", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinMultipleUppercase)
{
	const char* c = "BROWN";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Brown", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinMultipleLowercase)
{
	const char* c = "lady";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Lady", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinMultipleUnaffected)
{
	const char* c = "9711";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("9711", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinWord)
{
	const char* c = "ApplE";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Apple", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinSentence)
{
	const char* c = "PRINTER INK";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(11, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Printer Ink", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinSentencePunctuationInMiddle)
{
	const char* c = "RE/wind=cool";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Re/Wind=Cool", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinSentencePunctuationAtStart)
{
	const char* c = "/!\\alert imminent";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(17, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("/!\\Alert Imminent", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinSentencePunctuationAtEnd)
{
	const char* c = "you tell me ;)";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(14, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("You Tell Me ;)", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinAmountOfBytes)
{
	const char* c = "[Arrest]";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, BasicLatinNotEnoughSpace)
{
	const char* c = "assassin";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Ass", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedSingleUppercase)
{
	// LATIN CAPITAL LETTER DZ WITH CARON

	const char* c = "\xC7\x84";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC7\x85", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedSingleLowercase)
{
	// ARMENIAN SMALL LIGATURE MEN XEH

	const char* c = "\xEF\xAC\x97";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xD5\x84\xD5\xAD", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedSingleTitlecase)
{
	// LATIN SMALL LETTER DZ

	const char* c = "\xC7\xB2";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC7\xB2", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedSingleUnaffected)
{
	// ARABIC SMALL WAW

	const char* c = "\xDB\xA5";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xDB\xA5", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedMultipleUppercase)
{
	// LATIN CAPITAL LETTER C WITH HOOK
	// LATIN CAPITAL LETTER LJ
	// LATIN CAPITAL LETTER O WITH TILDE AND DIAERESIS

	const char* c = "\xC6\x87\xC7\x87\xE1\xB9\x8E";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC6\x87\xC7\x89\xE1\xB9\x8F", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedMultipleLowercase)
{
	// GREEK SMALL LETTER IOTA WITH DASIA AND OXIA
	// GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI AND YPOGEGRAMMENI
	// GREEK SMALL LETTER ETA WITH DASIA AND YPOGEGRAMMENI
	// GREEK SMALL LETTER ETA WITH PSILI AND OXIA

	const char* c = "\xE1\xBC\xB5\xE1\xBE\x87\xE1\xBE\x91\xE1\xBC\xA4";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xE1\xBC\xBD\xE1\xBE\x87\xE1\xBE\x91\xE1\xBC\xA4", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedMultipleTitlecase)
{
	// LATIN SMALL LETTER SHARP S
	// LATIN SMALL LETTER DZ
	// LATIN SMALL LIGATURE IJ

	const char* c = "\xC3\x9F\xC7\xB3\xC4\xB3";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Ss\xC7\xB3\xC4\xB3", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedMultipleUnaffected)
{
	// ARABIC PERCENT SIGN
	// COMBINING DOT ABOVE RIGHT
	// HEBREW POINT HATAF PATAH

	const char* c = "\xD9\xAA\xCD\x98\xD6\xB2";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xD9\xAA\xCD\x98\xD6\xB2", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedWord)
{
	const char* c = "\xCF\x84\xE1\xBD\xB4\xCE\xBD";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCE\xA4\xE1\xBD\xB4\xCE\xBD", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedSentence)
{
	const char* c = "\xC3\x81rv\xC3\xADzt\xC5\xB1r\xC5\x91 t\xC3\xBCk\xC3\xB6rf\xC3\xBAr\xC3\xB3g\xC3\xA9p";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(31, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC3\x81rv\xC3\xADzt\xC5\xB1r\xC5\x91 T\xC3\xBCk\xC3\xB6rf\xC3\xBAr\xC3\xB3g\xC3\xA9p", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedAmountOfBytes)
{
	// 00DF 0116 01D9 01F0
	// 0053 0073 0117 01DA 01F0

	const char* c = "\xC3\x9F\xC4\x96\xC7\x99\xC7\xB0";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(8, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, GeneralCategoryCaseMappedNotEnoughSpace)
{
	// 0149 00D8 03B0
	// 02BC 004E 00F8 03B0

	const char* c = "\xC5\x89\xC3\x98\xCE\xB0";
	const size_t s = 7;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(5, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xCA\xBCN\xC3\xB8", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitle, InvalidCodepointSingle)
{
	const char* c = "\xF0\x92";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, InvalidCodepointMultiple)
{
	const char* c = "\xCB\xC0\xC0\xFA";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(12, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, InvalidCodepointAmountOfBytes)
{
	const char* c = "\xAB\xAC\xCA";
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, InvalidCodepointNotEnoughSpace)
{
	const char* c = "\xDF\xDF\xDF";
	const size_t s = 8;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(6, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xEF\xBF\xBD\xEF\xBF\xBD", b);
	EXPECT_ERROREQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
}

TEST_F(Utf8ToTitle, ErrorsIsReset)
{
	const char* c = "cAPS lOCK";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = 11111;

	EXPECT_EQ(9, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Caps Lock", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, InvalidData)
{
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(0, utf8totitle(nullptr, 1, nullptr, 0, &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersFits)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };
	strcpy(data, "GaMeS");

	const char* i = data;
	size_t is = 5;
	char* o = data + 5;
	size_t os = 5;

	EXPECT_EQ(5, utf8totitle(i, is, o, os, &errors));
	EXPECT_UTF8EQ("GaMeSGames", data);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersStartsEqual)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data;
	size_t is = 16;
	char* o = data;
	size_t os = 58;

	EXPECT_EQ(0, utf8totitle(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersEndsEqual)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 21;
	size_t is = 43;
	char* o = data + 8;
	size_t os = 58;

	EXPECT_EQ(0, utf8totitle(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersInputStartsInTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 44;
	size_t is = 56;
	char* o = data + 26;
	size_t os = 20;

	EXPECT_EQ(0, utf8totitle(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersInputEndsInTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data;
	size_t is = 57;
	char* o = data + 19;
	size_t os = 46;

	EXPECT_EQ(0, utf8totitle(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersInputInsideTarget)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 86;
	size_t is = 22;
	char* o = data + 68;
	size_t os = 109;

	EXPECT_EQ(0, utf8totitle(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersTargetStartsInInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 73;
	size_t is = 25;
	char* o = data + 88;
	size_t os = 36;

	EXPECT_EQ(0, utf8totitle(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersTargetEndsInInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 27;
	size_t is = 68;
	char* o = data + 10;
	size_t os = 39;

	EXPECT_EQ(0, utf8totitle(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}

TEST_F(Utf8ToTitle, OverlappingParametersTargetInsideInput)
{
	int32_t errors = UTF8_ERR_NONE;

	char data[128] = { 0 };

	const char* i = data + 14;
	size_t is = 59;
	char* o = data + 26;
	size_t os = 11;

	EXPECT_EQ(0, utf8totitle(i, is, o, os, &errors));
	EXPECT_ERROREQ(UTF8_ERR_OVERLAPPING_PARAMETERS, errors);
}