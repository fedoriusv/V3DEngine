#include "TextureVK.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "scene/TextureManager.h"

#ifdef _VULKAN_RENDER_
#include "context/DebugVK.h"
#include "RendererVK.h"
#include "CommandBufferVK.h"
#include "BufferVK.h"

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
        case ETextureTarget::eTextureCubeMap:
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

VkImageViewType getImageViewTypeVK(ETextureTarget target)
{
    switch (target)
    {
    case ETextureTarget::eTexture1D:
    case ETextureTarget::eTextureBuffer:
        return VK_IMAGE_VIEW_TYPE_1D;

    case ETextureTarget::eTexture1DArray:
        return VK_IMAGE_VIEW_TYPE_1D_ARRAY;

    case ETextureTarget::eTextureRectangle:
    case ETextureTarget::eTexture2D:
    case ETextureTarget::eTexture2DMSAA:
        return VK_IMAGE_VIEW_TYPE_2D;

    case ETextureTarget::eTexture2DArray:
        return VK_IMAGE_VIEW_TYPE_2D_ARRAY;

    case ETextureTarget::eTexture3D:
    case ETextureTarget::eTexture3DMSAA:
        return VK_IMAGE_VIEW_TYPE_3D;

    case ETextureTarget::eTextureCubeMap:
        return VK_IMAGE_VIEW_TYPE_CUBE;

    default:
        ASSERT(false, "invalid type");
    };

    return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
}

