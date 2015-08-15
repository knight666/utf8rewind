#include "property-base.hpp"

#include "property-convert-utf16.hpp"

extern "C" {
	#include "../internal/codepoint.h"
};

int main(int argc, char** argv)
{
	quickcheck::check<PropertyConvertUtf16>("Convert UTF-16 to UTF-8 and back.", 10000);

	return 0;
}