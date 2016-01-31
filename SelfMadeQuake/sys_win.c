#include <Windows.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"

/****************************************
 * Command line arguments parsing logic *
 ****************************************/
#define MAX_NUM_ARGS 50
typedef enum parserRC {
	PARSER_SUCCESS,
	PARSER_INVALID_ARG,
	PARSER_TOO_MANY_ARGS,
	PARSER_FAILURE
} ParserRC;

// Parse command line arguments for the program
ParserRC parseCmdLineArgs(char* lpCmdLine) {
	ParserRC parserRC = PARSER_SUCCESS;
	int argsCount = 0;  // A counter for the number of command Line parameters passed
	char* gameArgs[MAX_NUM_ARGS]; // An array that will store char* pointers to each string's array.

	while (0 != *lpCmdLine) {
		if (argsCount >= MAX_NUM_ARGS) {
			parserRC = PARSER_TOO_MANY_ARGS;
			break;
		}

		gameArgs[argsCount] = lpCmdLine;
		argsCount += 1;

		// Read the current argument
		while (*lpCmdLine != 0 && isNonWhitespace(*lpCmdLine)) {
			lpCmdLine++;  // Increment the pointer
		}

		// Skip any additional whitespace.
		while (0 != *lpCmdLine && isWhitespace(*lpCmdLine)) {
			*lpCmdLine = 0;  // Split the string
			lpCmdLine++;  // Increment the pointer
		}
	}

	// Find the index of each param - findArrayIndex("-param param_arg", gameArgs, argsCount)
	// convert string int pointers to int: stringToInt(char* strInt);
	int result = -123123;
	int test = findStringArrayIndex("-number", gameArgs, argsCount);
	if (test != -1) {
		result = stringToInt(gameArgs[test + 1]);
	}

	return (parserRC);
}

/****************************
 * Entry point for windows. *
 ****************************/
// WinAPI documentation: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633559(v=vs.85).aspx
int
#if !defined(_MAC)
#if defined(_M_CEE_PURE)
__clrcall
#else
WINAPI
#endif
#else
CALLBACK
#endif
WinMain(
	// A handle to the current instance of the application.
	_In_ HINSTANCE		hInstance,
	// Legacy
	_In_opt_ HINSTANCE	hPrevInstance,
	// The command line for the application, excluding the program name.
	// To retrieve the entire command line, use the GetCommandLine function.
	_In_ LPSTR			lpCmdLine,
	// Controls how the window is to be shown (based on the properties in the windows program).
	_In_ int			nCmdShow
	) {
	if (parseCmdLineArgs(lpCmdLine) != PARSER_SUCCESS) {
		return (1);
	}
	return (0);
}
