#include "RenderTarget.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;

CRenderTarget::CRenderTarget()
: m_colorTexture(nullptr)
, m_depthTexture(nullptr)

, m_color(core::Vector4D(0.0f))

, m_clearColorBuffer(true)
, m_clearDepthBuffer(true)

, m_hasClearColor(true)
, m_hasClearDepth(true)

, m_depthSize(16)

, m_imageFormat(EImageFormat::eRGBA)
, m_imageType(EImageType::eUnsignedByte)

, m_viewportSize(Dimension2D(0U, 0U))
{
}

CRenderTarget::~CRenderTarget()
{
}

const TexturePtr& CRenderTarget::getColorTexture() const
{
    return m_colorTexture;
}

TexturePtr& CRenderTarget::getColorTexture()
{
    return m_colorTexture;
}

const TexturePtr& CRenderTarget::getDepthTexture() const
{
    return m_depthTexture;
}

TexturePtr& CRenderTarget::getDepthTexture()
{
    return m_depthTexture;
}

void CRenderTarget::setClearColor(const core::Vector4D& color)
{
    m_color = color;
}

const core::Vector4D& CRenderTarget::getCearColor() const
{
    return m_color;
}

void CRenderTarget::setViewportSize(const core::Dimension2D& size)
{
    m_viewportSize = size;
}

const core::Dimension2D& CRenderTarget::getViewportSize() const
{
    return m_viewportSize;
}

void CRenderTarget::setColorTexture(const TexturePtr& texture)
{
    m_colorTexture = texture;
}

void CRenderTarget::setDepthTexture(const TexturePtr& texture)
{
    m_depthTexture = texture;
}

void CRenderTarget::setClearColorBuffer(bool clear)
{
    m_clearColorBuffer = clear;
}

bool CRenderTarget::getClearColorBuffer() const
{
    return m_clearColorBuffer;
}

bool CRenderTarget::hasClearColorTarget() const
{
    return m_hasClearColor;
}

void CRenderTarget::setClearDepthBuffer(bool clear)
{
    m_clearDepthBuffer = clear;
}

bool CRenderTarget::getClearDepthBuffer() const
{
    return m_clearDepthBuffer;
}

bool CRenderTarget::hasClearDepthTarget() const
{
    return m_hasClearDepth;
}

EImageFormat CRenderTarget::getImageFormat() const
{
    return m_imageFormat;
}

EImageType CRenderTarget::getImageType() const
{
    return m_imageType;
}

bool CRenderTarget::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderTarget: Not exist xml element");
        return false;
    }

    u32 width = root->IntAttribute("width");
    u32 height = root->IntAttribute("height");
    if (width > 0 && height > 0)
    {
        if (!core::isPowerOf2(width))
        {
            LOG_WARNING("CRenderTarget: Render Target width must be power of 2 - %d", width);
        }

        if (!core::isPowerOf2(height))
        {
            LOG_WARNING("CRenderTarget: Render Target height must be power of 2 - %d", height);
        }
    }

    f64 ratio = root->DoubleAttribute("ratio");
    if (ratio > 0.0)
    {
        width = (u32)(WINDOW->getSize().width * ratio);
        height = (u32)(WINDOW->getSize().height * ratio);
    }

    CRenderTarget::setViewportSize(Dimension2D(width, height));

    const tinyxml2::XMLElement* colorElement = root->FirstChildElement("color");
    if (colorElement)
    {
        m_hasClearColor = colorElement->BoolAttribute("active");
        m_clearColorBuffer = colorElement->BoolAttribute("clear");
        u32 format = colorElement->IntAttribute("format");

        EImageFormat imageFormat = eRGBA;
        EImageType imageType = eUnsignedShort;
        std::function<void(u32)> formatColor = [&imageFormat, &imageType](u32 format)
        {
            switch (format)
            {
            case 8888:
                imageFormat = eRGBA;
                imageType = eUnsignedShort;
                break;

            case 888:
                imageFormat = eRGB;
                imageType = eUnsignedShort;
                break;

            case 565:
                imageFormat = eRGB;
                imageType = eUnsignedShort_565;
                break;

            case 4444:
                imageFormat = eRGBA;
                imageType = eUnsignedShort_4444;
                break;

            default:
                imageFormat = eRGBA;
                imageType = eUnsignedShort;
                LOG_WARNING("CRenderTarget: Color format unknown %d. Set defaut: 8888", format);
                break;
            }
        };

        formatColor(format);
        m_imageType = imageType;
        m_imageFormat = imageFormat;

        if (colorElement->Attribute("color"))
        {
            std::string colorStr = colorElement->Attribute("color");
            u32 color = strtoul(colorStr.c_str(), NULL, 16);

            const f32 k_colorKoef = 0.003922f;

            f32 alpha = core::clamp(((color >> 24) & 0xFF) * k_colorKoef, 0.0f, 1.0f);
            f32 red = core::clamp(((color >> 16) & 0xFF) * k_colorKoef, 0.0f, 1.0f);
            f32 green = core::clamp(((color >> 8) & 0xFF) * k_colorKoef, 0.0f, 1.0f);
            f32 blue = core::clamp((color & 0xFF) * k_colorKoef, 0.0f, 1.0f);

            m_color.set(red, green, blue, alpha);
        }
    }

    const tinyxml2::XMLElement* depthElement = root->FirstChildElement("depth");
    if (depthElement)
    {
        m_hasClearDepth = colorElement->BoolAttribute("active");
        m_clearDepthBuffer = colorElement->BoolAttribute("clear");
        s32 size = colorElement->IntAttribute("size");
        if (size)
        {
            if (core::isPowerOf2(size) || size > 32)
            {
                LOG_ERROR("CRenderTarget: Depth component size must be 16, 24 or 32 - %d", size);
            }
            m_depthSize = size;
        }
    }

    return true;
}