#pragma once

#include "common.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"

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

        CommandBufferVK(VkCommandPool pool, VkCommandBufferLevel level);
        ~CommandBufferVK();

        void                    clearColorImage(VkImage image, VkImageLayout imageLayout, const core::Vector4D& color, const VkImageSubresourceRange& subresourceRange);
        void                    clearDepthStencilImage(VkImage image, VkImageLayout imageLayout, f32 depth, u32 stencil, const VkImageSubresourceRange& subresourceRange);

        void                    beginRenderPass();
        void                    endRenderPass();

        void                    imageMemoryBarrier(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, const VkImageSubresourceRange& subresourceRange);
        
        void                    copyBufferToImage(VkBuffer buffer, VkImage image, VkImageLayout layout, const VkImageSubresourceRange& subresourceRange);
        void                    copyImageToImage();
        void                    copyImageToBuffer();
        void                    copyBufferToBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, u64 size);
        void                    blitImage();
        void                    resolveImage();

    private:

        VkCommandBufferLevel    m_bufferLevel;
        VkCommandBuffer         m_commandBuffer;
        VkCommandPool           m_commandPool;

        VkDevice                m_device;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_