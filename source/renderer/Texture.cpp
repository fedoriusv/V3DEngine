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
	, m_minFilter(ETextureFilter::eLinearMipmapLinear)
	, m_magFilter(ETextureFilter::eLinear)
    , m_wrap(EWrapType::eClampToEdge)
    , m_anisotropicLevel(EAnisotropic::eAnisotropic16x)
    , m_enable(false)
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

    bool success = false;

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

        case ETextureTarget::eUnknown:
        default:

            ASSERT(true && "Invalid Select Texture target");
            return false;
    }

#ifdef USE_DEVIL
    success = loadDevIL();
#else
    u32 size = stream->size();
    u8* data = new u8[size];
    if (size > 0)
    {
        stream->seekBeg(0);
        stream->read(*data);
    }
#endif //USE_DEVIL
    
    stream->close();

    return success;
}

#ifdef USE_DEVIL
bool CTexture::loadDevIL()
{
    std::function<wchar_t*(const char*)> charToWChar = [](const char* text)
    {
        size_t size = strlen(text) + 1;
        wchar_t* wa = new wchar_t[size];
        mbstowcs(wa, text, size);
        return wa;
    };

    std::string file = CResource::getResourseName();
    
    ilInit();
    ILboolean success = ilLoadImage(charToWChar(file.c_str()));
    ASSERT(success == IL_TRUE && "Invalid Texture");
    if (!success)
    {
        return false;
    }

    CTexture::clear();

    for (u32 i = 0; i < m_data.size(); ++i)
    {
        m_data[i]._width = ilGetInteger(IL_IMAGE_WIDTH);
        m_data[i]._height = ilGetInteger(IL_IMAGE_HEIGHT);
        m_data[i]._depth = ilGetInteger(IL_IMAGE_DEPTH);
        ILenum format = ilGetInteger(IL_IMAGE_FORMAT);
        m_data[i]._format = convertILFormat(format);
        ILenum type = ilGetInteger(IL_IMAGE_TYPE);
        m_data[i]._type = convertILType(type);

        ilConvertImage(format, type);

        u32 size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
        m_data[i]._data = (ILubyte*)malloc(size);
        memcpy(m_data[i]._data, ilGetData(), size);
    }

    return true;
}

EImageFormat CTexture::convertILFormat(u32 format)
{
    switch (format)
    {
        case IL_COLOR_INDEX:
            return EImageFormat::eColorIndex;

        case IL_ALPHA:
            return EImageFormat::eAlpha;

        case IL_RGB:
            return EImageFormat::eRGB;

        case IL_RGBA:
            return EImageFormat::eRGBA;

        case IL_BGR:
            return EImageFormat::eBGR;

        case IL_BGRA:
            return EImageFormat::eBGRA;

        case IL_LUMINANCE:
            return EImageFormat::eLumiance;

        case IL_LUMINANCE_ALPHA:
            return EImageFormat::eLuminanceAlpha;

        default:
            ASSERT(true && "Invalid IL Format");
            return EImageFormat::eRGB;
    }

    return EImageFormat::eRGB;
}

EImageType CTexture::convertILType(u32 type)
{
    switch (type)
    {
    case IL_BYTE:
        return EImageType::eByte;

    case IL_UNSIGNED_BYTE:
        return EImageType::eUnsignedByte;

    case IL_SHORT:
        return EImageType::eShort;

    case IL_UNSIGNED_SHORT:
        return EImageType::eUnsignedShort;

    case IL_INT:
        return EImageType::eInt;

    case IL_UNSIGNED_INT:
        return EImageType::eUnsignedInt;

    case IL_FLOAT:
        return EImageType::eFloat;

    case IL_DOUBLE:
        return EImageType::eDouble;

    case IL_HALF:
        return EImageType::eHalf;

    default:
        ASSERT(true && "Invalid IL Type");
        return EImageType::eUnsignedByte;
    }

    return EImageType::eUnsignedByte;
}

#endif //USE_DEVIL

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