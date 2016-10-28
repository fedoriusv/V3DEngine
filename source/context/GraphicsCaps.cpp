#include "GraphicsCaps.h"

namespace v3d
{
namespace renderer
{

GpraphicsCaps::GpraphicsCaps()
    : m_samplesCount(1U)
    , m_maxTextureUnits(0U)
    , m_maxAnisotropy(0U)
    , m_maxPatchVertices(0U)


    , m_maxColorAttachments(0)
    , m_memoryBlockSize(0)
{
}

GpraphicsCaps::~GpraphicsCaps()
{
}

u32 GpraphicsCaps::getSamplesCount() const
{
    return m_samplesCount;
}

u32 GpraphicsCaps::getTextureUnitsCount() const
{
    return m_maxTextureUnits;
}

f32 GpraphicsCaps::getMaxAnisotropySize() const
{
    return m_maxAnisotropy;
}

u32 GpraphicsCaps::getMaxPatchVertices() const
{
    return m_maxPatchVertices;
}

u32 GpraphicsCaps::getMaxColorAttachments() const
{
    return m_maxColorAttachments;
}

u64 GpraphicsCaps::getMemoryBlockSize() const
{
    return m_memoryBlockSize;
}

void GpraphicsCaps::setSamplesCount(u32 count)
{
    m_samplesCount = count;
}

void GpraphicsCaps::setTextureUnitsCount(u32 count)
{
    m_maxTextureUnits = count;
}

void GpraphicsCaps::setMaxAnisotropySize(f32 size)
{
    m_maxAnisotropy = size;
}

void GpraphicsCaps::setMaxPatchVertices(u32 size)
{
    m_maxPatchVertices = size;
}

void GpraphicsCaps::setMaxColorAttachments(u32 count)
{
    m_maxColorAttachments = count;
}

void GpraphicsCaps::setMemoryBlockSize(u64 size)
{
    m_memoryBlockSize = size;
}

bool GpraphicsCaps::isSupportMultisample() const
{
    return m_samplesCount > 1;
}

//VkPhysicalDeviceLimits::nonCoherentAtomSize

} //namespace renderer
} //namespace v3d
