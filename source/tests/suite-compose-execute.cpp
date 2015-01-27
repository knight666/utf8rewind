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

TEST(ComposeExecute, SingleBasicLatin)
{
	const char* i = "L";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x004C, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleUnaffected)
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

TEST(ComposeExecute, SingleInvalidCodepoint)
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

TEST(ComposeExecute, SingleSequenceUnaffectedTwoCodepoints)
{
	const char* i = "\xC6\xB9\xCC\x9A";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x01B9, compose_execute(&state));
	EXPECT_CPEQ(0x031A, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceUnaffectedMultipleCodepoints)
{
	const char* i = "\xCE\x90\xCC\x9D\xCD\x9A";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0390, compose_execute(&state));
	EXPECT_CPEQ(0x031D, compose_execute(&state));
	EXPECT_CPEQ(0x035A, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceComposeBasicLatin)
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

TEST(ComposeExecute, SingleSequenceComposeTwoCodepoints)
{
	const char* i = "\xD0\x93\xCC\x81";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0403, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceComposeBlocked)
{
	const char* i = "a\xCC\x80\xCC\x81";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x00E0, compose_execute(&state));
	EXPECT_CPEQ(0x0301, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceComposeMultipleCodepoints)
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

TEST(ComposeExecute, SingleSequenceComposeSkipOne)
{
	const char* i = "D\xCC\x9B\xCC\x87";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1E0A, compose_execute(&state));
	EXPECT_CPEQ(0x031B, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceComposeSkipOneInCenter)
{
	const char* i = "D\xCC\x9B\xCC\xA3\xCC\x87";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1E0C, compose_execute(&state));
	EXPECT_CPEQ(0x031B, compose_execute(&state));
	EXPECT_CPEQ(0x0307, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceComposeSkipBlocked)
{
	const char* i = "a\xD6\xAE\xCC\x80\xCC\x81";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x00E0, compose_execute(&state));
	EXPECT_CPEQ(0x05AE, compose_execute(&state));
	EXPECT_CPEQ(0x0301, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceHangulLV)
{
	const char* i = "\xE1\x84\x80\xE1\x85\xA2";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xAC1C, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceHangulLVMissing)
{
	const char* i = "\xE1\x84\x81";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1101, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceHangulST)
{
	const char* i = "\xEA\xBD\x88\xE1\x86\xB6";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xAF57, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SingleSequenceHangulSTDecomposed)
{
	const char* i = "\xE1\x84\x8B\xE1\x85\xA4\xE1\x86\xA8";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xC59A, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, MultipleBasicLatin)
{
	const char* i = "space";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0073, compose_execute(&state));
	EXPECT_CPEQ(0x0070, compose_execute(&state));
	EXPECT_CPEQ(0x0061, compose_execute(&state));
	EXPECT_CPEQ(0x0063, compose_execute(&state));
	EXPECT_CPEQ(0x0065, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, MultipleUnaffected)
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

TEST(ComposeExecute, MultipleSequenceCompose)
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

TEST(ComposeExecute, MultipleSequenceComposeAndUnaffected)
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

TEST(ComposeExecute, MultipleSequenceUnaffectedAndCompose)
{
	const char* i = "\xC5\xBF\xE1\x84\x80\xE1\x85\xA9\xE1\x86\xB3";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x017F, compose_execute(&state));
	EXPECT_CPEQ(0xACF8, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, MultipleSequenceHangul)
{
	const char* i = "\xE1\x84\x8B\xE1\x85\xA5\xE1\x86\xB6\xE1\x84\x8B\xE1\x85\xAF\xE1\x84\x8C\xE1\x85\xB5\xE1\x86\xA8";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xC5C3, compose_execute(&state));
	EXPECT_CPEQ(0xC6CC, compose_execute(&state));
	EXPECT_CPEQ(0xC9C2, compose_execute(&state));
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