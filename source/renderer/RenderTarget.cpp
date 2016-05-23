#include "RenderTarget.h"
#include "utils/Logger.h"
#include "Engine.h"
#include "RenderBuffer.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

using namespace core;

CRenderTarget::SAttachments::SAttachments()
    : _index(0U)
    , _active(true)
    , _attachmentType(EAttachmentsType::eEmptyAttach)
    , _output(EAttachmentsOutput::eEmptyOutput)
    , _format(EImageFormat::eRGBA)
    , _type(EImageType::eUnsignedByte)

    , _texture(nullptr)
    , _buffer(nullptr)
{
}

CRenderTarget::SAttachments::SAttachments(const SAttachments&& attach)
{
    _index = attach._index;
    _active = attach._active;

    _attachmentType = attach._attachmentType;
    _output = attach._output;

    _format = attach._format;
    _type = attach._type;

    _texture = attach._texture;
    _buffer = attach._buffer;
}

CRenderTarget::SAttachments::~SAttachments()
{
    /*if (_texture)
    {
        delete _texture;
        _texture = nullptr;
    }*/

    if (_buffer)
    {
        delete _buffer;
        _buffer = nullptr;
    }
}

CRenderTarget::SAttachments& CRenderTarget::SAttachments::operator=(const SAttachments&& attach)
{
    if (this == &attach)
    {
        return *this;
    }

    _index = attach._index;
    _active = attach._active;

    _attachmentType = attach._attachmentType;
    _output = attach._output;

    _format = attach._format;
    _type = attach._type;

    _texture = attach._texture;
    _buffer = attach._buffer;

    return *this;
}

const std::string CRenderTarget::k_defaultName = "default";

CRenderTarget::CRenderTarget()
    : m_color(core::Vector4D(0.0f))
    , m_depth(1.0f)
    , m_viewport(0U, 0U, 0U, 0U)
    , m_MSAA(false)

    , m_clearColorBuffer(true)
    , m_clearDepthBuffer(true)
    , m_clearStencilBuffer(false)
{
    ITarget::setTargetType(ITarget::ETagetType::eRenderTarget);
    ITarget::setName(k_defaultName);

    u32 width = (u32)(WINDOW->getSize().width);
    u32 height = (u32)(WINDOW->getSize().height);
    CRenderTarget::setViewport(Rect32(0, 0, width, height));
}

CRenderTarget::~CRenderTarget()
{
    m_attachmentsList.clear();
}

