/*!
	\mainpage Overview

	\tableofcontents

	\section introduction Introduction

	`utf8rewind` is a cross-platform and open source C library designed to
	extend the default string handling functions in order to add support for
	UTF-8 encoded text.
	
	\section features Features

	* **Conversion to and from UTF-8** - utf8rewind provides functions for
	converting from and to [wide](\ref widetoutf8), [UTF-16](\ref utf16toutf8)
	and [UTF-32](\ref utf32toutf8) encoded text.

	* **Case mapping** - The library also provides function for converting text
	to [uppercase](\ref utf8toupper), [lowercase](\ref utf8tolower) and
	[titlecase](\ref utf8totitle).

	* **Normalization** - With #utf8normalize, you can normalize UTF-8 encoded
	text without first converting it to UTF-32.

	* **Seeking** - Using #utf8seek, you can seek forwards and backwards in
	UTF-8 encoded text.

	* **Cross-platform** - utf8rewind is written in plain C, which means it can
	be used on any platform with a compliant C compiler. Currently, Windows,
	Linux and Mac versions are available.

	* **Easy to integrate** - The library consists of only 13 public functions
	and requires no initialization. Any C or C++ project can add utf8rewind
	without breaking existing code.

	* **Simple bindings** - No structs are used in the public interface, only
	pointers. Even if you don't use C, if the language of your choice allows
	bindings to C functions (e.g. Python), you could integrate utf8rewind.

	* **No heap allocations** - All allocations in UTF-8 happen on the stack.
	You provide the memory, without having to override `malloc`. This makes the
	library perfectly tailored to game engines, integrated systems and other
	performance-critical or memory-constrained projects.

	* **Safety** - Over 1200 automated unit and integration tests guarantee the
	safety and security of the library.

	For a full summary of the interface, please refer to the
	[interface page](\ref utf8rewind.h).

	\section license Licensing

	This project is licensed under the MIT license, a full copy of which
	should have been provided with the project.

	\section building Building the project

	All supported platforms use [GYP](http://code.google.com/p/gyp/) to
	generate a solution. This generated solution can be used to compile the
	project and its dependencies.

	\subsection building-windows Building on Windows with Visual Studio

	\note You will need to have Visual Studio 2010 or above installed.

	Open a command window at the project's root.

	If you have multiple versions of Visual Studio installed, you must first
	specify the version you want to use:

		set GYP_MSVS_VERSION=2012

	Use GYP to generate a solution:

		tools\gyp\gyp --depth utf8rewind.gyp

	Open the solution in Visual Studio. You can use the different build
	configurations to generate a static library.

	\subsection building-linux Building on Linux with GCC

	Open a command window at the project's root.

	Ensure you have all dependencies installed using your favorite preferred
	package manager:

		sudo apt-get install gyp gcc g++

	Use GYP to generate a Makefile:

		gyp --depth= utf8rewind.gyp

	Build the project using `make`:

		make

	For a release build, specify the build type:

		make BUILDTYPE=Release

	The generated Makefile does not contain a "clean" target. In order to do
	a full rebuild, you must delete the files in the "output" directory
	manually.

	\subsection building-mac Building on Mac OS X with XCode

	\note Building on Mac OS X is currently untested. Please let us know if
	you can help us in this regard.

	Open a command window at the project's root.

	Use GYP to generate a solution:

		tools\gyp\gyp --depth utf8rewind.gyp

	Open the solution in XCode and you can build the library and tests.

	\subsection running-tests Running the tests

	After generating a solution, build and run the "tests-rewind" project.
	Verify that all tests pass on your system configuration before continuing.

	\section helping-out Helping out

	As a user, you can help the project in a number of ways, in order of
	difficulty:

	* **Use it** - Designers of a public interface often have very different
	ideas about usability than those actually using it. By using the library,
	you are helping the project spread and could potentially improve it by us
	taking your project into consideration when we design the API.

	* **Spread the word** - If you find `utf8rewind` useful, recommend it to
	your friends and coworkers.

	* **Complain** - No library is perfect and `utf8rewind` is no exception.
	If you find a fault but lack the means (time, resources, etc.) to fix it,
	sending complaints to the proper channels can help the project out a lot.

	* **Write a failing test** - If a feature is not working as intended, you
	can prove it by writing a failing test. By sending the test to us, we can
	make the adjustments necessary for it to pass.

	* **Write a patch** - Patches include a code change that help tests to
	pass. A patch must always include a set of tests that fail to pass without
	the patch. All patches will be reviewed and possibly cleaned up before
	being accepted.

	\section contact Contact

	For inquiries, complaints and patches, please contact `{quinten}{lansu} {at} {gmail}.{com}`. Remove the brackets to get a valid e-mail address.
*/