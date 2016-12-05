#include "FenceVK.h"
#include "utils/Logger.h"
#include "context/DebugVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

FenceVK::FenceVK(VkDevice device)
    : m_device(device)
    , m_fence(VK_NULL_HANDLE)
{
}

FenceVK::~FenceVK()
{
    ASSERT(!m_fence, "fence already exist");
}

bool FenceVK::create(bool signaled)
{
    if (!m_fence)
    {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

        VkResult result = vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_fence);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("FenceVK::create: vkCreateFence error %s", vk::DebugVK::errorString(result).c_str());
            return false;
        }
    }

    return true;
}

void FenceVK::destroy()
{
    if (m_fence)
    {
        vkDestroyFence(m_device, m_fence, nullptr);
        m_fence = VK_NULL_HANDLE;
    }
}

bool FenceVK::reset()
{
    VkResult result = vkResetFences(m_device, 1, &m_fence);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("FenceVK::reset: vkWaitForFences error %s", vk::DebugVK::errorString(result).c_str());
        return false;
    }

    return true;
}

bool FenceVK::wait()
{
    VkResult result = vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, UINT64_MAX);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("FenceVK::wait: vkWaitForFences error %s", vk::DebugVK::errorString(result).c_str());
        return false;
    }

    return true;
}

bool FenceVK::status()
{
    VkResult result = vkGetFenceStatus(m_device, m_fence);
    if (result == VK_SUCCESS)
    {
        return true;
    }
    else if (result == VK_NOT_READY)
    {
        return false;
    }
    else
    {
        LOG_ERROR("FenceVK::status: vkGetFenceStatus error %s", vk::DebugVK::errorString(result).c_str());
        return false;
    }

    return false;
}

VkFence FenceVK::fence() const
{
    return m_fence;
}

} //namespace vk
} //namespace renderer
} //namespace v3d
