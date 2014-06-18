## Introduction ##

`utf8rewind` is a C library designed to extend default string handling functions in order to add support for UTF-8 encoded text. Besides providing functions to deal with UTF-8 encoded text, it also provides functions for converting to and from UTF-16 encoded text, the default on Windows.

## Licensing ##

This project is licensed under the MIT license, a full copy of which should have been provided with the project.

## Installation ###

There are two ways to use the library: you can either create a static library and link to that or paste the source files directly into your source. This is a viable alternative because the project consists of only two files: 'utf8rewind.h' (header) and 'utf8rewind.c' (source).

## Building the project ##

All supported platforms use [GYP](http://code.google.com/p/gyp/) to generate a solution. This generated solution can be used to compile the project and its dependencies.

### Building on Windows with Visual Studio ###

You will need to have Visual Studio 2010 or above installed.

Open a command window at the project's root.

Execute the following to generate a solution:

	tools\gyp\gyp --depth --format=msvs utf8rewind.gyp

Open the solution in Visual Studio and you can build the library and tests.

### Building on Linux with GCC ###

Open a command window at the project's root.

First, make sure you have all dependencies installed using your favorite package manager.

	sudo apt-get install gyp gcc g++

Next, execute the following command to generate a makefile:

	gyp --depth=./ --format=make utf8rewind.gyp

Now you can build the project:

	make

For a release build, specify the build type:

	make BUILDTYPE=Release

### Building on Mac OS X using Xcode ###

Building on Mac OS X is currently untested. Please let us know if you can help us in this regard.

Open a command window at the project's root.

Execute the following to generate a solution:

	tools\gyp\gyp --depth --format=xcode utf8rewind.gyp

Open the solution in Xcode and you can build the library and tests.

### Using the source directly ###

Copy 'include/utf8rewind/utf8rewind.h' and 'source/utf8rewind.c' directly into your existing solution. Make sure you specify that the source file should be compiled as C code (`/TC` in Visual Studio). Include the header from your source and start using it.

### Running the tests ###

After generating a solution, build and run the "tests-rewind" project. Verify that all tests pass on your system before continuing.

## Helping out ##

As a user, you can help the project in a number of ways, in order of difficulty:

* **Use it** - Designers of a public interface often have very different ideas about usability than those actually using it. By using the library, you are helping the project spread and could potentially improve it by us taking your project into consideration when we design the API.

* **Spread the word** - If you find `utf8rewind` useful, recommend it to your friends and coworkers.

* **Complain** - No library is perfect and `utf8rewind` is no exception. If you find a fault but lack the means (time, resources, etc.) to fix it, sending complaints to the proper channels can help the project out a lot.

* **Write a failing test** - If a feature is not working as intended, you can prove it by writing a failing test. By sending the test to us, we can make the adjustments necessary for it to pass.

* **Write a patch** - Patches include a code change that help tests to pass. A patch must always include a set of tests that fail to pass without the patch. All patches will be reviewed and possibly cleaned up before being accepted.

## Contact ##

For inquiries, complaints and patches, please contact `{quinten}{lansu} {at} {gmail}.{com}`. Remove the brackets to get a valid e-mail address.