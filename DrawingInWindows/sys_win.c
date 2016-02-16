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

/********************
 * RGB Drawing code *
 ********************/
int colorPixelRGB(
	const byte r,
	const byte g,
	const byte b)
{
	static int RED_POS = 16;  // 3rd byte
	static int GREEN_POS = 8;  // 2nd byte
	static int BLUE_POS = 0;  // 1st byte
	return((r << RED_POS) | (g << GREEN_POS) | (b << BLUE_POS));
}


void drawRandomPixelsRGB(
	const unsigned int bufferWidth,
	const unsigned int bufferHeight,
	int* backBuffer)
{
	if (NULL == backBuffer)
	{
		return;
	}

	for (
		unsigned int y = 0;
		y < bufferHeight;
		y += 1)
	{
		for (
			unsigned int x = 0;
			x < bufferWidth;
			backBuffer++,  // Increment the pointer
			x += 1)
		{
			byte red = randomRGBIntensity();
			byte green = randomRGBIntensity();
			byte blue = randomRGBIntensity();

			// Insert the RBG info into the correct bits in the bitmap
			*backBuffer = colorPixelRGB(red, green, blue);
		}
	}
}

void drawLineRGB(
	const unsigned int topLeftX,
	const unsigned int topLeftY,
	const unsigned int width,
	const unsigned int height,
	const byte r,
	const byte g,
	const byte b,
	const unsigned int bufferWidth,
	const unsigned int bufferHeight,
	int* backBuffer)
{
	if (topLeftX >= bufferWidth || topLeftY >= bufferHeight)
	{
		return;
	}

	int color = colorPixelRGB(r, g, b);

	backBuffer += (topLeftY * bufferWidth) + (topLeftX);

	for (
		unsigned int y = 0;
		y < height &&
		y + topLeftY < bufferHeight;
		y += 1)
	{
		unsigned int x;
		for (
			x = 0;
			x < width &&
			x + topLeftX < bufferWidth;
			backBuffer++,  // Increment the pointer
			x += 1)
		{
			*backBuffer = color;
		}
		backBuffer += bufferWidth - x;
	}
}

void drawRectRGB(
	const unsigned int topLeftX,
	const unsigned int topLeftY,
	const unsigned int width,
	const unsigned int height,
	const unsigned int thickness,
	const byte r,
	const byte g,
	const byte b,
	const unsigned int bufferWidth,
	const unsigned int bufferHeight,
	int* backBuffer)
{
	if (topLeftX >= bufferWidth || topLeftY >= bufferHeight)
	{
		return;
	}

	int color = colorPixelRGB(r, g, b);

	// Draw top line
	drawLineRGB(topLeftX, topLeftY, width, thickness, r, g, b, bufferWidth, bufferHeight, backBuffer);

	// Draw bottom line
	drawLineRGB(topLeftX, topLeftY + (height - thickness), width, thickness, r, g, b, bufferWidth, bufferHeight, backBuffer);

	// Draw sides
	backBuffer += ((topLeftY + thickness) * bufferWidth) + (topLeftX);
	for (
		unsigned int y = 0;
		y < height - 2 * thickness &&
		y + topLeftY < bufferHeight;
		y += 1)
	{
		unsigned int x;
		for (
			x = 0;
			x < width &&
			x + topLeftX < bufferWidth;
			backBuffer++,  // Increment the pointer
			x += 1)
		{
			if (x < thickness || x >= width - thickness)
			{
				*backBuffer = color;
			}
		}
		backBuffer += bufferWidth - x;
	}
}

/***********************
* Palette Drawing code *
************************/
void drawRandomPixelsPalette(
	const unsigned int bufferWidth,
	const unsigned int bufferHeight,
	byte* backBuffer)
{
	if (NULL == backBuffer)
	{
		return;
	}

	for (
		unsigned int y = 0;
		y < bufferHeight;
		y += 1)
	{
		for (
			unsigned int x = 0;
			x < bufferWidth;
			backBuffer++,  // Increment the pointer
			x += 1)
		{
			// Insert the RBG info into the correct bits in the bitmap
			*backBuffer = randomPaletteColor();
		}
	}
}

void drawLine(
	const unsigned int topLeftX,
	const unsigned int topLeftY,
	const unsigned int width,
	const unsigned int height,
	const byte color,
	const unsigned int bufferWidth,
	const unsigned int bufferHeight,
	byte* backBuffer)
{
	if (topLeftX >= bufferWidth || topLeftY >= bufferHeight)
	{
		return;
	}

	backBuffer += (topLeftY * bufferWidth + topLeftX) * PALETTE_BYTES;

	for (
		unsigned int y = 0;
		y < height &&
		y + topLeftY < bufferHeight;
		y += 1)
	{
		unsigned int x;
		for (
			x = 0;
			x < width &&
			x + topLeftX < bufferWidth;
			backBuffer++,  // Increment the pointer
			x += 1)
		{
			*backBuffer = color;
		}
		backBuffer += (bufferWidth - x) * PALETTE_BYTES;
	}
}

