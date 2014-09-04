#include "tests-base.hpp"

#include "utf8rewind.h"

class ConformanceSuite
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		errors = 0;
		input = nullptr;
		input_size = 0;
		output = nullptr;

		// load file into memory

		file.open("testdata/UTF-8-test.txt", std::ios_base::in);
		ASSERT_TRUE(file.is_open());

		file.seekg(0, std::ios::end);
		input_size = (size_t)file.tellg();
		file.seekg(0, std::ios::beg);

		ASSERT_EQ(20337, input_size);

		input = new char[input_size];
		memset(input, 0, input_size);
		file.read(input, input_size);

		file.close();

		size_t decoded_size = 0;

		decoded_size = utf8toutf32(input, input_size, nullptr, 0, &errors);
		ASSERT_EQ(80628, decoded_size);
		ASSERT_EQ(0, errors);

		output = new unicode_t[decoded_size + 1];

		utf8toutf32(input, input_size, output, decoded_size, &errors);
		ASSERT_EQ(0, errors);

		output[decoded_size] = 0;
	}

	void TearDown()
	{
		if (input != nullptr)
		{
			delete [] input;
		}

		if (output != nullptr)
		{
			delete [] output;
		}
	}

	char* input;
	size_t input_size;
	unicode_t* output;
	size_t output_size;
	std::fstream file;
	int32_t errors;

};

TEST_F(ConformanceSuite, ReadTheEnd)
{
	unicode_t* the_end = output + 20077;
	EXPECT_EQ('T', the_end[0]);
	EXPECT_EQ('H', the_end[1]);
	EXPECT_EQ('E', the_end[2]);
	EXPECT_EQ(' ', the_end[3]);
	EXPECT_EQ('E', the_end[4]);
	EXPECT_EQ('N', the_end[5]);
	EXPECT_EQ('D', the_end[6]);
}