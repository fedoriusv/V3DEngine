#include "TextureD3D.h"

namespace v3d
{
namespace renderer
{
namespace d3d
{

using namespace core;

CTextureD3D::CTextureD3D(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level)
    : m_target(target)
    , m_format(format)
    , m_type(type)
    , m_size(size)

    , m_enable(false)

    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)

    , m_minFilter(level > 0 ? eLinearMipmapLinear : eLinear)
    , m_magFilter(eLinear)
    , m_mipmapLevel(level)

    , m_initialized(false)
{
}

CTextureD3D::CTextureD3D(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level)
    : m_target(eTextureCubeMap)
    , m_format(format)
    , m_type(type)
    , m_size(Dimension3D(size.width, size.height, 1))

    , m_enable(false)

    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)

    , m_minFilter(level > 0 ? eLinearMipmapLinear : eLinear)
    , m_magFilter(eLinear)
    , m_mipmapLevel(level)

    , m_initialized(false)
{
}

CTextureD3D::~CTextureD3D()
{
}

void CTextureD3D::bind(u32 unit)
{
}

void CTextureD3D::unbind()
{
}

bool CTextureD3D::isValid() const
{
    return false;
}

bool CTextureD3D::isEnable() const
{
    return false;
}

void CTextureD3D::update(u32 offset, u32 size, const void* data, u32 level)
{
}

void CTextureD3D::update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level)
{
}

void CTextureD3D::update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 level)
{
}

void CTextureD3D::update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level)
{
}

void CTextureD3D::read(void* data, u32 level) const
{
}

void CTextureD3D::read(u32 cubemapSide, void* data, u32 level) const
{
}

void CTextureD3D::setFilterType(ETextureFilter min, ETextureFilter mag)
{
}

void CTextureD3D::setWrap(EWrapType wrap)
{
}

void CTextureD3D::setAnisotropicLevel(EAnisotropic level)
{
}

void CTextureD3D::fill(const void* data, u32 offset, u32 size, u32 level)
{
}

void CTextureD3D::fill(const void* data, const core::Dimension2D& offset, const core::Dimension2D& size, u32 level)
{
}

void CTextureD3D::fill(const void* data, const core::Dimension3D& offset, const core::Dimension3D& size, u32 level)
{
}

} //namespace d3d
} //namespace renderer
} //namespace v3d
