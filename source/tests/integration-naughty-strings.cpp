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

TEST_F(NaughtyStrings, NumericStrings)
{
	EXPECT_STREQ(L"0", helpers::wide(ReadSection(202, 1)).c_str());
	EXPECT_STREQ(L"1", helpers::wide(ReadSection(204, 1)).c_str());
	EXPECT_STREQ(L"1.00", helpers::wide(ReadSection(206, 4)).c_str());
	EXPECT_STREQ(L"$1.00", helpers::wide(ReadSection(211, 5)).c_str());
	EXPECT_STREQ(L"1/2", helpers::wide(ReadSection(217, 3)).c_str());
	EXPECT_STREQ(L"1E2", helpers::wide(ReadSection(221, 3)).c_str());
	EXPECT_STREQ(L"1E02", helpers::wide(ReadSection(225, 4)).c_str());
	EXPECT_STREQ(L"1E+02", helpers::wide(ReadSection(230, 5)).c_str());
	EXPECT_STREQ(L"-1", helpers::wide(ReadSection(236, 2)).c_str());
	EXPECT_STREQ(L"-1.00", helpers::wide(ReadSection(239, 5)).c_str());
	EXPECT_STREQ(L"-$1.00", helpers::wide(ReadSection(245, 6)).c_str());
	EXPECT_STREQ(L"-1/2", helpers::wide(ReadSection(252, 4)).c_str());
	EXPECT_STREQ(L"-1E2", helpers::wide(ReadSection(257, 4)).c_str());
	EXPECT_STREQ(L"-1E02", helpers::wide(ReadSection(262, 5)).c_str());
	EXPECT_STREQ(L"-1E+02", helpers::wide(ReadSection(268, 6)).c_str());
	EXPECT_STREQ(L"1/0", helpers::wide(ReadSection(275, 3)).c_str());
	EXPECT_STREQ(L"0/0", helpers::wide(ReadSection(279, 3)).c_str());
	EXPECT_STREQ(L"-2147483648/-1", helpers::wide(ReadSection(283, 14)).c_str());
	EXPECT_STREQ(L"-9223372036854775808/-1", helpers::wide(ReadSection(298, 23)).c_str());
	EXPECT_STREQ(L"0.00", helpers::wide(ReadSection(322, 4)).c_str());
	EXPECT_STREQ(L"0..0", helpers::wide(ReadSection(327, 4)).c_str());
	EXPECT_STREQ(L".", helpers::wide(ReadSection(332, 1)).c_str());
	EXPECT_STREQ(L"0.0.0", helpers::wide(ReadSection(334, 5)).c_str());
	EXPECT_STREQ(L"0,00", helpers::wide(ReadSection(340, 4)).c_str());
	EXPECT_STREQ(L"0,,0", helpers::wide(ReadSection(345, 4)).c_str());
	EXPECT_STREQ(L",", helpers::wide(ReadSection(350, 1)).c_str());
	EXPECT_STREQ(L"0,0", helpers::wide(ReadSection(352, 3)).c_str());
	EXPECT_STREQ(L"0,0,0", helpers::wide(ReadSection(352, 5)).c_str());
	EXPECT_STREQ(L"0.0/0", helpers::wide(ReadSection(358, 5)).c_str());
	EXPECT_STREQ(L"1.0/0.0", helpers::wide(ReadSection(364, 7)).c_str());
	EXPECT_STREQ(L"0.0/0.0", helpers::wide(ReadSection(372, 7)).c_str());
	EXPECT_STREQ(L"1,0/0,0", helpers::wide(ReadSection(380, 7)).c_str());
	EXPECT_STREQ(L"0,0/0,0", helpers::wide(ReadSection(388, 7)).c_str());
	EXPECT_STREQ(L"--1", helpers::wide(ReadSection(396, 3)).c_str());
	EXPECT_STREQ(L"-", helpers::wide(ReadSection(400, 1)).c_str());
	EXPECT_STREQ(L"-.", helpers::wide(ReadSection(402, 2)).c_str());
	EXPECT_STREQ(L"-,", helpers::wide(ReadSection(405, 2)).c_str());
	EXPECT_STREQ(L"999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999", helpers::wide(ReadSection(408, 96)).c_str());
	EXPECT_STREQ(L"NaN", helpers::wide(ReadSection(505, 3)).c_str());
	EXPECT_STREQ(L"Infinity", helpers::wide(ReadSection(509, 8)).c_str());
	EXPECT_STREQ(L"-Infinity", helpers::wide(ReadSection(518, 9)).c_str());
	EXPECT_STREQ(L"0x0", helpers::wide(ReadSection(528, 3)).c_str());
	EXPECT_STREQ(L"0xffffffff", helpers::wide(ReadSection(532, 10)).c_str());
	EXPECT_STREQ(L"0xffffffffffffffff", helpers::wide(ReadSection(543, 18)).c_str());
	EXPECT_STREQ(L"0xabad1dea", helpers::wide(ReadSection(562, 10)).c_str());
	EXPECT_STREQ(L"123456789012345678901234567890123456789", helpers::wide(ReadSection(573, 39)).c_str());
}