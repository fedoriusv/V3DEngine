#pragma once

#include "common.h"
#include "event/InputEventKeyboard.h"
#include "renderer/Renderer.h"

namespace v3d
{
namespace platform
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class EPlatformType
    {
        ePlatformNull = -1,
        ePlatformWindows,
        ePlatformLinux,
        ePlatformMacOSX
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    struct WindowParam
    {
        renderer::ERenderType   _renderType;
        core::Dimension2D       _size;
        core::Point2D           _position;
        bool                    _isFullscreen;
        bool                    _isResizeble;
        bool                    _isVisible;

        WindowParam()
            : _size(core::Dimension2D(1024U, 768U))
            , _position(core::Point2D(0U, 0U))
            , _isFullscreen(false)
            , _isResizeble(false)
            , _isVisible(true)
        {
        }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class Platform;

    class Window
    {
    public:

        Window(const WindowParam& param);
        virtual                         ~Window();

        virtual void                    minimize() = 0;
        virtual void                    maximize() = 0;
        virtual void                    restore() = 0;
        virtual void                    setFullScreen(bool value = true) = 0;
        virtual void                    setResizeble(bool value = true) = 0;
        virtual void                    setCaption(const std::string& text) = 0;
        virtual void                    setPosition(const core::Point2D& pos) = 0;
        virtual void                    setCursorPosition(const core::Point2D& pos) = 0;

        virtual bool                    isMaximized() const = 0;
        virtual bool                    isMinimized() const = 0;
        virtual bool                    isActive() const = 0;
        virtual bool                    isFocused() const = 0;
        virtual const core::Point2D&    getPosition() const = 0;
        virtual void                    getCursorPosition(core::Point2D& pos) const = 0;


        bool                            isFullscreen() const;
        bool                            isResizeble() const;
        const core::Dimension2D&        getSize() const;
        const EPlatformType             getPlatformType() const;

        virtual bool                    begin() = 0;
        virtual bool                    end() = 0;

        virtual void                    close() = 0;

        const event::SKeyCodes&         getKeyCodes();

    protected:

        friend                          Platform;

        virtual void                    create() = 0;

        virtual void                    addKeyCodes() = 0;

        mutable WindowParam             m_param;
        EPlatformType                   m_platformType;

        event::SKeyCodes                m_keys;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    using WindowPtr  =                   std::shared_ptr<Window>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace platform
} //namespace v3d
