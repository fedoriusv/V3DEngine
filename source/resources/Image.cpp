#include "Image.h"
#include "utils/Logger.h"

namespace v3d
{
namespace resources
{

using namespace renderer;
using namespace stream;

CImage::CImage()
    : m_format(EImageFormat::eRGB)
    , m_type(EImageType::eUnsignedByte)
    , m_size(core::Dimension2D(0, 0))
    , m_data(nullptr)
{
}

CImage::CImage(EImageFormat format, EImageType type, const IStreamPtr& stream)
    : m_format(format)
    , m_type(type)
    , m_size(core::Dimension2D(0, 0))
    , m_data(nullptr)
{
    ASSERT(stream->size() > 0, "Data is nullptr");
    if (stream->size() > 0)
    {
        m_data = new u8[CImage::getByteCount()];
        stream->seekBeg(0);
        stream->read(m_data, CImage::getByteCount(), 1);
        stream->close();
    }
}

CImage::CImage(EImageFormat format, EImageType type, const core::Dimension2D& size, const u8* data)
    : m_format(format)
    , m_type(type)
    , m_size(size)
    , m_data(nullptr)
{
    ASSERT(data, "Data is nullptr");
    if (data)
    {
        m_data = new u8[CImage::getByteCount()];
        memcpy(m_data, data, CImage::getByteCount());
    }
}

CImage::~CImage()
{
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

CImage& CImage::operator=(const CImage& image)
{
    if (this == &image)
    {
        return *this;
    }

    IResource::operator=(image);

    CImage::setFormat(image.m_format);
    CImage::setType(image.m_type);
    m_size = image.m_size;

    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_data = new u8[CImage::getByteCount()];
    memcpy(m_data, image.m_data, CImage::getByteCount());

    return *this;
}

bool CImage::operator==(const CImage& image)
{
    if (m_size != image.m_size)
    {
        return false;
    }

    if (m_format != image.m_format || m_type != image.m_type)
    {
        return false;
    }
   
    if (memcmp(m_data, image.m_data, CImage::getByteCount()) != 0)
    {
        return true;
    }

    return false;
}

void CImage::init(const IStreamPtr& stream)
{
    IResource::setStream(stream);
}

bool CImage::load()
{
    const stream::IStreamPtr& stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CImage: Empty Stream with name [%s]", IResource::getResourseName().c_str());
        return false;
    }

    stream->seekBeg(0);

    u32 format;
    stream->read(format);
    CImage::setFormat(EImageFormat(format));

    u32 type;
    stream->read(type);
    CImage::setType(EImageType(type));

    core::Dimension3D dimension;
    stream->read(&dimension, sizeof(core::Dimension3D), 1);
    m_size.set(dimension.width, dimension.height);
    ASSERT(dimension.depth == 1, "Unsupported image size");

    u32 size = 0;
    stream->read(size);
    ASSERT(size == CImage::getByteCount(), "Sizes not equal");
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_data = new u8[size];
    stream->read(m_data, size);

    IResource::setStream(nullptr);
    IResource::setLoaded(true);

    return true;
}

u32 CImage::getByteCount() const
{
    return m_size.getArea() * CImage::getBytePerPixelCount();
}

u32 CImage::getPixelCount() const
{
    return m_size.getArea();
}

u32 CImage::getBytePerPixelCount() const
{
    return ImageFormat::componentCount(m_format) * ImageFormat::typeSize(m_type);
}

const u8* CImage::getPixel(u32 index) const
{
    if (m_size.isNull() || !m_data)
    {
        return nullptr;
    }
    ASSERT(CImage::getBytePerPixelCount() * index < CImage::getPixelCount(), "Range out");

    return &m_data[CImage::getBytePerPixelCount() * index];
}

void CImage::setPixel(u32 index, const u8* data)
{
    if (m_size.isNull() || !m_data)
    {
        return;
    }
    ASSERT(CImage::getBytePerPixelCount() * index < CImage::getPixelCount(), "Range out");

    memcpy(&m_data[CImage::getBytePerPixelCount() * index], data, CImage::getBytePerPixelCount());
}

void CImage::setPixel(const core::Point2D& point, const u8* data)
{
    u32 index = point.y * m_size.width + point.x;
    CImage::setPixel(index, data);
}

void CImage::fill(const core::Rect32& rect, const u8* data)
{
    ASSERT(rect.getArea() <= (s32)m_size.getArea(), "Range out");

    u32 index = 0;
    while (index < CImage::getPixelCount())
    {
        u32 pointY = index / m_size.width;
        u32 pointX = index - pointY * m_size.width;
        if (rect.isPointInside(core::Point2D(pointX, pointY)))
        {
            memcpy(&m_data[CImage::getBytePerPixelCount() * index], data, CImage::getBytePerPixelCount());
        }
        ++index;
    }
}

CImage* CImage::copyRect(const core::Rect32& rect) const
{
    if (rect.getArea() <= 0)
    {
        ASSERT(false, "Empty rect");
        return nullptr;
    }

    CImage* newImage = new CImage();

    newImage->setFormat(m_format);
    newImage->setType(m_type);
    newImage->m_size.set(rect.getWidth(), rect.getHeight());
    newImage->m_data = new u8[newImage->getByteCount()];

   /* u32 index = 0;
    while (index < newImage->getByteCount())
    {
        u32 pointY = index / newImage->m_size.width;
        u32 pointX = index - pointY * newImage->m_size.width;
        core::Rect32 size(0, 0, m_size.width, m_size.height);
        if (size.isPointInside(core::Vector2DU(pointX, pointY)));
        {
            memcpy(&newImage->m_data[CImage::getBytePerPixelCount() * index], &m_data[], CImage::getBytePerPixelCount());
        }
        ++index;
    }*/

    return newImage;
}

EImageFormat CImage::getFormat() const
{
    return m_format;
}

EImageType CImage::getType() const
{
    return m_type;
}

const u8* CImage::getData() const
{
    return m_data;
}
const core::Dimension2D& CImage::getSize() const
{
    return m_size;
}

void CImage::setFormat(renderer::EImageFormat format)
{
    m_format = format;
}

void CImage::setType(renderer::EImageType type)
{
    m_type = type;
}

} //namespace resources
} //namespace v3d
