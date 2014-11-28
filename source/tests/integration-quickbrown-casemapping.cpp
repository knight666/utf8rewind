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
	EXPECT_STREQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  Wolther spillede p\xc3\xa5 xylofon.", i.c_str());

	std::string au = helpers::uppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(271, 95);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, DanishLowercase)
{
	std::string i = ReadRegular(271, 95);
	EXPECT_STREQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  Wolther spillede p\xc3\xa5 xylofon.", i.c_str());

	std::string al = helpers::lowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(271, 95);
	EXPECT_STREQ(el.c_str(), al.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanFirstUppercase)
{
	std::string i = ReadRegular(503, 64);
	EXPECT_STREQ("  Falsches \xC3\x9C" "ben von Xylophonmusik qu\xC3\xA4lt jeden gr\xC3\xB6\xC3\x9F" "eren Zwerg", i.c_str());

	std::string au = helpers::uppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(503, 64);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanFirstLowercase)
{
	std::string i = ReadRegular(503, 64);
	EXPECT_STREQ("  Falsches \xC3\x9C" "ben von Xylophonmusik qu\xC3\xA4lt jeden gr\xC3\xB6\xC3\x9F" "eren Zwerg", i.c_str());

	std::string al = helpers::lowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(503, 64);
	EXPECT_STREQ(el.c_str(), al.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanSecondUppercase)
{
	std::string i = ReadRegular(642, 59);
	EXPECT_STREQ("  Zw\xC3\xB6lf Boxk\xC3\xA4mpfer jagten Eva quer \xC3\xBC" "ber den Sylter Deich", i.c_str());

	std::string au = helpers::uppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(642, 59);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanSecondLowercase)
{
	std::string i = ReadRegular(642, 59);
	EXPECT_STREQ("  Zw\xC3\xB6lf Boxk\xC3\xA4mpfer jagten Eva quer \xC3\xBC" "ber den Sylter Deich", i.c_str());

	std::string al = helpers::lowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(642, 59);
	EXPECT_STREQ(el.c_str(), al.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanThirdUppercase)
{
	std::string i = ReadRegular(767, 30);
	EXPECT_STREQ("  Heiz\xC3\xB6lr\xC3\xBC" "cksto\xC3\x9F" "abd\xC3\xA4mpfung", i.c_str());

	std::string au = helpers::uppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(767, 30);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanThirdLowercase)
{
	std::string i = ReadRegular(767, 30);
	EXPECT_STREQ("  Heiz\xC3\xB6lr\xC3\xBC" "cksto\xC3\x9F" "abd\xC3\xA4mpfung", i.c_str());

	std::string al = helpers::lowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(767, 30);
	EXPECT_STREQ(el.c_str(), al.c_str());
}

TEST_F(QuickbrownCaseMapping, GreekFirstUppercase)
{
	std::string i = ReadRegular(911, 105);
	EXPECT_STREQ("  \xCE\x93\xCE\xB1\xCE\xB6\xCE\xAD\xCE\xB5\xCF\x82 \xCE\xBA\xCE\xB1\xE1\xBD\xB6 \xCE\xBC\xCF\x85\xCF\x81\xCF\x84\xCE\xB9\xE1\xBD\xB2\xCF\x82 \xCE\xB4\xE1\xBD\xB2\xCE\xBD \xCE\xB8\xE1\xBD\xB0 \xCE\xB2\xCF\x81\xE1\xBF\xB6 \xCF\x80\xCE\xB9\xE1\xBD\xB0 \xCF\x83\xCF\x84\xE1\xBD\xB8 \xCF\x87\xCF\x81\xCF\x85\xCF\x83\xCE\xB1\xCF\x86\xE1\xBD\xB6 \xCE\xBE\xCE\xAD\xCF\x86\xCF\x89\xCF\x84\xCE\xBF", i.c_str());

	std::string au = helpers::uppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(911, 106);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, GreekFirstLowercase)
{
	std::string i = ReadRegular(911, 105);
	EXPECT_STREQ("  \xCE\x93\xCE\xB1\xCE\xB6\xCE\xAD\xCE\xB5\xCF\x82 \xCE\xBA\xCE\xB1\xE1\xBD\xB6 \xCE\xBC\xCF\x85\xCF\x81\xCF\x84\xCE\xB9\xE1\xBD\xB2\xCF\x82 \xCE\xB4\xE1\xBD\xB2\xCE\xBD \xCE\xB8\xE1\xBD\xB0 \xCE\xB2\xCF\x81\xE1\xBF\xB6 \xCF\x80\xCE\xB9\xE1\xBD\xB0 \xCF\x83\xCF\x84\xE1\xBD\xB8 \xCF\x87\xCF\x81\xCF\x85\xCF\x83\xCE\xB1\xCF\x86\xE1\xBD\xB6 \xCE\xBE\xCE\xAD\xCF\x86\xCF\x89\xCF\x84\xCE\xBF", i.c_str());

	std::string al = helpers::lowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(911, 105);
	EXPECT_STREQ(el.c_str(), al.c_str());
}