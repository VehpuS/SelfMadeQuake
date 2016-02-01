#include "common.h"
#include "winquake.h"

// Event processing
// An application-defined function that processes messages sent to a window.
// The WNDPROC type defines a pointer to this callback function.
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633573(v=vs.85).aspx
LRESULT CALLBACK MainWndProc(
	_In_ HWND   hwnd,	// Handle to the window this event is connected to
	_In_ UINT   uMsg,	// Message - numerical identifier for the message
	_In_ WPARAM wParam,	// Additional info on the message
	_In_ LPARAM lParam	// Additional info on the message
	) {
	// catch any relevant messages here
	switch (uMsg) {
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
	_In_ int nCmdShow
	) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadImage(	NULL, IDC_ARROW, IMAGE_CURSOR,
							0, 0, LR_DEFAULTSIZE);
	wc.lpszClassName = "GameLoop";

	if (!RegisterClassEx(&wc)) {
		return (EXIT_FAILURE);
	}

	// Generate window style and actual width / height
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	RECT rect;
	rect.top = rect.left = 0;
	rect.right = 800;
	rect.bottom = 600;
	AdjustWindowRect(&rect, windowStyle, FALSE);
	int mainWindowWidth = rect.right - rect.left;
	int mainWindowHeight = rect.bottom - rect.top;

	// Create the window and draw it
	HWND mainWindow = CreateWindowEx(	0,
										"GameLoop",
										"GameLoop Window",
										windowStyle,
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										mainWindowWidth,
										mainWindowHeight,
										NULL,
										NULL,
										hInstance,
										0);

	ShowWindow(mainWindow, SW_SHOWDEFAULT);

	// Test window size
	HDC DeviceContext = GetDC(mainWindow);

	PatBlt(DeviceContext, 0, 0, 400, 300, BLACKNESS);
	PatBlt(DeviceContext, 400, 300, 400, 300, BLACKNESS);
	ReleaseDC(mainWindow, DeviceContext);

	return (EXIT_SUCCESS);
}
