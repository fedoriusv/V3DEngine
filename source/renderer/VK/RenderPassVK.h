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

    class RenderPassVK
    {
    public:

        RenderPassVK();
        ~RenderPassVK();

        bool            create();
        void            destroy();

    private:

        VkRenderPass    m_renderPass;

        VkDevice        m_device;

       bool             m_initialized;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

