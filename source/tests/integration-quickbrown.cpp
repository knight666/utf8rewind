#include "tests-base.hpp"

#include "utf8rewind.h"

class QuickbrownWide
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		errors = 0;
		output = nullptr;

		memset(input, 0, 8192);

		// load into file

		file.open("testdata/quickbrown.txt", std::ios_base::in);
		ASSERT_TRUE(file.is_open());

		file.read(input, 8192);
		ASSERT_EQ(4833, strlen(input));

		file.close();
	}

	void TearDown()
	{
		if (output != nullptr)
		{
			delete [] output;
		}
	}

	void ReadSection(size_t position, size_t length)
	{
		ASSERT_TRUE(position < 4833 && position + length < 4833);

		size_t decoded_size = utf8towc(input + position, length, nullptr, 0, &errors);
		ASSERT_TRUE(decoded_size > 0);
		ASSERT_EQ(0, errors);

		if (output != nullptr)
		{
			delete [] output;
		}

		size_t output_length = decoded_size / sizeof(wchar_t);

		output = new wchar_t[output_length + 1];
		output[output_length] = 0;

		utf8towc(input + position, length, output, decoded_size, &errors);
		ASSERT_EQ(0, errors);
	}

	char input[8192];
	wchar_t* output;
	std::fstream file;
	int32_t errors;

};

TEST_F(QuickbrownWide, Danish)
{
	ReadSection(271, 95);
	EXPECT_STREQ(L"  Quizdeltagerne spiste jordb\xE6r med fl\xF8" L"de, mens cirkusklovnen\n  Wolther spillede p\xE5 xylofon.", output);
}

TEST_F(QuickbrownWide, German)
{
	ReadSection(503, 64);
	EXPECT_STREQ(L"  Falsches \xDC" L"ben von Xylophonmusik qu\xE4lt jeden gr\xF6\xDF" L"eren Zwerg", output);

	ReadSection(642, 59);
	EXPECT_STREQ(L"  Zw\xF6lf Boxk\xE4mpfer jagten Eva quer \xFC" L"ber den Sylter Deich", output);

	ReadSection(767, 30);
	EXPECT_STREQ(L"  Heiz\xF6lr\xFC" L"cksto\xDF" L"abd\xE4mpfung", output);
}

TEST_F(QuickbrownWide, Greek)
{
	ReadSection(911, 105);
	EXPECT_STREQ(L"  \x393\x3B1\x3B6\x3AD\x3B5\x3C2 \
\x3BA\x3B1\x1F76 \x3BC\x3C5\x3C1\x3C4\x3B9\x1F72\x3C2 \
\x3B4\x1F72\x3BD \x3B8\x1F70 \x3B2\x3C1\x1FF6 \x3C0\
\x3B9\x1F70 \x3C3\x3C4\x1F78 \x3C7\x3C1\x3C5\x3C3\x3B1\
\x3C6\x1F76 \x3BE\x3AD\x3C6\x3C9\x3C4\x3BF", output);

	ReadSection(1086, 66);
	EXPECT_STREQ(L"  \x39E\x3B5\x3C3\x3BA\x3B5\x3C0\x3AC\
\x3B6\x3C9 \x3C4\x1F74\x3BD \x3C8\x3C5\x3C7\x3BF\x3C6\
\x3B8\x3CC\x3C1\x3B1 \x3B2\x3B4\x3B5\x3BB\x3C5\x3B3\x3BC\
\x3AF\x3B1", output);
}

TEST_F(QuickbrownWide, English)
{
	ReadSection(1228, 45);
	EXPECT_STREQ(L"  The quick brown fox jumps over the lazy dog", output);
}

TEST_F(QuickbrownWide, Spanish)
{
	ReadSection(1302, 108);
	EXPECT_STREQ(L"  El ping\xFCino Wenceslao hizo kil\xF3metros bajo exhaustiva lluvia y \n\
  fr\xEDo, a\xF1oraba a su querido cachorro.", output);
}

TEST_F(QuickbrownWide, French)
{
	ReadSection(1527, 361);
	EXPECT_STREQ(L"  Portez ce vieux whisky au juge blond qui fume sur son \xEEle int\xE9rieure, \xE0\n\
  c\xF4t\xE9 de l'alc\xF4ve ovo\xEF" L"de, o\xF9 les b\xFB" L"ches se consument dans l'\xE2tre, ce\n\
  qui lui permet de penser \xE0 la c\xE6nogen\xE8se de l'\xEAtre dont il est question\n\
  dans la cause ambigu\xEB entendue \xE0 Mo\xFF, dans un capharna\xFCm qui,\n\
  pense-t-il, diminue \xE7\xE0 et l\xE0 la qualit\xE9 de son \x153uvre. ", output);
}

TEST_F(QuickbrownWide, IrishGaelic)
{
	ReadSection(2196, 76);
	EXPECT_STREQ(L"  D'fhuascail \xCDosa, \xDArmhac na h\xD3ighe Beannaithe, p\xF3r \xC9" L"ava agus \xC1" L"dhaimh", output);
}

TEST_F(QuickbrownWide, Icelandic)
{
	ReadSection(2441, 63);
	EXPECT_STREQ(L"  K\xE6mi n\xFD \xF6xi h\xE9r ykist \xFEj\xF3" L"fum n\xFA b\xE6\xF0i v\xEDl og \xE1" L"drepa", output);

	ReadSection(2506, 46);
	EXPECT_STREQ(L"  S\xE6v\xF6r gr\xE9t \xE1\xF0" L"an \xFEv\xED \xFAlpan var \xF3n\xFDt", output);
}