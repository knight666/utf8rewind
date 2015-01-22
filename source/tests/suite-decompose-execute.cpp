#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/decomposition.h"
}

#include "helpers-streams.hpp"
#include "helpers-strings.hpp"

TEST(DecomposeExecute, Initialize)
{
	const char* i = "\xE3\x81\x8B\xE3\x82\x99";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(i, state.input->src);
	EXPECT_EQ(il, state.input->src_size);
	EXPECT_EQ(UnicodeProperty_Normalization_Compose, state.input->property);
	EXPECT_EQ(UnicodeProperty_Normalization_Decompose, state.output->property);
	EXPECT_EQ(0, state.output->current);
}

TEST(DecomposeExecute, SingleUnaffected)
{
	const char* i = "\xC2\xA0";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x00A0, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleDecompose)
{
	const char* i = "\xC3\x9A";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(2, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0055, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0301, Yes, 230);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleNonStarter)
{
	const char* i = "\xCC\x84";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0304, Yes, 230);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleSequenceOrdered)
{
	const char* i = "\xC4\x80\xCC\x88";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0041, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0304, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x0308, Yes, 230);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleSequenceOutOfOrder)
{
	const char* i = "A\xCC\x84\xCC\x9D";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0041, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0304, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x031D, Yes, 220);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleUnaffected)
{
	const char* i = "\xE8\x90\xBD\xE9\xB9\xBF\xE2\x85\xB6\xE2\x85\xAC";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x843D, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x9E7F, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x2176, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x216C, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleDecompose)
{
	const char* i = "\xE3\x82\xAC\xE3\x83\x85\xE1\xBD\x8A";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(2, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x30AB, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x3099, Yes, 8);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(2, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x30C4, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x3099, Yes, 8);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x039F, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0313, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x0300, Yes, 230);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleNonStarter)
{
	const char* i = "\xCC\x9B\xCC\x80\xCE\xB7";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x031B, Yes, 216);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0300, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x03B7, Yes, 0);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, ContinueAfterEnd)
{
	const char* i = "buh";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	EXPECT_EQ(1, decompose_execute(&state));
	EXPECT_EQ(1, decompose_execute(&state));

	EXPECT_EQ(0, decompose_execute(&state));
	EXPECT_EQ(0, decompose_execute(&state));
	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, NotEnoughData)
{
	const char* i = "";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(0, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(0, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(nullptr, state.input);
	EXPECT_EQ(nullptr, state.output);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, InvalidData)
{
	const char* i = nullptr;
	size_t il = 81;

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(0, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(0, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(nullptr, state.input);
	EXPECT_EQ(nullptr, state.output);

	EXPECT_EQ(0, decompose_execute(&state));
}