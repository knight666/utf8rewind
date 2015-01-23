#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/composition.h"
}

#include "helpers-strings.hpp"

TEST(ComposeExecute, Initialize)
{
	const char* i = "\xCE\xB9\xCC\x88";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_EQ(i, state.input->src);
	EXPECT_EQ(il, state.input->src_size);
	EXPECT_EQ(UnicodeProperty_Normalization_Compose, state.input->property);
	EXPECT_EQ(0, state.buffer_current);
}

TEST(ComposeExecute, Unchanged)
{
	const char* i = "\xE1\xB8\x8A";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1E0A, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, Compose)
{
	const char* i = "E\xCC\x84";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0112, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, ComposeMultipleCodepoints)
{
	const char* i = "\xCE\xA9\xCC\x94\xCD\x82\xCD\x85";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1FAF, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, ComposeInvalidCodepoint)
{
	const char* i = "\xCF";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xFFFD, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, ComposeAndUnchanged)
{
	const char* i = "\xCF\x89\xCC\x81\xE1\xB6\xA8";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x03CE, compose_execute(&state));
	EXPECT_CPEQ(0x1DA8, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, UnchangedAndCompose)
{
	const char* i = "\xC5\xBF\xE1\x84\x80\xE1\x85\xA9\xE1\x86\xB3";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x017F, compose_execute(&state));
	EXPECT_CPEQ(0xACEC, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, MultipleUnchanged)
{
	const char* i = "\xC4\x92\xE1\xB8\x94\xCC\x84\xC3\x80";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0112, compose_execute(&state));
	EXPECT_CPEQ(0x1E14, compose_execute(&state));
	EXPECT_CPEQ(0x0304, compose_execute(&state));
	EXPECT_CPEQ(0x00C0, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, MultipleComposed)
{
	const char* i = "\xCE\xB1\xCD\x85\xCE\xB7\xCD\x82\xE2\x8A\x91\xCC\xB8";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1FB3, compose_execute(&state));
	EXPECT_CPEQ(0x1FC6, compose_execute(&state));
	EXPECT_CPEQ(0x22E2, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, ContinueAfterEnd)
{
	const char* i = "\xE3\x8E\x9E.";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x339E, compose_execute(&state));
	EXPECT_CPEQ(0x002E, compose_execute(&state));

	EXPECT_CPEQ(0, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, NotEnoughData)
{
	const char* i = "";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(0, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(0, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, InvalidData)
{
	const char* i = nullptr;
	size_t il = 5;

	StreamState input;
	EXPECT_EQ(0, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(0, compose_initialize(&state, &input, 0));

	EXPECT_EQ(nullptr, state.input);

	EXPECT_CPEQ(0, compose_execute(&state));
}