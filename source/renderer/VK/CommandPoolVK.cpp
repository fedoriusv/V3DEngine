#include "CommandPoolVK.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _VULKAN_RENDER_
#include "context/DebugVK.h"
#include "RendererVK.h"
#include "CommandBufferVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

extern VulkanDevice g_vulkanDevice;

CommandPoolVK::CommandPoolVK(const ContextVKPtr context, VkCommandBufferLevel level)
    : m_commandPool(VK_NULL_HANDLE)
    , m_level(level)

    , m_creatFlags(0)
    , m_resetFlags(0)

    , m_queueFamilyIndex(0)
{
    m_queueFamilyIndex = context->getVulkanQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);

    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.pNext = nullptr;
    commandPoolCreateInfo.flags = m_creatFlags;
    commandPoolCreateInfo.queueFamilyIndex = m_queueFamilyIndex;

    VkResult result = vkCreateCommandPool(g_vulkanDevice.device, &commandPoolCreateInfo, nullptr, &m_commandPool);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("CommandPoolVK::CommandPoolVK: vkCreateCommandPool. Error: %s", DebugVK::errorString(result).c_str());
    }
}

CommandPoolVK::~CommandPoolVK()
{
    for (auto& commandBuffer : m_commandBufferList)
    {
        delete commandBuffer;
        commandBuffer = nullptr;
    }
    m_commandBufferList.clear();

    if (m_commandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(g_vulkanDevice.device, m_commandPool, nullptr);
        m_commandPool = nullptr;
    }
}

bool CommandPoolVK::reset()
{
    VkResult result = vkResetCommandPool(g_vulkanDevice.device, m_commandPool, m_resetFlags);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("CommandPoolVK::reset: vkResetCommandPool. Error: %s", DebugVK::errorString(result).c_str());
        return false;
    }

    return true;
}

CommandBufferVK* CommandPoolVK::commandBuffer()
{
    //TODO:
    if (m_commandBufferList.empty())
    {
        m_commandBufferList.push_back(new CommandBufferVK(m_commandPool, m_level));
    }

    return m_commandBufferList.front();
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
