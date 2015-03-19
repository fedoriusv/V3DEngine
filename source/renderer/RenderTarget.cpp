#include "RenderTarget.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;

CRenderTarget::SAttachments::SAttachments()
: _index(0U)
, _type(eEmptyAttach)
, _output(eTextureOutput)
, _format(8888)

, _texture(nullptr)
, _rboID(0U)
{
}

CRenderTarget::CRenderTarget()
: m_color(core::Vector4D(0.0f))
, m_viewport(0, 0, 0, 0)

, m_clearColorBuffer(true)
, m_clearDepthBuffer(true)
, m_clearStencilBuffer(false)
, m_name("default")
{
    u32 width = (u32)(WINDOW->getSize().width);
    u32 height = (u32)(WINDOW->getSize().height);
    CRenderTarget::setViewport(Rect(0, 0, width, height));
}

CRenderTarget::~CRenderTarget()
{
    m_attachmentsList.clear();
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

const TexturePtr& CRenderTarget::getStencilTexture() const
{
    std::deque<SAttachments>::const_iterator attach;
    for (std::deque<SAttachments>::const_iterator item = m_attachmentsList.cbegin(); item < m_attachmentsList.cend(); ++item)
    {
        if ((*item)._type == eStencilAttach)
        {
            attach = item;
            break;
        }
    }

    return (*attach)._texture;
}

TexturePtr& CRenderTarget::getStencilTexture()
{
    std::deque<SAttachments>::iterator attach;
    for (std::deque<SAttachments>::iterator item = m_attachmentsList.begin(); item < m_attachmentsList.end(); ++item)
    {
        if ((*item)._type == eStencilAttach)
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

const core::Vector4D& CRenderTarget::getClearColor() const
{
    return m_color;
}

void CRenderTarget::setViewport(const core::Rect& size)
{
    m_viewport = size;
}

const core::Rect& CRenderTarget::getViewport() const
{
    return m_viewport;
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

bool CRenderTarget::getClearStencilBuffer() const
{
    return m_clearStencilBuffer;
}

const std::string& CRenderTarget::getName() const
{
    return m_name;
}

bool CRenderTarget::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderTarget: Not exist xml element <targets>");
        return false;
    }

    if (!root->Attribute("name"))
    {
        LOG_ERROR("CRenderTarget: render target have not name");
        return false;
    }
    m_name = root->Attribute("name");

    u32 x = root->UnsignedAttribute("x");
    u32 y = root->UnsignedAttribute("y");
    u32 width = root->UnsignedAttribute("width");
    u32 height = root->UnsignedAttribute("height");

    f64 ratio = root->DoubleAttribute("ratio");
    if (ratio > 0.0)
    {
        x = 0;
        y = 0;
        width = (u32)(WINDOW->getSize().width * ratio);
        height = (u32)(WINDOW->getSize().height * ratio);
    }

    CRenderTarget::setViewport(Rect(x, y, x + width, y + height));

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

    //color
    const tinyxml2::XMLElement* colorElement = root->FirstChildElement("color");
    if (colorElement)
    {
        bool hasClearColor = colorElement->BoolAttribute("active");
        if (hasClearColor)
        {
            bool clearColorBuffer = colorElement->BoolAttribute("clear");
            CRenderTarget::setClearColorBuffer(clearColorBuffer);
            
            s32 format = colorElement->IntAttribute("format");
            u32 attachCount = colorElement->UnsignedAttribute("attach");
            
            EAttachmentsOutput output = eTextureOutput;
            if (colorElement->Attribute("output"))
            {
                std::string outputStr = colorElement->Attribute("output");
                
                std::function<EAttachmentsOutput(std::string&)> getAttachmentoutput = [](std::string& str)->EAttachmentsOutput
                {
                    if (str == "texture")
                    {
                        return eTextureOutput;
                    }
                    else if (str == "render")
                    {
                        return eRenderOutput;
                    }
                    
                    LOG_ERROR("CRenderTarget: output format %s unknown. Set render to texure", str.c_str());
                    return eTextureOutput;
                };

                output = getAttachmentoutput(outputStr);
            }

            for (u32 index = 0; index < attachCount; ++index)
            {
                CRenderTarget::attachTarget(eColorAttach, index, format, output);
            }
        }
    }

    //depth
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

            EAttachmentsOutput output = eTextureOutput;
            if (depthElement->Attribute("output"))
            {
                std::string outputStr = depthElement->Attribute("output");

                std::function<EAttachmentsOutput(std::string&)> getAttachmentOutput = [](std::string& str) -> EAttachmentsOutput
                {
                    if (str == "texture")
                    {
                        return eTextureOutput;
                    }
                    else if (str == "render")
                    {
                        return eRenderOutput;
                    }

                    LOG_ERROR("CRenderTarget: output format %s unknown. Set render to texure", str.c_str());
                    return eTextureOutput;
                };

                output = getAttachmentOutput(outputStr);
            }

            CRenderTarget::attachTarget(eDepthAttach, 0, size, output);
        }
    }

    //stencil
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

            EAttachmentsOutput output = eTextureOutput;
            if (colorElement->Attribute("output"))
            {
                std::string outputStr = colorElement->Attribute("output");

                std::function<EAttachmentsOutput(std::string&)> getAttachmentOutput = [](std::string& str) -> EAttachmentsOutput
                {
                    if (str == "texture")
                    {
                        return eTextureOutput;
                    }
                    else if (str == "render")
                    {
                        return eRenderOutput;
                    }

                    LOG_ERROR("CRenderTarget: output format %s unknown. Set render to texure", str.c_str());
                    return eTextureOutput;
                };

                output = getAttachmentOutput(outputStr);
            }

            CRenderTarget::attachTarget(eStencilAttach, 0, size, output);
        }
    }

    return true;
}

void CRenderTarget::attachTarget(EAttachmentsType type, u32 index, u32 format, EAttachmentsOutput output)
{
    SAttachments target;

    target._type = type;
    target._index = index;
    target._format = format;
    target._output = output;
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
