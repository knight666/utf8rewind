/*!
	\page examples Examples

	\tableofcontents

	\section example-changes Changes to existing code

	Suppose you maintain a client application written in C. In order for
	clients to login, the application needs to check their credentials. This
	is accomplished by hashing the specified password with a salt.

	We want to improve the security of the application by allowing the full
	range of Unicode codepoints in the passwords. We can accomplish this by
	encoding the codepoints using UTF-8. The beauty of this change is that the
	password algorithm doesn't have to be changed. Because all functions
	already work on `char` arrays, you can encode Unicode as UTF-8, but treat
	it as ASCII.

	A password like \"M&ocirc;ntiPyth&ocirc;nikdenH&ocirc;lieGr&acirc;ilen\"
	would be encoded as

		"M\xC3\xB4Pyth\xC3\xB4" "denH\xC3\xB4Gr\xC3\xA2" "en"

	which is backwards-compatible with ASCII and calls to `strcpy` and `strcat`
	still work as expected.

	When converting your project to work with UTF-8 encoded text, there are
	only two surface areas you have to make changes in: input and display.

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

	\section example-user-input Dealing with user input

	In the previous example, the password field in the client application only
	accepted ASCII. This is how the password field is currently implemented:

	\code{.c}
		static const size_t g_PasswordInputMaximum = 255;
		static char g_PasswordInput[256];

		int PasswordField_EnterCharacter(char input)
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
*/