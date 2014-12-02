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
	EXPECT_UTF8EQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  Wolther spillede p\xc3\xa5 xylofon.", i.c_str());

	std::string eu = ReadUppercase(271, 95);
	EXPECT_UTF8EQ("  QUIZDELTAGERNE SPISTE JORDB\xc3\x86R MED FL\xc3\x98" "DE, MENS CIRKUSKLOVNEN\x0a  WOLTHER SPILLEDE P\xc3\x85 XYLOFON.", eu.c_str());
	
	size_t l = utf8toupper(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* au = new char[l + 1];
	utf8toupper(i.c_str(), i.size() - 1, au, l, &errors);
	au[l] = 0;

	EXPECT_UTF8EQ("  QUIZDELTAGERNE SPISTE JORDB\xc3\x86R MED FL\xc3\x98" "DE, MENS CIRKUSKLOVNEN\x0a  WOLTHER SPILLEDE P\xc3\x85 XYLOFON.", au);

	delete au;
}

TEST_F(QuickbrownCaseMapping, DanishLowercase)
{
	std::string i = ReadRegular(271, 95);
	EXPECT_UTF8EQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  Wolther spillede p\xc3\xa5 xylofon.", i.c_str());

	std::string el = ReadLowercase(271, 95);
	EXPECT_UTF8EQ("  quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  wolther spillede p\xc3\xa5 xylofon.", el.c_str());

	size_t l = utf8tolower(i.c_str(), i.size() - 1, nullptr, 0, &errors);
	EXPECT_NE(0, l);
	ASSERT_EQ(0, errors);

	char* al = new char[l + 1];
	utf8tolower(i.c_str(), i.size() - 1, al, l, &errors);
	al[l] = 0;

	EXPECT_UTF8EQ("  quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  wolther spillede p\xc3\xa5 xylofon.", al);

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