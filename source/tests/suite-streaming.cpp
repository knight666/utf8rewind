#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/streaming.h"
}

#include "helpers-normalization.hpp"
#include "helpers-strings.hpp"

TEST(Streaming, Initialized)
{
	const char* i = "loneliness";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(i, state.src);
	EXPECT_EQ(il, state.src_size);
	EXPECT_EQ(UnicodeProperty_Normalization_Compose, state.property);
	EXPECT_EQ(0, state.current);
	EXPECT_EQ(1, state.stable);
}

TEST(Streaming, SingleNoChange)
{
	const char* i = "A\xCC\x83";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(2, state.current);
	EXPECT_UTF8EQ("A\xCC\x83", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, stream_execute(&state));
	EXPECT_EQ(0, state.current);
	EXPECT_UTF8EQ("", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());
}

TEST(Streaming, SingleReorder)
{
	const char* i = "A\xCC\x83\xCC\x82";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(3, state.current);
	EXPECT_UTF8EQ("A\xCC\x83\xCC\x82", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, stream_execute(&state));
	EXPECT_EQ(0, state.current);
	EXPECT_UTF8EQ("", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());
}

TEST(Streaming, SingleInvalidCodepoint)
{
	const char* i = "\xF4";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(1, state.current);
	EXPECT_UTF8EQ("\xEF\xBF\xBD", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, stream_execute(&state));
	EXPECT_EQ(0, state.current);
	EXPECT_UTF8EQ("", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());
}

TEST(Streaming, MultipleNoChange)
{
	const char* i = "a\xCC\x80\xCC\x81" "E\xCC\x8C";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(3, state.current);
	EXPECT_UTF8EQ("a\xCC\x80\xCC\x81", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(2, state.current);
	EXPECT_UTF8EQ("E\xCC\x8C", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, stream_execute(&state));
	EXPECT_EQ(0, state.current);
	EXPECT_UTF8EQ("", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());
}

TEST(Streaming, MultipleReorder)
{
	const char* i = "a\xCC\x95\xCC\x80\xD6\xAE\xCC\x80" "b";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(5, state.current);
	EXPECT_UTF8EQ("a\xD6\xAE\xCC\x80\xCC\x80\xCC\x95", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(1, state.current);
	EXPECT_UTF8EQ("b", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, stream_execute(&state));
	EXPECT_EQ(0, state.current);
	EXPECT_UTF8EQ("", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());
}

TEST(Streaming, MultipleInvalidCodepoint)
{
	const char* i = "\xF4\x9A\x13";
	size_t il = strlen(i);

	StreamState state;
	EXPECT_EQ(1, stream_initialize(&state, i, il, UnicodeProperty_Normalization_Compose));

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(1, state.current);
	EXPECT_UTF8EQ("\xEF\xBF\xBD", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(1, stream_execute(&state));
	EXPECT_EQ(1, state.current);
	EXPECT_UTF8EQ("\x13", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());

	EXPECT_EQ(0, stream_execute(&state));
	EXPECT_EQ(0, state.current);
	EXPECT_UTF8EQ("", helpers::utf8(state.codepoint, state.current * sizeof(unicode_t)).c_str());
}