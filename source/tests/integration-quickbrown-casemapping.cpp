#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-strings.hpp"

class QuickbrownCaseMapping
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		errors = 0;

		fileRegular.open("testdata/quickbrown.txt", std::ios_base::in);
		ASSERT_TRUE(fileRegular.is_open());

		fileUppercase.open("testdata/quickbrown_uppercase.txt", std::ios_base::in);
		ASSERT_TRUE(fileUppercase.is_open());

		fileLowercase.open("testdata/quickbrown_lowercase.txt", std::ios_base::in);
		ASSERT_TRUE(fileLowercase.is_open());
	}

	void TearDown()
	{
		fileLowercase.close();
		fileUppercase.close();
		fileRegular.close();
	}

	std::string ReadRegular(size_t position, size_t length)
	{
		std::string result;

		fileRegular.seekg(position, std::ios::beg);
		if (fileRegular.eof())
		{
			return result;
		}

		result.resize(length + 1);
		fileRegular.read(&result[0], length);

		return result;
	}

	std::string ReadUppercase(size_t position, size_t length)
	{
		std::string result;

		fileUppercase.seekg(position, std::ios::beg);
		if (fileUppercase.eof())
		{
			return result;
		}

		result.resize(length + 1);
		fileUppercase.read(&result[0], length);

		return result;
	}

	std::string ReadLowercase(size_t position, size_t length)
	{
		std::string result;

		fileLowercase.seekg(position, std::ios::beg);
		if (fileLowercase.eof())
		{
			return result;
		}

		result.resize(length + 1);
		fileLowercase.read(&result[0], length);

		return result;
	}

	std::fstream fileRegular;
	std::fstream fileUppercase;
	std::fstream fileLowercase;
	int32_t errors;

};

