#include "property-base.hpp"

#if _WINDOWS
	#include <Windows.h>
#endif

#include "property-convert-utf16.hpp"
#include "property-convert-utf32.hpp"

extern "C" {
	#include "../internal/codepoint.h"
};

class LoggingStream
	: public std::ostream
	, public std::streambuf
{

public:

	typedef std::ostream::traits_type traits;

	LoggingStream()
		: std::ostream(this)
	{
	}
	virtual ~LoggingStream()
	{
	}

	virtual traits::int_type overflow(traits::int_type codepoint) override
	{
		if (traits::eq_int_type(codepoint, traits::eof()))
		{
			return 0;
		}

		m_Buffer.push_back(traits::to_char_type(codepoint));

		if (codepoint == traits::to_int_type('\n'))
		{
		#if _WINDOWS
			::OutputDebugStringA(m_Buffer.c_str());
		#endif

			std::cout << m_Buffer;

			m_Buffer.clear();
		}

		return 1;
	}

private:

	std::string m_Buffer;

};

int main(int argc, char** argv)
{
	LoggingStream logging;

	quickcheck::check<PropertyConvertUtf16>("Convert UTF-16 to UTF-8 and back.", 10000, 0, false, logging);
	quickcheck::check<PropertyConvertUtf32>("Convert UTF-32 to UTF-8 and back.", 10000, 0, false, logging);

	return 0;
}