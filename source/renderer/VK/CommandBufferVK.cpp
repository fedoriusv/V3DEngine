#include "CommandBufferVK.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _VULKAN_RENDER_
#include "context/DebugVK.h"
#include "RendererVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

CommandBufferVK::CommandBufferVK(VkCommandPool pool, VkCommandBufferLevel level)
    : m_commandBuffer(VK_NULL_HANDLE)
    , m_bufferLevel(level)
    , m_commandPool(pool)

    , m_device(VK_NULL_HANDLE)
{
    m_device = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanDevice();

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = nullptr;
    commandBufferAllocateInfo.commandPool = m_commandPool;
    commandBufferAllocateInfo.level = m_bufferLevel;
    commandBufferAllocateInfo.commandBufferCount = 1;

    VkResult result = vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, &m_commandBuffer);
    if (result != VK_SUCCESS)
    {
        

    }
}

CommandBufferVK::~CommandBufferVK()
{
    if (m_commandBuffer != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_commandBuffer);
        m_commandBuffer = VK_NULL_HANDLE;
    }
}

void CommandBufferVK::clearColorImage(VkImage image, VkImageLayout imageLayout, const core::Vector4D& color, const VkImageSubresourceRange& subresourceRange)
{
    VkClearColorValue clearColorValue = {};
    clearColorValue.float32[0] = color[0];
    clearColorValue.float32[1] = color[1];
    clearColorValue.float32[2] = color[2];
    clearColorValue.float32[3] = color[3];

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdClearColorImage(m_commandBuffer, image, imageLayout, &clearColorValue, 1, &subresourceRange);
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::clearDepthStencilImage(VkImage image, VkImageLayout imageLayout, f32 depth, u32 stencil, const VkImageSubresourceRange& subresourceRange)
{
    VkClearDepthStencilValue clrearDepthStencilValue = {};
    clrearDepthStencilValue.depth = depth;
    clrearDepthStencilValue.stencil = stencil;

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdClearDepthStencilImage(m_commandBuffer, image, imageLayout, &clrearDepthStencilValue, 1, &subresourceRange);
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::beginRenderPass()
{
}

void CommandBufferVK::endRenderPass()
{
}

void CommandBufferVK::imageMemoryBarrier(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, const VkImageSubresourceRange& subresourceRange)
{
    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = nullptr;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.oldLayout = oldImageLayout;
    imageMemoryBarrier.newLayout = newImageLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // Source layouts (old)
    // Source access mask controls actions that have to be finished on the old layout
    // before it will be transitioned to the new layout
    switch (oldImageLayout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        // Image layout is undefined (or does not matter)
        // Only valid as initial layout
        // No flags required, listed only for completeness
        imageMemoryBarrier.srcAccessMask = 0;
        break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        // Image is preinitialized
        // Only valid as initial layout for linear images, preserves memory contents
        // Make sure host writes have been finished
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        // Image is a color attachment
        // Make sure any writes to the color buffer have been finished
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        // Image is a depth/stencil attachment
        // Make sure any writes to the depth/stencil buffer have been finished
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        // Image is a transfer source 
        // Make sure any reads from the image have been finished
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        // Image is a transfer destination
        // Make sure any writes to the image have been finished
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        // Image is read by a shader
        // Make sure any shader reads from the image have been finished
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        break;
    }

    // Target layouts (new)
    // Destination access mask controls the dependency for the new image layout
    switch (newImageLayout)
    {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        // Image will be used as a transfer destination
        // Make sure any writes to the image have been finished
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        // Image will be used as a transfer source
        // Make sure any reads from and writes to the image have been finished
        imageMemoryBarrier.srcAccessMask = imageMemoryBarrier.srcAccessMask | VK_ACCESS_TRANSFER_READ_BIT;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        // Image will be used as a color attachment
        // Make sure any writes to the color buffer have been finished
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        // Image layout will be used as a depth/stencil attachment
        // Make sure any writes to depth/stencil buffer have been finished
        imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        // Image will be read in a shader (sampler, input attachment)
        // Make sure any writes to the image have been finished
        if (imageMemoryBarrier.srcAccessMask == 0)
        {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        }
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        break;
    }

    VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdPipelineBarrier(m_commandBuffer, srcStageFlags, destStageFlags, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::copyBufferToImage(VkBuffer buffer, VkImage image, VkImageLayout layout, const VkImageSubresourceRange& subresourceRange)
{
    std::vector<VkBufferImageCopy> bufferCopyRegions;
    //u32 buffOffset = 0;

    //for (u32 layer = 0; layer < layersCount; ++layer)
    //{
    //    for (u32 mip = 0; mip < mipLevel; ++mip)
    //    {
    //        VkBufferImageCopy bufferCopyRegion = {};
    //       /* bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    //        bufferCopyRegion.imageSubresource.mipLevel = mip;
    //        bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    //        bufferCopyRegion.imageSubresource.layerCount = layersCount;*/
    //        //bufferCopyRegion.imageExtent = extent;
    //        //bufferCopyRegion.imageOffset = offset;
    //        //bufferCopyRegion.bufferRowLength = ;
    //        //bufferCopyRegion.bufferImageHeight = ;
    //        //bufferCopyRegion.bufferOffset = buffOffset;

    //        bufferCopyRegions.push_back(bufferCopyRegion);

    //       // offset += static_cast<uint32_t>(tex2D[i].size());
    //    }
    //}

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdCopyBufferToImage(m_commandBuffer, buffer, image, layout, static_cast<u32>(bufferCopyRegions.size()), bufferCopyRegions.data());
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::copyImageToImage(VkImage srcImage, VkImage dstImage, VkImageLayout srcLayout, VkImageLayout dstLayout, const VkImageSubresourceRange& subresourceRange)
{
    std::vector<VkImageCopy> imageCopyRegions;
    //TODO:

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdCopyImage(m_commandBuffer, srcImage, srcLayout, dstImage, dstLayout, static_cast<u32>(imageCopyRegions.size()), imageCopyRegions.data());
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::copyImageToBuffer(VkImage image, VkBuffer buffer, VkImageLayout layout, const VkImageSubresourceRange& subresourceRange)
{
    std::vector<VkBufferImageCopy> bufferCopyRegions;
    //TODO:

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdCopyImageToBuffer(m_commandBuffer, image, layout, buffer, static_cast<u32>(bufferCopyRegions.size()), bufferCopyRegions.data());
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::copyBufferToBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, u64 size)
{
    VkBufferCopy bufferCopyRegion = {};
    bufferCopyRegion.srcOffset = 0;
    bufferCopyRegion.dstOffset = 0;
    bufferCopyRegion.size = static_cast<VkDeviceSize>(size);

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdCopyBuffer(m_commandBuffer, srcBuffer, dstBuffer, 1, &bufferCopyRegion);
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::blitImage()
{
}

void CommandBufferVK::resolveImage()
{
}

void CommandBufferVK::setViewport(const core::Rect32& viewportPos, f32 minDepth, f32 maxDepth)
{
    VkViewport viewport = {};
    viewport.x = viewportPos.getLeftX();
    viewport.y = viewportPos.getTopY();
    viewport.width = viewportPos.getWidth();
    viewport.height = viewportPos.getHeight();
    viewport.minDepth = core::clamp(minDepth, 0.0f, 1.0f);
    viewport.maxDepth = core::clamp(maxDepth, 0.0f, 1.0f);

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
    
}

void CommandBufferVK::setBlendConstant(const core::Vector4D& color)
{
    f32 blendConstants[4];
    blendConstants[0] = color.x;
    blendConstants[1] = color.y;
    blendConstants[2] = color.z;
    blendConstants[3] = color.w;

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdSetBlendConstants(m_commandBuffer, blendConstants);
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::setDepthBounds(f32 minDepth, f32 maxDepth)
{
    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdSetDepthBounds(m_commandBuffer, minDepth, maxDepth);
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
