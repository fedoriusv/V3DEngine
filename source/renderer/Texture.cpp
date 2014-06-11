#include "Texture.h"
#include "utils/Logger.h"
#ifdef USE_DEVIL
#   include "IL/il.h"
#endif

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

void CTexture::init(stream::IStream* stream)
{
    CResource::setStream(stream);
}

bool CTexture::load()
{
    stream::IStream* stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("Empty Stream with name [%s] form Texture", CResource::getResourseName().c_str());
        return false;
    }

    /*u32 size = stream->size();
    u8* data = new u8[size];
    if (size > 0)
    {
        stream->seekBeg(0);
        stream->read(*data);
    }*/
    stream->close();

#ifdef USE_DEVIL
    return loadDevIL();
#endif //USE_DEVIL

    return false;
}

#ifdef USE_DEVIL
bool CTexture::loadDevIL()
{
    /*std::string file = CResource::getResourseName();
    bool success = ilLoadImage((wchar_t*)file.c_str());
    ASSERT(success == 1 && "Invalid Texture");

    m_data. = ilGetInteger(IL_IMAGE_WIDTH);
    _textureData._iHeight = ilGetInteger(IL_IMAGE_HEIGHT);
    _textureData._iDepth = ilGetInteger(IL_IMAGE_DEPTH);
    _textureData._eFormat = (IMAGE_FORMAT)ilGetInteger(IL_IMAGE_FORMAT);
    _textureData._eType = (IMAGE_TYPE)ilGetInteger(IL_IMAGE_TYPE);

    ilConvertImage(_textureData._eFormat, _textureData._eType);

    if (_textureData._data != NULL)
    {
        delete _textureData._data;
        _textureData._data = NULL;
    }

    _textureData._data = (ILubyte*)malloc(ilGetInteger(IL_IMAGE_SIZE_OF_DATA));
    memcpy(_textureData._data, ilGetData(), ilGetInteger(IL_IMAGE_SIZE_OF_DATA));*/

    //return (success == 1) ? true : false;
    return false;
}
#endif //USE_DEVIL

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