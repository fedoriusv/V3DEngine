#include "Texture.h"

using namespace v3d;
using namespace v3d::renderer;

CTexture::CTexture()
	: CResource()
	, m_textureID(0)
	, m_target(ETextureTarget::eUnknown)
	, m_minFilter(ETextureFilter::eLinear)
	, m_magFilter(ETextureFilter::eLinear)
    , m_wrap(EWrapType::eClampToEdge)
    , m_anisotropicLevel(EAnisotropic::eAnisotropic16x)
{
	m_type = EObjectType::eTypeTexture;
}


CTexture::~CTexture()
{
    CTexture::clear();
    m_data.clear();
}

void CTexture::clear()
{
    for (TextureData::iterator i = m_data.begin(); i < m_data.end(); ++i)
    {
        if ((*i).data != nullptr)
        {
            free((*i).data);
            (*i).data = nullptr;
        }
    }
}

u32 CTexture::getTextureID() const
{
	return m_textureID;
}

ETextureTarget CTexture::getTarget() const
{
	return m_target;
}

ETextureFilter CTexture::getMinFiler() const
{
	return m_minFilter;
}

ETextureFilter CTexture::getMagFiler() const
{
	return m_magFilter;
}

void CTexture::setFilterType(ETextureFilter min, ETextureFilter mag)
{
	m_minFilter = min;
	m_magFilter = mag;
}

void CTexture::setAnisotropicLevel(EAnisotropic level)
{
    m_anisotropicLevel = level;
}

EAnisotropic CTexture::getAnisotropic()const
{
    return m_anisotropicLevel;
}

void CTexture::setWrap(EWrapType wrap)
{
    m_wrap = wrap;
}

EWrapType CTexture::getWrap() const
{
    return m_wrap;
}