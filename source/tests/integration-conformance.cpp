#include "tests-base.hpp"

#include "utf8rewind.h"

class ConformanceSuite
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		errors = 0;
		output = nullptr;

		// load into file

		file.open("testdata/UTF-8-test.txt", std::ios_base::in);
		ASSERT_TRUE(file.is_open());

		input = new char[24000];
		memset(input, 0, 24000);
		file.read(input, 24000);

		size_t length = strlen(input);

		file.close();
	}

	void TearDown()
	{
		delete [] input;

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
	wchar_t* output;
	unicode_t codepoint;
	std::fstream file;
	int32_t errors;

};

TEST_F(ConformanceSuite, CorrectlyEncoded)
{
	ReadSection(3640, 11);
	EXPECT_STREQ(L"\x3BA\x1F79\x3C3\x3BC\x3B5", output);
}

TEST_F(ConformanceSuite, FirstPossibleSequence)
{
	codepoint = 0;
	utf8toutf32(input + 4117, 1, &codepoint, sizeof(unicode_t), &errors);
	ASSERT_EQ(0, errors);
	EXPECT_EQ(0, codepoint);

	ReadCodepoint(4197, 2);
	EXPECT_EQ(0x80, codepoint);

	ReadCodepoint(4278, 3);
	EXPECT_EQ(0x800, codepoint);

	ReadCodepoint(4360, 4);
	EXPECT_EQ(0x10000, codepoint);

	ReadCodepoint(4443, 5);
	EXPECT_EQ(0x200000, codepoint);

	ReadCodepoint(4527, 6);
	EXPECT_EQ(0x4000000, codepoint);
}

TEST_F(ConformanceSuite, LastPossibleSequence)
{
	ReadCodepoint(4852, 1);
	EXPECT_EQ(0x7F, codepoint);

	ReadCodepoint(4932, 2);
	EXPECT_EQ(0x7FF, codepoint);
}