#include "Window.h"

namespace v3d
{
namespace platform
{

using namespace core;

Window::Window(const WindowParam& param)
    : m_param(param)
    , m_platformType(EPlatformType::ePlatformNull)
{
}

Window::~Window()
{
}

bool Window::isFullscreen() const
{
    return m_param._isFullscreen;
}

bool Window::isResizeble() const
{
    return m_param._isResizeble;
}

const Dimension2D& Window::getSize() const
{
    return m_param._size;
}

const EPlatformType Window::getPlatformType() const
{
    return m_platformType;
}

const event::SKeyCodes& Window::getKeyCodes()
{
    return m_keys;
}

} //namespace platform
} //namespace v3d
