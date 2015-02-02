#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/decomposition.h"
}

#include "helpers-streams.hpp"
#include "helpers-strings.hpp"

TEST(DecomposeExecute, Initialize)
{
	/*
		U+304B U+3099
		     Y      Y
		     0      8
	*/

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

TEST(DecomposeExecute, SingleUnaffected)
{
	/*
		U+00A0
		     Y
		     0
	*/

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
	/*
		U+00DA
		     N
		     0
	*/

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

TEST(DecomposeExecute, SingleInvalidCodepoint)
{
	/*
		U+FFFD
		     Y
		     0
	*/

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

TEST(DecomposeExecute, SequenceUnaffectedOrdered)
{
	/*
		U+006F U+031D U+030B
		     Y      Y      Y
		     0    220    230
	*/

	const char* i = "o\xCC\x9D\xCC\x8B";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x006F, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x031D, Yes, 220);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x030B, Yes, 230);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, SequenceUnaffectedUnordered)
{
	/*
		U+0041 U+0304 U+031D
		     Y      Y      Y
		     0    230    220
	*/

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

TEST(DecomposeExecute, SequenceDecomposedOrdered)
{
	/*
		U+0100 U+0308
		     N      Y
		     0    230
	*/

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

TEST(DecomposeExecute, SequenceDecomposedUnordered)
{
	/*
		U+011D U+0316
		     N      Y
		     0    220
	*/

	const char* i = "\xC4\x9D\xCC\x96";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x0067, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0302, Yes, 230);
	CHECK_STREAM_ENTRY(*state.output, 2, 0x0316, Yes, 220);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, HangulUnaffected)
{
	/*
		U+110C
		     Y
		     0
	*/

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

TEST(DecomposeExecute, HangulDecomposedTwoCodepoints)
{
	/*
		U+AC70
		     N
		     0
	*/

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

TEST(DecomposeExecute, HangulDecomposedThreeCodepoints)
{
	/*
		U+C9AC
		     N
		     0
	*/

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
	/*
		U+0048 U+0061 U+006E U+0064
		     Y      Y      Y      Y
		     0      0      0      0
	*/

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
	/*
		U+843D U+9E7F U+2176 U+216C
		     Y      Y      Y      Y
		     0      0      0      0
	*/

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
	/*
		U+30AC U+30C5 U+1F4A
		     N      N      N
		     0      0      0
	*/

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
	/*
		U+031B U+0300 U+03B7
		     Y      Y      Y
		   216    230      0
	*/

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
	/*
		U+FFFD U+FFFD
		     Y      Y
		     0      0
	*/

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
	/*
		U+00EA U+0333 U+014D U+0323 U+0346
		     N      Y      N      Y      Y
		     0    220      0    220    230
	*/

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
	/*
		U+00CA U+0347 U+00C3 U+035C U+0348 U+00ED U+031B
		     N      Y      N      Y      Y      N      Y
		     0    220      0    233    220      0    216
	*/

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

TEST(DecomposeExecute, MultipleSequenceDoNotReorder)
{
	/*
		U+09C7 U+0334 U+09BE
		     Y      Y      Y
		     0      1      0
	*/

	const char* i = "\xE0\xA7\x87\xCC\xB4\xE0\xA6\xBE";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	StreamState output = { 0 };

	DecomposeState state;
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(2, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x09C7, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x0334, Yes, 1);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x09BE, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleHangul)
{
	/*
		U+C900 U+110C U+116E U+11B3
		     N      Y      Y      Y
		     0      0      0      0
	*/

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

	EXPECT_EQ(2, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x110C, Yes, 0);
	CHECK_STREAM_ENTRY(*state.output, 1, 0x116E, Yes, 0);
	EXPECT_FALSE(state.output->stable);

	EXPECT_EQ(1, decompose_execute(&state));
	CHECK_STREAM_ENTRY(*state.output, 0, 0x11B3, Yes, 0);
	EXPECT_TRUE(state.output->stable);

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, ContinueAfterEnd)
{
	/*
		U+0062 U+0075 U+0068
		     Y      Y      Y
		     0      0      0
	*/

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