#include "common.h"
#include "winquake.h"

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
	_In_ HINSTANCE hInstance,
	// Legacy
	_In_opt_ HINSTANCE hPrevInstance,
	// The command line for the application, excluding the program name.
	// To retrieve the entire command line, use the GetCommandLine function.
	_In_ LPSTR lpCmdLine,
	// Controls how the window is to be shown (based on the properties in the windows program).
	_In_ int nCmdShow
	) {
	if (parseCmdLine(lpCmdLine, MAX_NUM_ARGS, gameArgs, &gameArgsCount) != PARSER_SUCCESS) {
		return (EXIT_FAILURE);
	}

	// TEST: input  -call the program with -number **any int number** output: make sure it is parsed.
	// Find the index of each param - findArrayIndex("-param param_arg", gameArgs, argsCount)
	// convert string int pointers to int: stringToInt(char* strInt);
	int result = -123123;
	int test = findStringArrayIndex("-number", gameArgs, gameArgsCount);
	if (test != -1) {
		result = stringToInt(gameArgs[test + 1]);
	}
	else {
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
