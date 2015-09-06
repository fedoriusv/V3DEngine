#include "DriverContext.h"

using namespace v3d;
using namespace renderer;

CDriverContext::CDriverContext(const platform::WindowPtr& window)
    : m_window(window)
    , m_samplersCount(0U)
    , m_maxTextureUnits(0U)
    , m_maxAnisotropy(0U)
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

u32 CDriverContext::getSamplersCount() const
{
    return m_samplersCount;
}

s32 CDriverContext::getTextureUnits() const
{
    return m_maxTextureUnits;
}

f32 CDriverContext::getMaxAnisotropySize() const
{
    return m_maxAnisotropy;
}