#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/composition.h"
}

#include "helpers-strings.hpp"

TEST(ComposeExecute, Initialize)
{
	/*
		U+03B9 U+0308
		     Y      M
		     0    230
	*/

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

TEST(ComposeExecute, StartSingleBasicLatin)
{
	/*
		U+004C
		     Y
		     0
	*/

	const char* i = "L";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x004C, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, StartSingleStarter)
{
	/*
		U+1E0A
		     Y
		     0
	*/

	const char* i = "\xE1\xB8\x8A";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1E0A, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, StartSingleNonStarter)
{
	/*
		U+059A
		     Y
		   222
	*/

	const char* i = "\xD6\x9A";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x059A, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, StartSingleInvalidCodepoint)
{
	/*
		U+FFFD
		     Y
		     0
	*/

	const char* i = "\xCF";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xFFFD, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, StartMultipleBasicLatin)
{
	/*
		U+0073 U+0070 U+0061 U+0063 U+0065
		     Y      Y      Y      Y      Y
		     0      0      0      0      0
	*/

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

TEST(ComposeExecute, StartMultipleStarter)
{
	/*
		U+0112 U+1E14 U+10341 U+00C0
		     Y      Y       Y      Y
		     0      0       0      0
	*/

	const char* i = "\xC4\x92\xE1\xB8\x94\xF0\x90\x8D\x81\xC3\x80";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0112, compose_execute(&state));
	EXPECT_CPEQ(0x1E14, compose_execute(&state));
	EXPECT_CPEQ(0x10341, compose_execute(&state));
	EXPECT_CPEQ(0x00C0, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, StartMultipleNonStarter)
{
	/*
		U+0F72 U+20F0 U+035C
		     Y      Y      Y
		   130    230    233
	*/

	const char* i = "\xE0\xBD\xB2\xE2\x83\xB0\xCD\x9C";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0F72, compose_execute(&state));
	EXPECT_CPEQ(0x20F0, compose_execute(&state));
	EXPECT_CPEQ(0x035C, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, StartMultipleInvalidCodepoint)
{
	/*
		U+FFFD U+FFFD U+FFFD
		     Y      Y      Y
		     0      0      0
	*/

	const char* i = "\xCF\xDD\xEF";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xFFFD, compose_execute(&state));
	EXPECT_CPEQ(0xFFFD, compose_execute(&state));
	EXPECT_CPEQ(0xFFFD, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceUnaffected)
{
	/*
		U+0390 U+031D U+035A
		     Y      Y      Y
		     0    220    220
	*/

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

TEST(ComposeExecute, SequenceCompose)
{
	/*
		U+0413 U+0301
		     Y      M
		     0    230
	*/

	const char* i = "\xD0\x93\xCC\x81";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0403, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceComposeMultipleCodepoints)
{
	/*
		U+03A9 U+0314 U+0342 U+0345
		     Y      M      M      M
		     0    230    230    240
	*/

	const char* i = "\xCE\xA9\xCC\x94\xCD\x82\xCD\x85";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1FAF, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceForwardPrecedence)
{
	/*
		U+0061 U+0328 U+0301
		     Y      M      M
		     0    202    230
	*/

	const char* i = "a\xCC\xA8\xCC\x81";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0105, compose_execute(&state));
	EXPECT_CPEQ(0x0301, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceForwardPrecedenceMultipleCodepoints)
{
	/*
		U+0044 U+031B U+0323 U+0307
		     Y      M      M      M
		     0    216    220    230
	*/

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

TEST(ComposeExecute, SequenceBlockEquivalence)
{
	/*
		U+0061 U+0300 U+0301
		     Y      M      M
		     0    230    230
	*/

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

TEST(ComposeExecute, SequenceBlockEquivalenceMultipleCodepoints)
{
	/*
		U+0061 U+0328 U+030C U+0300 U+0301
		     Y      M      M      M      M
		     0    202    230    230    230
	*/

	// "0061 0322 030C 0300 0301"

	const char* i = "a\xCC\xA2\xCC\x8C\xCC\x80\xCC\x81";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x01CE, compose_execute(&state));
	EXPECT_CPEQ(0x0322, compose_execute(&state));
	EXPECT_CPEQ(0x0300, compose_execute(&state));
	EXPECT_CPEQ(0x0301, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceBlockUnordered)
{
	/*
		U+0045 U+0301 U+0327
		     Y      M      M
		     0    230    202
	*/

	const char* i = "E\xCC\x81\xCC\xA7";
	size_t il = strlen(i);

	std::string seq = helpers::sequence(i, UnicodeProperty_Normalization_Compose);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x00C9, compose_execute(&state));
	EXPECT_CPEQ(0x0327, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceSkipUnstable)
{
	/*
		U+0044 U+031B U+0307
		     Y      M      M
		     0    216    230
	*/

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

TEST(ComposeExecute, SequeneceBlocked)
{
	/*
		U+0061 U+05AE U+2DEE U+0300 U+0315 U+0062
		     Y      Y      Y      M      Y      Y
		     0    228    230    230    232      0
	*/

	const char* i = "a\xD6\xAE\xE2\xB7\xAE\xCC\x80\xCC\x95" "b";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0061, compose_execute(&state));
	EXPECT_CPEQ(0x05AE, compose_execute(&state));
	EXPECT_CPEQ(0x2DEE, compose_execute(&state));
	EXPECT_CPEQ(0x0300, compose_execute(&state));
	EXPECT_CPEQ(0x0315, compose_execute(&state));
	EXPECT_CPEQ(0x0062, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceSkipStable)
{
	/*
		U+0044 U+005A U+030C
		     Y      Y      M
		     0      0    230
	*/

	const char* i = "DZ\xCC\x8C";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x0044, compose_execute(&state));
	EXPECT_CPEQ(0x017D, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceSkipNonStarter)
{
	/*
		U+115B9 U+0334 U+115AF
		      Y      Y       M
		      0      1       0
	*/

	const char* i = "\xF0\x91\x96\xB9\xCC\xB4\xF0\x91\x96\xAF";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x115B9, compose_execute(&state));
	EXPECT_CPEQ(0x0334, compose_execute(&state));
	EXPECT_CPEQ(0x115AF, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceHangulLV)
{
	/*
		U+1100 U+1162
		     Y      M
		     0      0
	*/

	const char* i = "\xE1\x84\x80\xE1\x85\xA2";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xAC1C, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceHangulLVMissing)
{
	/*
		U+1101
		     Y
		     0
	*/

	const char* i = "\xE1\x84\x81";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x1101, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceHangulST)
{
	/*
		U+AF48 U+11B6
		     Y      M
		     0      0
	*/

	const char* i = "\xEA\xBD\x88\xE1\x86\xB6";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xAF57, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, SequenceHangulSTDecomposed)
{
	/*
		U+110B U+1164 U+11A8
		     Y      M      M
		     0      0      0
	*/

	const char* i = "\xE1\x84\x8B\xE1\x85\xA4\xE1\x86\xA8";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xC599, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, MultipleSequenceCompose)
{
	/*
		U+03B1 U+0345 U+03B7 U+0342 U+2291 U+0338
		     Y      M      Y      M      Y      M
		     0    240      0    230      0      1
	*/

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
	/*
		U+03C9 U+0301 U+1DA8
		     Y      M      Y
		     0    230      0
	*/

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
	/*
		U+017F U+1100 U+1169 U+11B3
		     Y      Y      M      M
		     0      0      0      0
	*/

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

TEST(ComposeExecute, MultipleSequenceComposeSkipNonStarter)
{
	/*
		U+1112 U+1170 U+0334 U+11AE
		     Y      M      Y      M
		     0      0      1      0
	*/

	const char* i = "\xE1\x84\x92\xE1\x85\xB0\xCC\xB4\xE1\x86\xAE";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xD6FC, compose_execute(&state));
	EXPECT_CPEQ(0x0334, compose_execute(&state));
	EXPECT_CPEQ(0x11AE, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, MultipleSequenceComposeEquivalentCCC)
{
	/*
		U+0061 U+1D16D U+302B U+05AE U+0300 U+0062
		     Y       Y      Y      Y      M      Y
		     0     226    228    228    230      0
	*/

	const char* i = "a\xF0\x9D\x85\xAD\xE3\x80\xAB\xD6\xAE\xCC\x80" "b";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0x00E0, compose_execute(&state));
	EXPECT_CPEQ(0x1D16D, compose_execute(&state));
	EXPECT_CPEQ(0x302B, compose_execute(&state));
	EXPECT_CPEQ(0x05AE, compose_execute(&state));
	EXPECT_CPEQ(0x0062, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, MultipleSequenceHangul)
{
	/*
		U+110B U+1165 U+11B6 U+110B U+116F U+110C U+1175 U+11A8
		     Y      M      M      Y      M      Y      M      M
		     0      0      0      0      0      0      0      0
	*/

	const char* i = "\xE1\x84\x8B\xE1\x85\xA5\xE1\x86\xB6\xE1\x84\x8B\xE1\x85\xAF\xE1\x84\x8C\xE1\x85\xB5\xE1\x86\xA8";
	size_t il = strlen(i);

	StreamState input;
	EXPECT_EQ(1, stream_initialize(&input, i, il, 0));

	ComposeState state;
	EXPECT_EQ(1, compose_initialize(&state, &input, 0));

	EXPECT_CPEQ(0xC5C3, compose_execute(&state));
	EXPECT_CPEQ(0xC6CC, compose_execute(&state));
	EXPECT_CPEQ(0xC9C1, compose_execute(&state));
	EXPECT_CPEQ(0, compose_execute(&state));
}

TEST(ComposeExecute, ContinueAfterEnd)
{
	/*
		U+339E U+002E
		     Y      Y
		     0      0
	*/

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