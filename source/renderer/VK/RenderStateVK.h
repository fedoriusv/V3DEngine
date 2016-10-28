#pragma once

#include "renderer/RenderState.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"

namespace v3d
{
namespace renderer
{
namespace vk
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for render states management. Vulkan render only.
    */
    class RenderStateVK final : public RenderState
    {
    public:

        RenderStateVK();
        RenderStateVK(const RenderStateVK& state) = delete;
        RenderStateVK& operator=(const RenderStateVK& state) = delete;

        ~RenderStateVK();

        VkPipelineRasterizationStateCreateInfo getPipelineRasterizationStateCreateInfo();

        void         bind() override;

    private:

        VkPipelineRasterizationStateCreateInfo m_pipelineRasterizationStateCreateInfo;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
