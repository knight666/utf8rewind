#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/streaming.h"
}

#include "helpers-normalization.hpp"
#include "helpers-strings.hpp"

TEST(Streaming, Initialized)
{
	const char* i = "loneliness";
	size_t il = strlen(i);

	StreamState state;
	stream_initialize(&state, &i, &il, UnicodeProperty_Normalization_Compose);

	EXPECT_EQ(i, *state.src);
	EXPECT_EQ(&il, state.src_size);
	EXPECT_EQ(UnicodeProperty_Normalization_Compose, state.property);
	EXPECT_EQ(0, state.current);
	EXPECT_EQ(1, state.stable);
	EXPECT_EQ(ReorderResult_Next, state.stage);
}

TEST(Streaming, TwoFlushes)
{
	unicode_t input[] = { 0x0061, 0x0315, 0x0300, 0x05AE, 0x0300, 0x0062 };
	std::string converted = helpers::utf8(input, sizeof(input));

	const char* i = converted.c_str();
	size_t il = converted.length();

	StreamState state;
	stream_initialize(&state, &i, &il, UnicodeProperty_Normalization_Compose);

	stream_execute(&state);
	EXPECT_EQ(5, state.current);
	EXPECT_UTF8EQ("a\\u5AE\\u300\\u300\\u315", helpers::identifiable(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	stream_execute(&state);
	EXPECT_EQ(1, state.current);
	EXPECT_UTF8EQ("b", helpers::identifiable(state.codepoint, state.current * sizeof(unicode_t)).c_str());
}