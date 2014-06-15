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

    bool success = false;

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
    std::string file = CResource::getStreamName();
    ILboolean success = ilLoadImage(L"textures/box.jpg");
    ASSERT(success == 1 && "Invalid Texture");

    for (u32 i = 0; i < m_data.size(); ++i)
    {
        m_data[i].width = ilGetInteger(IL_IMAGE_WIDTH);
        m_data[i].height = ilGetInteger(IL_IMAGE_HEIGHT);
        m_data[i].depth = ilGetInteger(IL_IMAGE_DEPTH);
        m_data[i].format = (EImageFormat)ilGetInteger(IL_IMAGE_FORMAT);
        m_data[i].type = (EImageType)ilGetInteger(IL_IMAGE_TYPE);

        ilConvertImage((ILenum)m_data[i].format, (ILenum)m_data[i].type);

        if (m_data[i].data != nullptr)
        {
            delete m_data[i].data;
            m_data[i].data = nullptr;
        }

        m_data[i].data = (ILubyte*)malloc(ilGetInteger(IL_IMAGE_SIZE_OF_DATA));
        memcpy(m_data[i].data, ilGetData(), ilGetInteger(IL_IMAGE_SIZE_OF_DATA));
    }

    return (success == IL_TRUE) ? true : false;
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