#pragma once

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

// Declare additional functions for releasing interfaces and macros for error handling and retrieving the module's base address.
// Below is a FUNCTION template, it avoids the necessity of specifying an exact type for each variable. 
// Instead, C++ provides the capability of defining functions using placeholder types, called template type parameters. 

template<class Interface> inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

// Preprocessor macros...
// http://www.cplusplus.com/doc/tutorial/preprocessor/
// If 'Assert' hasn't been defined by a previous '#define' statement, then execute the following code...

#ifndef Assert
	
	#if defined( DEBUG ) || defined( _DEBUG )
	
		// * Replace any occurences of 'Assert(b)' with 'do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)'
		// * A do{}while(0) loop will execute exactly one time.
		// * It helps grouping multiple statements into a single one, so that a function-like macro can actually be used as a function.
		//		https://stackoverflow.com/a/257424/5058405
		// * OutputDebugStringA() is defined in debugapi.h, some Windows library. It sends a string to the debugger for display.
		// * In this case it sends "Assert b" as the string.

		#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
	
	#else
		#define Assert(b)
	#endif
#endif

#ifndef HINST_THISCOMPONENT

	// EXTERN_C is defined in winnt.h, a Windows library.
	// EXTERN_C is defined as 'extern "C"' within winnt.h.
	// It is to do with C linking...
	
	// IMAGE_DOS_HEADER is a structure defined in winnt.h.
	// It seems to be a DOS .exe header
	// This creates an object called __ImageBase 

	EXTERN_C IMAGE_DOS_HEADER __ImageBase;

	// This is a function-like macro that takes one parameter
	#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

// Declare methods for initializing the class, creating and discarding resources, handling the message loop, rendering content, and the windows procedure.
class DemoApp
{
public:

	DemoApp();
	~DemoApp();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	// Process and dispatch messages
	void RunMessageLoop();

	// Declare pointers for an ID2D1Factory object, an ID2D1HwndRenderTarget object, and two ID2D1SolidColorBrush objects as class members.
private:

	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	// Draw content.
	HRESULT OnRender();

	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
	);

	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

};