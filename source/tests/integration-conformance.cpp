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
		input_size = file.tellg();
		file.seekg(0, std::ios::beg);

		ASSERT_EQ(20337, input_size);

		input = new char[input_size];
		memset(input, 0, input_size);
		file.read(input, input_size);

		file.close();

		// output has to be stitched together due to null character in data

		size_t decoded_size = 0;

		output_size = 80624;
		output = new unicode_t[output_size + 1];

		const char* src = input;
		size_t src_size = input_size;
		unicode_t* dst = output;
		size_t dst_size = output_size;

		decoded_size = utf8toutf32(src, src_size, nullptr, 0, &errors);
		ASSERT_EQ(16436, decoded_size);
		ASSERT_EQ(0, errors);

		utf8toutf32(src, src_size, dst, dst_size, &errors);
		ASSERT_EQ(0, errors);

		src += 4118;
		src_size -= 4118;
		dst += decoded_size / sizeof(unicode_t);
		dst_size -= decoded_size;

		decoded_size = utf8toutf32(src, src_size, nullptr, 0, &errors);
		ASSERT_EQ(64188, decoded_size);
		ASSERT_EQ(0, errors);

		utf8toutf32(src, src_size, dst, dst_size, &errors);
		ASSERT_EQ(0, errors);
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
	unicode_t* the_end = output + 20076;
	EXPECT_EQ('T', the_end[0]);
	EXPECT_EQ('H', the_end[1]);
	EXPECT_EQ('E', the_end[2]);
	EXPECT_EQ(' ', the_end[3]);
	EXPECT_EQ('E', the_end[4]);
	EXPECT_EQ('N', the_end[5]);
	EXPECT_EQ('D', the_end[6]);
}