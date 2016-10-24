#pragma once

#include "common.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"
#include "renderer/Texture.h"
#include "MemoryManagerVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for texture management. VK render only.
    */
    class TextureVK : public Texture
    {
    public:

        TextureVK(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 mipCount = 1U);
        TextureVK(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 mipCount = 1U);
        ~TextureVK();

        void                            bind(u32 unit) override;
        void                            unbind() override;

        bool                            isValid()  const override;
        bool                            isEnable() const override;

        void                            update(u32 offset, u32 size, const void* data, u32 mipLevel = 0U) override;
        void                            update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel = 0U) override;
        void                            update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 mipLevel = 0U) override;
        void                            update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel = 0U) override;

        void                            read(void const* data, u32 mipLevel = 0U) const override;
        void                            read(u32 cubemapSide, void const* data, u32 mipLevel = 0U) const override;

        void                            fill(const void* data, u32 offset = 0U, u32 size = 0U, u32 mipLevel = 0U) override;
        void                            fill(const void* data, const core::Dimension2D& offset = core::Dimension2D(), const core::Dimension2D& size = core::Dimension2D(), u32 mipLevel = 0U) override;
        void                            fill(const void* data, const core::Dimension3D& offset = core::Dimension3D(), const core::Dimension3D& size = core::Dimension3D(), u32 mipLevel = 0U) override;

        ETextureTarget                  getTarget() const override;
        ETextureFilter                  getMinFilter() const override;
        ETextureFilter                  getMagFilter() const override;
        EWrapType                       getWrap() const override;
        EAnisotropic                    getAnisotropic() const override;
        u32                             getMipmapLevels() const override;
        const core::Dimension3D&        getSize() const override;
        EImageFormat                    getFormat() const override;
        EImageType                      getType() const override;

        void                            setFilterType(ETextureFilter min, ETextureFilter mag) override;
        void                            setWrap(EWrapType wrap) override;
        void                            setAnisotropicLevel(EAnisotropic level) override;

        bool                            create(const void* data, u32 srcSize) override;
        bool                            create(VkImage image);
        void                            destroy() override;
        void                            copyData(const TexturePtr& texture) override;

    private:

        bool                            createImageView(VkFormat format, const VkImageSubresourceRange& imageSubresourceRange);

        const ETextureTarget            m_target;
        const EImageFormat              m_format;
        const EImageType                m_type;
        const core::Dimension3D         m_size;
        const u32                       m_mipmapLevel;

        std::atomic<bool>               m_enable;

        std::atomic<ETextureFilter>     m_minFilter;
        std::atomic<ETextureFilter>     m_magFilter;
        std::atomic<EAnisotropic>       m_anisotropicLevel;
        std::atomic<EWrapType>          m_wrap;

    private:
        VkDevice                        m_device;
        u32                             m_queueFamilyIndex;

        SMemoryVK                       m_memory;
        VkImage                         m_image;

        VkImageView                     m_imageView;
        VkImageAspectFlags              m_aspectFlags;
        VkImageLayout                   m_imageLayout;
        VkImageUsageFlags               m_usage;
        VkImageCreateFlags              m_flags;
        VkImageFormatProperties         m_imageProps;

        std::atomic<bool>               m_initialized;
   };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
