#include "RenderTarget.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

using namespace core;

CRenderTarget::SAttachments::SAttachments()
: _index(0U)
, _type(eEmptyAttach)
, _output(eEmptyOutput)
, _format(8888)

, _texture(nullptr)
, _bufferId(0U) //TODO: will need replace to current render module
{
}

CRenderTarget::SAttachments::~SAttachments()
{
    if (_texture)
    {
        delete _texture;
        _texture = nullptr;
    }

    ASSERT(_bufferId == 0, "Buffer doesn't deleted");
}

CRenderTarget::CRenderTarget()
    : m_color(core::Vector4D(0.0f))
    , m_viewport(0U, 0U, 0U, 0U)
    , m_MSAA(false)

    , m_clearColorBuffer(true)
    , m_clearDepthBuffer(true)
    , m_clearStencilBuffer(false)
{
    m_targetType = ITarget::ETagetType::eRenderTarget;
    m_name = "default";

    u32 width = (u32)(WINDOW->getSize().width);
    u32 height = (u32)(WINDOW->getSize().height);
    CRenderTarget::setViewport(Rect32(0, 0, width, height));
}

CRenderTarget::~CRenderTarget()
{
    m_attachmentsList.clear();
}

const CTexture* CRenderTarget::getColorTexture(u32 index) const
{
    auto findPred = [index](const SAttachments& attach) -> bool
    {
        if (attach._type == eColorAttach && attach._index == index)
        {
            return true;
        }

        return false;
    };

    std::deque<SAttachments>::const_iterator item = std::find_if(m_attachmentsList.cbegin(), m_attachmentsList.cend(), findPred);
    if (item != m_attachmentsList.cend())
    {
        return (*item)._texture;
    }

    return nullptr;
}

CTexture* CRenderTarget::getColorTexture(u32 index)
{
    auto findPred = [index](const SAttachments& attach) -> bool
    {
        if (attach._type == eColorAttach && attach._index == index)
        {
            return true;
        }

        return false;
    };

    std::deque<SAttachments>::iterator item = std::find_if(m_attachmentsList.begin(), m_attachmentsList.end(), findPred);
    if (item != m_attachmentsList.cend())
    {
        return (*item)._texture;
    }

    return nullptr;
}

const CTexture* CRenderTarget::getDepthTexture() const
{
    auto findPred = [](const SAttachments& attach) -> bool
    {
        if (attach._type == eDepthAttach)
        {
            return true;
        }

        return false;
    };

    std::deque<SAttachments>::const_iterator item = std::find_if(m_attachmentsList.cbegin(), m_attachmentsList.cend(), findPred);
    if (item != m_attachmentsList.cend())
    {
        return (*item)._texture;
    }

    return nullptr;
}

CTexture* CRenderTarget::getDepthTexture()
{
    auto findPred = [](const SAttachments& attach) -> bool
    {
        if (attach._type == eDepthAttach)
        {
            return true;
        }

        return false;
    };

    std::deque<SAttachments>::iterator item = std::find_if(m_attachmentsList.begin(), m_attachmentsList.end(), findPred);
    if (item != m_attachmentsList.cend())
    {
        return (*item)._texture;
    }

    return nullptr;
}

const CTexture* CRenderTarget::getStencilTexture() const
{
    auto findPred = [](const SAttachments& attach) -> bool
    {
        if (attach._type == eStencilAttach)
        {
            return true;
        }

        return false;
    };

    std::deque<SAttachments>::const_iterator item = std::find_if(m_attachmentsList.cbegin(), m_attachmentsList.cend(), findPred);
    if (item != m_attachmentsList.cend())
    {
        return (*item)._texture;
    }

    return nullptr;
}

CTexture* CRenderTarget::getStencilTexture()
{
    auto findPred = [](const SAttachments& attach) -> bool
    {
        if (attach._type == eStencilAttach)
        {
            return true;
        }

        return false;
    };

    std::deque<SAttachments>::iterator item = std::find_if(m_attachmentsList.begin(), m_attachmentsList.end(), findPred);
    if (item != m_attachmentsList.cend())
    {
        return (*item)._texture;
    }

    return nullptr;
}

void CRenderTarget::setClearColor(const core::Vector4D& color)
{
    m_color = color;
}

const core::Vector4D& CRenderTarget::getClearColor() const
{
    return m_color;
}

void CRenderTarget::setViewport(const core::Rect32& size)
{
    m_viewport = size;
}

const core::Rect32& CRenderTarget::getViewport() const
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
    CRenderTarget::setViewport(Rect32(x, y, x + width, y + height));

    if (root->Attribute("aa"))
    {
        const std::string aaStr = root->Attribute("aa");
        if (aaStr == "msaa")
        {
            m_MSAA = true;
        }
    }

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
                    //LOG_WARNING("CRenderTarget: Depth component size must be 16, 24 or 32 - %d", size);
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

} //namespace renderer
} //namespace v3d
