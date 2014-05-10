#include "WindowWin32.h"

#include <winuser.h>
#include "FedoriaEngine.h"
#include "utils\Logger.h"

using namespace f3d;
using namespace f3d::platform;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CWindowWin32::CWindowWin32(const WindowParam& param)
	: CWindow(param)
	, m_window(nullptr)
	, m_context(nullptr)
{
	m_platformType = EPlatformType::ePlatformWin32;
}

CWindowWin32::~CWindowWin32()
{
}

HWND CWindowWin32::getHandleWindow() const
{
	return m_window;
}

void CWindowWin32::minimize()
{
	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_window, &wndpl);
	wndpl.showCmd = SW_SHOWMINNOACTIVE;
	SetWindowPlacement(m_window, &wndpl);
}

void CWindowWin32::maximize()
{
	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_window, &wndpl);
	wndpl.showCmd = SW_SHOWMAXIMIZED;
	SetWindowPlacement(m_window, &wndpl);
}

void CWindowWin32::restore()
{
	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_window, &wndpl);
	wndpl.showCmd = SW_SHOWNORMAL;
	SetWindowPlacement(m_window, &wndpl);
}

void CWindowWin32::setResizeble(bool value)
{
	if  (m_param.isFullscreen || m_param.isResizeble == value)
	{
		return;
	}

	LONG_PTR style = WS_POPUP;
	if (value)
	{
		style = WS_THICKFRAME | WS_SYSMENU | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	}
	else
	{
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	}
	SetWindowLongPtr(m_window, GWL_STYLE, style);

	RECT clientSize;
	clientSize.top    = 0;
	clientSize.left   = 0;
	clientSize.right  = m_param.size.width;
	clientSize.bottom = m_param.size.height;

	AdjustWindowRect(&clientSize, style, FALSE);

	const s32 realWidth  = clientSize.right - clientSize.left;
	const s32 realHeight = clientSize.bottom - clientSize.top;

	const s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	const s32 windowTop  = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

	SetWindowPos(m_window, HWND_TOP, windowLeft, windowTop, realWidth, realHeight,
		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);

	m_param.isResizeble = value;

}

