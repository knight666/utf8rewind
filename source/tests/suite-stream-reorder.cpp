#include "tests-base.hpp"

#include "helpers-streams.hpp"

TEST(StreamReorder, SingleCodepoint)
{
	StreamState stream = helpers::createStream("\xCC\x91");

	CHECK_STREAM_ENTRY(stream, 0, 0x0311, Yes, 230);

	EXPECT_EQ(1, stream_reorder(&stream));

	CHECK_STREAM_ENTRY(stream, 0, 0x0311, Yes, 230);
}

TEST(StreamReorder, UnaffectedTwoCodepoints)
{
	StreamState stream = helpers::createStream("\xCA\xA2\xCA\xA3");

	CHECK_STREAM_ENTRY(stream, 0, 0x02A2, Yes, 0);
	CHECK_STREAM_ENTRY(stream, 1, 0x02A3, Yes, 0);

	EXPECT_EQ(1, stream_reorder(&stream));

	CHECK_STREAM_ENTRY(stream, 0, 0x02A2, Yes, 0);
	CHECK_STREAM_ENTRY(stream, 1, 0x02A3, Yes, 0);
}

TEST(StreamReorder, UnaffectedMultipleCodepoints)
{
	StreamState stream = helpers::createStream("Bike");

	CHECK_STREAM_ENTRY(stream, 0, 'B', Yes, 0);
	CHECK_STREAM_ENTRY(stream, 1, 'i', Yes, 0);
	CHECK_STREAM_ENTRY(stream, 2, 'k', Yes, 0);
	CHECK_STREAM_ENTRY(stream, 3, 'e', Yes, 0);

	EXPECT_EQ(1, stream_reorder(&stream));
	
	CHECK_STREAM_ENTRY(stream, 0, 'B', Yes, 0);
	CHECK_STREAM_ENTRY(stream, 1, 'i', Yes, 0);
	CHECK_STREAM_ENTRY(stream, 2, 'k', Yes, 0);
	CHECK_STREAM_ENTRY(stream, 3, 'e', Yes, 0);
}

TEST(StreamReorder, ReorderedTwoCodepoints)
{
	StreamState stream = helpers::createStream("\xCC\x82\xCC\xB8");

	CHECK_STREAM_ENTRY(stream, 0, 0x0302, Yes, 230);
	CHECK_STREAM_ENTRY(stream, 1, 0x0338, Yes, 1);

	EXPECT_EQ(1, stream_reorder(&stream));

	CHECK_STREAM_ENTRY(stream, 0, 0x0338, Yes, 1);
	CHECK_STREAM_ENTRY(stream, 1, 0x0302, Yes, 230);
}

TEST(StreamReorder, ReorderedMultipleCodepoints)
{
	StreamState stream = helpers::createStream("a\xCC\x95\xCC\x80\xD6\xAE\xCC\x81");

	CHECK_STREAM_ENTRY(stream, 0, 0x0061, Yes, 0);
	CHECK_STREAM_ENTRY(stream, 1, 0x0315, Yes, 232);
	CHECK_STREAM_ENTRY(stream, 2, 0x0300, Yes, 230);
	CHECK_STREAM_ENTRY(stream, 3, 0x05AE, Yes, 228);
	CHECK_STREAM_ENTRY(stream, 4, 0x0301, Yes, 230);

	EXPECT_EQ(1, stream_reorder(&stream));

	CHECK_STREAM_ENTRY(stream, 0, 0x0061, Yes, 0);
	CHECK_STREAM_ENTRY(stream, 1, 0x05AE, Yes, 228);
	CHECK_STREAM_ENTRY(stream, 2, 0x0300, Yes, 230);
	CHECK_STREAM_ENTRY(stream, 3, 0x0301, Yes, 230);
	CHECK_STREAM_ENTRY(stream, 4, 0x0315, Yes, 232);
}

TEST(StreamReorder, ReorderedWorstCase)
{
	StreamState stream = helpers::createStream("\xCD\xA0\xCC\x91\xE0\xBC\xB5\xCB\xB4");

	CHECK_STREAM_ENTRY(stream, 0, 0x0360, Yes, 234);
	CHECK_STREAM_ENTRY(stream, 1, 0x0311, Yes, 230);
	CHECK_STREAM_ENTRY(stream, 2, 0x0F35, Yes, 220);
	CHECK_STREAM_ENTRY(stream, 3, 0x02F4, Yes, 0);

	EXPECT_EQ(1, stream_reorder(&stream));

	CHECK_STREAM_ENTRY(stream, 0, 0x02F4, Yes, 0);
	CHECK_STREAM_ENTRY(stream, 1, 0x0F35, Yes, 220);
	CHECK_STREAM_ENTRY(stream, 2, 0x0311, Yes, 230);
	CHECK_STREAM_ENTRY(stream, 3, 0x0360, Yes, 234);
}

TEST(StreamReorder, NotEnoughData)
{
	StreamState stream = { 0 };

	EXPECT_EQ(0, stream_reorder(&stream));
}