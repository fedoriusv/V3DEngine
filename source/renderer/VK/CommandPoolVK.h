#pragma once

#include "common.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"
#include "context/DeviceContextVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Command pool management. VK render only.
    */
    class CommandBufferVK;

    class CommandPoolVK
    {
    public:

        CommandPoolVK(const ContextVKPtr context, VkCommandBufferLevel level);
        ~CommandPoolVK();

        bool                        reset();

        CommandBufferVK*            commandBuffer();

    private:

        std::list<CommandBufferVK*> m_commandBufferList;

        VkCommandPool               m_commandPool;
        VkCommandBufferLevel        m_level;

        VkCommandPoolCreateFlags    m_creatFlags;
        VkCommandPoolResetFlags     m_resetFlags;

        u32                         m_queueFamilyIndex;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
