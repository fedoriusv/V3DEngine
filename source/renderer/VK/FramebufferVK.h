#pragma once

#include "renderer/RenderTarget.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"

namespace v3d
{
namespace renderer
{
namespace vk
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class FramebufferVK : public RenderTarget
    {
    public:

        FramebufferVK();
        ~FramebufferVK();

        void bind() override;
        void unbind() override;

    private:

        bool create() override;
        void destroy() override;


        void                        createAttachment(SAttachments& attach, const core::Rect32& rect);

        VkFramebuffer               m_framebuffer;
        std::vector<VkImageView>    m_attacmentsView;

        bool                        m_initialized;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
