#include "tests-base.hpp"

extern "C" {
	#include "../internal/streaming.h"
}

#include "helpers-streams.hpp"

TEST(StreamRead, Initialize)
{
	const char* i = "loneliness";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(i, state.src);
	EXPECT_EQ(il, state.src_size);
	EXPECT_EQ(UnicodeProperty_Normalization_Compose, state.property);
	EXPECT_EQ(0, state.current);
	EXPECT_TRUE(state.stable);
}

TEST(StreamRead, SingleCodepointStarter)
{
	const char* i = "\xCB\xBC";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x02FC, Yes, 0);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, SingleCodepointNonStarter)
{
	const char* i = "\xCC\x9D";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x031D, Yes, 220);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, SingleCodepointInvalid)
{
	const char* i = "\xF4";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0xFFFD, Yes, 0);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, MultipleCodepointsStarter)
{
	const char* i = "\xCF\xB4\xD0\x86\xD0\x94";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x03F4, Yes, 0);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x0406, Yes, 0);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x0414, Yes, 0);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, MultipleCodepointsNonStarterOrdered)
{
	const char* i = "\xCC\xBB\xCD\x8B";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(2, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x033B, Yes, 220);
	CHECK_STREAM_ENTRY(state, 1, 0x034B, Yes, 230);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, MultipleCodepointsNonStarterOutOfOrder)
{
	const char* i = "\xCD\x8B\xCC\xBB";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(2, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x034B, Yes, 230);
	CHECK_STREAM_ENTRY(state, 1, 0x033B, Yes, 220);
	EXPECT_FALSE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, MultipleCodepointsInvalid)
{
	const char* i = "\xF4\x9A\xC0";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0xFFFD, Yes, 0);

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0xFFFD, Yes, 0);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, SingleSequenceOrdered)
{
	const char* i = "A\xCC\x83";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(2, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x0041, Yes, 0);
	CHECK_STREAM_ENTRY(state, 1, 0x0303, Maybe, 230);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, SingleSequenceOutOfOrder)
{
	const char* i = "O\xCC\x84\xCC\xA8";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(3, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x004F, Yes, 0);
	CHECK_STREAM_ENTRY(state, 1, 0x0304, Maybe, 230);
	CHECK_STREAM_ENTRY(state, 2, 0x0328, Maybe, 202);
	EXPECT_FALSE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, SingleSequenceEndStarterMaybe)
{
	const char* i = "\xE0\xA7\x87\xCC\xB4\xE0\xA6\xBE";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(2, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x09C7, Yes, 0);
	CHECK_STREAM_ENTRY(state, 1, 0x0334, Yes, 1);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x09BE, Maybe, 0);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, SingleSequenceEndNonStarterMaybe)
{
	const char* i = "\xC4\x92\xCC\xB4\xCC\x80";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(3, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x0112, Yes, 0);
	CHECK_STREAM_ENTRY(state, 1, 0x0334, Yes, 1);
	CHECK_STREAM_ENTRY(state, 2, 0x0300, Maybe, 230);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, MultipleSequencesOrdered)
{
	const char* i = "a\xCC\x80\xCC\x81" "E\xCC\x8C";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(3, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x0061, Yes, 0);
	CHECK_STREAM_ENTRY(state, 1, 0x0300, Maybe, 230);
	CHECK_STREAM_ENTRY(state, 2, 0x0301, Maybe, 230);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(2, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x0045, Yes, 0);
	CHECK_STREAM_ENTRY(state, 1, 0x030C, Maybe, 230);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, MultipleSequencesOutOfOrder)
{
	const char* i = "a\xCC\x95\xCC\x80\xD6\xAE\xCC\x80" "b";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(5, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x0061, Yes, 0);
	CHECK_STREAM_ENTRY(state, 1, 0x0315, Yes, 232);
	CHECK_STREAM_ENTRY(state, 2, 0x0300, Maybe, 230);
	CHECK_STREAM_ENTRY(state, 3, 0x05AE, Yes, 228);
	CHECK_STREAM_ENTRY(state, 4, 0x0300, Maybe, 230);
	EXPECT_FALSE(state.stable);

	EXPECT_EQ(1, stream_read(&state));
	CHECK_STREAM_ENTRY(state, 0, 0x0062, Yes, 0);
	EXPECT_TRUE(state.stable);

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, ContinueAfterEnd)
{
	const char* i = "(c)";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_read(&state));
	EXPECT_EQ(1, stream_read(&state));
	EXPECT_EQ(1, stream_read(&state));

	EXPECT_EQ(0, stream_read(&state));
	EXPECT_EQ(0, stream_read(&state));
	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, NotEnoughData)
{
	const char* i = "";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(0, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(0, stream_read(&state));
}

TEST(StreamRead, InvalidData)
{
	const char* i = nullptr;
	size_t il = 15;

	StreamState state;
	EXPECT_EQ(0, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(0, stream_read(&state));
}