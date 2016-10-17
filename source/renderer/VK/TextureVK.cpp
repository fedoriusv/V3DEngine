#include "TextureVK.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_
#   include "context/DebugVK.h"
#   include "RendererVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

using namespace core;

VkImageType getImageTypeVK(ETextureTarget target)
{
    switch (target)
    {
        case ETextureTarget::eTexture1D:
        case ETextureTarget::eTextureBuffer:
            return VK_IMAGE_TYPE_1D;

        case ETextureTarget::eTextureRectangle:
        case ETextureTarget::eTexture1DArray:
        case ETextureTarget::eTexture2D:
        case ETextureTarget::eTexture2DMSAA:
            return VK_IMAGE_TYPE_2D;

        case ETextureTarget::eTexture2DArray:
        case ETextureTarget::eTexture3D:
        case ETextureTarget::eTexture3DMSAA:
            return VK_IMAGE_TYPE_3D;

        default:
            ASSERT(false, "invalid type");
    };

    return VK_IMAGE_TYPE_MAX_ENUM;
}

VkSampleCountFlagBits getSampleCountVK(u32 size)
{
    switch (size)
    {
    case 1:
        return VK_SAMPLE_COUNT_1_BIT;
    case 2:
        return VK_SAMPLE_COUNT_2_BIT;
    case 4:
        return VK_SAMPLE_COUNT_4_BIT;
    case 8:
        return VK_SAMPLE_COUNT_8_BIT;
    case 16:
        return VK_SAMPLE_COUNT_16_BIT;
    case 32:
        return VK_SAMPLE_COUNT_32_BIT;
    case 64:
        return VK_SAMPLE_COUNT_64_BIT;
    default:
        ASSERT(false, "Invalid size");
    }

    return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
}

u32 getArrayLayersCountVK(ETextureTarget target, const Dimension3D& size)
{
    u32 arrayLayer = 1;

    switch (target)
    {
    case ETextureTarget::eTexture1D:
    case ETextureTarget::eTextureBuffer:
        arrayLayer = 1;
        break;

    case ETextureTarget::eTexture1DArray:
        arrayLayer = size.height;
        break;

    case ETextureTarget::eTexture2D:
    case ETextureTarget::eTexture2DMSAA:
    case ETextureTarget::eTextureRectangle:
        arrayLayer = 1;
        break;

    case ETextureTarget::eTexture2DArray:
        arrayLayer = size.depth;
        break;

    case ETextureTarget::eTexture3D:
    case ETextureTarget::eTexture3DMSAA:
        arrayLayer = 1;
        break;
    }

    return arrayLayer;
}

VkExtent3D getImageExtentVK(ETextureTarget target, const Dimension3D& size)
{
    VkExtent3D imageSize = { size.width, size.height, size.depth };

    switch (target)
    {
    case ETextureTarget::eTexture1D:
    case ETextureTarget::eTextureBuffer:
        imageSize = { size.width, 1, 1 };
        break;

    case ETextureTarget::eTexture1DArray:
        imageSize = { size.width, 1, 1 };
        break;

    case ETextureTarget::eTexture2D:
    case ETextureTarget::eTexture2DMSAA:
    case ETextureTarget::eTextureRectangle:
        imageSize = { size.width, size.height, 1 };
        break;

    case ETextureTarget::eTexture2DArray:
        imageSize = { size.width, size.height, 1 };
        break;

    case ETextureTarget::eTexture3D:
    case ETextureTarget::eTexture3DMSAA:
        imageSize = { size.width, size.height, size.depth };
        break;
    }

    return imageSize;
}

