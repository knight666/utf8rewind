#pragma once

// STL

#include <sstream>

// QuickCheck++

#include "quickcheck/quickcheck.hh"

// utf8rewind

#include "utf8rewind.h"

// Helpers

#include "../helpers/helpers-strings.hpp"

namespace quickcheck {

	template<>
	void printArgument(std::ostream& out, size_t n, const std::string& a)
	{
		out << "  " << n << ": \"" << helpers::printable(a) << "\" (" << helpers::identifiable(a) << ")" << std::endl;
	}

}