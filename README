## Introduction ##

`utf8rewind` is a C library designed to extend default string handling functions in order to add support for UTF-8 encoded text. Besides providing functions to deal with UTF-8 encoded text, it also provides functions for converting to and from UTF-16 encoded text, the default on Windows.

## Licensing ##

This project is licensed under the MIT license, a full copy of which should have been provided with the project.

## Installation ###

There are two ways to use the library: you can either create a static library and link to that or paste the source files directly into your source. This is a viable alternative because the project consists of only two files: 'utf8rewind.h' (header) and 'utf8rewind.c' (source).

### Generating a solution ###

Use [GYP](http://code.google.com/p/gyp/) to generate a solution, like so:

	tools\gyp\gyp --depth --format=msvs2010 utf8rewind.gyp

The project has only been tested as compiling and running using Visual Studio 2010 on Windows, but GYP should generate workable output for other platforms as well.

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