VkFormat getImageFormatVK(EImageFormat format, EImageType type)
{
    switch (type)
    {
    case EImageType::eByte:
        switch (format)
        {
        case EImageFormat::eRed:
            return VK_FORMAT_R8_SNORM;
        case EImageFormat::eRG:
            return VK_FORMAT_R8G8_SNORM;
        case EImageFormat::eRGB:
            return VK_FORMAT_R8G8B8_SNORM;
        case EImageFormat::eRGBA:
            return VK_FORMAT_R8G8B8A8_SNORM;

        case EImageFormat::eDepthComponent:
            return VK_FORMAT_UNDEFINED;
        case EImageFormat::eStencilIndex:
            return VK_FORMAT_UNDEFINED;
        }

    /*case EImageType::eUnsignedByte:
        switch (format)
        {
        case EImageFormat::eRed:
            return VK_FORMAT_R8_UNORM;
        case EImageFormat::eRG:
            return;
        case EImageFormat::eRGB:
            return;
        case EImageFormat::eRGBA:
            return;
        case EImageFormat::eDepthComponent:
            return;
        case EImageFormat::eStencilIndex:
            return;
        }

    case EImageType::eShort:
        switch (format)
        {
        case EImageFormat::eRed:
            return VK_FORMAT_R16_SNORM;
        case EImageFormat::eRG:
            return;
        case EImageFormat::eRGB:
            return;
        case EImageFormat::eRGBA:
            return;
        case EImageFormat::eDepthComponent:
            return;
        case EImageFormat::eStencilIndex:
            return;
        }

    case EImageType::eUnsignedShort:
        switch (format)
        {
        case EImageFormat::eRed:
            return VK_FORMAT_R16_UNORM;
        case EImageFormat::eRG:
            return;
        case EImageFormat::eRGB:
            return;
        case EImageFormat::eRGBA:
            return;
        case EImageFormat::eDepthComponent:
            return;
        case EImageFormat::eStencilIndex:
            return;
        }

    case EImageType::eInt:
        switch (format)
        {
        case EImageFormat::eRed:
            return;
        case EImageFormat::eRG:
            return;
        case EImageFormat::eRGB:
            return;
        case EImageFormat::eRGBA:
            return;
        case EImageFormat::eDepthComponent:
            return;
        case EImageFormat::eStencilIndex:
            return;
        }

    case EImageType::eUnsignedInt:
        switch (format)
        {
        case EImageFormat::eRed:
            return;
        case EImageFormat::eRG:
            return;
        case EImageFormat::eRGB:
            return;
        case EImageFormat::eRGBA:
            return;
        case EImageFormat::eDepthComponent:
            return;
        case EImageFormat::eStencilIndex:
            return;
        }

    case EImageType::eUnsignedInt24_8:
        switch (format)
        {
        case EImageFormat::eRed:
            return;
        case EImageFormat::eRG:
            return;
        case EImageFormat::eRGB:
            return;
        case EImageFormat::eRGBA:
            return;
        case EImageFormat::eDepthComponent:
            return;
        case EImageFormat::eStencilIndex:
            return;
        }

    case EImageType::eHalfFloat:
        switch (format)
        {
        case EImageFormat::eRed:
            return;
        case EImageFormat::eRG:
            return;
        case EImageFormat::eRGB:
            return;
        case EImageFormat::eRGBA:
            return;
        case EImageFormat::eDepthComponent:
            return;
        case EImageFormat::eStencilIndex:
            return;
        }

    case EImageType::eFloat:
        switch (format)
        {
        case EImageFormat::eRed:
            return;
        case EImageFormat::eRG:
            return;
        case EImageFormat::eRGB:
            return;
        case EImageFormat::eRGBA:
            return;
        case EImageFormat::eDepthComponent:
            return;
        case EImageFormat::eStencilIndex:
            return;
        }*/
    }

    return VK_FORMAT_UNDEFINED;
}

TextureVK::TextureVK(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level)
    : m_target(target)
    , m_format(format)
    , m_type(type)
    , m_size(size)

    , m_enable(false)

    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)

    , m_minFilter(level > 0 ? eLinearMipmapLinear : eLinear)
    , m_magFilter(eLinear)
    , m_mipmapLevel(level)

    , m_device(VK_NULL_HANDLE)
    , m_queueFamilyIndex(0)
    , m_image(VK_NULL_HANDLE)
    , m_imageView(VK_NULL_HANDLE)
    , m_imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
    , m_usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
    , m_deviceMemory(VK_NULL_HANDLE)
    , m_mappable(false)

    , m_initialized(false)
{
    LOG_DEBUG("TextureVK::TextureVK constructor %x", this);
    TextureVK::create();
    if (data)
    {
        switch (m_target)
        {
        case ETextureTarget::eTexture1D:
        case ETextureTarget::eTextureBuffer:
            TextureVK::update(0, size.width, data, level);
            break;

        case ETextureTarget::eTexture1DArray:
        case ETextureTarget::eTexture2D:
        case ETextureTarget::eTexture2DMSAA:
        case ETextureTarget::eTextureRectangle:
            TextureVK::update(Dimension2D(0U, 0U), Dimension2D(size.width, size.height), data, level);
            break;

        case ETextureTarget::eTexture2DArray:
        case ETextureTarget::eTexture3D:
        case ETextureTarget::eTexture3DMSAA:
            TextureVK::update(Dimension3D(0U, 0U, 0U), size, data, level);
            break;

        default:
            ASSERT(false, "Invalid target");
            break;
        }
    }
}

