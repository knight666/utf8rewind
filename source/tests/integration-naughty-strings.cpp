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
}