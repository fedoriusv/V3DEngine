#include "RenderBuffer.h"

namespace v3d
{

using namespace core;

namespace renderer
{

CRenderBuffer::CRenderBuffer()
    : m_size(Dimension2D(0, 0))
    , m_format(EImageFormat::eRGB)
    , m_type(EImageType::eUnsignedByte)

    , m_changed(true)
{
}

CRenderBuffer::~CRenderBuffer()
{
}

void CRenderBuffer::setSize(const core::Dimension2D& size)
{
    if (m_size != size)
    {
        m_size = size;
        m_changed = true;
    }

}

const Dimension2D& CRenderBuffer::getSize() const
{
    return m_size;
}

EImageFormat CRenderBuffer::getFormat() const
{
    return m_format;
}

EImageType CRenderBuffer::getType() const
{
    return m_type;
}

void CRenderBuffer::setFormat(EImageFormat format)
{
    if (m_format != format)
    {
        m_format = format;
        m_changed = true;
    }
}

void CRenderBuffer::setType(EImageType type)
{
    if (m_type != type)
    {
        m_type = type;
        m_changed = true;
    }
}

} //namespace renderer
} //namespace v3d
