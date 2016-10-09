#include "GraphicsCaps.h"

namespace v3d
{
namespace renderer
{

GpraphicsCaps::GpraphicsCaps()
    : m_samplesCount(0U)
    , m_maxTextureUnits(0U)
    , m_maxAnisotropy(0U)
    , m_maxPatchVertices(0U)
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

} //namespace renderer
} //namespace v3d
