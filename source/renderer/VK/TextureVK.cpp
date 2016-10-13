#include "TextureVK.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{

using namespace core;

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

    , m_image(VK_NULL_HANDLE)
    , m_imageView(VK_NULL_HANDLE)
    , m_imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
    , m_deviceMemory(VK_NULL_HANDLE)

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
void TextureVK::update(u32 offset, u32 size, const void * data, u32 level)
{
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
    return ETextureTarget();
}
ETextureFilter TextureVK::getMinFiler() const
{
    return ETextureFilter();
}
ETextureFilter TextureVK::getMagFiler() const
{
    return ETextureFilter();
}
EWrapType TextureVK::getWrap() const
{
    return EWrapType();
}
EAnisotropic TextureVK::getAnisotropic() const
{
    return EAnisotropic();
}
u32 TextureVK::getMipmapLevel() const
{
    return u32();
}
const core::Dimension3D & TextureVK::getSize() const
{
    // TODO: insert return statement here
}
EImageFormat TextureVK::getFormat() const
{
    return EImageFormat();
}
EImageType TextureVK::getType() const
{
    return EImageType();
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
    //// Create optimal tiled target image
    //VkImageCreateInfo imageCreateInfo = vkTools::initializers::imageCreateInfo();
    //imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    //imageCreateInfo.format = format;
    //imageCreateInfo.mipLevels = texture.mipLevels;
    //imageCreateInfo.arrayLayers = 1;
    //imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    //imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    //imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
    //imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    //// Set initial layout of the image to undefined
    //imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //imageCreateInfo.extent = { texture.width, texture.height, 1 };
    //imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    //VK_CHECK_RESULT(vkCreateImage(device, &imageCreateInfo, nullptr, &texture.image));
    //TODO:
    return false;
}

void TextureVK::destroy()
{
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
