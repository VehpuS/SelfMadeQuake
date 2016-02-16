#pragma once
#ifndef _WINQUAKE
#define _WINQUAKE
#include <Windows.h>

/**************
* Timer Code *
**************/
// get timer information from windows
void WIN_initProgramTimer(
	double* const secondsPerTick,
	int64_t* const lastMeasuredTicks,
	bool* const timerIsInitialized);

// calculate the total elapsed time since the game started
float WIN_getTotalElapsedTime(
	int64_t* const lastMeasuredTicks,
	double* const gameTimePassed);


/********************
* Input processing *
********************/
// An application-defined function that processes messages sent to a window.
// The WNDPROC type defines a pointer to this callback function.
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633573(v=vs.85).aspx
LRESULT CALLBACK WIN_processSystemMessages(
	_In_ HWND   hwnd,	// Handle to the window this event is connected to
	_In_ UINT   uMsg,	// Message - numerical identifier for the message
	_In_ WPARAM wParam,	// Additional info on the message
	_In_ LPARAM lParam	// Additional info on the message
	);

// Get all OS messages and process them
void WIN_checkInput(void);

/****************
* Window setup *
****************/
HWND WIN_generateWindowByResolution(
	HINSTANCE hInstance,
	const LPCTSTR className,
	const LPCTSTR windowName,
	const int xPos,
	const int yPos,
	const int nWidth,
	const int nHeight,
	bool fullscreen);


/****************
 * Window setup *   copy into C file when complete
 ****************/
HWND WIN_generateWindowByResolution(
	HINSTANCE hInstance,
	const LPCTSTR className,
	const LPCTSTR windowName,
	const int xPos,
	const int yPos,
	const int nWidth,
	const int nHeight,
	bool fullscreen)
{

	// Generate window style and actual width / height
	DWORD dwExStyle = 0;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	RECT rect;
	rect.top = rect.left = 0;
	rect.right = nWidth;
	rect.bottom = nHeight;
	int mainWindowWidth = rect.right - rect.left;
	int mainWindowHeight = rect.bottom - rect.top;

	// Check if a fullscreen window is requested
	if (fullscreen)
	{
		DEVMODE dmScreenSettings = {0};
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = nWidth;
		dmScreenSettings.dmPelsHeight = nHeight;
		dmScreenSettings.dmBitsPerPel = PALETTE_BYTES * 8;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;

		if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN))
		{
			dwExStyle = WS_EX_APPWINDOW;
			windowStyle = WS_POPUP;
		}
		else
		{
			fullscreen = false;
		}
	}

	AdjustWindowRectEx(&rect, windowStyle, FALSE, dwExStyle);

	// Create the window and return it
	HWND window = CreateWindowEx(
		dwExStyle,
		className,
		windowName,
		windowStyle,
		xPos,
		yPos,
		mainWindowWidth,
		mainWindowHeight,
		NULL,
		NULL,
		hInstance,
		0);

	if (fullscreen)
	{
		SetWindowLong(window, GWL_STYLE, 0);
	}
	return (window);
}

void WIN_blackoutWindow(HWND window, const int width, const int height);

// a "clone" of the BITMAPINFO struct in windows that predefines the size of the bitmap array
typedef struct dibinfo_s {
	BITMAPINFOHEADER bmiHeader;  // windows defined struct
	RGBQUAD acolors[NUM_OF_COLORS_IN_PALETTE];  // color palette.
} dibinfo_t;

void generateRandomColorPalette(RGBQUAD colorPalette[NUM_OF_COLORS_IN_PALETTE])
{
	colorPalette[0].rgbRed = 0;
	colorPalette[0].rgbGreen = 0;
	colorPalette[0].rgbBlue = 0;

	for (unsigned int i = 1; i < NUM_OF_COLORS_IN_PALETTE; i += 1)
	{
		colorPalette[i].rgbRed = randomRGBIntensity();
		colorPalette[i].rgbGreen = randomRGBIntensity();
		colorPalette[i].rgbBlue = randomRGBIntensity();
	}
}

void WIN_displayBitMap(
	HWND window,
	BITMAPINFO* bitMapInfo,
	const int bufferWidth,
	const int bufferHeight,
	void const* backBuffer)
{
	HDC deviceContext = GetDC(window);
	StretchDIBits(
		deviceContext,
		0, 0, bufferWidth, bufferHeight,  // Destination coordinates & size
		0, 0, bufferWidth, bufferHeight,  // Source coordinates & size
		backBuffer,
		bitMapInfo,  // Cast from our own struct to the default windows struct
		DIB_RGB_COLORS, SRCCOPY);
	DeleteDC(deviceContext);
}

/***************************
* Exit point for windows. *
***************************/
void WIN_shutdownQuake();

#endif  // _WINQUAKE
