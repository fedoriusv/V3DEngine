#include "Texture.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace renderer;

CTexture::CTexture()
    : CResource()
    , m_textureID(0)
    , m_target(eTextureUnknown)
    , m_enable(false)
    , m_minFilter(eLinearMipmapLinear)
    , m_magFilter(eLinear)
    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)
{
}

CTexture::~CTexture()
{
    CTexture::clear();
    m_data.clear();
}

void CTexture::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

bool CTexture::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CTexture: Empty Stream with name [%s] form Texture", CResource::getResourseName().c_str());
        return false;
    }

    switch (m_target)
    {
        case ETextureTarget::eTexture1D:
        case ETextureTarget::eTexture2D:
        case ETextureTarget::eTexture3D:

            m_data.resize(1);
            break;

        case ETextureTarget::eTextureCubeMap:

            m_data.resize(6);
            break;

        case ETextureTarget::eTextureUnknown:
        default:

            ASSERT(true, "Invalid Select Texture target");
            return false;
    }

    stream->seekBeg(0);
    for (u32 i = 0; i < m_data.size(); ++i)
    {
        stream->read(m_data[i]._width);
        stream->read(m_data[i]._height);
        stream->read(m_data[i]._depth);
        
        s32 format = 0;
        stream->read(format);
        m_data[i]._format = (EImageFormat)format;

        s32 type = 0;
        stream->read(type);
        m_data[i]._type = (EImageType)type;

        u32 size = 0;
        stream->read(size);
        m_data[i]._data = (u8*)malloc(size);
        stream->read(m_data[i]._data, size);
    }

    return true;
}

void CTexture::clear()
{
    for (TextureData::iterator i = m_data.begin(); i < m_data.end(); ++i)
    {
        if ((*i)._data != nullptr)
        {
            free((*i)._data);
            (*i)._data = nullptr;
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

bool CTexture::isEnable() const
{
    return m_enable;
}
