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

    class FenceVK
    {
    public:

        FenceVK(VkDevice device);
        ~FenceVK();

        bool        create(bool signaled = true);
        void        destroy();

        bool        reset();
        bool        wait();

        bool        status();

        VkFence     fence() const;

    private:

        VkDevice    m_device;
        VkFence     m_fence;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
