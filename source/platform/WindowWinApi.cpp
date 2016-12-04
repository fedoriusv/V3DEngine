#include "WindowWinApi.h"
#include "event/EventManager.h"

#include <winuser.h>

#include "Engine.h"
#include "utils\Logger.h"

#if _DIRECTINPUT_
#   pragma comment(lib, "dinput8.lib")
#   pragma comment(lib, "dxguid.lib")
#endif

namespace v3d
{
namespace platform
{

using namespace event;

WindowWinApi::WindowWinApi(const WindowParam& param)
    : Window(param)
    , m_window(NULL)
{
    m_platformType = EPlatformType::ePlatformWindows;
}

WindowWinApi::~WindowWinApi()
{
}

HWND WindowWinApi::getHandleWindow() const
{
    return m_window;
}

void WindowWinApi::minimize()
{
    WINDOWPLACEMENT wndpl;
    wndpl.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_window, &wndpl);
    wndpl.showCmd = SW_SHOWMINNOACTIVE;
    SetWindowPlacement(m_window, &wndpl);
}

void WindowWinApi::maximize()
{
    WINDOWPLACEMENT wndpl;
    wndpl.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_window, &wndpl);
    wndpl.showCmd = SW_SHOWMAXIMIZED;
    SetWindowPlacement(m_window, &wndpl);
}

void WindowWinApi::restore()
{
    WINDOWPLACEMENT wndpl;
    wndpl.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_window, &wndpl);
    wndpl.showCmd = SW_SHOWNORMAL;
    SetWindowPlacement(m_window, &wndpl);
}

void WindowWinApi::setResizeble(bool value)
{
    if (m_param._isFullscreen || m_param._isResizeble == value)
    {
        return;
    }

    DWORD style = WS_POPUP;
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
    clientSize.top = 0;
    clientSize.left = 0;
    clientSize.right = m_param._size.width;
    clientSize.bottom = m_param._size.height;

    AdjustWindowRect(&clientSize, style, FALSE);

    const s32 realWidth = clientSize.right - clientSize.left;
    const s32 realHeight = clientSize.bottom - clientSize.top;

    const s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
    const s32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

    SetWindowPos(m_window, HWND_TOP, windowLeft, windowTop, realWidth, realHeight,
        SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);

    m_param._isResizeble = value;

}

