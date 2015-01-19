#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/composition.h"
}

TEST(ComposeExecute, Initialized)
{
	const char* i = "other";
	size_t il = strlen(i);

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(i, state.streaming.src);
	EXPECT_EQ(il, state.streaming.src_size);
	EXPECT_EQ(UnicodeProperty_Normalization_Compose, state.streaming.property);
	EXPECT_EQ(0, state.current);
	EXPECT_EQ(1, state.next);
}

TEST(ComposeExecute, SingleNoChange)
{
	const char* i = "\xE1\xB8\x8A";
	size_t il = strlen(i);

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(0x1E0A, compose_execute(&state));

	EXPECT_EQ(0, compose_execute(&state));
}