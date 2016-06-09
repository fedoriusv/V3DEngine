#include "Texture.h"
#include "utils/Logger.h"
#include "resources/Image.h"
#include "Engine.h"

namespace v3d
{
namespace renderer
{

using namespace core;

CTexture::CTexture()
    : m_impl(nullptr)
{
}

CTexture::CTexture(ETextureTarget target, EImageFormat format, EImageType type, u32 size, const void* data, u32 level)
    : m_impl(DRIVER_CONTEXT->createTexture(target, format, type, Dimension3D(size, 1, 1), data, level))
{
    if (m_impl)
    {
        ASSERT(m_impl->getTarget() == eTexture1D || m_impl->getTarget() == eTextureBuffer, "Invalid target");

        ASSERT(m_impl->getTarget() == eTextureBuffer
            && m_impl->getMipmapLevel() > 0, "Unsupported mipmap target");
    }
}

CTexture::CTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data, u32 level)
    : m_impl(DRIVER_CONTEXT->createTexture(target, format, type, Dimension3D(size.width, size.height, 1), data, level))
{
    if (m_impl)
    {
        ASSERT(m_impl->getTarget() == eTexture1DArray || m_impl->getTarget() == eTexture2D
            || m_impl->getTarget() == eTextureRectangle || m_impl->getTarget() == eTexture2DMSAA, "Invalid target");

        if ((m_impl->getTarget() == eTextureRectangle || m_impl->getTarget() == eTexture2DMSAA))
        {
            ASSERT(m_impl->getMipmapLevel() == 0, "Unsupported mipmap target");
        }

    }
}

CTexture::CTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level)
    : m_impl(DRIVER_CONTEXT->createTexture(target, format, type, size, data, level))
{
    if (m_impl)
    {
        ASSERT(m_impl->getTarget() == eTexture2DArray || m_impl->getTarget() == eTexture3D
            || m_impl->getTarget() == eTexture3DMSAA, "Invalid target");

        ASSERT(m_impl->getTarget() == eTexture3DMSAA
            && m_impl->getMipmapLevel() > 0, "Unsupported mipmap target");
    }
}

CTexture::CTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level)
    : m_impl(DRIVER_CONTEXT->createCubeTexture(format, type, size, data, level))
{
    if (m_impl)
    {
        ASSERT(m_impl->getTarget() == eTextureCubeMap, "Invalid target");
    }
}

CTexture::~CTexture()
{
}

void CTexture::bind(u32 unit)
{
    if (m_impl)
    {
        m_impl->bind(unit);
    }
}

void CTexture::unbind()
{
    if (m_impl)
    {
        m_impl->unbind();
    }
}

bool CTexture::isValid() const
{
    if (m_impl)
    {
        m_impl->isValid();
    }

    return false;
}

void CTexture::update(u32 offset, u32 size, const void* data, u32 level)
{
    if (m_impl)
    {
        m_impl->update(offset, size, data, level);
    }
}

void CTexture::update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level)
{
    if (m_impl)
    {
        m_impl->update(offset, size, data, level);
    }
}

void CTexture::update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 level)
{
    if (m_impl)
    {
        m_impl->update(offset, size, data, level);
    }
}

void CTexture::update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level)
{
    if (m_impl)
    {
        m_impl->update(cubemapSide, offset, size, data, level);
    }
}

void CTexture::read(void* data, u32 level) const
{
    if (m_impl)
    {
        m_impl->read(data, level);
    }
}

void CTexture::read(u32 cubemapSide, void* data, u32 level) const
{
    if (m_impl)
    {
        m_impl->read(cubemapSide, data, level);
    }
}

void CTexture::fill(const void* data, u32 offset, u32 size, u32 level)
{
    if (m_impl)
    {
        m_impl->fill(data, offset, size, level);
    }
}

void CTexture::fill(const void* data, const core::Dimension2D& offset, const core::Dimension2D& size, u32 level)
{
    if (m_impl)
    {
        m_impl->fill(data, offset, size, level);
    }
}

void CTexture::fill(const void* data, const core::Dimension3D& offset, const core::Dimension3D& size, u32 level)
{
    if (m_impl)
    {
        m_impl->fill(data, offset, size, level);
    }
}

ETextureTarget CTexture::getTarget() const
{
    if (m_impl)
    {
        return m_impl->getTarget();
    }

    return ETextureTarget::eTextureNull;
}

ETextureFilter CTexture::getMinFiler() const
{
    if (m_impl)
    {
        return m_impl->getMinFiler();
    }

    return ETextureFilter::eNearest;
}

ETextureFilter CTexture::getMagFiler() const
{
    if (m_impl)
    {
        return m_impl->getMagFiler();
    }

    return ETextureFilter::eNearest;
}

EWrapType CTexture::getWrap() const
{
    if (m_impl)
    {
        return m_impl->getWrap();
    }

    return EWrapType::eRepeat;
}

EAnisotropic CTexture::getAnisotropic()const
{
    if (m_impl)
    {
        return m_impl->getAnisotropic();
    }

    return EAnisotropic::eAnisotropicNone;
}

u32 CTexture::getMipmapLevel() const
{
    if (m_impl)
    {
        return m_impl->getMipmapLevel();
    }

    return 0;
}

const Dimension3D& CTexture::getSize() const
{
    if (m_impl)
    {
        return m_impl->getSize();
    }

    return Dimension3D(0, 0, 0);
}

EImageFormat CTexture::getFormat() const
{
    if (m_impl)
    {
        return m_impl->getFormat();
    }

    return EImageFormat::eFormatCount;
}

EImageType CTexture::getType() const
{
    if (m_impl)
    {
        return m_impl->getType();
    }

    return EImageType::eImageTypeCount;
}

void CTexture::setFilterType(ETextureFilter min, ETextureFilter mag)
{
    if (m_impl)
    {
        return m_impl->setFilterType(min, mag);
    }
}

void CTexture::setAnisotropicLevel(EAnisotropic level)
{
    if (m_impl)
    {
        return m_impl->setAnisotropicLevel(level);
    }
}

TexturePtr CTexture::getImpl() const
{
    return m_impl;
}

TexturePtr CTexture::clone() const
{
    if (!m_impl)
    {
        return nullptr;
    }

    TexturePtr texure;
    if (CTexture::getTarget() == ETextureTarget::eTextureCubeMap)
    {
        texure = new CTexture(m_impl->getFormat(), m_impl->getType(), Dimension2D(m_impl->getSize().width, m_impl->getSize().height),
            nullptr, m_impl->getMipmapLevel());
    }
    else
    {
        texure = new CTexture(m_impl->getTarget(), m_impl->getFormat(), m_impl->getType(), m_impl->getSize(), nullptr, m_impl->getMipmapLevel());
    }
    texure->copyData(m_impl);

    return texure;
}

void CTexture::setWrap(EWrapType wrap)
{
    if (m_impl)
    {
        return m_impl->setWrap(wrap);
    }
}

bool CTexture::isEnable() const
{
    if (m_impl)
    {
        return m_impl->isEnable();
    }

    return false;
}

void CTexture::copyData(const TexturePtr& texture)
{
    if (m_impl)
    {
        m_impl->copyData(texture);
    }
}

} //namespace renderer
} //namespace v3d