void drawRectPalette(
	const unsigned int topLeftX,
	const unsigned int topLeftY,
	const unsigned int width,
	const unsigned int height,
	const unsigned int thickness,
	const byte color,
	const unsigned int bufferWidth,
	const unsigned int bufferHeight,
	byte* backBuffer)
{
	if (topLeftX >= bufferWidth || topLeftY >= bufferHeight)
	{
		return;
	}

	// Draw top line
	drawLine(topLeftX, topLeftY, width, thickness, color, bufferWidth, bufferHeight, backBuffer);

	// Draw bottom line
	drawLine(topLeftX, topLeftY + (height - thickness), width, thickness, color, bufferWidth, bufferHeight, backBuffer);

	// Draw sides
	backBuffer += ((topLeftY + thickness) * bufferWidth + topLeftX) * PALETTE_BYTES;
	for (
		unsigned int y = 0;
		y < height - 2 * thickness &&
		y + topLeftY < bufferHeight;
		y += 1)
	{
		unsigned int x;
		for (
			x = 0;
			x < width &&
			x + topLeftX < bufferWidth;
			backBuffer++,  // Increment the pointer
			x += 1)
		{
			if (x < thickness || x >= width - thickness)
			{
				*backBuffer = color;
			}
		}
		backBuffer += (bufferWidth - x) * PALETTE_BYTES;
	}
}

/************
* Test Code *
*************/
// Test window size by splitting it into four equal rectangles
void testWindowSize(HWND window, const int width, const int height, const DWORD color)
{
	HDC DeviceContext = GetDC(window);
	PatBlt(DeviceContext, 0, 0, width / 2, height / 2, color);

	PatBlt(DeviceContext, width / 2, height / 2, width / 2, height / 2, color);

	ReleaseDC(window, DeviceContext);
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
		// Posted to the window with the keyboard focus when a non-system key is released
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
	bool palette = false;

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

	void* backBuffer;
	BITMAPINFO* bitMapInfo;

	if (palette)
	{
		// Palette drawing
		// initialize a bitmap used for drawing
		void* backBufferPalette = malloc(windowWidth * windowHeight * PALETTE_BYTES);

		dibinfo_t bitMapInfoPalette = {0};

		// define bitmap info
		bitMapInfoPalette.bmiHeader.biSize = sizeof(bitMapInfoPalette.bmiHeader);
		bitMapInfoPalette.bmiHeader.biWidth = windowWidth;
		bitMapInfoPalette.bmiHeader.biHeight = -windowHeight;  // Negative to ensure 0, 0 is top left 
		bitMapInfoPalette.bmiHeader.biPlanes = 1;
		bitMapInfoPalette.bmiHeader.biBitCount = PALETTE_BYTES * BITS_PER_BYTE;
		bitMapInfoPalette.bmiHeader.biCompression = BI_RGB;

		generateRandomColorPalette(bitMapInfoPalette.acolors);

		backBuffer = backBufferPalette;
		bitMapInfo = (BITMAPINFO*)&bitMapInfoPalette;
	}
	else
	{
		// for RGB drawing
		// initialize a bitmap used for drawing
		void* backBufferRGB = malloc(windowWidth * windowHeight * RGB_BYTES);

		// Bitmap - https://msdn.microsoft.com/en-us/library/windows/desktop/dd183375(v=vs.85).aspx
		BITMAPINFO bitMapInfoRGB = {0};

		// define bitmap info
		bitMapInfoRGB.bmiHeader.biSize = sizeof(bitMapInfoRGB.bmiHeader);
		bitMapInfoRGB.bmiHeader.biWidth = windowWidth;
		bitMapInfoRGB.bmiHeader.biHeight = -windowHeight;  // Negative to ensure 0, 0 is top left 
		bitMapInfoRGB.bmiHeader.biPlanes = 1;
		bitMapInfoRGB.bmiHeader.biBitCount = RGB_BYTES * BITS_PER_BYTE;
		bitMapInfoRGB.bmiHeader.biCompression = BI_RGB;

		backBuffer = backBufferRGB;
		bitMapInfo = &bitMapInfoRGB;
	}

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
		for (
			int updateLoops = 0;
			gameLoopElapsedTime > targetTimestep &&
			updateLoops < MAX_UPDATE_LOOPS_BETWEEN_RENDER;
			updateLoops += 1)
		{
			updateFrame(targetTimestep);
			gameLoopElapsedTime -= targetTimestep;
		}

		if (palette)
		{
			drawRandomPixelsPalette(windowWidth, windowHeight, backBuffer);
			drawRectPalette(20, 30, 300, 200, 4, randomPaletteColor(), windowWidth, windowHeight, backBuffer);
			drawRectPalette(50, 60, 900, 250, 9, randomPaletteColor(), windowWidth, windowHeight, backBuffer);
			drawRectPalette(200, 300, 100, 100, 20, randomPaletteColor(), windowWidth, windowHeight, backBuffer);
		}
		else
		{
			drawRandomPixelsRGB(windowWidth, windowHeight, backBuffer);
			drawRectRGB(20, 30, 300, 200, 4, 255, 0, 0, windowWidth, windowHeight, backBuffer);
			drawRectRGB(50, 60, 900, 250, 9, 70, 50, 200, windowWidth, windowHeight, backBuffer);
			drawRectRGB(200, 300, 100, 100, 20, 40, 40, 40, windowWidth, windowHeight, backBuffer);
		}

		renderFrame(gameLoopElapsedTime, targetTimestep);
		
		WIN_displayBitMap(mainWindow, bitMapInfo, windowWidth, windowHeight, backBuffer);
	}

	free(backBuffer);
	
	return (EXIT_SUCCESS);
}
