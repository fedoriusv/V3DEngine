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

    /**
    * Command buffer. VK render only.
    */
    class CommandBufferVK
    {
    public:

        CommandBufferVK();
        ~CommandBufferVK();

        void imageMemoryBarrier(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange);
        void copyBufferToImage(VkBuffer buffer, VkImage image, VkImageLayout layout, u32 layersCount, u32 mipLevels /*, const VkExtent3D& extent, */);
        void copyImageToBuffer();

    private:

        VkCommandBufferLevel    m_bufferLevel;
        VkCommandBuffer         m_commandBuffer;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d
