#include "property-base.hpp"

#include "property-convert-utf16.hpp"

extern "C" {
	#include "../internal/codepoint.h"
};

int main(int argc, char** argv)
{
	PropertyConvertUtf16 convertUtf16;
	convertUtf16.check(10000, MAX_LEGAL_UNICODE, false);

	return 0;
}