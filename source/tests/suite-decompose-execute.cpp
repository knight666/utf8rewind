#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/decomposition.h"
}

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

TEST(DecomposeExecute, Unchanged)
{
	const char* i = "\xC2\xA0";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	EXPECT_UTF8EQ("\xC2\xA0", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, Decompose)
{
	const char* i = "\xC3\x9A";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(2, decompose_execute(&state));
	EXPECT_UTF8EQ("U\xCC\x81", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, NonStarter)
{
	const char* i = "\xCC\x84";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	EXPECT_UTF8EQ("\xCC\x84", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, DecomposeWithNonStarter)
{
	const char* i = "\xC4\x80\xCC\x88";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(3, decompose_execute(&state));
	EXPECT_UTF8EQ("A\xCC\x84\xCC\x88", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, decompose_execute(&state));
}

TEST(DecomposeExecute, MultipleUnchanged)
{
	const char* i = "\xE8\x90\xBD\xE9\xB9\xBF\xE2\x85\xB6\xE2\x85\xAC";
	size_t il = strlen(i);

	StreamState input;
	StreamState output;
	DecomposeState state;

	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));
	EXPECT_EQ(1, decompose_initialize(&state, &input, &output, 0));

	EXPECT_EQ(1, decompose_execute(&state));
	EXPECT_UTF8EQ("\xE8\x90\xBD", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(1, decompose_execute(&state));
	EXPECT_UTF8EQ("\xE9\xB9\xBF", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(1, decompose_execute(&state));
	EXPECT_UTF8EQ("\xE2\x85\xB6", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(1, decompose_execute(&state));
	EXPECT_UTF8EQ("\xE2\x85\xAC", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

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
	EXPECT_UTF8EQ("\xE3\x82\xAB\xE3\x82\x99", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(2, decompose_execute(&state));
	EXPECT_UTF8EQ("\xE3\x83\x84\xE3\x82\x99", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(3, decompose_execute(&state));
	EXPECT_UTF8EQ("\xCE\x9F\xCC\x93\xCC\x80", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

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
	EXPECT_UTF8EQ("\xCC\x9B\xCC\x80\xCE\xB7", helpers::utf8(state.output->codepoint, state.output->current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, decompose_execute(&state));
}