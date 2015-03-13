#include "Window.h"

using namespace v3d;
using namespace platform;
using namespace core;

CWindow::CWindow(const WindowParam& param)
    : m_param(param)
    , m_platformType(EPlatformType::ePlatformNull)
{
}

CWindow::~CWindow()
{
}

bool CWindow::isFullscreen() const
{
    return m_param._isFullscreen;
}

bool CWindow::isResizeble() const
{
    return m_param._isResizeble;
}

const Dimension2D& CWindow::getSize() const
{
    return m_param._size;
}

const Point2D& CWindow::getPosition() const
{
    return m_param._position;
}

const EPlatformType CWindow::getPlatformType() const
{
    return m_platformType;
}

const EDriverType CWindow::getDriverType() const
{
    return m_param._driverType;
}

const event::SKeyCodes& CWindow::getKeyCodes()
{
    return m_keys;
}
