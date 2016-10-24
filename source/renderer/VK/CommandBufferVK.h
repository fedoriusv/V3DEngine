#pragma once

#include "common.h"

#ifdef _VULKAN_RENDER_
#   include "vulkan/vulkan.h"
#endif //_VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CommandBufferVK;

    class CommandPoolVK
    {
    public:

        CommandPoolVK();
        ~CommandPoolVK();

        bool reset();

    private:

        std::list<CommandBufferVK*> m_commandBufferList;

        VkCommandPool               m_commandPool;
        VkCommandPoolCreateFlags    m_creatFlags;
        VkCommandPoolResetFlags     m_resetFlags;

        VkDevice                    m_device;
        u32                         m_queueFamilyIndex;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Command buffer. VK render only.
    */
    class CommandBufferVK
    {
    public:

        CommandBufferVK(VkCommandPool pool, VkCommandBufferLevel level);
        ~CommandBufferVK();

        void                    imageMemoryBarrier(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange);
        void                    copyBufferToImage(VkBuffer buffer, VkImage image, VkImageLayout layout, u32 layersCount, u32 mipLevels /*, const VkExtent3D& extent, */);
        void                    copyImageToBuffer();
        void                    copyBufferToBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, u64 size);

    private:

        VkCommandBufferLevel    m_bufferLevel;
        VkCommandBuffer         m_commandBuffer;

        VkDevice                m_device;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d