void CWindowWin32::setFullScreen(bool value)
{
	LONG_PTR style = WS_POPUP;
	if (!value)
	{
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	}
	SetWindowLongPtr(m_window, GWL_STYLE, style);

	RECT clientSize;
	clientSize.top    = 0;
	clientSize.left   = 0;
	clientSize.right  = m_param.size.width;
	clientSize.bottom = m_param.size.height;

	AdjustWindowRect(&clientSize, style, FALSE);

	const s32 realWidth  = clientSize.right - clientSize.left;
	const s32 realHeight = clientSize.bottom - clientSize.top;

	const s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	const s32 windowTop  = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

	if (value)
	{
		SetWindowPos(m_window, HWND_TOP, 0, 0, realWidth, realHeight, SWP_SHOWWINDOW);

		DEVMODE dmScreenSettings;
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = m_param.size.width;
		dmScreenSettings.dmPelsHeight = m_param.size.height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
	
		LONG res = ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		if (res != DISP_CHANGE_SUCCESSFUL)
		{
			// try again without forcing display frequency
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			res = ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		}
	}
	else
	{
		LONG res = ChangeDisplaySettings(NULL, 0);
		
		SetWindowPos(m_window, HWND_TOP, windowLeft, windowTop, realWidth, realHeight,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	m_param.isFullscreen = value;
}

bool CWindowWin32::isMaximized() const
{
	WINDOWPLACEMENT plc;
	plc.length = sizeof(WINDOWPLACEMENT);
	
	bool ret = false;
	if (GetWindowPlacement(m_window, &plc))
	{
		ret = (plc.showCmd & SW_SHOWMAXIMIZED) != 0;
	}

	return ret;
}

bool CWindowWin32::isMinimized() const
{
	WINDOWPLACEMENT plc;
	plc.length = sizeof(WINDOWPLACEMENT);
	
	bool ret = false;
	if (GetWindowPlacement(m_window, &plc))
	{
		ret = (plc.showCmd & SW_SHOWMINIMIZED) != 0;
	}

	return ret;
}

bool CWindowWin32::isActive() const
{
	bool ret = (GetActiveWindow() == m_window);
	return ret;
}

bool CWindowWin32::isFocused() const
{
	bool ret = (GetFocus() == m_window);
	return ret;
}

void CWindowWin32::setCaption(const std::string& text)
{
	SetWindowTextA(m_window, text.c_str());
}

void CWindowWin32::setPosition(const core::Dimension2D& pos)
{
	if (m_param.isFullscreen)
	{
		return;
	}

	/*RECT rect = { xpos, ypos, xpos, ypos };
	AdjustWindowRectEx(&rect, window->win32.dwStyle,FALSE, window->win32.dwExStyle);*/
	SetWindowPos(m_window, NULL, pos.width, pos.height, 0, 0,
		SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

	m_param.position = pos;
}

bool CWindowWin32::begin()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}

	return true;
}

bool CWindowWin32::end()
{
	return SwapBuffers( m_context ) != FALSE;
}

void CWindowWin32::create()
{
	LOG_INFO("Create Windows")

	HINSTANCE hInstance = GetModuleHandle(NULL);
	LPCWSTR className = __TEXT("ValeraWin32");

	// Register Class
	WNDCLASSEX wcex;
	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = NULL;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon          = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = className;
	wcex.hIconSm        = 0;
	
	RegisterClassEx(&wcex);
		
	// calculate client size
	RECT clientSize;
	clientSize.top    = 0;
	clientSize.left   = 0;
	clientSize.right  = m_param.size.width;
	clientSize.bottom = m_param.size.height;

	/*DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD dwExStyle = WS_EX_APPWINDOW;*/

	DWORD dwStyle   = WS_POPUP;
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	if ( !m_param.isFullscreen )
	{
		//dwStyle   = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		dwStyle   = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;;
		//dwExStyle = WS_EX_APPWINDOW;
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	AdjustWindowRectEx(&clientSize, dwStyle, FALSE, dwExStyle);

	const s32 realWidth  = clientSize.right - clientSize.left;
	const s32 realHeight = clientSize.bottom - clientSize.top;
	s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	s32 windowTop  = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

	if ( windowLeft < 0 )
	{
		windowLeft = 0;
	}
	if ( windowTop < 0 )
	{
		windowTop = 0;
	}

	if ( m_param.isFullscreen )
	{
		windowLeft = 0;
		windowTop  = 0;
	}

	// create window
	HWND HWnd = CreateWindowEx(dwExStyle, className, __TEXT(""), dwStyle, windowLeft, windowTop, 
					realWidth, realHeight, NULL, NULL, hInstance, NULL);
	
	ShowWindow(HWnd, SW_SHOWNORMAL);
	UpdateWindow(HWnd);
	
	m_window = HWnd;
	
	HDC hDc = GetDC( m_window );
	m_context = hDc;

	LOG_INFO("Window Size (%d, %d)", m_param.size.width, m_param.size.height)
}

void CWindowWin32::close()
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
	PostQuitMessage(0);
	PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
	
	DestroyWindow(m_window);
	HINSTANCE hInstance = GetModuleHandle(NULL);
	UnregisterClass(__TEXT("ValeraWin32"), hInstance);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			SetWindowLongPtr(hWnd, 0, (LONG_PTR)cs->lpCreateParams);

			break;
		}

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			const int scancode = (lParam >> 16) & 0xff;
			//const int key = translateKey(wParam, lParam);
		
			f3d::event::SKeyboardInputEventPtr event = std::make_shared<f3d::event::SKeyboardInputEvent>();
			event->m_event = f3d::event::eKeyboardPressDown;

			f3d::CFedoriaEngine::getInstance()->getInputEventHandler()->pushEvent(event);

			return 0;
		}

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			/*v3d::event::SKeyboardInputEvent event;
			event.m_event = v3d::event::eKeyboardPressUp;

			v3d::CValeraEngine::getInstance()->getInputEventHandler()->pushEvent(event);*/

			return 0;
		}
		
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			/*v3d::event::SMouseInputEvent event;
			event.m_event = v3d::event::eLeftMousePressedDown;

			v3d::CValeraEngine::getInstance()->getInputEventHandler()->pushEvent(event);*/

			return 0;
		}

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			/*v3d::event::SMouseInputEvent event;
			event.m_event = v3d::event::eLeftMousePressedUp;

			v3d::CValeraEngine::getInstance()->getInputEventHandler()->pushEvent(event);*/

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			//
			return 0;
		}

		case WM_MOUSEWHEEL:
		{
			//
			return 0;
		}
		
		case WM_SIZE:
			{
				//
				return 0;
			}

		case WM_SHOWWINDOW:
		{
							  break;
		}

		case WM_ACTIVATE:
		{
			BOOL focused = LOWORD(wParam) != WA_INACTIVE;
			BOOL iconified = HIWORD(wParam) ? TRUE : FALSE;

			if (focused && iconified)
			{
				// This is a workaround for window iconification using the
				// taskbar leading to windows being told they're focused and
				// iconified and then never told they're defocused
				focused = FALSE;
			}

			return 0;
		}
		
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