VkSampleCountFlagBits getSampleCountVK(u32 size, ETextureTarget target)
{
    if (target == eTexture2DMSAA || target == eTexture3DMSAA)
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
    }

    return VK_SAMPLE_COUNT_1_BIT;
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

    case ETextureTarget::eTextureCubeMap:
        arrayLayer = k_textureCubemapSideCount;
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

    case ETextureTarget::eTextureCubeMap:
        imageSize = { size.width, size.height, 1 };
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

    case EImageType::eUnsignedByte:
        switch (format)
        {
        case EImageFormat::eRed:
            return VK_FORMAT_R8_UNORM;
        case EImageFormat::eRG:
            return VK_FORMAT_R8G8_UNORM;
        case EImageFormat::eRGB:
            return VK_FORMAT_R8G8B8_UNORM;
        case EImageFormat::eRGBA:
            return VK_FORMAT_R8G8B8A8_UNORM;

        case EImageFormat::eDepthComponent:
            return VK_FORMAT_UNDEFINED;
        case EImageFormat::eStencilIndex:
            return VK_FORMAT_UNDEFINED;
        }

    case EImageType::eShort:
        switch (format)
        {
        case EImageFormat::eRed:
            return VK_FORMAT_R16_SNORM;
        case EImageFormat::eRG:
            return VK_FORMAT_R16G16_SNORM;
        case EImageFormat::eRGB:
            return VK_FORMAT_R16G16B16_SNORM;
        case EImageFormat::eRGBA:
            return VK_FORMAT_R16G16B16A16_SNORM;

        case EImageFormat::eDepthComponent:
            return VK_FORMAT_UNDEFINED;
        case EImageFormat::eStencilIndex:
            return VK_FORMAT_UNDEFINED;
        }

    case EImageType::eUnsignedShort:
        switch (format)
        {
        case EImageFormat::eRed:
            return VK_FORMAT_R16_UNORM;
        case EImageFormat::eRG:
            return VK_FORMAT_R16G16_UNORM;
        case EImageFormat::eRGB:
            return VK_FORMAT_R16G16B16_UNORM;
        case EImageFormat::eRGBA:
            return VK_FORMAT_R16G16B16A16_UNORM;

        case EImageFormat::eDepthComponent:
            return VK_FORMAT_UNDEFINED;
        case EImageFormat::eStencilIndex:
            return VK_FORMAT_UNDEFINED;
        }

    /*case EImageType::eInt:
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

TextureVK::TextureVK(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 mipCount)
    : m_target(target)
    , m_format(format)
    , m_type(type)
    , m_size(size)
    , m_mipmapLevel(m_target == ETextureTarget::eTexture2DMSAA || m_target == ETextureTarget::eTexture3DMSAA ? 1 : mipCount)

    , m_enable(false)

    , m_minFilter(mipCount > 1 ? eLinearMipmapLinear : eLinear)
    , m_magFilter(eLinear)
    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)

    , m_device(VK_NULL_HANDLE)
    , m_queueFamilyIndex(0)
    , m_image(VK_NULL_HANDLE)
    , m_imageView(VK_NULL_HANDLE)
    , m_aspectFlags(0)
    , m_imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
    , m_usage(0)
    , m_flags(0)

    , m_initialized(false)
{
    LOG_DEBUG("TextureVK::TextureVK constructor %x", this);

    if (m_target == ETextureTarget::eTextureRectangle || m_target == ETextureTarget::eTexture2DMSAA || m_target == ETextureTarget::eTexture3DMSAA)
    {
        ASSERT(!data, "data must be nullptr");
        if (format == EImageFormat::eDepthComponent || format == EImageFormat::eStencilIndex)
        {
            m_usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            m_aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        }
        else
        {
            m_usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        }
    }
    else if (m_target == ETextureTarget::eTextureBuffer)
    {
        m_usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
        m_aspectFlags = VK_IMAGE_ASPECT_METADATA_BIT;
    }
    else
    {
        m_usage  = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
    }
}

TextureVK::TextureVK(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 mipCount)
    : m_target(ETextureTarget::eTextureCubeMap)
    , m_format(format)
    , m_type(type)
    , m_size(Dimension3D(size.width, size.height, 1))

    , m_enable(false)

    , m_anisotropicLevel(eAnisotropic16x)
    , m_wrap(eClampToEdge)

    , m_minFilter(mipCount > 1 ? eLinearMipmapLinear : eLinear)
    , m_magFilter(eLinear)
    , m_mipmapLevel(mipCount)

    , m_device(VK_NULL_HANDLE)
    , m_queueFamilyIndex(0)
    , m_image(VK_NULL_HANDLE)
    , m_memory(k_invalidMemory)
    , m_imageView(VK_NULL_HANDLE)
    , m_aspectFlags(0)
    , m_imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
    , m_usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
    , m_flags(VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)

    , m_initialized(false)
{
    LOG_DEBUG("TextureVK::TextureVK constructor %x", this);
}

TextureVK::~TextureVK()
{
    LOG_DEBUG("TextureVK::TextureVK destructor %x", this);

    ASSERT(m_image == VK_NULL_HANDLE, "m_image already exist");
    ASSERT(m_memory._memory == VK_NULL_HANDLE, "m_memory already exist");
}

bool TextureVK::create(const void* data, u32 srcSize)
{
    if (m_initialized)
    {
        return true;
    }

    if (m_target == ETextureTarget::eTextureRectangle ||
        m_target == ETextureTarget::eTexture2DMSAA ||
        m_target == ETextureTarget::eTexture3DMSAA)
    {
        ASSERT(!data, "not support data for attachment format");
        return false;
    }

    m_device = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanDevice();
    m_queueFamilyIndex = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);

    VkFormat format = getImageFormatVK(m_format, m_type);
    VkImageType imageType = getImageTypeVK(m_target);
    u32 countLayers = getArrayLayersCountVK(m_target, m_size);

    VkPhysicalDevice physicalDevice = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanPhysicalDevice();
    vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, imageType, VK_IMAGE_TILING_OPTIMAL, m_usage, m_flags, &m_imageProps);
    ASSERT(m_mipmapLevel <= m_imageProps.maxMipLevels, "unsupport mipmap level");

    VkFormatProperties formatProperties = {};
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

    if (m_usage & VK_IMAGE_USAGE_SAMPLED_BIT)
    {
        ASSERT(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT, "Unsupport used feature");
    }

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;
    imageCreateInfo.flags = m_flags;
    imageCreateInfo.imageType = imageType;
    imageCreateInfo.format = format;
    imageCreateInfo.mipLevels = m_mipmapLevel;
    imageCreateInfo.arrayLayers = countLayers;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
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
        LOG_ERROR("TextureVK::create: vkCreateImage. Error: %s", DebugVK::errorString(result).c_str());
        return false;
    }

    VkMemoryPropertyFlags memoryProps = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    const bool transient = (m_usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) ? true : false;
    if (transient)
    {
        memoryProps |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;

        //vkGetDeviceMemoryCommitment();
    }

    MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
    m_memory = memoryManager->allocateImage(*memoryManager->getSimpleAllocator(), m_image, memoryProps);
    if (m_memory._memory == VK_NULL_HANDLE)
    {
        LOG_ERROR("TextureVK::create: allocateImage return invalid memory");
        return false;
    }

    u32 mipMapSize = srcSize;
    const void* mipmapData = data;
    if (m_mipmapLevel > 1)
    {
        mipMapSize = scene::TextureManager::getInstance()->calculateMipmapDataSize(m_size, m_format, m_type, m_mipmapLevel);
        mipmapData = scene::TextureManager::getInstance()->generateMipMaps(m_size, data, m_format, m_type, m_mipmapLevel);
        if (!mipmapData)
        {
            LOG_ERROR("TextureVK::create: can't create mipmap data");
            return false;
        }
    }

    BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eWriteStatic);
    if (!stagingBuffer->create(mipMapSize, mipmapData))
    {
        LOG_ERROR("TextureVK::create: can't create stagingBuffer");
        stagingBuffer->destroy();
        delete stagingBuffer;

        return false;
    }

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = 0;
    imageSubresourceRange.levelCount = m_mipmapLevel;
    imageSubresourceRange.baseArrayLayer = 0;
    imageSubresourceRange.layerCount = countLayers;

    CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();
    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    commandBuffer->copyBufferToImage(stagingBuffer->getBuffer(), m_image, m_imageLayout, imageSubresourceRange);

    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    stagingBuffer->destroy();
    delete stagingBuffer;

    if (!TextureVK::createImageView(format, imageSubresourceRange, true))
    {
        return false;
    }

    m_initialized = true;

    return true;
}

bool TextureVK::create(VkImage image)
{
    if (m_initialized)
    {
        return true;
    }

    if (m_target != ETextureTarget::eTextureRectangle &&
        m_target != ETextureTarget::eTexture2DMSAA &&
        m_target != ETextureTarget::eTexture3DMSAA)
    {
        return false;
    }

    m_device = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanDevice();
    m_queueFamilyIndex = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);

    VkFormat format = getImageFormatVK(m_format, m_type);
    VkImageType imageType = getImageTypeVK(m_target);

    VkPhysicalDevice physicalDevice = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanPhysicalDevice();
    vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, imageType, VK_IMAGE_TILING_OPTIMAL, m_usage, m_flags, &m_imageProps);

    VkFormatProperties formatProperties = {};
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

    if (image == VK_NULL_HANDLE)
    {
        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = nullptr;
        imageCreateInfo.flags = 0;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = format;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.samples = getSampleCountVK(ENGINE_CONTEXT->getSamplesCount(), m_target);
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
            LOG_ERROR("TextureVK::create: vkCreateImage. Error: %s", DebugVK::errorString(result).c_str());
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
    }
    else
    {
        m_image = image;
    }

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = 0;
    imageSubresourceRange.levelCount = 1;
    imageSubresourceRange.baseArrayLayer = 0;
    imageSubresourceRange.layerCount = 1;

    if (!TextureVK::createImageView(format, imageSubresourceRange, false))
    {
        return false;
    }

    m_initialized = true;

    return false;
}

void TextureVK::destroy()
{
    if (m_imageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(m_device, m_imageView, nullptr);
        m_imageView = nullptr;
    }

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

void TextureVK::bind(u32 unit)
{
    //TODO:
}

void TextureVK::unbind()
{
    //TODO:
}

bool TextureVK::isValid() const
{
    return m_initialized;
}

bool TextureVK::isEnable() const
{
    return m_enable;
}

void TextureVK::update(u32 offset, u32 size, const void* data, u32 mipLevel)
{
    BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eWriteStatic);
    u32 dataSize = size * ImageFormat::componentCount(m_format) * ImageFormat::typeSize(m_type);
    if (!stagingBuffer->create(dataSize, data))
    {
        LOG_ERROR("TextureVK::update: can't create stagingBuffer");
        stagingBuffer->destroy();
        delete stagingBuffer;

        return;
    }

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = mipLevel;
    imageSubresourceRange.levelCount = 1;
    imageSubresourceRange.baseArrayLayer = 0;
    imageSubresourceRange.layerCount = 1;

    CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();

    VkImageLayout originImageLayout = m_imageLayout;
    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    commandBuffer->copyBufferToImage(stagingBuffer->getBuffer(), m_image, m_imageLayout, imageSubresourceRange);

    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, originImageLayout, imageSubresourceRange);
    m_imageLayout = originImageLayout;

    stagingBuffer->destroy();
    delete stagingBuffer;
}

void TextureVK::update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel)
{
    BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eWriteStatic);
    u32 dataSize = size.getArea() * ImageFormat::componentCount(m_format) * ImageFormat::typeSize(m_type);
    if (!stagingBuffer->create(dataSize, data))
    {
        LOG_ERROR("TextureVK::update: can't create stagingBuffer");
        stagingBuffer->destroy();
        delete stagingBuffer;

        return;
    }

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = mipLevel;
    imageSubresourceRange.levelCount = 1;
    imageSubresourceRange.baseArrayLayer = (m_target == ETextureTarget::eTexture1DArray) ? size.height : 0;
    imageSubresourceRange.layerCount = 1;

    CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();

    VkImageLayout originImageLayout = m_imageLayout;
    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    commandBuffer->copyBufferToImage(stagingBuffer->getBuffer(), m_image, m_imageLayout, imageSubresourceRange);

    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, originImageLayout, imageSubresourceRange);
    m_imageLayout = originImageLayout;

    stagingBuffer->destroy();
    delete stagingBuffer;
}

void TextureVK::update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 mipLevel)
{
    BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eWriteStatic);
    u32 dataSize = size.getArea() * ImageFormat::componentCount(m_format) * ImageFormat::typeSize(m_type);
    if (!stagingBuffer->create(dataSize, data))
    {
        LOG_ERROR("TextureVK::update: can't create stagingBuffer");
        stagingBuffer->destroy();
        delete stagingBuffer;

        return;
    }

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = mipLevel;
    imageSubresourceRange.levelCount = 1;
    imageSubresourceRange.baseArrayLayer = (m_target == ETextureTarget::eTexture2DArray) ? size.depth : 0;
    imageSubresourceRange.layerCount = 1;

    CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();

    VkImageLayout originImageLayout = m_imageLayout;
    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    commandBuffer->copyBufferToImage(stagingBuffer->getBuffer(), m_image, m_imageLayout, imageSubresourceRange);

    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, originImageLayout, imageSubresourceRange);
    m_imageLayout = originImageLayout;

    stagingBuffer->destroy();
    delete stagingBuffer;
}

void TextureVK::update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel)
{
    BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eWriteStatic);
    u32 dataSize = size.getArea() * ImageFormat::componentCount(m_format) * ImageFormat::typeSize(m_type);
    if (!stagingBuffer->create(dataSize, data))
    {
        LOG_ERROR("TextureVK::update: can't create stagingBuffer");
        stagingBuffer->destroy();
        delete stagingBuffer;

        return;
    }

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = mipLevel;
    imageSubresourceRange.levelCount = 1;
    imageSubresourceRange.baseArrayLayer = cubemapSide;
    imageSubresourceRange.layerCount = 1;

    CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();

    VkImageLayout originImageLayout = m_imageLayout;
    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    commandBuffer->copyBufferToImage(stagingBuffer->getBuffer(), m_image, m_imageLayout, imageSubresourceRange);

    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, originImageLayout, imageSubresourceRange);
    m_imageLayout = originImageLayout;

    stagingBuffer->destroy();
    delete stagingBuffer;
}

void TextureVK::read(void* const data, u32 mipLevel) const
{
    BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eReadStatic);
    u32 dataSize = m_size.getArea() * ImageFormat::componentCount(m_format) * ImageFormat::typeSize(m_type);
    if (!stagingBuffer->create(dataSize, nullptr))
    {
        LOG_ERROR("TextureVK::update: can't create stagingBuffer");
        stagingBuffer->destroy();
        delete stagingBuffer;

        return;
    }

    u32 currentLayer = 0; //TODO:

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = mipLevel;
    imageSubresourceRange.levelCount = 1;
    imageSubresourceRange.baseArrayLayer = currentLayer;
    imageSubresourceRange.layerCount = 1;

    CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();

    VkImageLayout originImageLayout = m_imageLayout;
    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    commandBuffer->copyImageToBuffer(m_image, stagingBuffer->getBuffer(), m_imageLayout, imageSubresourceRange);

    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, originImageLayout, imageSubresourceRange);
    m_imageLayout = originImageLayout;

    stagingBuffer->destroy();
    delete stagingBuffer;
}

void TextureVK::read(u32 cubemapSide, void* const data, u32 mipLevel) const
{
    BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eReadStatic);
    u32 dataSize = m_size.getArea() * ImageFormat::componentCount(m_format) * ImageFormat::typeSize(m_type);
    if (!stagingBuffer->create(dataSize, nullptr))
    {
        LOG_ERROR("TextureVK::update: can't create stagingBuffer");
        stagingBuffer->destroy();
        delete stagingBuffer;

        return;
    }

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = mipLevel;
    imageSubresourceRange.levelCount = 1;
    imageSubresourceRange.baseArrayLayer = cubemapSide;
    imageSubresourceRange.layerCount = 1;

    CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();

    VkImageLayout originImageLayout = m_imageLayout;
    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    commandBuffer->copyImageToBuffer(m_image, stagingBuffer->getBuffer(), m_imageLayout, imageSubresourceRange);

    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, originImageLayout, imageSubresourceRange);
    m_imageLayout = originImageLayout;

    stagingBuffer->destroy();
    delete stagingBuffer;
}

void TextureVK::fill(const void* data, u32 offset, u32 size, u32 mipLevel)
{
    //TODO:
}

void TextureVK::fill(const void* data, const core::Dimension2D& offset, const core::Dimension2D& size, u32 mipLevel)
{
    //TODO:
}

void TextureVK::fill(const void* data, const core::Dimension3D& offset, const core::Dimension3D& size, u32 mipLevel)
{
    //TODO:
}

ETextureTarget TextureVK::getTarget() const
{
    return m_target;
}

ETextureFilter TextureVK::getMinFilter() const
{
    return m_minFilter;
}

ETextureFilter TextureVK::getMagFilter() const
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

u32 TextureVK::getMipmapLevels() const
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
    //TODO:
}

void TextureVK::setWrap(EWrapType wrap)
{
    //TODO:
}

void TextureVK::setAnisotropicLevel(EAnisotropic level)
{
    //TODO:
}

void TextureVK::copyData(Texture* texture)
{
    CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();

    TextureVK* srcTexture = static_cast<TextureVK*>(texture);
    ASSERT(m_mipmapLevel == srcTexture->m_mipmapLevel, "Different mipmap level");

    u32 countLayers = 1; //TODO:

    VkImageSubresourceRange imageSubresourceRange = {};
    imageSubresourceRange.aspectMask = m_aspectFlags;
    imageSubresourceRange.baseMipLevel = 0;
    imageSubresourceRange.levelCount = m_mipmapLevel;
    imageSubresourceRange.baseArrayLayer = 0;
    imageSubresourceRange.layerCount = countLayers;

    VkImageLayout originSrcImageLayout = srcTexture->m_imageLayout;
    commandBuffer->imageMemoryBarrier(srcTexture->m_image, srcTexture->m_aspectFlags, srcTexture->m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, imageSubresourceRange);
    srcTexture->m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    VkImageLayout originDstImageLayout = m_imageLayout;
    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageSubresourceRange);
    m_imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    commandBuffer->copyImageToImage(srcTexture->m_image, m_image, srcTexture->m_imageLayout, m_imageLayout, imageSubresourceRange);

    commandBuffer->imageMemoryBarrier(srcTexture->m_image, srcTexture->m_aspectFlags, srcTexture->m_imageLayout, originSrcImageLayout, imageSubresourceRange);
    srcTexture->m_imageLayout = originSrcImageLayout;

    commandBuffer->imageMemoryBarrier(m_image, m_aspectFlags, m_imageLayout, originDstImageLayout, imageSubresourceRange);
    m_imageLayout = originDstImageLayout;
}

VkImage TextureVK::getImage() const
{
    return m_image;
}

VkImageView TextureVK::getImageView() const
{
    return m_imageView;
}

bool TextureVK::createImageView(VkFormat format, const VkImageSubresourceRange& imageSubresourceRange, bool createSubresource)
{
    if (m_image == VK_NULL_HANDLE)
    {
        ASSERT(false, "m_image is null handle");
        return false;
    }

    VkImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.viewType = getImageViewTypeVK(m_target);
    imageViewCreateInfo.format = format;
    imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY , VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
    imageViewCreateInfo.subresourceRange = imageSubresourceRange;
    imageViewCreateInfo.image = m_image;

    VkResult result = vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_imageView);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("TextureVK::createImageView: vkCreateImageView. Error: %s", DebugVK::errorString(result).c_str());
        return false;
    }

    if (createSubresource)
    {
        //TODO: create subresources
    }

    return false;
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