TEST_F(QuickbrownCaseMapping, DanishUppercase)
{
	std::string i = ReadRegular(271, 95);
	EXPECT_UTF8EQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xC3\xB8" "de, mens cirkusklovnen\n\
  Wolther spillede p\xC3\xA5 xylofon.", i.c_str());

	std::string eu = ReadUppercase(271, 95);
	EXPECT_UTF8EQ("  QUIZDELTAGERNE SPISTE JORDB\xc3\x86R MED FL\xC3\x98" "DE, MENS CIRKUSKLOVNEN\n\
  WOLTHER SPILLEDE P\xC3\x85 XYLOFON.", eu.c_str());
	
	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  QUIZDELTAGERNE SPISTE JORDB\xc3\x86R MED FL\xC3\x98" "DE, MENS CIRKUSKLOVNEN\n\
  WOLTHER SPILLEDE P\xC3\x85 XYLOFON.", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, DanishLowercase)
{
	std::string i = ReadRegular(271, 95);
	EXPECT_UTF8EQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xC3\xB8" "de, mens cirkusklovnen\n\
  Wolther spillede p\xC3\xA5 xylofon.", i.c_str());

	std::string el = ReadLowercase(271, 95);
	EXPECT_UTF8EQ("  quizdeltagerne spiste jordb\xc3\xa6r med fl\xC3\xB8" "de, mens cirkusklovnen\n\
  wolther spillede p\xC3\xA5 xylofon.", el.c_str());

	size_t l = utf8tolower(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  quizdeltagerne spiste jordb\xc3\xa6r med fl\xC3\xB8" "de, mens cirkusklovnen\n\
  wolther spillede p\xC3\xA5 xylofon.", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, GermanFirstUppercase)
{
	std::string i = ReadRegular(503, 64);
	EXPECT_UTF8EQ("  Falsches \xC3\x9C" "ben von Xylophonmusik qu\xC3\xA4lt jeden gr\xC3\xB6\xC3\x9F" "eren Zwerg", i.c_str());

	std::string eu = ReadUppercase(503, 64);
	EXPECT_UTF8EQ("  FALSCHES \xc3\x9c" "BEN VON XYLOPHONMUSIK QU\xc3\x84LT JEDEN GR\xC3\x96SSEREN ZWERG", eu.c_str());

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  FALSCHES \xC3\x9C" "BEN VON XYLOPHONMUSIK QU\xC3\x84LT JEDEN GR\xC3\x96SSEREN ZWERG", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, GermanFirstLowercase)
{
	std::string i = ReadRegular(503, 64);
	EXPECT_STREQ("  Falsches \xC3\x9C" "ben von Xylophonmusik qu\xC3\xA4lt jeden gr\xC3\xB6\xC3\x9F" "eren Zwerg", i.c_str());

	std::string el = ReadLowercase(503, 64);
	EXPECT_UTF8EQ("  falsches \xC3\xBC" "ben von xylophonmusik qu\xC3\xA4lt jeden gr\xC3\xB6\xC3\x9F" "eren zwerg", el);

	size_t l = utf8tolower(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  falsches \xC3\xBC" "ben von xylophonmusik qu\xC3\xA4lt jeden gr\xC3\xB6\xC3\x9F" "eren zwerg", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, GermanSecondUppercase)
{
	std::string i = ReadRegular(642, 59);
	EXPECT_UTF8EQ("  Zw\xC3\xB6lf Boxk\xC3\xA4mpfer jagten Eva quer \xC3\xBC" "ber den Sylter Deich", i.c_str());

	std::string eu = ReadUppercase(642, 59);
	EXPECT_UTF8EQ("  ZW\xC3\x96LF BOXK\xC3\x84MPFER JAGTEN EVA QUER \xC3\x9C" "BER DEN SYLTER DEICH", eu.c_str());

	size_t l = utf8tolower(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  ZW\xC3\x96LF BOXK\xC3\x84MPFER JAGTEN EVA QUER \xC3\x9C" "BER DEN SYLTER DEICH", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, GermanSecondLowercase)
{
	std::string i = ReadRegular(642, 59);
	EXPECT_UTF8EQ("  Zw\xC3\xB6lf Boxk\xC3\xA4mpfer jagten Eva quer \xC3\xBC" "ber den Sylter Deich", i.c_str());

	std::string el = ReadLowercase(642, 59);
	EXPECT_UTF8EQ("  zw\xC3\xB6lf boxk\xC3\xA4mpfer jagten eva quer \xC3\xBC" "ber den sylter deich", el.c_str());

	size_t l = utf8tolower(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  zw\xC3\xB6lf boxk\xC3\xA4mpfer jagten eva quer \xC3\xBC" "ber den sylter deich", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, GermanThirdUppercase)
{
	std::string i = ReadRegular(767, 30);
	EXPECT_STREQ("  Heiz\xC3\xB6lr\xC3\xBC" "cksto\xC3\x9F" "abd\xC3\xA4mpfung", i.c_str());

	std::string eu = ReadUppercase(767, 30);
	EXPECT_UTF8EQ("  HEIZ\xC3\x96LR\xC3\x9C" "CKSTOSSABD\xC3\x84MPFUNG", eu.c_str());

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  HEIZ\xC3\x96LR\xC3\x9C" "CKSTOSSABD\xC3\x84MPFUNG", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, GermanThirdLowercase)
{
	std::string i = ReadRegular(767, 30);
	EXPECT_STREQ("  Heiz\xC3\xB6lr\xC3\xBC" "cksto\xC3\x9F" "abd\xC3\xA4mpfung", i.c_str());

	std::string el = ReadLowercase(767, 30);
	EXPECT_UTF8EQ("  heiz\xC3\xB6lr\xC3\xBC" "cksto\xC3\x9F" "abd\xC3\xA4mpfung", el.c_str());

	size_t l = utf8tolower(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  heiz\xC3\xB6lr\xC3\xBC" "cksto\xC3\x9F" "abd\xC3\xA4mpfung", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, GreekFirstUppercase)
{
	std::string i = ReadRegular(911, 105);
	EXPECT_UTF8EQ("  \xCE\x93\xCE\xB1\xCE\xB6\xCE\xAD\xCE\xB5\xCF\x82 \xCE\xBA\xCE\xB1\xE1\xBD\xB6 \xCE\xBC\xCF\x85\xCF\x81\xCF\x84\xCE\xB9\xE1\xBD\xB2\xCF\x82 \xCE\xB4\xE1\xBD\xB2\xCE\xBD \xCE\xB8\xE1\xBD\xB0 \xCE\xB2\xCF\x81\xE1\xBF\xB6 \xCF\x80\xCE\xB9\xE1\xBD\xB0 \xCF\x83\xCF\x84\xE1\xBD\xB8 \xCF\x87\xCF\x81\xCF\x85\xCF\x83\xCE\xB1\xCF\x86\xE1\xBD\xB6 \xCE\xBE\xCE\xAD\xCF\x86\xCF\x89\xCF\x84\xCE\xBF", i.c_str());

	std::string eu = ReadUppercase(911, 106);
	EXPECT_UTF8EQ("  \xCE\x93\xCE\x91\xCE\x96\xCE\x88\xCE\x95\xCE\xA3 \xCE\x9A\xCE\x91\xE1\xBF\x9A \xCE\x9C\xCE\xA5\xCE\xA1\xCE\xA4\xCE\x99\xE1\xBF\x88\xCE\xA3 \xCE\x94\xE1\xBF\x88\xCE\x9D \xCE\x98\xE1\xBE\xBA \xCE\x92\xCE\xA1\xCE\xA9\xCD\x82 \xCE\xA0\xCE\x99\xE1\xBE\xBA \xCE\xA3\xCE\xA4\xE1\xBF\xB8 \xCE\xA7\xCE\xA1\xCE\xA5\xCE\xA3\xCE\x91\xCE\xA6\xE1\xBF\x9A \xCE\x9E\xCE\x88\xCE\xA6\xCE\xA9\xCE\xA4\xCE\x9F", eu.c_str());

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  \xCE\x93\xCE\x91\xCE\x96\xCE\x88\xCE\x95\xCE\xA3 \xCE\x9A\xCE\x91\xE1\xBF\x9A \xCE\x9C\xCE\xA5\xCE\xA1\xCE\xA4\xCE\x99\xE1\xBF\x88\xCE\xA3 \xCE\x94\xE1\xBF\x88\xCE\x9D \xCE\x98\xE1\xBE\xBA \xCE\x92\xCE\xA1\xCE\xA9\xCD\x82 \xCE\xA0\xCE\x99\xE1\xBE\xBA \xCE\xA3\xCE\xA4\xE1\xBF\xB8 \xCE\xA7\xCE\xA1\xCE\xA5\xCE\xA3\xCE\x91\xCE\xA6\xE1\xBF\x9A \xCE\x9E\xCE\x88\xCE\xA6\xCE\xA9\xCE\xA4\xCE\x9F", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, GreekFirstLowercase)
{
	std::string i = ReadRegular(911, 105);
	EXPECT_UTF8EQ("  \xCE\x93\xCE\xB1\xCE\xB6\xCE\xAD\xCE\xB5\xCF\x82 \xCE\xBA\xCE\xB1\xE1\xBD\xB6 \xCE\xBC\xCF\x85\xCF\x81\xCF\x84\xCE\xB9\xE1\xBD\xB2\xCF\x82 \xCE\xB4\xE1\xBD\xB2\xCE\xBD \xCE\xB8\xE1\xBD\xB0 \xCE\xB2\xCF\x81\xE1\xBF\xB6 \xCF\x80\xCE\xB9\xE1\xBD\xB0 \xCF\x83\xCF\x84\xE1\xBD\xB8 \xCF\x87\xCF\x81\xCF\x85\xCF\x83\xCE\xB1\xCF\x86\xE1\xBD\xB6 \xCE\xBE\xCE\xAD\xCF\x86\xCF\x89\xCF\x84\xCE\xBF", i.c_str());

	std::string el = ReadLowercase(911, 105);
	EXPECT_UTF8EQ("  \xCE\xB3\xCE\xB1\xCE\xB6\xCE\xAD\xCE\xB5\xCF\x82 \xCE\xBA\xCE\xB1\xE1\xBD\xB6 \xCE\xBC\xCF\x85\xCF\x81\xCF\x84\xCE\xB9\xE1\xBD\xB2\xCF\x82 \xCE\xB4\xE1\xBD\xB2\xCE\xBD \xCE\xB8\xE1\xBD\xB0 \xCE\xB2\xCF\x81\xE1\xBF\xB6 \xCF\x80\xCE\xB9\xE1\xBD\xB0 \xCF\x83\xCF\x84\xE1\xBD\xB8 \xCF\x87\xCF\x81\xCF\x85\xCF\x83\xCE\xB1\xCF\x86\xE1\xBD\xB6 \xCE\xBE\xCE\xAD\xCF\x86\xCF\x89\xCF\x84\xCE\xBF", el.c_str());

	size_t l = utf8tolower(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  \xCE\xB3\xCE\xB1\xCE\xB6\xCE\xAD\xCE\xB5\xCF\x82 \xCE\xBA\xCE\xB1\xE1\xBD\xB6 \xCE\xBC\xCF\x85\xCF\x81\xCF\x84\xCE\xB9\xE1\xBD\xB2\xCF\x82 \xCE\xB4\xE1\xBD\xB2\xCE\xBD \xCE\xB8\xE1\xBD\xB0 \xCE\xB2\xCF\x81\xE1\xBF\xB6 \xCF\x80\xCE\xB9\xE1\xBD\xB0 \xCF\x83\xCF\x84\xE1\xBD\xB8 \xCF\x87\xCF\x81\xCF\x85\xCF\x83\xCE\xB1\xCF\x86\xE1\xBD\xB6 \xCE\xBE\xCE\xAD\xCF\x86\xCF\x89\xCF\x84\xCE\xBF", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, GreekSecondUppercase)
{
	std::string i = ReadRegular(1086, 66);
	EXPECT_UTF8EQ("  \xCE\x9E\xCE\xB5\xCF\x83\xCE\xBA\xCE\xB5\xCF\x80\xCE\xAC\xCE\xB6\xCF\x89 \xCF\x84\xE1\xBD\xB4\xCE\xBD \xCF\x88\xCF\x85\xCF\x87\xCE\xBF\xCF\x86\xCE\xB8\xCF\x8C\xCF\x81\xCE\xB1 \xCE\xB2\xCE\xB4\xCE\xB5\xCE\xBB\xCF\x85\xCE\xB3\xCE\xBC\xCE\xAF\xCE\xB1", i);

	std::string eu = ReadUppercase(1087, 66);
	EXPECT_UTF8EQ("  \xCE\x9E\xCE\x95\xCE\xA3\xCE\x9A\xCE\x95\xCE\xA0\xCE\x86\xCE\x96\xCE\xA9 \xCE\xA4\xE1\xBF\x8A\xCE\x9D \xCE\xA8\xCE\xA5\xCE\xA7\xCE\x9F\xCE\xA6\xCE\x98\xCE\x8C\xCE\xA1\xCE\x91 \xCE\x92\xCE\x94\xCE\x95\xCE\x9B\xCE\xA5\xCE\x93\xCE\x9C\xCE\x8A\xCE\x91", eu.c_str());

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  \xCE\x9E\xCE\x95\xCE\xA3\xCE\x9A\xCE\x95\xCE\xA0\xCE\x86\xCE\x96\xCE\xA9 \xCE\xA4\xE1\xBF\x8A\xCE\x9D \xCE\xA8\xCE\xA5\xCE\xA7\xCE\x9F\xCE\xA6\xCE\x98\xCE\x8C\xCE\xA1\xCE\x91 \xCE\x92\xCE\x94\xCE\x95\xCE\x9B\xCE\xA5\xCE\x93\xCE\x9C\xCE\x8A\xCE\x91", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, GreekSecondLowercase)
{
	std::string i = ReadRegular(1086, 66);
	EXPECT_UTF8EQ("  \xCE\x9E\xCE\xB5\xCF\x83\xCE\xBA\xCE\xB5\xCF\x80\xCE\xAC\xCE\xB6\xCF\x89 \xCF\x84\xE1\xBD\xB4\xCE\xBD \xCF\x88\xCF\x85\xCF\x87\xCE\xBF\xCF\x86\xCE\xB8\xCF\x8C\xCF\x81\xCE\xB1 \xCE\xB2\xCE\xB4\xCE\xB5\xCE\xBB\xCF\x85\xCE\xB3\xCE\xBC\xCE\xAF\xCE\xB1", i);

	std::string el = ReadLowercase(1086, 66);
	EXPECT_UTF8EQ("  \xCE\xBE\xCE\xB5\xCF\x83\xCE\xBA\xCE\xB5\xCF\x80\xCE\xAC\xCE\xB6\xCF\x89 \xCF\x84\xE1\xBD\xB4\xCE\xBD \xCF\x88\xCF\x85\xCF\x87\xCE\xBF\xCF\x86\xCE\xB8\xCF\x8C\xCF\x81\xCE\xB1 \xCE\xB2\xCE\xB4\xCE\xB5\xCE\xBB\xCF\x85\xCE\xB3\xCE\xBC\xCE\xAF\xCE\xB1", el.c_str());

	size_t l = utf8tolower(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  \xCE\xBE\xCE\xB5\xCF\x83\xCE\xBA\xCE\xB5\xCF\x80\xCE\xAC\xCE\xB6\xCF\x89 \xCF\x84\xE1\xBD\xB4\xCE\xBD \xCF\x88\xCF\x85\xCF\x87\xCE\xBF\xCF\x86\xCE\xB8\xCF\x8C\xCF\x81\xCE\xB1 \xCE\xB2\xCE\xB4\xCE\xB5\xCE\xBB\xCF\x85\xCE\xB3\xCE\xBC\xCE\xAF\xCE\xB1", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, EnglishUppercase)
{
	std::string i = ReadRegular(1228, 45);
	EXPECT_UTF8EQ("  The quick brown fox jumps over the lazy dog", i);

	std::string eu = ReadUppercase(1229, 45);
	EXPECT_UTF8EQ("  THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", eu);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, EnglishLowercase)
{
	std::string i = ReadRegular(1228, 45);
	EXPECT_UTF8EQ("  The quick brown fox jumps over the lazy dog", i);

	std::string el = ReadLowercase(1228, 45);
	EXPECT_UTF8EQ("  the quick brown fox jumps over the lazy dog", el);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  the quick brown fox jumps over the lazy dog", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, SpanishUppercase)
{
	std::string i = ReadRegular(1302, 108);
	EXPECT_UTF8EQ("  El ping\xC3\xBCino Wenceslao hizo kil\xC3\xB3metros bajo exhaustiva lluvia y \n\
  fr\xC3\xADo, a\xC3\xB1oraba a su querido cachorro.", i);

	std::string eu = ReadUppercase(1303, 108);
	EXPECT_UTF8EQ("  EL PING\xC3\x9CINO WENCESLAO HIZO KIL\xC3\x93METROS BAJO EXHAUSTIVA LLUVIA Y \n\
  FR\xC3\x8DO, A\xC3\x91ORABA A SU QUERIDO CACHORRO.", eu);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  EL PING\xC3\x9CINO WENCESLAO HIZO KIL\xC3\x93METROS BAJO EXHAUSTIVA LLUVIA Y \n\
  FR\xC3\x8DO, A\xC3\x91ORABA A SU QUERIDO CACHORRO.", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, SpanishLowercase)
{
	std::string i = ReadRegular(1302, 108);
	EXPECT_UTF8EQ("  El ping\xC3\xBCino Wenceslao hizo kil\xC3\xB3metros bajo exhaustiva lluvia y \n\
  fr\xC3\xADo, a\xC3\xB1oraba a su querido cachorro.", i);

	std::string el = ReadLowercase(1302, 108);
	EXPECT_UTF8EQ("  el ping\xC3\xBCino wenceslao hizo kil\xC3\xB3metros bajo exhaustiva lluvia y \n\
  fr\xC3\xADo, a\xC3\xB1oraba a su querido cachorro.", el);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  el ping\xC3\xBCino wenceslao hizo kil\xC3\xB3metros bajo exhaustiva lluvia y \n\
  fr\xC3\xADo, a\xC3\xB1oraba a su querido cachorro.", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, FrenchFirstUppercase)
{
	std::string i = ReadRegular(1527, 361);
	EXPECT_UTF8EQ("  Portez ce vieux whisky au juge blond qui fume sur son \xC3\xAEle int\xC3\xA9rieure, \xC3\xA0\n\
  c\xC3\xB4t\xC3\xA9 de l'alc\xC3\xB4ve ovo\xC3\xAF" "de, o\xC3\xB9 les b\xC3\xBB" "ches se consument dans l'\xC3\xA2tre, ce\n\
  qui lui permet de penser \xC3\xA0 la c\xC3\xA6nogen\xC3\xA8se de l'\xC3\xAAtre dont il est question\n\
  dans la cause ambigu\xC3\xAB entendue \xC3\xA0 Mo\xC3\xBF, dans un capharna\xC3\xBCm qui,\n\
  pense-t-il, diminue \xC3\xA7\xC3\xA0 et l\xC3\xA0 la qualit\xC3\xA9 de son \xC5\x93uvre. ", i);

	std::string eu = ReadUppercase(1528, 361);
	EXPECT_UTF8EQ("  PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME SUR SON \xC3\x8ELE INT\xC3\x89RIEURE, \xC3\x80\n\
  C\xC3\x94T\xC3\x89 DE L'ALC\xC3\x94VE OVO\xC3\x8F" "DE, O\xC3\x99 LES B\xC3\x9B" "CHES SE CONSUMENT DANS L'\xC3\x82TRE, CE\n\
  QUI LUI PERMET DE PENSER \xC3\x80 LA C\xC3\x86NOGEN\xC3\x88SE DE L'\xC3\x8ATRE DONT IL EST QUESTION\n\
  DANS LA CAUSE AMBIGU\xC3\x8B ENTENDUE \xC3\x80 MO\xC5\xB8, DANS UN CAPHARNA\xC3\x9CM QUI,\n\
  PENSE-T-IL, DIMINUE \xC3\x87\xC3\x80 ET L\xC3\x80 LA QUALIT\xC3\x89 DE SON \xC5\x92UVRE. ", eu);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME SUR SON \xC3\x8ELE INT\xC3\x89RIEURE, \xC3\x80\n\
  C\xC3\x94T\xC3\x89 DE L'ALC\xC3\x94VE OVO\xC3\x8F" "DE, O\xC3\x99 LES B\xC3\x9B" "CHES SE CONSUMENT DANS L'\xC3\x82TRE, CE\n\
  QUI LUI PERMET DE PENSER \xC3\x80 LA C\xC3\x86NOGEN\xC3\x88SE DE L'\xC3\x8ATRE DONT IL EST QUESTION\n\
  DANS LA CAUSE AMBIGU\xC3\x8B ENTENDUE \xC3\x80 MO\xC5\xB8, DANS UN CAPHARNA\xC3\x9CM QUI,\n\
  PENSE-T-IL, DIMINUE \xC3\x87\xC3\x80 ET L\xC3\x80 LA QUALIT\xC3\x89 DE SON \xC5\x92UVRE. ", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, FrenchFirstLowercase)
{
	std::string i = ReadRegular(1527, 361);
	EXPECT_UTF8EQ("  Portez ce vieux whisky au juge blond qui fume sur son \xC3\xAEle int\xC3\xA9rieure, \xC3\xA0\n\
  c\xC3\xB4t\xC3\xA9 de l'alc\xC3\xB4ve ovo\xC3\xAF" "de, o\xC3\xB9 les b\xC3\xBB" "ches se consument dans l'\xC3\xA2tre, ce\n\
  qui lui permet de penser \xC3\xA0 la c\xC3\xA6nogen\xC3\xA8se de l'\xC3\xAAtre dont il est question\n\
  dans la cause ambigu\xC3\xAB entendue \xC3\xA0 Mo\xC3\xBF, dans un capharna\xC3\xBCm qui,\n\
  pense-t-il, diminue \xC3\xA7\xC3\xA0 et l\xC3\xA0 la qualit\xC3\xA9 de son \xC5\x93uvre. ", i);

	std::string el = ReadLowercase(1527, 361);
	EXPECT_UTF8EQ("  portez ce vieux whisky au juge blond qui fume sur son \xC3\xAEle int\xC3\xA9rieure, \xC3\xA0\n\
  c\xC3\xB4t\xC3\xA9 de l'alc\xC3\xB4ve ovo\xC3\xAF" "de, o\xC3\xB9 les b\xC3\xBB" "ches se consument dans l'\xC3\xA2tre, ce\n\
  qui lui permet de penser \xC3\xA0 la c\xC3\xA6nogen\xC3\xA8se de l'\xC3\xAAtre dont il est question\n\
  dans la cause ambigu\xC3\xAB entendue \xC3\xA0 mo\xC3\xBF, dans un capharna\xC3\xBCm qui,\n\
  pense-t-il, diminue \xC3\xA7\xC3\xA0 et l\xC3\xA0 la qualit\xC3\xA9 de son \xC5\x93uvre. ", el);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  portez ce vieux whisky au juge blond qui fume sur son \xC3\xAEle int\xC3\xA9rieure, \xC3\xA0\n\
  c\xC3\xB4t\xC3\xA9 de l'alc\xC3\xB4ve ovo\xC3\xAF" "de, o\xC3\xB9 les b\xC3\xBB" "ches se consument dans l'\xC3\xA2tre, ce\n\
  qui lui permet de penser \xC3\xA0 la c\xC3\xA6nogen\xC3\xA8se de l'\xC3\xAAtre dont il est question\n\
  dans la cause ambigu\xC3\xAB entendue \xC3\xA0 mo\xC3\xBF, dans un capharna\xC3\xBCm qui,\n\
  pense-t-il, diminue \xC3\xA7\xC3\xA0 et l\xC3\xA0 la qualit\xC3\xA9 de son \xC5\x93uvre. ", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, FrenchSecondUppercase)
{
	std::string i = ReadRegular(1890, 117);
	EXPECT_UTF8EQ("  l'\xC3\xAEle exigu\xC3\xAB\n  O\xC3\xB9 l'ob\xC3\xA8se jury m\xC3\xBBr\n\
  F\xC3\xAAte l'ha\xC3\xAF volap\xC3\xBCk,\n\
  \xC3\x82ne ex a\xC3\xA9quo au whist,\n\
  \xC3\x94tez ce v\xC5\x93u d\xC3\xA9\xC3\xA7u.", i);

	std::string eu = ReadUppercase(1891, 117);
	EXPECT_UTF8EQ("  L'\xC3\x8ELE EXIGU\xC3\x8B\n  O\xC3\x99 L'OB\xC3\x88SE JURY M\xC3\x9BR\n\
  F\xC3\x8ATE L'HA\xC3\x8F VOLAP\xC3\x9CK,\n\
  \xC3\x82NE EX A\xC3\x89QUO AU WHIST,\n\
  \xC3\x94TEZ CE V\xC5\x92U D\xC3\x89\xC3\x87U.", eu);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  L'\xC3\x8ELE EXIGU\xC3\x8B\n\
  O\xC3\x99 L'OB\xC3\x88SE JURY M\xC3\x9BR\n\
  F\xC3\x8ATE L'HA\xC3\x8F VOLAP\xC3\x9CK,\n\
  \xC3\x82NE EX A\xC3\x89QUO AU WHIST,\n\
  \xC3\x94TEZ CE V\xC5\x92U D\xC3\x89\xC3\x87U.", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, FrenchSecondLowercase)
{
	std::string i = ReadRegular(1890, 117);
	EXPECT_UTF8EQ("  l'\xC3\xAEle exigu\xC3\xAB\n\
  O\xC3\xB9 l'ob\xC3\xA8se jury m\xC3\xBBr\n\
  F\xC3\xAAte l'ha\xC3\xAF volap\xC3\xBCk,\n\
  \xC3\x82ne ex a\xC3\xA9quo au whist,\n\
  \xC3\x94tez ce v\xC5\x93u d\xC3\xA9\xC3\xA7u.", i);

	std::string el = ReadLowercase(1890, 117);
	EXPECT_UTF8EQ("  l'\xC3\xAEle exigu\xC3\xAB\n\
  o\xC3\xB9 l'ob\xC3\xA8se jury m\xC3\xBBr\n\
  f\xC3\xAAte l'ha\xC3\xAF volap\xC3\xBCk,\n\
  \xC3\xA2ne ex a\xC3\xA9quo au whist,\n\
  \xC3\xB4tez ce v\xC5\x93u d\xC3\xA9\xC3\xA7u.", el);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  l'\xC3\xAEle exigu\xC3\xAB\n\
  o\xC3\xB9 l'ob\xC3\xA8se jury m\xC3\xBBr\n\
  f\xC3\xAAte l'ha\xC3\xAF volap\xC3\xBCk,\n\
  \xC3\xA2ne ex a\xC3\xA9quo au whist,\n\
  \xC3\xB4tez ce v\xC5\x93u d\xC3\xA9\xC3\xA7u.", al);

	delete al;
}

TEST_F(QuickbrownCaseMapping, FrenchThirdUppercase)
{
	std::string i = ReadRegular(2009, 148);
	EXPECT_UTF8EQ("  Le c\xC5\x93ur d\xC3\xA9\xC3\xA7u mais l'\xC3\xA2me plut\xC3\xB4t na\xC3\xAFve, Lou\xC3\xBFs r\xC3\xAAva de crapa\xC3\xBCter en\n\
  cano\xC3\xAB au del\xC3\xA0 des \xC3\xAEles, pr\xC3\xA8s du m\xC3\xA4lstr\xC3\xB6m o\xC3\xB9 br\xC3\xBBlent les nov\xC3\xA6.", i);

	std::string eu = ReadUppercase(2010, 148);
	EXPECT_UTF8EQ("  LE C\xC5\x92UR D\xC3\x89\xC3\x87U MAIS L'\xC3\x82ME PLUT\xC3\x94T NA\xC3\x8FVE, LOU\xC5\xB8S R\xC3\x8AVA DE CRAPA\xC3\x9CTER EN\n\
  CANO\xC3\x8B AU DEL\xC3\x80 DES \xC3\x8ELES, PR\xC3\x88S DU M\xC3\x84LSTR\xC3\x96M O\xC3\x99 BR\xC3\x9BLENT LES NOV\xC3\x86.", eu);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  LE C\xC5\x92UR D\xC3\x89\xC3\x87U MAIS L'\xC3\x82ME PLUT\xC3\x94T NA\xC3\x8FVE, LOU\xC5\xB8S R\xC3\x8AVA DE CRAPA\xC3\x9CTER EN\n\
  CANO\xC3\x8B AU DEL\xC3\x80 DES \xC3\x8ELES, PR\xC3\x88S DU M\xC3\x84LSTR\xC3\x96M O\xC3\x99 BR\xC3\x9BLENT LES NOV\xC3\x86.", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, FrenchThirdLowercase)
{
	std::string i = ReadRegular(2009, 148);
	EXPECT_UTF8EQ("  Le c\xC5\x93ur d\xC3\xA9\xC3\xA7u mais l'\xC3\xA2me plut\xC3\xB4t na\xC3\xAFve, Lou\xC3\xBFs r\xC3\xAAva de crapa\xC3\xBCter en\n\
  cano\xC3\xAB au del\xC3\xA0 des \xC3\xAEles, pr\xC3\xA8s du m\xC3\xA4lstr\xC3\xB6m o\xC3\xB9 br\xC3\xBBlent les nov\xC3\xA6.", i);

	std::string el = ReadLowercase(2009, 148);
	EXPECT_UTF8EQ("  le c\xC5\x93ur d\xC3\xA9\xC3\xA7u mais l'\xC3\xA2me plut\xC3\xB4t na\xC3\xAFve, lou\xC3\xBFs r\xC3\xAAva de crapa\xC3\xBCter en\n\
  cano\xC3\xAB au del\xC3\xA0 des \xC3\xAEles, pr\xC3\xA8s du m\xC3\xA4lstr\xC3\xB6m o\xC3\xB9 br\xC3\xBBlent les nov\xC3\xA6.", el);

	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  le c\xC5\x93ur d\xC3\xA9\xC3\xA7u mais l'\xC3\xA2me plut\xC3\xB4t na\xC3\xAFve, lou\xC3\xBFs r\xC3\xAAva de crapa\xC3\xBCter en\n\
  cano\xC3\xAB au del\xC3\xA0 des \xC3\xAEles, pr\xC3\xA8s du m\xC3\xA4lstr\xC3\xB6m o\xC3\xB9 br\xC3\xBBlent les nov\xC3\xA6.", al);

	delete al;
}