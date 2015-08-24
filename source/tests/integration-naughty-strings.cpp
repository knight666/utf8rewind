#include "tests-base.hpp"

#include "../helpers/helpers-strings.hpp"

#define NAUGHTY_STRINGS_LENGTH 10370

class NaughtyStrings
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		file.open("testdata/big-list-of-naughty-strings-master/blns.txt", std::ios_base::in);
		ASSERT_TRUE(file.is_open());
	}

	void TearDown()
	{
		file.close();
	}

	std::string ReadSection(size_t position, size_t length)
	{
		std::string result;

		file.seekg(position, std::ios::beg);
		if (file.eof())
		{
			return result;
		}

		result.resize(length + 1);
		file.read(&result[0], length);

		return result;
	}

	std::fstream file;

};

TEST_F(NaughtyStrings, ReservedStrings)
{
	EXPECT_STREQ(L"undefined", helpers::wide(ReadSection(68, 9)).c_str());
	EXPECT_STREQ(L"undef", helpers::wide(ReadSection(78, 5)).c_str());
	EXPECT_STREQ(L"null", helpers::wide(ReadSection(84, 4)).c_str());
	EXPECT_STREQ(L"NULL", helpers::wide(ReadSection(89, 4)).c_str());
	EXPECT_STREQ(L"nil", helpers::wide(ReadSection(94, 3)).c_str());
	EXPECT_STREQ(L"NIL", helpers::wide(ReadSection(98, 3)).c_str());
	EXPECT_STREQ(L"true", helpers::wide(ReadSection(102, 4)).c_str());
	EXPECT_STREQ(L"false", helpers::wide(ReadSection(107, 5)).c_str());
	EXPECT_STREQ(L"True", helpers::wide(ReadSection(113, 4)).c_str());
	EXPECT_STREQ(L"False", helpers::wide(ReadSection(118, 5)).c_str());
	EXPECT_STREQ(L"None", helpers::wide(ReadSection(124, 4)).c_str());
	EXPECT_STREQ(L"\\", helpers::wide(ReadSection(129, 1)).c_str());
	EXPECT_STREQ(L"\\\\", helpers::wide(ReadSection(131, 2)).c_str());
}