void WindowWinApi::setFullScreen(bool value)
{
    if (m_param._isFullscreen == value)
    {
        return;
    }

    DWORD style = WS_POPUP;
    if (!value)
    {
        style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    }
    SetWindowLongPtr(m_window, GWL_STYLE, style);

    RECT clientSize;
    clientSize.top = 0;
    clientSize.left = 0;
    clientSize.right = m_param._size.width;
    clientSize.bottom = m_param._size.height;

    AdjustWindowRect(&clientSize, style, FALSE);

    const s32 realWidth = clientSize.right - clientSize.left;
    const s32 realHeight = clientSize.bottom - clientSize.top;

    const s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
    const s32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

    if (value)
    {
        SetWindowPos(m_window, HWND_TOP, 0, 0, realWidth, realHeight, SWP_SHOWWINDOW);

        DEVMODE dmScreenSettings;
        EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = m_param._size.width;
        dmScreenSettings.dmPelsHeight = m_param._size.height;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

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

    m_param._isFullscreen = value;
}

bool WindowWinApi::isMaximized() const
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

bool WindowWinApi::isMinimized() const
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

bool WindowWinApi::isActive() const
{
    bool ret = (GetActiveWindow() == m_window);
    return ret;
}

bool WindowWinApi::isFocused() const
{
    bool ret = (GetFocus() == m_window);
    return ret;
}

void WindowWinApi::setCaption(const std::string& text)
{
    SetWindowTextA(m_window, text.c_str());
}

void WindowWinApi::setPosition(const core::Point2D& pos)
{
    if (m_param._isFullscreen)
    {
        return;
    }

    /*RECT rect = { xpos, ypos, xpos, ypos };
    AdjustWindowRectEx(&rect, window->win32.dwStyle,FALSE, window->win32.dwExStyle);*/
    SetWindowPos(m_window, NULL, pos.x, pos.y, 0, 0,
        SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

    m_param._position = pos;
}

const core::Point2D& WindowWinApi::getPosition() const
{
    RECT rect;
    GetWindowRect(m_window, &rect);

    m_param._position.x = rect.left;
    m_param._position.y = rect.top;

    return m_param._position;
}

void WindowWinApi::setCursorPosition(const core::Point2D& pos)
{
    SetCursorPos(pos.x, pos.y);
}

void WindowWinApi::getCursorPosition(core::Point2D& pos) const
{
    POINT mouse;
    GetCursorPos(&mouse);

    pos.x = static_cast<s32>(mouse.x);
    pos.y = static_cast<s32>(mouse.y);
}

bool WindowWinApi::begin()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    msg.message = WM_NULL;

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

bool WindowWinApi::end()
{
    return true;
}

void WindowWinApi::create()
{
    LOG_INFO("Create Windows");

    HINSTANCE hInstance = GetModuleHandle(NULL);
    LPCWSTR className = __TEXT("V3DWin32");

    // Register Class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WindowWinApi::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = className;
    wcex.hIconSm = 0;

    RegisterClassEx(&wcex);

    // calculate client size
    RECT clientSize;
    clientSize.top = 0;
    clientSize.left = 0;
    clientSize.right = m_param._size.width;
    clientSize.bottom = m_param._size.height;

    /*DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    DWORD dwExStyle = WS_EX_APPWINDOW;*/

    DWORD dwStyle = WS_POPUP;
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    if (!m_param._isFullscreen)
    {
        //dwStyle   = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;;
        //dwExStyle = WS_EX_APPWINDOW;
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    }

    AdjustWindowRectEx(&clientSize, dwStyle, FALSE, dwExStyle);

    const s32 realWidth = clientSize.right - clientSize.left;
    const s32 realHeight = clientSize.bottom - clientSize.top;
    s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
    s32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

    if (windowLeft < 0)
    {
        windowLeft = 0;
    }
    if (windowTop < 0)
    {
        windowTop = 0;
    }

    if (m_param._isFullscreen)
    {
        windowLeft = 0;
        windowTop = 0;
    }

    // create window
    HWND HWnd = CreateWindowEx(dwExStyle, className, __TEXT(""), dwStyle, windowLeft, windowTop,
        realWidth, realHeight, NULL, NULL, hInstance, NULL);

    ShowWindow(HWnd, SW_SHOWNORMAL);
    UpdateWindow(HWnd);

    m_window = HWnd;

    LOG_INFO("Window Size (%d, %d)", m_param._size.width, m_param._size.height);

    WindowWinApi::addKeyCodes();

#if _DIRECTINPUT_
    m_controllersInfo.init(hInstance, m_window);
#endif
}

void WindowWinApi::close()
{
#if _DIRECTINPUT_
    m_controllersInfo.release();
#endif

    MSG msg;
    PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
    PostQuitMessage(0);
    PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);

    DestroyWindow(m_window);
    HINSTANCE hInstance = GetModuleHandle(NULL);
    UnregisterClass(__TEXT("V3DWin32"), hInstance);
}

void WindowWinApi::addKeyCodes()
{
    m_keys.add(eKeyUknown, 0x00);
    m_keys.add(eKeyLButton, 0x01);
    m_keys.add(eKeyRButton, 0x02);
    m_keys.add(eKeyCancel, 0x03);
    m_keys.add(eKeyMButton, 0x04);
    m_keys.add(eKeyXButton1, 0x05);
    m_keys.add(eKeyXButton2, 0x06);
    m_keys.add(eKeyBack, 0x08);
    m_keys.add(eKeyTab, 0x09);
    m_keys.add(eKeyClear, 0x0C);
    m_keys.add(eKeyReturn, 0x0D);
    m_keys.add(eKeyShift, 0x10);
    m_keys.add(eKeyControl, 0x11);
    m_keys.add(eKeyMenu, 0x12);
    m_keys.add(eKeyPause, 0x13);
    m_keys.add(eKeyCapital, 0x14);
    m_keys.add(eKeyKana, 0x15);
    m_keys.add(eKeyHanguel, 0x15);
    m_keys.add(eKeyHangul, 0x15);
    m_keys.add(eKeyJunja, 0x17);
    m_keys.add(eKeyFinal, 0x18);
    m_keys.add(eKeyHanja, 0x19);
    m_keys.add(eKeyKanji, 0x19);
    m_keys.add(eKeyEscape, 0x1B);
    m_keys.add(eKeyConvert, 0x1C);
    m_keys.add(eKeyNonconvert, 0x1D);
    m_keys.add(eKeyAccept, 0x1E);
    m_keys.add(eKeyModechange, 0x1F);
    m_keys.add(eKeySpace, 0x20);
    m_keys.add(eKeyPrior, 0x21);
    m_keys.add(eKeyNext, 0x22);
    m_keys.add(eKeyEnd, 0x23);
    m_keys.add(eKeyHome, 0x24);
    m_keys.add(eKeyLeft, 0x25);
    m_keys.add(eKeyUp, 0x26);
    m_keys.add(eKeyRight, 0x27);
    m_keys.add(eKeyDown, 0x28);
    m_keys.add(eKeySelect, 0x29);
    m_keys.add(eKeyPrint, 0x2A);
    m_keys.add(eKeyExecut, 0x2B);
    m_keys.add(eKeySnapshot, 0x2C);
    m_keys.add(eKeyInsert, 0x2D);
    m_keys.add(eKeyDelete, 0x2E);
    m_keys.add(eKeyHelp, 0x2F);
    m_keys.add(eKeyKey_0, 0x30);
    m_keys.add(eKeyKey_1, 0x31);
    m_keys.add(eKeyKey_2, 0x32);
    m_keys.add(eKeyKey_3, 0x33);
    m_keys.add(eKeyKey_4, 0x34);
    m_keys.add(eKeyKey_5, 0x35);
    m_keys.add(eKeyKey_6, 0x36);
    m_keys.add(eKeyKey_7, 0x37);
    m_keys.add(eKeyKey_8, 0x38);
    m_keys.add(eKeyKey_9, 0x39);
    m_keys.add(eKeyKey_A, 0x41);
    m_keys.add(eKeyKey_B, 0x42);
    m_keys.add(eKeyKey_C, 0x43);
    m_keys.add(eKeyKey_D, 0x44);
    m_keys.add(eKeyKey_E, 0x45);
    m_keys.add(eKeyKey_F, 0x46);
    m_keys.add(eKeyKey_G, 0x47);
    m_keys.add(eKeyKey_H, 0x48);
    m_keys.add(eKeyKey_I, 0x49);
    m_keys.add(eKeyKey_J, 0x4A);
    m_keys.add(eKeyKey_K, 0x4B);
    m_keys.add(eKeyKey_L, 0x4C);
    m_keys.add(eKeyKey_M, 0x4D);
    m_keys.add(eKeyKey_N, 0x4E);
    m_keys.add(eKeyKey_O, 0x4F);
    m_keys.add(eKeyKey_P, 0x50);
    m_keys.add(eKeyKey_Q, 0x51);
    m_keys.add(eKeyKey_R, 0x52);
    m_keys.add(eKeyKey_S, 0x53);
    m_keys.add(eKeyKey_T, 0x54);
    m_keys.add(eKeyKey_U, 0x55);
    m_keys.add(eKeyKey_V, 0x56);
    m_keys.add(eKeyKey_W, 0x57);
    m_keys.add(eKeyKey_X, 0x58);
    m_keys.add(eKeyKey_Y, 0x59);
    m_keys.add(eKeyKey_Z, 0x5A);
    m_keys.add(eKeyLWin, 0x5B);
    m_keys.add(eKeyRWin, 0x5C);
    m_keys.add(eKeyApps, 0x5D);
    m_keys.add(eKeySleep, 0x5F);
    m_keys.add(eKeyNumpad0, 0x60);
    m_keys.add(eKeyNumpad1, 0x61);
    m_keys.add(eKeyNumpad2, 0x62);
    m_keys.add(eKeyNumpad3, 0x63);
    m_keys.add(eKeyNumpad4, 0x64);
    m_keys.add(eKeyNumpad5, 0x65);
    m_keys.add(eKeyNumpad6, 0x66);
    m_keys.add(eKeyNumpad7, 0x67);
    m_keys.add(eKeyNumpad8, 0x68);
    m_keys.add(eKeyNumpad9, 0x69);
    m_keys.add(eKeyMultiply, 0x6A);
    m_keys.add(eKeyAdd, 0x6B);
    m_keys.add(eKeySeparator, 0x6C);
    m_keys.add(eKeySubtract, 0x6D);
    m_keys.add(eKeyDecimal, 0x6E);
    m_keys.add(eKeyDivide, 0x6F);
    m_keys.add(eKeyF1, 0x70);
    m_keys.add(eKeyF2, 0x71);
    m_keys.add(eKeyF3, 0x72);
    m_keys.add(eKeyF4, 0x73);
    m_keys.add(eKeyF5, 0x74);
    m_keys.add(eKeyF6, 0x75);
    m_keys.add(eKeyF7, 0x76);
    m_keys.add(eKeyF8, 0x77);
    m_keys.add(eKeyF9, 0x78);
    m_keys.add(eKeyF10, 0x79);
    m_keys.add(eKeyF11, 0x7A);
    m_keys.add(eKeyF12, 0x7B);
    m_keys.add(eKeyF13, 0x7C);
    m_keys.add(eKeyF14, 0x7D);
    m_keys.add(eKeyF15, 0x7E);
    m_keys.add(eKeyF16, 0x7F);
    m_keys.add(eKeyF17, 0x80);
    m_keys.add(eKeyF18, 0x81);
    m_keys.add(eKeyF19, 0x82);
    m_keys.add(eKeyF20, 0x83);
    m_keys.add(eKeyF21, 0x84);
    m_keys.add(eKeyF22, 0x85);
    m_keys.add(eKeyF23, 0x86);
    m_keys.add(eKeyF24, 0x87);
    m_keys.add(eKeyNumlock, 0x90);
    m_keys.add(eKeyScroll, 0x91);
    m_keys.add(eKeyLShift, 0xA0);
    m_keys.add(eKeyRShift, 0xA1);
    m_keys.add(eKeyLControl, 0xA2);
    m_keys.add(eKeyRControl, 0xA3);
    m_keys.add(eKeyLMenu, 0xA4);
    m_keys.add(eKeyRMenu, 0xA5);
    m_keys.add(eKeyPlus, 0xBB);
    m_keys.add(eKeyComma, 0xBC);
    m_keys.add(eKeyMinus, 0xBD);
    m_keys.add(eKeyPeriod, 0xBE);
    m_keys.add(eKeyAttn, 0xF6);
    m_keys.add(eKeyCrSel, 0xF7);
    m_keys.add(eKeyExSel, 0xF8);
    m_keys.add(eKeyErEof, 0xF9);
    m_keys.add(eKeyPlay, 0xFA);
    m_keys.add(eKeyZoom, 0xFB);
    m_keys.add(eKeyPA1, 0xFD);
    m_keys.add(eKeyOem_Clear, 0xFE);
}

#if _DIRECTINPUT_
WindowWinApi::SControllerInfo& WindowWinApi::getControllersInfo()
{
    return m_controllersInfo;
}

WindowWinApi::SControllerInfo::SControllerInfo()
: _directInput(nullptr)
, _window(nullptr)
{
}

bool WindowWinApi::SControllerInfo::init(HINSTANCE instance, HWND window)
{
    _window = window;

    if (DI_OK != (DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInput, NULL)))
    {
        LOG_WARNING("SControllerInfo: Could not create DirectInput8 Object");
        return false;
    }

    if (!_directInput || (_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, SControllerInfo::enumJoysticks, this, DIEDFL_ATTACHEDONLY)))
    {
        LOG_WARNING("SControllerInfo: Could not enum DirectInput8 controllers");
        return false;
    }

    return true;
}

