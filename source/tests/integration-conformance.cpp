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

	void ReadSection(size_t position, size_t length)
	{
		ASSERT_TRUE(position < 20377 && position + length < 20377);

		size_t decoded_size = utf8towide(input + position, length, nullptr, 0, &errors);
		ASSERT_TRUE(decoded_size > 0);
		ASSERT_EQ(0, errors);

		if (output != nullptr)
		{
			delete [] output;
		}

		size_t output_length = decoded_size / sizeof(wchar_t);

		output = new wchar_t[output_length + 1];
		output[output_length] = 0;

		utf8towide(input + position, length, output, decoded_size, &errors);
		ASSERT_EQ(0, errors);
	}

	void ReadCodepoint(size_t position, size_t length)
	{
		ASSERT_TRUE(position < 20377 && position + length < 20377);

		codepoint = 0;

		size_t decoded_size = utf8toutf32(input + position, length, &codepoint, sizeof(unicode_t), &errors);
		ASSERT_EQ(4, decoded_size);
		ASSERT_EQ(0, errors);
	}

	char* input;
	size_t input_size;
	wchar_t* output;
	unicode_t codepoint;
	std::fstream file;
	int32_t errors;

};

TEST_F(ConformanceSuite, ConvertAll)
{
	// output has to be stitched together due to null character in data

	size_t decoded_size = 0;

	size_t total_size = 80624;
	unicode_t* decoded = new unicode_t[total_size + 1];

	const char* src = input;
	size_t src_size = input_size;
	unicode_t* dst = decoded;
	size_t dst_size = total_size;

	decoded_size = utf8toutf32(src, src_size, nullptr, 0, &errors);
	ASSERT_EQ(16436, decoded_size);
	ASSERT_EQ(0, errors);

	utf8toutf32(src, src_size, dst, dst_size, &errors);
	ASSERT_EQ(0, errors);

	src += 4118;
	src_size -= 4118;
	dst += (decoded_size / sizeof(unicode_t));
	dst_size -= decoded_size;

	decoded_size = utf8toutf32(src, src_size, nullptr, 0, &errors);
	ASSERT_EQ(64188, decoded_size);
	ASSERT_EQ(0, errors);

	utf8toutf32(src, src_size, dst, dst_size, &errors);
	ASSERT_EQ(0, errors);

	unicode_t* the_end = dst + 15967;
	EXPECT_EQ('T', the_end[0]);
	EXPECT_EQ('H', the_end[1]);
	EXPECT_EQ('E', the_end[2]);
	EXPECT_EQ(' ', the_end[3]);
	EXPECT_EQ('E', the_end[4]);
	EXPECT_EQ('N', the_end[5]);
	EXPECT_EQ('D', the_end[6]);

	int i = 0;
	//decoded[decoded_size] = 0;
}