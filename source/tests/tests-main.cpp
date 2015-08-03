#include "tests-base.hpp"

#include "utf8rewind.h"

int main(int argc, char** argv)
{
	const char* input = "Hello World!";

	static const size_t output_size = 256;
	char output[output_size];
	wchar_t output_wide[output_size];
	const char* input_seek;
	size_t converted_size;
	int32_t errors;

	memset(output, 0, output_size * sizeof(char));
	memset(output_wide, 0, output_size * sizeof(wchar_t));

	/*
		Convert input to uppercase:

		"Hello World!" -> "HELLO WORLD!"
	*/

	converted_size = utf8toupper(
		input, strlen(input),
		output, output_size - 1,
		&errors);
	if (converted_size == 0 ||
		errors != UTF8_ERR_NONE)
	{
		return -1;
	}

	/*
		Convert UTF-8 input to wide (UTF-16 or UTF-32) encoded text:

		"HELLO WORLD!" -> L"HELLO WORLD!"
	*/

	converted_size = utf8towide(
		output, strlen(output),
		output_wide, (output_size - 1) * sizeof(wchar_t),
		&errors);
	if (converted_size == 0 ||
		errors != UTF8_ERR_NONE)
	{
		return -1;
	}

	/*
		Seek in input:

		"Hello World!" -> "World!"
	*/

	input_seek = utf8seek(input, input, 6, SEEK_SET);

	::testing::InitGoogleTest(&argc, argv);

	int result = RUN_ALL_TESTS();

	return result;
}