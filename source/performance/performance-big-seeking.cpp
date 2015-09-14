#include "performance-base.hpp"

class BigSeeking
	: public performance::Suite
{

public:

	virtual void setup() override
	{
		m_file.open("testdata/big.txt", std::ios_base::in);

		std::stringstream ss;
		ss << m_file.rdbuf();

		m_contents = ss.str();

		m_file.close();
	}

	std::fstream m_file;
	std::string m_contents;

};

PERF_TEST_F(BigSeeking, Begin)
{
	const char* s = m_contents.c_str();

	const char* n = utf8seek(s, s, (off_t)utf8len(s) - 1, SEEK_SET);
	ASSERT_EQ(s + m_contents.length() - 1, n);
}

PERF_TEST_F(BigSeeking, CurrentForwards)
{
	const char* s = m_contents.c_str();

	const char* n = utf8seek(s, s, (off_t)utf8len(s) - 1, SEEK_CUR);
	ASSERT_EQ(s + m_contents.length() - 1, n);
}

PERF_TEST_F(BigSeeking, CurrentBackwards)
{
	const char* s = m_contents.c_str();
	const char* e = s + m_contents.length();

	const char* n = utf8seek(e, s, -(off_t)utf8len(s) + 1, SEEK_CUR);
	ASSERT_EQ(s + 1, n);
}

PERF_TEST_F(BigSeeking, End)
{
	const char* s = m_contents.c_str();
	const char* e = s + m_contents.length();

	const char* n = utf8seek(e, s, (off_t)utf8len(s) - 1, SEEK_END);
	ASSERT_EQ(s + 1, n);
}

PERF_TEST_F(BigSeeking, IncrementalForwards)
{
	const char* s = m_contents.c_str();
	const char* e = s + m_contents.length();
	const char* c = s;
	const char* n = c;

	do
	{
		c = n;
		n = utf8seek(c, s, 1, SEEK_CUR);
	}
	while (n != c && n != e);

	ASSERT_EQ(e, n);
}

PERF_TEST_F(BigSeeking, IncrementalBackwards)
{
	const char* s = m_contents.c_str();
	const char* e = s + m_contents.length();
	const char* c = e;
	const char* n = c;

	do
	{
		c = n;
		n = utf8seek(c, s, -1, SEEK_CUR);
	}
	while (n != c && n != s);

	ASSERT_EQ(s, n);
}