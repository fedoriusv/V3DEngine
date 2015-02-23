#include "RenderTarget.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;

CRenderTarget::CRenderTarget()
: m_color(core::Vector4D(0.0f))
, m_viewportSize(Dimension2D(0U, 0U))

, m_clearColorBuffer(true)
, m_clearDepthBuffer(true)
, m_clearStencilBuffer(false)
{
}

CRenderTarget::~CRenderTarget()
{
}

const TexturePtr& CRenderTarget::getColorTexture(u32 index) const
{
    std::deque<SAttachments>::const_iterator attach;
    for (std::deque<SAttachments>::const_iterator item = m_attachmentsList.cbegin(); item < m_attachmentsList.cend(); ++item)
    {
        if ((*item)._type == eColorAttach && (*item)._index == index)
        {
            attach = item;
            break;
        }
    }

    return (*attach)._texture;
}

TexturePtr& CRenderTarget::getColorTexture(u32 index)
{
    std::deque<SAttachments>::iterator attach;
    for (std::deque<SAttachments>::iterator item = m_attachmentsList.begin(); item < m_attachmentsList.end(); ++item)
    {
        if ((*item)._type == eColorAttach && (*item)._index == index)
        {
            attach = item;
            break;
        }
    }

    return (*attach)._texture;
}

const TexturePtr& CRenderTarget::getDepthTexture() const
{
    std::deque<SAttachments>::const_iterator attach;
    for (std::deque<SAttachments>::const_iterator item = m_attachmentsList.cbegin(); item < m_attachmentsList.cend(); ++item)
    {
        if ((*item)._type == eDepthAttach)
        {
            attach = item;
            break;
        }
    }

    return (*attach)._texture;
}

TexturePtr& CRenderTarget::getDepthTexture()
{
    std::deque<SAttachments>::iterator attach;
    for (std::deque<SAttachments>::iterator item = m_attachmentsList.begin(); item < m_attachmentsList.end(); ++item)
    {
        if ((*item)._type == eDepthAttach)
        {
            attach = item;
            break;
        }
    }

    return (*attach)._texture;
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

void CRenderTarget::setClearColorBuffer(bool clear)
{
    m_clearColorBuffer = clear;
}

void CRenderTarget::setClearDepthBuffer(bool clear)
{
    m_clearDepthBuffer = clear;
}

void CRenderTarget::setClearStencilBuffer(bool clear)
{
    m_clearStencilBuffer = clear;
}

bool CRenderTarget::getClearColorBuffer() const
{
    return m_clearColorBuffer;
}

bool CRenderTarget::getClearDepthBuffer() const
{
    return m_clearDepthBuffer;
}

bool CRenderTarget::getclearStencilBuffer() const
{
    return m_clearStencilBuffer;
}

bool CRenderTarget::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderTarget: Not exist xml element");
        return false;
    }

    u32 width = root->UnsignedAttribute("width");
    u32 height = root->UnsignedAttribute("height");
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


    if (root->Attribute("color"))
    {
        std::string colorStr = root->Attribute("color");
        u32 color = strtoul(colorStr.c_str(), NULL, 16);

        const f32 k_colorKoef = 0.003922f;

        f32 alpha = core::clamp(((color >> 24) & 0xFF) * k_colorKoef, 0.0f, 1.0f);
        f32 red = core::clamp(((color >> 16) & 0xFF) * k_colorKoef, 0.0f, 1.0f);
        f32 green = core::clamp(((color >> 8) & 0xFF) * k_colorKoef, 0.0f, 1.0f);
        f32 blue = core::clamp((color & 0xFF) * k_colorKoef, 0.0f, 1.0f);

        m_color.set(red, green, blue, alpha);
    }

    const tinyxml2::XMLElement* colorElement = root->FirstChildElement("color");
    if (colorElement)
    {
        bool hasClearColor = colorElement->BoolAttribute("active");
        if (hasClearColor)
        {
            bool clearColorBuffer = colorElement->BoolAttribute("clear");
            CRenderTarget::setClearColorBuffer(clearColorBuffer);
            
            const tinyxml2::XMLElement* attachElement = colorElement->FirstChildElement("attach");
            while (attachElement)
            {
                u32 index = attachElement->UnsignedAttribute("index");
                s32 format = attachElement->IntAttribute("format");
                CRenderTarget::attachTarget(eColorAttach, index, format);

                attachElement = attachElement->NextSiblingElement("attach");
            }
        }
    }

    const tinyxml2::XMLElement* depthElement = root->FirstChildElement("depth");
    if (depthElement)
    {
        bool hasClearDepth = depthElement->BoolAttribute("active");
        if (hasClearDepth)
        {
            bool clearDepthBuffer = depthElement->BoolAttribute("clear");
            CRenderTarget::setClearDepthBuffer(clearDepthBuffer);

            s32 size = depthElement->IntAttribute("format");
            if (size)
            {
                if (core::isPowerOf2(size) || size > 32)
                {
                    LOG_ERROR("CRenderTarget: Depth component size must be 16, 24 or 32 - %d", size);
                }
            }
            else
            {
                LOG_WARNING("CRenderTarget: Depth component size not found. Set default 16");
                size = 16;
            }

            CRenderTarget::attachTarget(eDepthAttach, 0, size);
        }
    }

    const tinyxml2::XMLElement* stencilElement = root->FirstChildElement("stencil");
    if (stencilElement)
    {
        bool hasClearDepth = stencilElement->BoolAttribute("active");
        if (hasClearDepth)
        {
            bool clearStencilBuffer = stencilElement->BoolAttribute("clear");
            CRenderTarget::setClearStencilBuffer(clearStencilBuffer);

            s32 size = stencilElement->IntAttribute("format");
            if (size)
            {
                if (core::isPowerOf2(size) || size > 32)
                {
                    LOG_ERROR("CRenderTarget: Stencil component size must be 16, 24 or 32 - %d", size);
                }
            }
            else
            {
                LOG_WARNING("CRenderTarget: Stencil component size not found. Set default 16");
                size = 16;
            }

            CRenderTarget::attachTarget(eStencilAttach, 0, size);
        }
    }

    return true;
}

void CRenderTarget::attachTarget(EAttachmentsType type, u32 index, u32 format)
{
    SAttachments target;

    target._type = type;
    target._index = index;
    target._format = format;
    target._texture = nullptr;

    if (type == eColorAttach)
    {
        m_attachmentsList.push_back(target);
    }
    else
    {
        m_attachmentsList.push_front(target);
    }
}
