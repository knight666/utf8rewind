#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/decomposition.h"
}

#include "helpers-strings.hpp"

TEST(DecomposeExecute, Decompose)
{
	const char* i = "\xC3\x9A";
	size_t il = strlen(i);

	StreamState stream;
	EXPECT_EQ(1, stream_initialize(&stream, i, il, UnicodeProperty_Normalization_Decompose));

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &stream));

	EXPECT_CPEQ(2, decompose_execute(&state));
	EXPECT_UTF8EQ("U\xCC\x81", helpers::utf8(state.codepoint, state.filled * sizeof(unicode_t)).c_str());

	EXPECT_CPEQ(0, decompose_execute(&state));
}