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

		filei.open("testdata/quickbrown.txt", std::ios_base::in);
		ASSERT_TRUE(filei.is_open());

		fileUppercase.open("testdata/quickbrown_uppercase.txt", std::ios_base::in);
		ASSERT_TRUE(fileUppercase.is_open());

		fileLowercase.open("testdata/quickbrown_lowercase.txt", std::ios_base::in);
		ASSERT_TRUE(fileLowercase.is_open());
	}

	void TearDown()
	{
		fileUppercase.close();
		fileLowercase.close();
		filei.close();
	}

	std::string ReadRegular(size_t position, size_t length)
	{
		std::string result;

		filei.seekg(position, std::ios::beg);
		if (filei.eof())
		{
			return result;
		}

		result.resize(length + 1);
		filei.read(&result[0], length);

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

	std::string ConvertToUppercase(const std::string& value)
	{
		std::string converted;

		size_t l = utf8toupper(value.c_str(), value.size() - 1, nullptr, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8toupper(value.c_str(), value.size() - 1, &converted[0], l, &errors);

		return converted;
	}

	std::string ConvertToLowercase(const std::string& value)
	{
		std::string converted;

		size_t l = utf8tolower(value.c_str(), value.size() - 1, nullptr, 0, &errors);
		if (l == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(l + 1);
		utf8tolower(value.c_str(), value.size() - 1, &converted[0], l, &errors);

		return converted;
	}

	std::fstream filei;
	std::fstream fileUppercase;
	std::fstream fileLowercase;
	int32_t errors;

};

TEST_F(QuickbrownCaseMapping, DanishUppercase)
{
	std::string i = ReadRegular(271, 95);
	EXPECT_STREQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  Wolther spillede p\xc3\xa5 xylofon.", i.c_str());

	std::string au = ConvertToUppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(271, 95);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, DanishLowercase)
{
	std::string i = ReadRegular(271, 95);
	EXPECT_STREQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  Wolther spillede p\xc3\xa5 xylofon.", i.c_str());

	std::string al = ConvertToLowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(271, 95);
	EXPECT_STREQ(el.c_str(), al.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanFirstUppercase)
{
	std::string i = ReadRegular(503, 64);
	EXPECT_STREQ("  Falsches \xC3\x9C" "ben von Xylophonmusik qu\xC3\xA4lt jeden gr\xC3\xB6\xC3\x9F" "eren Zwerg", i.c_str());

	std::string au = ConvertToUppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(503, 64);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanFirstLowercase)
{
	std::string i = ReadRegular(503, 64);
	EXPECT_STREQ("  Falsches \xC3\x9C" "ben von Xylophonmusik qu\xC3\xA4lt jeden gr\xC3\xB6\xC3\x9F" "eren Zwerg", i.c_str());

	std::string al = ConvertToLowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(503, 64);
	EXPECT_STREQ(el.c_str(), al.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanSecondUppercase)
{
	std::string i = ReadRegular(642, 59);
	EXPECT_STREQ("  Zw\xC3\xB6lf Boxk\xC3\xA4mpfer jagten Eva quer \xC3\xBC" "ber den Sylter Deich", i.c_str());

	std::string au = ConvertToUppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(642, 59);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanSecondLowercase)
{
	std::string i = ReadRegular(642, 59);
	EXPECT_STREQ("  Zw\xC3\xB6lf Boxk\xC3\xA4mpfer jagten Eva quer \xC3\xBC" "ber den Sylter Deich", i.c_str());

	std::string al = ConvertToLowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(642, 59);
	EXPECT_STREQ(el.c_str(), al.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanThirdUppercase)
{
	std::string i = ReadRegular(767, 30);
	EXPECT_STREQ("  Heiz\xC3\xB6lr\xC3\xBC" "cksto\xC3\x9F" "abd\xC3\xA4mpfung", i.c_str());

	std::string au = ConvertToUppercase(i);
	ASSERT_EQ(0, errors);

	std::string eu = ReadUppercase(767, 30);
	EXPECT_STREQ(eu.c_str(), au.c_str());
}

TEST_F(QuickbrownCaseMapping, GermanThirdLowercase)
{
	std::string i = ReadRegular(767, 30);
	EXPECT_STREQ("  Heiz\xC3\xB6lr\xC3\xBC" "cksto\xC3\x9F" "abd\xC3\xA4mpfung", i.c_str());

	std::string al = ConvertToLowercase(i);
	ASSERT_EQ(0, errors);

	std::string el = ReadLowercase(767, 30);
	EXPECT_STREQ(el.c_str(), al.c_str());
}