void WindowWinApi::SControllerInfo::release()
{
    for (u32 i = 0; i < k_maxControllers; ++i)
    {
        LPDIRECTINPUTDEVICE8 dev = _controllers[i]._joy;
        if (dev)
        {
            dev->Unacquire();
            dev->Release();
        }
        _controllers[i].reset();
    }

    if (_directInput)
    {
        _directInput->Release();
        _directInput = nullptr;
    }
}

BOOL CALLBACK WindowWinApi::SControllerInfo::enumJoysticks(LPCDIDEVICEINSTANCE lpddi, LPVOID cp)
{
    SControllerInfo* info = reinterpret_cast<SControllerInfo*>(cp);

    const GUID guid = lpddi->guidInstance;
    const std::wstring name = lpddi->tszProductName;
    s32 index = -1;

    for (u32 i = 0; i < info->k_maxControllers; ++i)
    {
        if (!info->_controllers[i]._connected)
        {
            if (DI_OK != info->_directInput->CreateDevice(guid, &info->_controllers[i]._joy, NULL))
            {
                LOG_WARNING("SControllerInfo: Could not create DirectInput device");
                return DIENUM_STOP;
            }

            index = i;
            break;
        }
    }

    if (index > -1)
    {
        SControllerState& controller = info->_controllers[index];
        controller.reset();

        DIDEVICEINSTANCE dev;
        dev.dwSize = sizeof(DIDEVICEINSTANCE);
        if (DI_OK != controller._joy->GetDeviceInfo(&dev))
        {
            LOG_WARNING("SControllerInfo: Cannot get device info");
            return DIENUM_STOP;
        }

        switch (dev.dwDevType & 0xff)
        {
        case DI8DEVTYPE_GAMEPAD:
            controller._type = eCGamepad;
            break;

        case DI8DEVTYPE_JOYSTICK:
            controller._type = eCJoystick;
            break;

        default:
            controller.reset();
            LOG_WARNING("SControllerInfo: Found unknown device");

            return DIENUM_STOP;
        }

        controller._devcaps.dwSize = sizeof(controller._devcaps);
        if (DI_OK != controller._joy->GetCapabilities(&controller._devcaps))
        {
            LOG_WARNING("SControllerInfo: Could not create DirectInput device");
            return DIENUM_STOP;
        }

        if (DI_OK != controller._joy->SetCooperativeLevel(info->_window, DISCL_BACKGROUND | DISCL_EXCLUSIVE))
        {
            LOG_WARNING("SControllerInfo: Could not set DirectInput device cooperative level");
            return DIENUM_STOP;
        }

        if (DI_OK != controller._joy->SetDataFormat(&c_dfDIJoystick))
        {
            LOG_WARNING("SControllerInfo: Could not set DirectInput device data format");
            return DIENUM_STOP;
        }

        if (DI_OK != controller._joy->Acquire())
        {
            LOG_WARNING("SControllerInfo: Could not set DirectInput cooperative level");
            return DIENUM_STOP;
        }

        if (DI_OK != controller._joy->GetDeviceState(sizeof(controller._lastState), &controller._lastState))
        {
            LOG_WARNING("SControllerInfo: Could not read DirectInput device state");
            return DIENUM_STOP;
        }

        controller._connected = true;
        controller._guid = guid;
        controller._name = std::string(name.begin(), name.end());
        controller._index = index;

        if (DI_OK != controller._joy->EnumObjects(SControllerInfo::enumObjectsCallback, &controller, DIDFT_ALL))
        {
            controller.reset();

            LOG_WARNING("SControllerInfo: Could not attach objects callback");
            return DIENUM_STOP;
        }

        LOG_INFO("SControllerInfo: Joystick %s connected with index %d", controller._name.c_str(), controller._index);
    }

    return DIENUM_CONTINUE;
}

