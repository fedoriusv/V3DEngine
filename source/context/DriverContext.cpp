#include "DriverContext.h"

namespace v3d
{
namespace renderer
{

CDriverContext::CDriverContext(const platform::WindowPtr& window)
    : m_window(window)
    , m_samplesCount(0U)
    , m_maxTextureUnits(0U)
    , m_maxAnisotropy(0U)

    , m_shaderVersion("")
    , m_renderMajorVersion(0)
    , m_renderMinorVersion(0)
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

u32 CDriverContext::getSamplesCount() const
{
    return m_samplesCount;
}

s32 CDriverContext::getTextureUnitsCount() const
{
    return m_maxTextureUnits;
}

f32 CDriverContext::getMaxAnisotropySize() const
{
    return m_maxAnisotropy;
}

const std::string& CDriverContext::getShaderVersion() const
{
    return m_shaderVersion;
}

s32 CDriverContext::getRenderMajorVersion() const
{
    return m_renderMajorVersion;
}

s32 CDriverContext::getRenderMinorrVersion() const
{
    return m_renderMinorVersion;
}

void CDriverContext::setSamplesCount(u32 count)
{
    m_samplesCount = count;
}

void CDriverContext::setTextureUnitsCount(u32 count)
{
    m_maxTextureUnits = count;
}

void CDriverContext::setMaxAnisotropySize(f32 size)
{
    m_maxAnisotropy = size;
}

} //namespace renderer
} //namespace v3d
