#include "property-base.hpp"

#include "property-convert-utf16.hpp"

int main(int argc, char** argv)
{
	PropertyConvertUtf16 convertUtf16;
	convertUtf16.check(10000);

	return 0;
}