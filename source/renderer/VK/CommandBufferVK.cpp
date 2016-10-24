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

    , m_device(VK_NULL_HANDLE)
{
    m_device = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanDevice();

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = nullptr;
    commandBufferAllocateInfo.commandPool = pool;
    commandBufferAllocateInfo.level = level;
    commandBufferAllocateInfo.commandBufferCount = 1;

    VkResult result = vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, &m_commandBuffer);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("CommandBufferVK::CommandBufferVK: vkAllocateCommandBuffers. Error: %s", DebugVK::errorString(result).c_str());
    }
}

CommandBufferVK::~CommandBufferVK()
{
}

void CommandBufferVK::imageMemoryBarrier(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange)
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

void CommandBufferVK::copyBufferToImage(VkBuffer buffer, VkImage image, VkImageLayout layout, u32 layersCount, u32 mipLevel /*,, const VkExtent3D& extent, */)
{
    std::vector<VkBufferImageCopy> bufferCopyRegions;
    u32 buffOffset = 0;

    for (u32 layer = 0; layer < layersCount; ++layer)
    {
        for (u32 mip = 0; mip < mipLevel; ++mip)
        {
            VkBufferImageCopy bufferCopyRegion = {};
           /* bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = mip;
            bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
            bufferCopyRegion.imageSubresource.layerCount = layersCount;*/
            //bufferCopyRegion.imageExtent = extent;
            //bufferCopyRegion.imageOffset = offset;
            //bufferCopyRegion.bufferRowLength = ;
            //bufferCopyRegion.bufferImageHeight = ;
            //bufferCopyRegion.bufferOffset = buffOffset;

            bufferCopyRegions.push_back(bufferCopyRegion);

           // offset += static_cast<uint32_t>(tex2D[i].size());
        }
    }

    if (m_bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        vkCmdCopyBufferToImage(m_commandBuffer, buffer, image, layout, bufferCopyRegions.size(), bufferCopyRegions.data());
    }
    else //VK_COMMAND_BUFFER_LEVEL_SECONDARY
    {
        //TODO: async command
        ASSERT(false, "not implemented");
    }
}

void CommandBufferVK::copyImageToBuffer()
{
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



CommandPoolVK::CommandPoolVK()
    : m_commandPool(VK_NULL_HANDLE)
    , m_creatFlags(0)
    , m_resetFlags(0)

    , m_device(VK_NULL_HANDLE)
    , m_queueFamilyIndex(0)
{
    m_device = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanDevice();
    m_queueFamilyIndex = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);

    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.pNext = nullptr;
    commandPoolCreateInfo.flags = m_creatFlags;
    commandPoolCreateInfo.queueFamilyIndex = m_queueFamilyIndex;

    VkResult result = vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("CommandPoolVK::CommandPoolVK: vkCreateCommandPool. Error: %s", DebugVK::errorString(result).c_str());
    }
}

CommandPoolVK::~CommandPoolVK()
{
    if (m_commandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);
        m_commandPool = nullptr;
    }
}

bool CommandPoolVK::reset()
{
    VkResult result = vkResetCommandPool(m_device, m_commandPool, m_resetFlags);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("CommandPoolVK::reset: vkResetCommandPool. Error: %s", DebugVK::errorString(result).c_str());
        return false;
    }

    return true;
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
