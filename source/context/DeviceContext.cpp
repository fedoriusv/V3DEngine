#include "DeviceContext.h"

namespace v3d
{
namespace renderer
{

DeviceContext::DeviceContext(const platform::WindowPtr window)
    : m_window(window)

    , m_shaderVersion("")
    , m_renderMajorVersion(0)
    , m_renderMinorVersion(0)

    , m_isVSync(true)
{
}

DeviceContext::~DeviceContext()
{
}

const platform::WindowPtr DeviceContext::getWindow() const
{
    return m_window;
}

bool DeviceContext::isVSync() const
{
    return m_isVSync;
}

const core::Dimension2D& DeviceContext::getWindowSize() const
{
    return m_window->getSize();
}

const std::string& DeviceContext::getShaderVersion() const
{
    return m_shaderVersion;
}

s32 DeviceContext::getRenderMajorVersion() const
{
    return m_renderMajorVersion;
}

s32 DeviceContext::getRenderMinorrVersion() const
{
    return m_renderMinorVersion;
}

u32 DeviceContext::getSamplesCount() const
{
    return m_graphicsCaps.getSamplesCount();
}

u32 DeviceContext::getTextureUnitsCount() const
{
    return m_graphicsCaps.getTextureUnitsCount();
}

f32 DeviceContext::getMaxAnisotropySize() const
{
    return m_graphicsCaps.getMaxAnisotropySize();
}

u32 DeviceContext::getMaxPatchVertices() const
{
    return m_graphicsCaps.getMaxPatchVertices();
}

u64 DeviceContext::getMemoryBlockSize() const
{
    return m_graphicsCaps.getMemoryBlockSize();
}

} //namespace renderer
} //namespace v3d