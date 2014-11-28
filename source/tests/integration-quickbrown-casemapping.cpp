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
		fileUppercase.close();
		fileLowercase.close();
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
	std::string regular = ReadRegular(271, 95);
	EXPECT_STREQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  Wolther spillede p\xc3\xa5 xylofon.", regular.c_str());

	size_t l = utf8toupper(regular.c_str(), regular.size() - 1, nullptr, 0, &errors);
	EXPECT_EQ(95, l);
	ASSERT_EQ(0, errors);

	std::string converted;
	converted.resize(l + 1);
	utf8toupper(regular.c_str(), regular.size() - 1, &converted[0], l, &errors);

	std::string expected = ReadUppercase(271, 95);

	EXPECT_STREQ(expected.c_str(), converted.c_str());
}

TEST_F(QuickbrownCaseMapping, DanishLowercase)
{
	std::string regular = ReadRegular(271, 95);
	EXPECT_STREQ("  Quizdeltagerne spiste jordb\xc3\xa6r med fl\xc3\xb8" "de, mens cirkusklovnen\x0a  Wolther spillede p\xc3\xa5 xylofon.", regular.c_str());

	size_t l = utf8tolower(regular.c_str(), regular.size() - 1, nullptr, 0, &errors);
	EXPECT_EQ(95, l);
	ASSERT_EQ(0, errors);

	std::string converted;
	converted.resize(l + 1);
	utf8tolower(regular.c_str(), regular.size() - 1, &converted[0], l, &errors);

	std::string expected = ReadLowercase(271, 95);

	EXPECT_STREQ(expected.c_str(), converted.c_str());
}