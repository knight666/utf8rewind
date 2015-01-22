#include "tests-base.hpp"

extern "C" {
	#include "../internal/database.h"
	#include "../internal/composition.h"
}

#include "helpers-streams.hpp"
#include "helpers-strings.hpp"

TEST(StreamReorder, Unchanged)
{
	StreamState stream = helpers::createStream("Bike");

	EXPECT_EQ(1, stream_reorder(&stream));
	
	CHECK_STREAM(stream, 0, 'B', Yes, 0);
	CHECK_STREAM(stream, 1, 'i', Yes, 0);
	CHECK_STREAM(stream, 2, 'k', Yes, 0);
	CHECK_STREAM(stream, 3, 'e', Yes, 0);
	CHECK_STREAM_END(stream, 4);
}

TEST(StreamReorder, Reorder)
{
	StreamState stream = helpers::createStream("\xCE\xA9\xCD\x85\xCC\x93\xCC\x80");

	EXPECT_EQ(1, stream_reorder(&stream));

	CHECK_STREAM(stream, 0, 0x03A9, Yes, 0);
	CHECK_STREAM(stream, 1, 0x0313, Yes, 230);
	CHECK_STREAM(stream, 2, 0x0300, Yes, 230);
	CHECK_STREAM(stream, 3, 0x0345, Yes, 240);
	CHECK_STREAM_END(stream, 4);
}