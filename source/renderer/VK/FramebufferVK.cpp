#include "FramebufferVK.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _VULKAN_RENDER_
#include "context/DebugVK.h"
#include "RendererVK.h"
#include "TextureVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

extern VulkanDevice g_vulkanDevice;

FramebufferVK::FramebufferVK()
    : m_framebuffer(VK_NULL_HANDLE)
    , m_initialized(false)
{
    LOG_DEBUG("FramebufferVK::FramebufferVK constructor %x", this);
}

FramebufferVK::~FramebufferVK()
{
    LOG_DEBUG("FramebufferVK::FramebufferVK destructor %x", this);

    ASSERT(m_framebuffer == VK_NULL_HANDLE, "m_framebuffer already exist");
}

void FramebufferVK::bind()
{
    //TODO:
}

void FramebufferVK::unbind()
{
    //TODO:
}

bool FramebufferVK::create()
{
    if(m_initialized)
    {
        return true;
    }

    const core::Rect32& rect = RenderTarget::getViewport();
    u32 width = rect.getWidth();
    u32 height = rect.getHeight();
    u32 x = rect.getLeftX();
    u32 y = rect.getTopY();
    if (width == 0)
    {
        width = ENGINE_CONTEXT->getWindowSize().width;
    }

    if (height == 0)
    {
        height = ENGINE_CONTEXT->getWindowSize().height;
    }

    RenderTarget::setViewport(core::Rect32(x, y, x + width, y + height));

    for (auto& attach : m_attachmentsList)
    {
        switch (attach._output)
        {
        case EAttachmentsOutput::eTextureOutput:
            FramebufferVK::createAttachment(attach, rect);
            break;

        case EAttachmentsOutput::eRenderOutput:
            ASSERT(false, "Unsupported for vulkan render");
            break;
        }
    }

    VkFramebufferCreateInfo framebufferCreateInfo = {};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.pNext = nullptr;
    framebufferCreateInfo.flags = 0;
    //framebufferCreateInfo.renderPass = ; //TODO:
    framebufferCreateInfo.attachmentCount = static_cast<u32>(m_attacmentsView.size());
    framebufferCreateInfo.pAttachments = m_attacmentsView.data();
    framebufferCreateInfo.width = width;
    framebufferCreateInfo.height = height;
    framebufferCreateInfo.layers = 1;

    VkResult result = vkCreateFramebuffer(g_vulkanDevice.device, &framebufferCreateInfo, nullptr, &m_framebuffer);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("FramebufferVK::create: vkCreateFramebuffer. Error: %s", DebugVK::errorString(result).c_str());
        return false;
    }

    m_initialized = true;

    return true;
}

void FramebufferVK::destroy()
{
    if (m_framebuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(g_vulkanDevice.device, m_framebuffer, nullptr);
        m_framebuffer = VK_NULL_HANDLE;
    }

    m_attacmentsView.clear();
}

void FramebufferVK::createAttachment(SAttachments& attach, const core::Rect32& rect)
{
    u32 samplerSize = ENGINE_CONTEXT->getSamplesCount();

    switch (attach._attachmentType)
    {
    case EAttachmentsType::eColorAttach:
    {
        if (!attach._active)
        {
            return;
        }

        u32 maxColorAttachments = ENGINE_CONTEXT->getMaxColorAttachments();
        if (attach._index >= maxColorAttachments)
        {
            LOG_ERROR("FramebufferVK::createAttachment: Range out Color attachment max %d index %d", maxColorAttachments, attach._index);
            ASSERT(false, "FramebufferVK: Range out Color attachment");
            return;
        }

        ASSERT(!attach._texture, "attach._texture already exist");
        TextureVK* texture = new TextureVK(m_MSAA ? ETextureTarget::eTexture2DMSAA : ETextureTarget::eTextureRectangle,
            attach._format, attach._type, core::Dimension3D(rect.getWidth(), rect.getHeight(), 1), nullptr, 1);
        if (!texture->create())
        {
            LOG_ERROR("FramebufferVK::createAttachment: couldn't crate image for index %d", attach._index);
            return;
        }

        m_attacmentsView.push_back(texture->getImageView());
        attach._texture = texture;

        break;
    }

    case EAttachmentsType::eDepthStencilAttach:
    {
        if (!attach._active)
        {
            return;
        }

        ASSERT(attach._format == EImageFormat::eDepthComponent, "Depth must be have only depth format");
        TextureVK* texture = new TextureVK(m_MSAA ? ETextureTarget::eTexture2DMSAA : ETextureTarget::eTextureRectangle, EImageFormat::eDepthComponent, 
            attach._type, core::Dimension3D(rect.getWidth(), rect.getHeight(), 1), nullptr, 1);
        if (!texture->create())
        {
            LOG_ERROR("FramebufferVK::createAttachment: couldn't crate image for index %d", attach._index);
            return;
        }

        m_attacmentsView.push_back(texture->getImageView());
        attach._texture = texture;

        break;
    }

    default:
    {
        LOG_ERROR("FramebufferVK::createAttachment: Not supported attach type %d", attach._attachmentType);
        ASSERT(false, "FramebufferVK: Not supported attach");
    }
    }
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
