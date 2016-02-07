#include "TextureD3D.h"

namespace v3d
{
namespace renderer
{
namespace d3d
{

using namespace core;

CTextureD3D::CTextureD3D()
{
}

CTextureD3D::~CTextureD3D()
{
}

void CTextureD3D::bind(u32 layer, u32 sampler)
{
}

void CTextureD3D::unbind(u32 layer, u32 sampler)
{
}

bool CTextureD3D::create()
{
    return false;
}

void CTextureD3D::destroy()
{
}

bool CTextureD3D::isValid()
{
    return false;
}

void CTextureD3D::setData(u32 size, void* data)
{
}

void CTextureD3D::setData(const Vector2DU& size, void* data, u32 cubemapSide)
{
}

void CTextureD3D::setData(const Vector3DU& size, void* data)
{
}

void CTextureD3D::updateData(u32 offset, u32 size, void* data)
{
}

void CTextureD3D::updateData(const Vector2DU& offset, const Vector2DU& size, void* data, u32 cubemapSide)
{
}

void CTextureD3D::updateData(const Vector3DU& offset, const Vector3DU& size, void* data)
{
}

void CTextureD3D::readData(void * data, u32 cubemapSide)
{
}

void CTextureD3D::copyToTexture2D(const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data)
{
}

} //namespace d3d
} //namespace renderer
} //namespace v3d
