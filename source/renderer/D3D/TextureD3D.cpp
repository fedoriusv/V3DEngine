#include "TextureD3D.h"

using namespace v3d;
using namespace renderer;

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

void CTextureD3D::copyToTexture2D(const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data)
{
}