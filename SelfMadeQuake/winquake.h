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
	const int nHeight);


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
	const int nHeight)
{

	// Generate window style and actual width / height
	DWORD dwExStyle = 0;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	RECT rect;
	rect.top = rect.left = 0;
	rect.right = nWidth;
	rect.bottom = nHeight;
	AdjustWindowRectEx(&rect, windowStyle, FALSE, dwExStyle);
	int mainWindowWidth = rect.right - rect.left;
	int mainWindowHeight = rect.bottom - rect.top;

	// Create the window and return it
	return CreateWindowEx(
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
}

void WIN_blackoutWindow(HWND window, const int width, const int height);

void WIN_drawBitMap(
	HWND window,
	const int bufferWidth,
	const int bufferHeight,
	void const* backBuffer)
{
	// Bitmap - https://msdn.microsoft.com/en-us/library/windows/desktop/dd183375(v=vs.85).aspx
	BITMAPINFO bitMapInfo = {0};

	// define bitmap info
	bitMapInfo.bmiHeader.biSize = sizeof(bitMapInfo.bmiHeader);
	bitMapInfo.bmiHeader.biWidth = bufferWidth;
	bitMapInfo.bmiHeader.biHeight = bufferHeight;
	bitMapInfo.bmiHeader.biPlanes = 1;
	bitMapInfo.bmiHeader.biBitCount = 32;
	bitMapInfo.bmiHeader.biCompression = BI_RGB;

	HDC deviceContext = GetDC(window);
	StretchDIBits(
		deviceContext,
		0, 0, bufferWidth, bufferHeight,  // Destination coordinates & size
		0, 0, bufferWidth, bufferHeight,  // Source coordinates & size
		backBuffer, &bitMapInfo,
		DIB_RGB_COLORS, SRCCOPY);
	DeleteDC(deviceContext);
}

/***************************
* Exit point for windows. *
***************************/
void WIN_shutdownQuake();

#endif  // _WINQUAKE
