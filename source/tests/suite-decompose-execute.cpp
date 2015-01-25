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
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(i, state.input->src);
	EXPECT_EQ(il, state.input->src_size);
	EXPECT_EQ(UnicodeProperty_Normalization_Compose, state.input->property);
	EXPECT_EQ(UnicodeProperty_Normalization_Decompose, state.output->property);
	EXPECT_EQ(0, state.output->current);
}

TEST(DecomposeExecute, SingleBasicLatin)
{
	const char* i = "B";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0042, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleUnaffected)
{
	const char* i = "\xC2\xA0";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
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
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
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
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0304, Yes, 230);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleInvalidCodepoint)
{
	const char* i = "\xC1";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0xFFFD, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleSequenceOrdered)
{
	const char* i = "\xC4\x80\xCC\x88";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0041, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0304, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x0308, Yes, 230);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleSequenceUnordered)
{
	const char* i = "A\xCC\x84\xCC\x9D";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0041, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0304, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x031D, Yes, 220);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleHangulUnaffected)
{
	const char* i = "\xE1\x84\x8C";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x110C, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleHangulDecomposeTwoCodepoints)
{
	const char* i = "\xEA\xB1\xB0";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(2, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x1100, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x1165, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SingleHangulDecomposeThreeCodepoints)
{
	const char* i = "\xEC\xA6\xAC";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x110C, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x1174, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x11AF, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleBasicLatin)
{
	const char* i = "Hand";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0048, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0061, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x006E, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0064, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleUnaffected)
{
	const char* i = "\xE8\x90\xBD\xE9\xB9\xBF\xE2\x85\xB6\xE2\x85\xAC";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
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
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
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
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x031B, Yes, 216);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0300, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x03B7, Yes, 0);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleInvalidCodepoints)
{
	const char* i = "\xBF\xF9\xCC";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0xFFFD, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0xFFFD, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0xFFFD, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleSequenceOrdered)
{
	const char* i = "\xC3\xAA\xCC\xB3\xC5\x8D\xCC\xA3\xCD\x86";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0065, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0302, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x0333, Yes, 220);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(4, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x006F, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0304, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x0323, Yes, 220);
	CHECK_STREAM_ENTRY(*state.output, 3, 0x0346, Yes, 230);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleSequenceUnordered)
{
	const char* i = "\xC3\x8A\xCD\x87\xC3\x83\xCD\x9C\xCD\x88\xC3\xAD\xCC\x9B";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0045, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0302, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x0347, Yes, 220);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(4, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0041, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0303, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x035C, Yes, 233);
	CHECK_STREAM_ENTRY(*state.output, 3, 0x0348, Yes, 220);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0069, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0301, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x031B, Yes, 216);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleHangul)
{
	const char* i = "\xEC\xA4\x80\xE1\x84\x8C\xE1\x85\xAE\xE1\x86\xB3";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x110C, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x116E, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x11AB, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x110C, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x116E, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x11B3, Yes, 0);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, ContinueAfterEnd)
{
	const char* i = "buh";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
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
	EXPECT_EQ(0, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
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
	EXPECT_EQ(0, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(0, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(nullptr, state.input);
	EXPECT_EQ(nullptr, state.output);

	EXPECT_EQ(0, decompose_execute(&state));
}