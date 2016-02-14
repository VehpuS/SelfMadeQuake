#include "common.h"
#include "winquake.h"
#include "instance.h"


// Resolution
int windowWidth = 800;
int windowHeight = 600;


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
	_In_ int nCmdShow)
{
	if (parseCmdLine(lpCmdLine, MAX_NUM_ARGS, gameArgs, &gameArgsCount) != PARSER_SUCCESS)
	{
		return (EXIT_FAILURE);
	}

	// TEST: input  -call the program with -number **any int number** output: make sure it is parsed.
	// Find the index of each parameter - findArrayIndex("-param param_arg", gameArgs, argsCount)
	// convert string int pointers to int: stringToInt(char* strInt);

	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WIN_processSystemMessages;  // The message handling function
	wc.hInstance = hInstance;
	wc.hCursor = LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR,
		0, 0, LR_DEFAULTSIZE);
	wc.lpszClassName = "GameLoop";

	if (!RegisterClassEx(&wc))
	{
		return (EXIT_FAILURE);
	}

	bool fullscreen = false;

	HWND mainWindow = WIN_generateWindowByResolution(
		hInstance,
		wc.lpszClassName,
		"GameLoop Window",
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		fullscreen);

	ShowWindow(mainWindow, SW_SHOWDEFAULT);

	int targetFPS = 72;

	runQuakeGameLoop(
		targetFPS,
		WIN_initProgramTimer,
		WIN_getTotalElapsedTime,
		WIN_checkInput);

	return (EXIT_SUCCESS);
}


/**************
* Timer Code *
**************/
// get timer information from windows
void WIN_initProgramTimer(
	double* const secondsPerTick,
	int64_t* const lastMeasuredTicks,
	bool* const timerIsInitialized)
{
	// Get the OS frequency to calculate time
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency(&ticksPerSecond);

	*secondsPerTick = 1.0 / (double)ticksPerSecond.QuadPart;

	LARGE_INTEGER initialTicks;
	QueryPerformanceCounter(&initialTicks);

	*lastMeasuredTicks = (int64_t)initialTicks.QuadPart;

	*timerIsInitialized = true;
}

// calculate the total elapsed time since the game started
float WIN_getTotalElapsedTime(
	int64_t* const lastMeasuredTicks,
	double* const gameTimePassed)
{
	// If this is called before the timers have been initialized - exit to indicate there is a problem.
#ifdef NDEBUG
	if (!isInitialized)
	{
		WIN_shutdownQuake();
		return (0);
	}
#endif // NDEBUG

	LARGE_INTEGER currentTicks;
	QueryPerformanceCounter(&currentTicks);

	__int64 ticksSinceLastMeasured = currentTicks.QuadPart - (__int64)*lastMeasuredTicks;
	*lastMeasuredTicks = (int64_t)currentTicks.QuadPart;

	double secondsElapsed = (double)ticksSinceLastMeasured * secondsPerTick;
	*gameTimePassed += secondsElapsed;
	return ((float)secondsElapsed);
}


/********************
 * Input processing *
 ********************/
static MSG WIN_sysMsg;  // will store a message from the OS
static LRESULT dispatchResult;

// An application-defined function that processes messages sent to a window.
// The WNDPROC type defines a pointer to this callback function.
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633573(v=vs.85).aspx
LRESULT CALLBACK WIN_processSystemMessages(
	_In_ HWND   hwnd,	// Handle to the window this event is connected to
	_In_ UINT   uMsg,	// Message - numerical identifier for the message
	_In_ WPARAM wParam,	// Additional info on the message
	_In_ LPARAM lParam	// Additional info on the message
	)
{
	LRESULT result = 0;

	// catch any relevant messages here
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_ACTIVATE:
		WIN_blackoutWindow(hwnd, windowWidth, windowHeight);
		break;
	case WM_KEYUP:
		// Posted to the window with the keyboard focus when a nonsystem key is released
		break;
	case WM_DESTROY:
		WIN_shutdownQuake();
		PostQuitMessage(0);
		break;
	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return (result);
}

// Get all OS messages and process them
void WIN_checkInput(void)
{
	while (PeekMessage(
		&WIN_sysMsg,	// A pointer to an MSG structure that receives message information
		NULL,			// if hWnd is NULL, both window messages and thread messages are processed
		0,				// The value of the first message in the range of messages to be examined
		0,				// The value of the last message in the range of messages to be examined
		PM_REMOVE)		// Specifies how messages are to be handled
		)
	{
		// Translate MSG
		bool messageTranslated = TranslateMessage(&WIN_sysMsg);

		// Will call MainWndProc
		dispatchResult = DispatchMessage(&WIN_sysMsg);
	}
}

void WIN_blackoutWindow(HWND window, const int width, const int height)
{
	HDC deviceContext = GetDC(window);
	PatBlt(deviceContext, 0, 0, width, height, BLACKNESS);
	ReleaseDC(window, deviceContext);
}

/***************************
* Exit point for windows. *
***************************/
void WIN_shutdownQuake()
{
	shutdownQuake();
}