BOOL CALLBACK WindowWinApi::SControllerInfo::enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, LPVOID cp)
{
    SControllerState* controller = reinterpret_cast<SControllerState*>(cp);

    if (pdidoi->dwType & DIDFT_AXIS)
    {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof(DIPROPRANGE);
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin = -1000;
        diprg.lMax = +1000;

        // Set the range for the axis
        if (DI_OK != controller->_joy->SetProperty(DIPROP_RANGE, &diprg.diph))
        {
            return DIENUM_STOP;
        }
    }

    return DIENUM_CONTINUE;
}

HRESULT WindowWinApi::SControllerInfo::updateInputState()
{
    DIJOYSTATE js;

    for (u32 i = 0; i < k_maxControllers; ++i)
    {
        if (!_controllers[i]._connected)
        {
            continue;
        }

        HRESULT result = _controllers[i]._joy->Poll();
        if (DI_OK != result)
        {
            result = _controllers[i]._joy->Acquire();
            while (result == DIERR_INPUTLOST)
            {
                result = _controllers[i]._joy->Acquire();
            }
        }

        if (DI_OK != _controllers[i]._joy->GetDeviceState(sizeof(DIJOYSTATE), &js))
        {
            continue;
        }

        bool changed = false;
        event::GamepadInputEventPtr event = std::make_shared<event::SGamepadInputEvent>();
        event->_gamepad = _controllers[i]._index;

        for (u16 j = 0; j < eButtonCount; ++j)
        {
            if (_controllers[i]._lastState.rgbButtons[j] != js.rgbButtons[j])
            {
                event->_event = eGamepadButtonInput;
                changed = true;

                break;
            }
        };

        if (_controllers[i]._lastState.lX != js.lX || _controllers[i]._lastState.lY != js.lY || _controllers[i]._lastState.lZ != js.lZ ||
            _controllers[i]._lastState.lRx != js.lRx || _controllers[i]._lastState.lRy != js.lRy || _controllers[i]._lastState.lRz != js.lRz)
        {
            event->_event = eGamepadDpadDirection;
            changed = true;
        }

        if (_controllers[i]._lastState.rgdwPOV[0] != js.rgdwPOV[0])
        {
            event->_event = eGamepadPOV;
            changed = true;
        }

        _controllers[i]._lastState = js;

        if (changed)
        {
   
            BYTE* byteButtons = js.rgbButtons;
            for (u16 j = 0; j < eButtonCount; ++j)
            {
                if (byteButtons[j] > 0)
                {
                    event->_buttons |= (1 << j);
                }
            };

            event->_axis[eAxis_X] = js.lX;
            event->_axis[eAxis_Y] = js.lY;
            event->_axis[eAxis_Z] = js.lZ;
            event->_axis[eAxis_R] = js.lRx;
            event->_axis[eAxis_U] = js.lRy;
            event->_axis[eAxis_V] = js.lRz;

            event->_pov = js.rgdwPOV[0];

            event::CEventManager::getInstance()->pushEvent(event);
        }
    }

    return S_OK;
}

