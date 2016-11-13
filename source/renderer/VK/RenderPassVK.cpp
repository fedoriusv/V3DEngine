#include "RenderPassVK.h"

#ifdef _VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{
RenderPassVK::RenderPassVK()
    : m_renderPass(VK_NULL_HANDLE)

    , m_device(VK_NULL_HANDLE)

    , m_initialized(false)
{
}

RenderPassVK::~RenderPassVK()
{
    ASSERT(m_renderPass == VK_NULL_HANDLE, "m_renderPass already exist");
}

bool RenderPassVK::create()
{
    if (m_initialized)
    {
        return true;
    }

    std::vector<VkAttachmentDescription> attachments; //Form framebuffer
    attachments;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = nullptr;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = static_cast<u32>(attachments.size());
    renderPassCreateInfo.pAttachments = attachments.data();
    //renderPassCreateInfo.dependencyCount = ;
    //renderPassCreateInfo.pDependencies = ;
    //renderPassCreateInfo.subpassCount = ;
    //renderPassCreateInfo.pSubpasses = ;

    VkResult result = vkCreateRenderPass(m_device, &renderPassCreateInfo, nullptr, &m_renderPass);
    if (result != VK_SUCCESS)
    {
        //LOG
        return false;
    }

    //

    m_initialized = true;

    return true;
}

void RenderPassVK::destroy()
{
    if (m_renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
        m_renderPass = VK_NULL_HANDLE;
    }
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
