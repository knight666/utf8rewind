#include "tests-base.hpp"

#include "utf8rewind.h"

void Font_RenderCodepoint(unicode_t codepoint)
{

}

void Font_RenderTextNormalized(const char* input)
{
	const char* src = NULL;
	const char* src_start;
	size_t src_size;
	char* converted = NULL;
	size_t converted_size = 0;
	size_t input_size = strlen(input);

	if (utf8isnormalized(input, input_size, UTF8_NORMALIZE_COMPOSE, NULL) != UTF8_NORMALIZATION_RESULT_YES)
	{
		int32_t errors;

		converted_size = utf8normalize(input, input_size, NULL, 0, UTF8_NORMALIZE_COMPOSE, &errors);
		if (converted_size > 0 &&
			errors == UTF8_ERR_NONE)
		{
			converted = (char*)malloc(converted_size);
			utf8normalize(input, input_size, converted, converted_size, UTF8_NORMALIZE_COMPOSE, NULL);
			converted[converted_size] = 0;

			src = (const char*)converted;
			src_size = converted_size;
		}
	}

	if (src == NULL)
	{
		src = (const char*)input;
		src_size = input_size;
	}

	src_start = src;

	while (src_size > 0)
	{
		const char* next;
		int32_t errors;

		next = utf8seek(src, src_start, 1, SEEK_CUR);
		if (next == src)
		{
			break;
		}

		unicode_t codepoint;
		utf8toutf32(src, (size_t)(next - src), &codepoint, sizeof(unicode_t), &errors);
		if (errors != UTF8_ERR_NONE)
		{
			break;
		}

		Font_RenderCodepoint(codepoint);

		src_size -= next - src;
		src = next;
	}

	if (converted != NULL)
	{
		free(converted);
		converted = NULL;
	}
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	Font_RenderTextNormalized("\xE3\x82\xB1\xE3\x82\x99");

	int result = RUN_ALL_TESTS();

	return result;
}