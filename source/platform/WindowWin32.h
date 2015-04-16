#ifndef _V3D_WINDOW_WIN32_H_
#define _V3D_WINDOW_WIN32_H_

#include "Window.h"
#include <windows.h>

#if _DIRECTINPUT_
#   include <dinput.h>
#endif

namespace v3d
{
namespace platform
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CWindowWin32 : public CWindow
    {
    public:

        CWindowWin32(const WindowParam& param);
        ~CWindowWin32();

        void    minimize()                                  override;
        void    maximize()                                  override;
        void    restore()                                   override;
        void    setFullScreen(bool value = false)           override;
        void    setResizeble(bool value = true)             override;
        void    setCaption(const std::string& text)         override;
        void    setPosition(const core::Point2D& pos)       override;
        void    setCursorPosition(const core::Point2D& pos) override;
        void    getCursorPosition(core::Point2D& pos)       override;
        const core::Point2D& getPosition()                  override;

        bool    isMaximized() const                         override;
        bool    isMinimized() const                         override;
        bool    isActive()    const                         override;
        bool    isFocused()   const                         override;

        bool    begin()                                     override;
        bool    end()                                       override;

        HWND    getHandleWindow() const;

    protected:

        void    create()                                    override;
        void    close()                                     override;

        void    addKeyCodes()                               override;

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        HWND    m_window;
        HDC     m_context;

#if _DIRECTINPUT_
        struct SControllerInfo
        {
            SControllerInfo();
            bool                        init(HINSTANCE instance, HWND window);
            void                        release();
            static BOOL CALLBACK        enumJoysticks(LPCDIDEVICEINSTANCE lpddi, LPVOID cp);
            static BOOL CALLBACK        enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, LPVOID cp);

            HRESULT                     updateInputState();

            enum EControllerType
            {
                eCUnknown = -1,
                eCJoystick,
                eCGamepad
            };

            struct SControllerState
            {
                SControllerState();

                s32                     _index;
                bool                    _connected;
                GUID                    _guid;
                std::string             _name;

                LPDIRECTINPUTDEVICE8    _joy;
                DIDEVCAPS               _devcaps;
                DIJOYSTATE              _lastState;

                EControllerType         _type;

                void                    reset();
            };

            HWND                        _window;
            LPDIRECTINPUT8              _directInput;

            static const u32            k_maxControllers = 4;
            SControllerState            _controllers[k_maxControllers];
        };

        SControllerInfo&                getControllersInfo();
        SControllerInfo                 m_controllersInfo;

#endif //_DIRECTINPUT_

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CWindowWin32>   WindowWin32Ptr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_WINDOW_WIN32_H_
