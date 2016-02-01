#include "common.h"
#include "winquake.h"

bool isRunning = true;
// Resolution
int windowWidth = 800;
int windowHeight = 600;

// Tests
// Test window size by spliting it into four equal rectangles
void testWindowSize(HWND window, const int width, const int height, const DWORD color) {
	HDC DeviceContext = GetDC(window);
	PatBlt(DeviceContext, 0, 0, width / 2, height / 2, color);

	PatBlt(DeviceContext, width / 2, height / 2, width / 2, height / 2, color);

	ReleaseDC(window, DeviceContext);
}


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
	LRESULT result = 0;

	// catch any relevant messages here
	switch (uMsg) {
	case WM_ACTIVATE:
	case WM_CREATE:
		testWindowSize(hwnd, windowWidth, windowHeight, BLACKNESS);
		break;
	case WM_KEYUP:
		// Posted to the window with the keyboard focus when a nonsystem key is released
		testWindowSize(hwnd, windowWidth, windowHeight, DSTINVERT);
		break;
	case WM_DESTROY:
		isRunning = false;
		break;
	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return (result);
}

HWND generateWindowByResolution(HINSTANCE hInstance,
								LPCTSTR className,
								LPCTSTR windowName,
								const int xPos,
								const int yPos,
								const int nWidth,
								const int nHeight) {

	// Generate window style and actual width / height
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	RECT rect;
	rect.top = rect.left = 0;
	rect.right = nWidth;
	rect.bottom = nHeight;
	AdjustWindowRect(&rect, windowStyle, FALSE);
	int mainWindowWidth = rect.right - rect.left;
	int mainWindowHeight = rect.bottom - rect.top;

	// Create the window and return it
	return CreateWindowEx(	0,
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

	HWND mainWindow = generateWindowByResolution(	hInstance,
													wc.lpszClassName,
													"GameLoop Window",
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													windowWidth,
													windowHeight);

	ShowWindow(mainWindow, SW_SHOWDEFAULT);

	MSG msg;  // will store a message from the OS
	LRESULT dispatchResult;
	while (isRunning) {
		// Check the OS for messages
		if (PeekMessage(&msg,			// A pointer to an MSG structure that receives message information
						NULL,			// if hWnd is NULL, both window messages and thread messages are processed
						0,				// The value of the first message in the range of messages to be examined
						0,				// The value of the last message in the range of messages to be examined
						PM_REMOVE)		// Specifies how messages are to be handled
						) {
			bool messageTranslated = TranslateMessage(&msg);
			// will call MainWndProc
			dispatchResult = DispatchMessage(&msg);

			// Update game loop
			// updateGameLoop();

			// Update graphics
			// drawGraphics();
		}
	}

	return (EXIT_SUCCESS);
}
