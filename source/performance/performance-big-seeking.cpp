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

	utf8seek(s, s, (off_t)utf8len(s) - 1, SEEK_SET);
}

PERF_TEST_F(BigSeeking, CurrentForwards)
{
	const char* s = m_contents.c_str();

	utf8seek(s, s, (off_t)utf8len(s) - 1, SEEK_CUR);
}

PERF_TEST_F(BigSeeking, CurrentBackwards)
{
	const char* s = m_contents.c_str();
	const char* e = s + m_contents.length();

	utf8seek(e, s, -(off_t)utf8len(s) + 1, SEEK_CUR);
}

PERF_TEST_F(BigSeeking, End)
{
	const char* s = m_contents.c_str();
	const char* e = s + m_contents.length();

	utf8seek(e, s, (off_t)utf8len(s) - 1, SEEK_END);
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
}