WindowWinApi::SControllerInfo::SControllerState::SControllerState()
    : _index(-1)
    , _connected(false)
    , _joy(nullptr)
    , _type(eCUnknown)
{
}

void WindowWinApi::SControllerInfo::SControllerState::reset()
{
    _connected = false;
    _name.clear();
    _index = -1;
    _type = eCUnknown;
}
#endif

LRESULT CALLBACK WindowWinApi::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifndef WHEEL_DELTA
#   define WHEEL_DELTA 120
#endif

    switch (message)
    {
        case WM_CREATE:
        {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            SetWindowLongPtr(hWnd, 0, (LONG_PTR)cs->lpCreateParams);

            SetTimer(hWnd, 0, 1000 / 30, NULL);

            return TRUE;
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

            return TRUE;
        }

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            SKeyCodes& keys = const_cast<SKeyCodes&>(ENGINE_WINDOW->getKeyCodes());

            event::KeyboardInputEventPtr event = std::make_shared<event::SKeyboardInputEvent>();
            event->_event = event::eKeyboardPressDown;
            event->_key = keys.get((u32)wParam);
            event->_character = (c8)wParam;

            event::CEventManager::getInstance()->pushEvent(event);

            return TRUE;
        }

        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            SKeyCodes& keys = const_cast<SKeyCodes&>(ENGINE_WINDOW->getKeyCodes());

            event::KeyboardInputEventPtr event = std::make_shared<event::SKeyboardInputEvent>();
            event->_event = event::eKeyboardPressUp;
            event->_key = keys.get((u32)wParam);;
            event->_character = (c8)wParam;

            event::CEventManager::getInstance()->pushEvent(event);

            return TRUE;
        }

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_MOUSEWHEEL:
        {
            event::MouseInputEventPtr event = std::make_shared<event::SMouseInputEvent>();
            event->_position.x = (s16)LOWORD(lParam);
            event->_position.y = (s16)HIWORD(lParam);
            event->_wheel = ((f32)((s16)HIWORD(wParam))) / (f32)WHEEL_DELTA;
            event->_event = eMouseUnknown;
            switch (message)
            {
            case WM_LBUTTONDOWN:
                event->_event = eLeftMousePressedDown;
                break;

            case WM_RBUTTONDOWN:
                event->_event = eRightMousePressedDown;
                break;

            case WM_MBUTTONDOWN:
                event->_event = eMiddleMousePressedDown;
                break;

            case WM_LBUTTONUP:
                event->_event = eLeftMousePressedUp;
                break;

            case WM_RBUTTONUP:
                event->_event = eRightMousePressedUp;
                break;

            case WM_MBUTTONUP:
                event->_event = eMiddleMousePressedUp;
                break;

            case WM_MOUSEMOVE:
                event->_event = eMouseMoved;
                break;

            case WM_MOUSEWHEEL:
                event->_event = eMouseWheel;
                break;

            default:
                event->_event = eMouseUnknown;
                break;
            };

            event::CEventManager::getInstance()->pushEvent(event);

            return TRUE;
        }

        case WM_TIMER:
        {
#if _DIRECTINPUT_
            platform::WindowWinApiPtr window = std::static_pointer_cast<platform::WindowWinApi>(ENGINE_WINDOW);
            if (DI_OK != window->getControllersInfo().updateInputState())
            {
                KillTimer(hWnd, 0);
            }
#endif
            return TRUE;
        }

        case WM_SIZE:
        {
            return TRUE;
        }

        case WM_SHOWWINDOW:
        {
            return TRUE;
        }

        case WM_DESTROY:
        {
            KillTimer(hWnd, 0);
            PostQuitMessage(0);

            return TRUE;
        }

    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

} //namespace platform
} //namespace v3d
