/*!
	\page examples Examples

	\tableofcontents

	\section example-changes Changes to existing code

	Suppose you maintain a client application written in C. In order for
	clients to login, the application needs to check their credentials. This
	is accomplished by hashing the specified password with a salt:

	\code
		uint8_t Login_CheckCredentials(const char* username, const char* password)
		{
			const char* salt;
			const char* hashed_password;
			char verify_password[256];
			char hashed_verify_password[256];

			// For the purposes of brevity, ignore the fact
			// that this is a terrible way to generate a salt,
			// because it has insufficient entropy.
			memset(verify_password, 0, 256);
			strcpy(verify_password, md5(password));
			strcat(verify_password, md5(username));

			memset(hashed_verify_password, 256, 0);
			strcpy(hashed_verify_password, md5(verify_password));

			return Database_CheckLogin(username, hashed_verify_password);
		}
	\endcode

	We want to improve the security of the application by allowing the full
	range of Unicode codepoints in the passwords. We can accomplish this by
	encoding the codepoints using UTF-8. The beauty of this change is that the
	password algorithm does not have to be modified. Because all functions
	already work on `char` arrays, you can encode Unicode as UTF-8, but treat
	the string as ASCII.

	A password like \"M&ocirc;ntiPyth&ocirc;nikdenH&ocirc;lieGr&acirc;ilen\"
	would be encoded as

		"M\xC3\xB4Pyth\xC3\xB4" "denH\xC3\xB4Gr\xC3\xA2" "en"

	which is backwards-compatible with ASCII. Calls to `strcpy` and `strcat`
	still work as expected, because the string does not contain
	NUL-terminators (`\0`), except to signify the end of data.

	When converting your project to work with UTF-8 encoded text, there are
	only two surface areas you will have to concern yourself with: input and
	display.

	We'll look at these individually.

	\section example-user-input Dealing with user input

	In the previous example, the password field in the client application only
	accepted ASCII. This is how the password field is currently implemented:

	\code{.c}
		static const size_t g_PasswordInputMaximum = 255;
		static char g_PasswordInput[256];

		uint8_t PasswordField_EnterCharacter(char input)
		{
			char text_input[2];

			if ((strlen(g_PasswordInput) + 1) > g_PasswordInputMaximum)
			{
				return 0;
			}

			text_input[0] = input;
			text_input[1] = 0;

			strcat(g_PasswordInput, text_input);

			return 1;
		}
	\endcode

	What we want to do is make sure the password field can accept Unicode
	input. To that end, we'll change the input type to #unicode_t, which can
	encode every valid Unicode codepoint.

	\code{.c}
		uint8_t PasswordField_EnterCharacter(char input)
	\endcode

	Every location that calls `PasswordField_EnterCharacter` will have to cast
	the parameter to #unicode_t, but luckily this is backwards-compatible.
	All ASCII codepoints are valid in Unicode as well.

	Inside the function, we'll want to convert the UTF-32 codepoint to UTF-8.
	To that end, we'll use #utf32toutf8.

	\code{.c}
		char text_input[8];
		static const size_t text_input_size = 7;
		size_t converted_size;
		int32_t errors = UTF8_ERR_NONE;

		converted_size = utf32toutf8(input, sizeof(unicode_t), text_input, text_input_size, &errors);
		if (converted_size == 0 ||
			errors != UTF8_ERR_NONE)
		{
			return 0;
		}
		text_input[converted_size] = 0;
	\endcode

	Although the call to #utf32toutf8 is guaranteed not to fail, because the
	maximum length of a Unicode codepoint in UTF-8 is 4 bytes, it's good
	practice to check for and respond to error conditions.

	Because the converted string can now consist of more than one byte, we'll
	have to change the check to see if we're out of bounds of the password
	input array.

	\code{.c}
		if ((strlen(g_PasswordInput) + strlen(text_input)) > g_PasswordInputMaximum)
		{
			return 0;
		}
	\endcode

	The rewritten version is now fully compatible with Unicode.

	\code{.c}
		static const size_t g_PasswordInputMaximum = 255;
		static char g_PasswordInput[256];

		uint8_t PasswordField_EnterCharacter(char input)
		{
			char text_input[8];
			size_t converted_size;
			int32_t errors = UTF8_ERR_NONE;

			converted_size = utf32toutf8(input, sizeof(unicode_t), text_input, 7, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				return 0;
			}
			text_input[converted_size] = 0;

			if ((strlen(g_PasswordInput) + strlen(text_input)) > g_PasswordInputMaximum)
			{
				return 0;
			}

			strcat(g_PasswordInput, text_input);

			return 1;
		}
	\endcode

	With only a few changes, we've upgraded a text field that previously only
	accepted ASCII to accept the full range of Unicode. And we didn't have to
	change the algorithm at all.

	\section example-display Displaying Unicode text

	Even though the user is now able to enter Unicode text, it won't show up
	right on the screen. That's because the font rendering implementation
	expects ASCII instead of Unicode.

	Let's take a look at the offending function:

	\code{.c}
		void InputField_Draw(int x, int y, const char* text)
		{
			const char* src = text;
			size_t i;

			FontBatch_Start("Arial20");

			for (i = 0; i < strlen(text); ++i)
			{
				FontBatch_AddCharacter(*src);

				src++;
			}

			FontBatch_End();
			FontBatch_Draw(x, y);
		}
	\endcode

	The issue here is that `FontBatch_AddCharacter` expects codepoints encoded
	as one byte per codepoint. Because UTF-8 is a variable-length encoding,
	this isn't necessarily true anymore.

	In order for the font renderer to display Unicode text, we'll need to
	convert the UTF-8 encoded text to UTF-32. To that end, we'll use
	#utf8toutf32.

	\code{.c}
		void InputField_Draw(int x, int y, const char* text)
		{
			size_t text_size = strlen(text);
			unicode_t* converted = NULL;
			size_t converted_size;
			int32_t errors = UTF8_ERR_NONE;
			size_t i;
			unicode_t* src;

			converted_size = utf8toutf32(text, text_size, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				goto cleanup;
			}

			converted = (unicode_t*)malloc(converted_size);

			utf8toutf32(text, text_size, converted, converted_size, NULL);

			FontBatch_Start("Arial20");

			src = decoded;

			for (i = 0; i < decoded_size / sizeof(unicode_t); ++i)
			{
				FontBatch_AddCharacter(*src);

				src++;
			}

			FontBatch_End();
			FontBatch_Draw(x, y);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}
		}
	\endcode

	After this conversion, the `FontBatch_AddCharacter` function would have to
	be modified in order to handle Unicode codepoints. Fortunately this is a
	string handling library, not a font rendering one, so I can get away with
	saying I'm leaving it as an exercise to the reader.

	One thing to keep in mind is that even though we convert the entire input
	string to UTF-32 before rendering it in this example, it's equally valid to
	read only one codepoint at a time. However, you'll need to use #utf8seek to
	move the cursor to the next codepoint.
*/