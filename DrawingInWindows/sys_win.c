#include "common.h"
#include "winquake.h"
#include "instance.h"

static bool isRunning = false;

// Resolution
int windowWidth = 800;
int windowHeight = 600;

/***************************
* Exit point for windows. *
***************************/
void WIN_shutdownQuake()
{
	shutdownQuake();
}


/************
* Test Code *
*************/
// Test window size by spliting it into four equal rectangles
void testWindowSize(HWND window, const int width, const int height, const DWORD color)
{
	HDC DeviceContext = GetDC(window);
	PatBlt(DeviceContext, 0, 0, width / 2, height / 2, color);

	PatBlt(DeviceContext, width / 2, height / 2, width / 2, height / 2, color);

	ReleaseDC(window, DeviceContext);
}

void drawRandomPixels(HWND window)
{
	void* backBuffer;
	backBuffer = malloc(windowWidth * windowHeight * BYTES_PER_PIXEL);

	int* bufferWritePointer = (int*)backBuffer;
	for (int height = 0; height < windowHeight; height += 1)
	{
		for (
			int width = 0;
			width < windowWidth;
			bufferWritePointer++,  // Increment the pointer
			width += 1)
		{
			int RED_POS = 16;  // 3rd byte
			char red = rand() % 256;
			int GREEN_POS = 8;  // 2nd byte
			char green = rand() % 256;
			int BLUE_POS = 0;  // 1st byte
			char blue = rand() % 256;

			// Insert the RBG info into the correct bits in the bitmap
			*bufferWritePointer = (
				(red << RED_POS) |
				(green << GREEN_POS) |
				(blue << BLUE_POS));
		}
	}

	WIN_drawBitMap(window, windowWidth, windowHeight, backBuffer);

	free(backBuffer);
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
MSG msg;  // will store a message from the OS
LRESULT dispatchResult;

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
		// Posted to the window with the keyboard focus when a nonsystem key is released
		break;
	case WM_ACTIVATE:
		break;
	case WM_KEYUP:
	case WM_DESTROY:
		WIN_shutdownQuake();
		isRunning = false;
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
	while (PeekMessage(&msg,		// A pointer to an MSG structure that receives message information
		NULL,		// if hWnd is NULL, both window messages and thread messages are processed
		0,			// The value of the first message in the range of messages to be examined
		0,			// The value of the last message in the range of messages to be examined
		PM_REMOVE)	// Specifies how messages are to be handled
		)
	{
		// Translate MSG
		bool messageTranslated = TranslateMessage(&msg);

		// Will call MainWndProc
		dispatchResult = DispatchMessage(&msg);
	}
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
	_In_ HINSTANCE hInstance,
	// Legacy
	_In_opt_ HINSTANCE hPrevInstance,
	// The command line for the application, excluding the program name.
	// To retrieve the entire command line, use the GetCommandLine function.
	_In_ LPSTR lpCmdLine,
	// Controls how the window is to be shown (based on the properties in the windows program).
	_In_ int nCmdShow)
{
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WIN_processSystemMessages;  // The message handling function
	wc.hInstance = hInstance;
	wc.hCursor = LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR,
		0, 0, LR_DEFAULTSIZE);
	wc.lpszClassName = "DrawingInWindows";

	if (!RegisterClassEx(&wc))
	{
		return (EXIT_FAILURE);
	}

	bool fullscreen = false;

	HWND mainWindow = WIN_generateWindowByResolution(
		hInstance,
		wc.lpszClassName,
		"DrawingInWindows Window",
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		fullscreen);

	ShowWindow(mainWindow, SW_SHOWDEFAULT);

	// initialize a bitmap used for drawing
	void* backBuffer;
	backBuffer = malloc(windowWidth * windowHeight * BYTES_PER_PIXEL);

	// Prepare the game loop
	int targetFPS = 60;

	// Run platform independent initialization
	initQuake();
	isRunning = true;

	// Setup timing logic
	WIN_initProgramTimer(&secondsPerTick, &lastMeasuredTicks, &timerIsInitialized);
	float targetTimestep = 1.0f / (float)targetFPS;
	float gameLoopElapsedTime = 0;
	float nextFrameStartTime = 0;

	// Designed using tips from http://gameprogrammingpatterns.com/game-loop.html
	while (isRunning)
	{
		// Update timer
		gameLoopElapsedTime = calculateGameLoopElapsedTime(
			nextFrameStartTime,
			&nextFrameStartTime,
			&lastMeasuredTicks,
			&gameTimePassed,
			WIN_getTotalElapsedTime);

		// Check the OS for input
		WIN_checkInput();

		// Start an update loop until we return to "real time" or we've looped too much
		for (int updateLoops = 0;
		gameLoopElapsedTime > targetTimestep &&
			updateLoops < MAX_UPDATE_LOOPS_BETWEEN_RENDER;
			updateLoops += 1)
		{
			updateFrame(targetTimestep);
			gameLoopElapsedTime -= targetTimestep;
		}

		drawRandomPixels(mainWindow);
		renderFrame(gameLoopElapsedTime, targetTimestep);
	}

	free(backBuffer);
	
	return (EXIT_SUCCESS);
}
