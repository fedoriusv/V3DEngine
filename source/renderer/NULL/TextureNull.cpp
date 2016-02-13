#include "TextureNull.h"

namespace v3d
{
namespace renderer
{

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

} //namespace renderer
} //namespace v3d
