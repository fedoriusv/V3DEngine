#include "TextureNull.h"

using namespace v3d;
using namespace renderer;

CTextureNull::CTextureNull()
{
}

CTextureNull::~CTextureNull()
{
}

void CTextureNull::bind(u32 layer, u32 sampler)
{
}

void CTextureNull::unbind(u32 layer, u32 sampler)
{
}

bool CTextureNull::create()
{
    return true;
}

void CTextureNull::destroy()
{
}

void CTextureNull::copyToTexture2D(const core::Dimension2D& offset, const core::Dimension2D& size, EImageFormat format, void* data)
{
}