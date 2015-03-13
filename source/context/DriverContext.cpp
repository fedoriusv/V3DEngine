#include "DriverContext.h"

using namespace v3d;
using namespace renderer;

CDriverContext::CDriverContext(const platform::WindowPtr& window)
    : m_window(window)
{
}

CDriverContext::~CDriverContext()
{
}

const platform::WindowPtr& CDriverContext::getWindow() const
{
    return m_window;
}

const core::Dimension2D& CDriverContext::getWindowSize() const
{
    return m_window->getSize();
}