TextureVK::TextureVK(EImageFormat format, EImageType type, const core::Dimension2D & size, const void * data[6], u32 level)
    : m_target(eTextureCubeMap)
    , m_format(format)
    , m_type(type)
    , m_size(Dimension3D(size.width, size.height, 1))

    , m_enable(false)

    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)

    , m_minFilter(level > 0 ? eLinearMipmapLinear : eLinear)
    , m_magFilter(eLinear)
    , m_mipmapLevel(level)

    , m_device(VK_NULL_HANDLE)
    , m_queueFamilyIndex(0)
    , m_image(VK_NULL_HANDLE)
    , m_imageView(VK_NULL_HANDLE)
    , m_imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
    , m_usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
    , m_deviceMemory(VK_NULL_HANDLE)
    , m_mappable(false)

    , m_initialized(false)
{
    LOG_DEBUG("TextureVK::TextureVK constructor %x", this);
    TextureVK::create();
    if (data)
    {
        for (u32 cubemapSide = 0; cubemapSide < k_textureCubemapSideCount; ++cubemapSide)
        {
            TextureVK::update(cubemapSide, Dimension2D(0U, 0U), size, data[cubemapSide], level);
        }
    }
}

TextureVK::~TextureVK()
{
    TextureVK::destroy();

    LOG_DEBUG("TextureVK::TextureVK destructor %x", this);
}

void TextureVK::bind(u32 unit)
{
}
void TextureVK::unbind()
{
}
bool TextureVK::isValid() const
{
    return false;
}
bool TextureVK::isEnable() const
{
    return false;
}
void TextureVK::update(u32 offset, u32 size, const void* data, u32 levels)
{
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.pQueueFamilyIndices = &m_queueFamilyIndex;
    bufferCreateInfo.queueFamilyIndexCount = 1;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkResult result = vkCreateBuffer(m_device, &bufferCreateInfo, nullptr, &stagingBuffer);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("TextureVK::update: vkCreateBuffer. Error %s", DebugVK::errorString(result).c_str());
        return;
    }

    MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
    SMemoryVK stagingMemory = memoryManager->allocateImage(*memoryManager->getSimpleAllocator(), m_image, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT /*| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT*/);
    if (stagingMemory._mapped == VK_NULL_HANDLE)
    {
        LOG_ERROR("TextureVK::create: allocateImage return invalid memory");
        return;
    }

    ASSERT(stagingMemory._mapped, "map is nullptr");
    memcpy(stagingMemory._mapped, data, size);

    //std::vector<VkBufferImageCopy> bufferCopyRegions;
    //uint32_t offset = 0;

    //for (uint32_t i = 0; i < levels; i++)
    //{
    //    VkBufferImageCopy bufferCopyRegion = {};
    //    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    //    bufferCopyRegion.imageSubresource.mipLevel = i;
    //    bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    //    bufferCopyRegion.imageSubresource.layerCount = 1;
    //    bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(tex2D[i].dimensions().x);
    //    bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(tex2D[i].dimensions().y);
    //    bufferCopyRegion.imageExtent.depth = 1;
    //    bufferCopyRegion.bufferOffset = offset;

    //    bufferCopyRegions.push_back(bufferCopyRegion);

    //    offset += static_cast<uint32_t>(tex2D[i].size());
    //}

    //vkCmdCopyBufferToImage();

    //memoryManager->free(*memoryManager->getSimpleAllocator(), stagingMemory);
    //vkDestroyBuffer(m_device, stagingBuffer, nullptr);
}

void TextureVK::update(const core::Dimension2D & offset, const core::Dimension2D & size, const void * data, u32 level)
{
}
void TextureVK::update(const core::Dimension3D & offset, const core::Dimension3D & size, const void * data, u32 level)
{
}
void TextureVK::update(u32 cubemapSide, const core::Dimension2D & offset, const core::Dimension2D & size, const void * data, u32 level)
{
}
void TextureVK::read(void * data, u32 level) const
{
}
void TextureVK::read(u32 cubemapSide, void * data, u32 level) const
{
}
void TextureVK::fill(const void * data, u32 offset, u32 size, u32 level)
{
}
void TextureVK::fill(const void * data, const core::Dimension2D & offset, const core::Dimension2D & size, u32 level)
{
}
void TextureVK::fill(const void * data, const core::Dimension3D & offset, const core::Dimension3D & size, u32 level)
{
}

ETextureTarget TextureVK::getTarget() const
{
    return m_target;
}

ETextureFilter TextureVK::getMinFiler() const
{
    return m_minFilter;
}

ETextureFilter TextureVK::getMagFiler() const
{
    return m_magFilter;
}

EWrapType TextureVK::getWrap() const
{
    return m_wrap;
}

EAnisotropic TextureVK::getAnisotropic() const
{
    return m_anisotropicLevel;
}

u32 TextureVK::getMipmapLevel() const
{
    return m_mipmapLevel;
}

const Dimension3D& TextureVK::getSize() const
{
    return m_size;
}

EImageFormat TextureVK::getFormat() const
{
    return m_format;
}

EImageType TextureVK::getType() const
{
    return m_type;
}