const TexturePtr CRenderTarget::getColorTexture(u32 index) const
{
    auto findPred = [index](const SAttachments& attach) -> bool
    {
        if (attach._attachmentType == EAttachmentsType::eColorAttach && attach._index == index)
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

    return TexturePtr();
}

TexturePtr CRenderTarget::getColorTexture(u32 index)
{
    auto findPred = [index](const SAttachments& attach) -> bool
    {
        if (attach._attachmentType == EAttachmentsType::eColorAttach && attach._index == index)
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

    return TexturePtr();
}

const TexturePtr CRenderTarget::getDepthTexture() const
{
    auto findPred = [](const SAttachments& attach) -> bool
    {
        if (attach._attachmentType == EAttachmentsType::eDepthAttach || attach._attachmentType == EAttachmentsType::eDepthStencilAttach)
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

    return TexturePtr();
}

TexturePtr CRenderTarget::getDepthTexture()
{
    auto findPred = [](const SAttachments& attach) -> bool
    {
        if (attach._attachmentType == EAttachmentsType::eDepthAttach || attach._attachmentType == EAttachmentsType::eDepthStencilAttach)
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

    return TexturePtr();
}

const TexturePtr CRenderTarget::getStencilTexture() const
{
    auto findPred = [](const SAttachments& attach) -> bool
    {
        if (attach._attachmentType == EAttachmentsType::eStencilAttach || attach._attachmentType == EAttachmentsType::eDepthStencilAttach)
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

    return TexturePtr();
}

TexturePtr CRenderTarget::getStencilTexture()
{
    auto findPred = [](const SAttachments& attach) -> bool
    {
        if (attach._attachmentType == EAttachmentsType::eStencilAttach || attach._attachmentType == EAttachmentsType::eDepthStencilAttach)
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

    return TexturePtr();
}

void CRenderTarget::setClearColor(const core::Vector4D& color)
{
    m_color = color;
}

const core::Vector4D& CRenderTarget::getClearColor() const
{
    return m_color;
}

void CRenderTarget::setClearDepth(f32 depth)
{
    m_depth = depth;
}

f32 CRenderTarget::getClearDepth() const
{
    return m_depth;
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
    CRenderTarget::setViewport(Rect32(x, y, width, height));

    if (root->Attribute("aa"))
    {
        const std::string aaStr = root->Attribute("aa");
        if (aaStr == "msaa")
        {
            m_MSAA = true;
        }
    }

    //color
    const tinyxml2::XMLElement* colorElement = root->FirstChildElement("color");
    if (colorElement)
    {
        bool clearColorBuffer = colorElement->BoolAttribute("clear");
        CRenderTarget::setClearColorBuffer(clearColorBuffer);

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

        const tinyxml2::XMLElement* attachElement = colorElement->FirstChildElement("attach");
        while (attachElement)
        {
            bool hasActiveColor = attachElement->BoolAttribute("active");
            
            u32 attachIndex = attachElement->UnsignedAttribute("val");
            EImageFormat format = EImageFormat::eRGBA;
            EImageType type = EImageType::eUnsignedByte;
            if (attachElement->Attribute("format"))
            {
                std::string formatStr = attachElement->Attribute("format");
                if (!formatParser(formatStr, format, type))
                {
                    LOG_ERROR("CRenderTarget: Color format [%s] unknown in [%s]. Skip", formatStr.c_str(), m_name.c_str());

                    attachElement = attachElement->NextSiblingElement("attach");
                    continue;
                }
            }
            else
            {
                LOG_ERROR("CRenderTarget: Color format don't exist in %s. Skip", m_name.c_str());

                attachElement = attachElement->NextSiblingElement("attach");
                continue;
            }

            EAttachmentsOutput output = EAttachmentsOutput::eEmptyOutput;
            if (attachElement->Attribute("output"))
            {
                std::string outputStr = attachElement->Attribute("output");

                std::function<EAttachmentsOutput(std::string&)> getAttachmentOutput = [](std::string& str)->EAttachmentsOutput
                {
                    if (str == "texture")
                    {
                        return EAttachmentsOutput::eTextureOutput;
                    }
                    else if (str == "render")
                    {
                        return EAttachmentsOutput::eRenderOutput;
                    }

                    LOG_ERROR("CRenderTarget: output format %s unknown", str.c_str());
                    return EAttachmentsOutput::eEmptyOutput;
                };

                output = getAttachmentOutput(outputStr);
            }

            CRenderTarget::attachTarget(EAttachmentsType::eColorAttach, attachIndex, format, type, output, hasActiveColor);

            attachElement = attachElement->NextSiblingElement("attach");
        }
    }

    //depth
    const tinyxml2::XMLElement* depthElement = root->FirstChildElement("depth");
    if (depthElement)
    {
        bool hasActiveDepth = depthElement->BoolAttribute("active");

        bool clearDepthBuffer = depthElement->BoolAttribute("clear");
        CRenderTarget::setClearDepthBuffer(clearDepthBuffer);

        f32 clearValue = depthElement->FloatAttribute("value");
        CRenderTarget::setClearDepth(clearValue);

        EImageFormat format = EImageFormat::eDepthComponent;
        EImageType type = EImageType::eFloat;
        if (depthElement->Attribute("format"))
        {
            std::string formatStr = depthElement->Attribute("format");
            if (!formatParser(formatStr, format, type))
            {
                LOG_WARNING("CRenderTarget: Depth format [%s] unknown in [%s]", formatStr.c_str(), m_name.c_str());
            }
        }
        else
        {
            LOG_WARNING("CRenderTarget: Depth format don't exist in %s", m_name.c_str());
        }

        EAttachmentsOutput output = EAttachmentsOutput::eEmptyOutput;
        if (depthElement->Attribute("output"))
        {
            std::string outputStr = depthElement->Attribute("output");

            std::function<EAttachmentsOutput(std::string&)> getAttachmentOutput = [](std::string& str) -> EAttachmentsOutput
            {
                if (str == "texture")
                {
                    return EAttachmentsOutput::eTextureOutput;
                }
                else if (str == "render")
                {
                    return EAttachmentsOutput::eRenderOutput;
                }

                LOG_ERROR("CRenderTarget: output format %s unknown", str.c_str());
                return EAttachmentsOutput::eEmptyOutput;
            };

            output = getAttachmentOutput(outputStr);
        }

        CRenderTarget::attachTarget(EAttachmentsType::eDepthAttach, 0, format, type, output, hasActiveDepth);
    }

    //stencil
    const tinyxml2::XMLElement* stencilElement = root->FirstChildElement("stencil");
    if (stencilElement)
    {
        bool hasActiveStencil = stencilElement->BoolAttribute("active");

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

        EAttachmentsOutput output = EAttachmentsOutput::eEmptyOutput;
        if (colorElement->Attribute("output"))
        {
            std::string outputStr = colorElement->Attribute("output");

            std::function<EAttachmentsOutput(std::string&)> getAttachmentOutput = [](std::string& str) -> EAttachmentsOutput
            {
                if (str == "texture")
                {
                    return EAttachmentsOutput::eTextureOutput;
                }
                else if (str == "render")
                {
                    return EAttachmentsOutput::eRenderOutput;
                }
                LOG_ERROR("CRenderTarget: output format %s unknown", str.c_str());
                return EAttachmentsOutput::eEmptyOutput;
            };

            output = getAttachmentOutput(outputStr);
        }

        //CRenderTarget::attachTarget(eStencilAttach, 0, format, type, output, hasActiveStencil);
    }

    //TODO: eDepthStencilAttach

    return true;
}

bool CRenderTarget::formatParser(const std::string& str, EImageFormat& format, EImageType& type)
{
    std::string::const_iterator current = str.cbegin();
    bool success = true;

    std::string component;
    while (current != str.cend())
    {
        if (atoi(&(*current)) <= 0)
        {
            component.push_back(*current);
            ++current;
        }
        else
        {
            break;
        }
    };

    auto componentToFormat = [&success](const std::string& component) ->EImageFormat
    {
        if (component == "r")
        {
            return EImageFormat::eRed;
        }
        else if (component == "rg")
        {
            return EImageFormat::eRG;
        }
        else if (component == "rgb")
        {
            return EImageFormat::eRGB;
        }
        if (component == "rgba")
        {
            return EImageFormat::eRGBA;
        }
        if (component == "depth")
        {
            return EImageFormat::eDepthComponent;
        }

        success = false;
        return EImageFormat::eRGB;
    };

    format = componentToFormat(component);

    std::string sizeStr;
    while (current != str.cend())
    {
        if (atoi(&(*current)) > 0)
        {
            sizeStr.push_back(*current);
            ++current;
        }
        else
        {
            break;
        }
    };
    u32 size = std::stoi(sizeStr);

    std::string modificator;
    while (current != str.cend())
    {
        if (atoi(&(*current)) <= 0)
        {
            modificator.push_back(*current);
            ++current;
        }
        else
        {
            break;
        }
    };

    auto componentType = [&success, &format](u32 size, const std::string& modificator) ->EImageType
    {
        switch (size)
        {
        case 8:
        {
            if (modificator == "u")
            {
                return EImageType::eUnsignedByte;
            }
            else if (modificator == "s")
            {
                return EImageType::eByte;
            }
        }
        break;

        case 16:
        {
            if (modificator == "u")
            {
                return EImageType::eUnsignedShort;
            }
            else if (modificator == "s")
            {
                return EImageType::eShort;
            }
        }
            break;

        case 24:
        case 32:
        {
            if (modificator == "u")
            {
                if (format == EImageFormat::eDepthComponent)
                {
                    return EImageType::eUnsignedInt24_8;
                }

                return EImageType::eUnsignedInt;
            }
            else if (modificator == "s")
            {
                return EImageType::eInt;
            }
            else if (modificator == "f")
            {
                return EImageType::eFloat;
            }
        }
            break;

        default:
            break;
        };
        
        success = false;
        return EImageType::eUnsignedByte;
    };

    type = componentType(size, modificator);

    return success;
}

void CRenderTarget::attachTarget(EAttachmentsType attach, u32 index, EImageFormat format, EImageType type, EAttachmentsOutput output, bool active)
{
    std::deque<SAttachments>::const_iterator iter = std::find_if(m_attachmentsList.cbegin(), m_attachmentsList.cend(), [index, attach](const SAttachments& attachment) -> bool
    {
        if (attachment._index == index && attachment._attachmentType == attach)
        {
            return true;
        }

        return false;
    });

    if (iter != m_attachmentsList.cend())
    {
        LOG_WARNING("Attach [%d] alredy present in RenderTarget", index);
        return;
    }

    SAttachments target;

    target._attachmentType = attach;
    target._index = index;
    target._active = active;
    target._format = format;
    target._type = type;
    target._output = output;
    target._texture = nullptr;
    target._buffer = nullptr;

    if (attach == EAttachmentsType::eColorAttach)
    {
        m_attachmentsList.push_front(std::move(target));
    }
    else
    {
        m_attachmentsList.push_back(std::move(target));
    }
}

} //namespace renderer
} //namespace v3d
