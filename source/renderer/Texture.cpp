#include "Texture.h"
#include "utils/Logger.h"

namespace v3d
{
namespace renderer
{

using namespace core;

CTexture::STextureData::STextureData()
    : _size(Dimension3D(1U, 1U, 1U))
    , _format(eColorIndex)
    , _type(eByte)
    , _data(nullptr)
{
}

CTexture::STextureData::~STextureData()
{
    STextureData::free();
}

void CTexture::STextureData::free()
{
    if (_data)
    {
        delete _data;
        _data = nullptr;
    }
}

void CTexture::STextureData::copy(const Dimension3D& size, EImageType type, void* data)
{
    if (_data == data)
    {
        return;
    }

    STextureData::free();

    auto typeSize = [](EImageType type) -> s32
    {
        switch (type)
        {
        case EImageType::eByte:
            return sizeof(s8);

        case EImageType::eUnsignedByte:
            return sizeof(u8);

        case EImageType::eShort:
            return sizeof(s16);

        case EImageType::eUnsignedShort:
        case EImageType::eUnsignedShort_565:
        case EImageType::eUnsignedShort_4444:
            return sizeof(u16);

        case EImageType::eInt:
            return sizeof(s32);

        case EImageType::eUnsignedInt:
            return sizeof(u32);

        case EImageType::eFloat:
            return sizeof(f32);

        default:
            return 0;
        }
    };

    _size = size;
    _type = type;
    _data = malloc(_size.width * _size.height * _size.depth * typeSize(_type));
    memcpy(_data, data, _size.width * _size.height * _size.depth * typeSize(_type));
}

CTexture::CTexture()
    : CResource()
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
        stream->read(&m_data[i], sizeof(core::Vector3DU), 1);

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
    m_data.clear();
    m_data.resize(0);
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

} //namespace renderer
} //namespace v3d