void TextureVK::setFilterType(ETextureFilter min, ETextureFilter mag)
{
}
void TextureVK::setWrap(EWrapType wrap)
{
}
void TextureVK::setAnisotropicLevel(EAnisotropic level)
{
}
void TextureVK::freeMemory(u32 texture)
{
}
u32 TextureVK::getTextureID() const
{
    return u32();
}
void TextureVK::reset()
{
}
u32 TextureVK::internalFormat(u32 format, u32 type)
{
    return u32();
}
void TextureVK::copyData(const TexturePtr & texture)
{
}

bool TextureVK::create()
{
    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = getImageTypeVK(m_target);
    imageCreateInfo.format = getImageFormatVK(m_format, m_type);
    imageCreateInfo.mipLevels = m_mipmapLevel;
    imageCreateInfo.arrayLayers = getArrayLayersCountVK(m_target, m_size);
    imageCreateInfo.samples = getSampleCountVK(ENGINE_CONTEXT->getSamplesCount());
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.pQueueFamilyIndices = &m_queueFamilyIndex;
    imageCreateInfo.queueFamilyIndexCount = 1;
    imageCreateInfo.initialLayout = m_imageLayout;
    imageCreateInfo.extent = getImageExtentVK(m_target, m_size);
    imageCreateInfo.usage = m_usage;

    VkResult result = vkCreateImage(m_device, &imageCreateInfo, nullptr, &m_image);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("TextureVK::create: vkCreateImage. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    VkMemoryPropertyFlags memoryProps = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    const bool transient = (m_usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) ? true : false;
    if (transient)
    {
        memoryProps |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
    }

    MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
    m_memory = memoryManager->allocateImage(*memoryManager->getSimpleAllocator(), m_image, memoryProps);
    if (m_memory._memory == VK_NULL_HANDLE)
    {
        LOG_ERROR("TextureVK::create: allocateImage return invalid memory");
        return false;
    }

    return true;
}

void TextureVK::destroy()
{
    if (m_memory._memory != VK_NULL_HANDLE)
    {
        MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
        memoryManager->free(*memoryManager->getSimpleAllocator(), m_memory);
        m_memory._memory = VK_NULL_HANDLE;
    }

    if (m_image != VK_NULL_HANDLE)
    {
        vkDestroyImage(m_device, m_image, nullptr);
    }
}

bool TextureVK::bindTexture(ETextureTarget target, u32 texture)
{
    return false;
}
void TextureVK::bindTexBuffer(u32 format, u32 texture, u32 buffer, u32 offset, u32 size)
{
}
bool TextureVK::bindTextureUnit(u32 unit)
{
    return false;
}
s32 TextureVK::getActiveTexture(u32 target)
{
    return s32();
}
s32 TextureVK::getActiveTextureUnit()
{
    return s32();
}

void TextureVK::setImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange)
{
    //// Create an image barrier object
    //VkImageMemoryBarrier imageMemoryBarrier = vkTools::initializers::imageMemoryBarrier();;
    //imageMemoryBarrier.oldLayout = oldImageLayout;
    //imageMemoryBarrier.newLayout = newImageLayout;
    //imageMemoryBarrier.image = image;
    //imageMemoryBarrier.subresourceRange = subresourceRange;

    //// Only sets masks for layouts used in this example
    //// For a more complete version that can be used with other layouts see vkTools::setImageLayout

    //// Source layouts (old)
    //switch (oldImageLayout)
    //{
    //case VK_IMAGE_LAYOUT_UNDEFINED:
    //    // Only valid as initial layout, memory contents are not preserved
    //    // Can be accessed directly, no source dependency required
    //    imageMemoryBarrier.srcAccessMask = 0;
    //    break;

    //case VK_IMAGE_LAYOUT_PREINITIALIZED:
    //    // Only valid as initial layout for linear images, preserves memory contents
    //    // Make sure host writes to the image have been finished
    //    imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    //    break;

    //case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    //    // Old layout is transfer destination
    //    // Make sure any writes to the image have been finished
    //    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    //    break;
    //}

    //// Target layouts (new)
    //switch (newImageLayout)
    //{
    //case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    //    // Transfer source (copy, blit)
    //    // Make sure any reads from the image have been finished
    //    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    //    break;

    //case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    //    // Transfer destination (copy, blit)
    //    // Make sure any writes to the image have been finished
    //    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    //    break;

    //case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    //    // Shader read (sampler, input attachment)
    //    imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    //    break;
    //}

    //// Put barrier on top of pipeline
    //VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    //VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    //// Put barrier inside setup command buffer
    //vkCmdPipelineBarrier( cmdBuffer, srcStageFlags, destStageFlags, VK_FLAGS_NONE, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
