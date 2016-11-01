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

    class FramebufferVK;

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

        const VkPipelineRasterizationStateCreateInfo&   getPipelineRasterizationStateCreateInfo() const;
        const VkPipelineColorBlendStateCreateInfo&      getPipelineColorBlendStateCreateInfo(const FramebufferVK* framebuffer) const;
        const VkPipelineDepthStencilStateCreateInfo&    getPipelineDepthStencilStateCreateInfo() const;
        const VkPipelineMultisampleStateCreateInfo&     getPipelineMultisampleStateCreateInfo() const;
        const VkPipelineInputAssemblyStateCreateInfo&   getPipelineInputAssemblyStateCreateInfo() const;
        const VkPipelineTessellationStateCreateInfo&    getPipelineTessellationStateCreateInfo() const;

        void                                            bind() override;

    private:

        void                                            updatePipelineRasterizationStateCreateInfo() const;
        void                                            updaePipelineColorBlendStateCreateInfo(const FramebufferVK* framebuffer) const;
        void                                            updatePipelineDepthStencilStateCreateInfo() const;
        void                                            updatePipelineMultisampleStateCreateInfo() const;
        void                                            updatePipelineInputAssemblyStateCreateInfo() const;
        void                                            updateVkPipelineTessellationStateCreateInfo() const;

        mutable VkPipelineRasterizationStateCreateInfo  m_pipelineRasterizationStateCreateInfo;
        mutable VkPipelineColorBlendStateCreateInfo     m_pipelineColorBlendStateCreateInfo;
        mutable VkPipelineDepthStencilStateCreateInfo   m_pipelineDepthStencilStateCreateInfo;
        mutable VkPipelineMultisampleStateCreateInfo    m_pipelineMultisampleStateCreateInfo;
        mutable VkPipelineInputAssemblyStateCreateInfo  m_pipelineInputAssemblyStateCreateInfo;
        mutable VkPipelineTessellationStateCreateInfo   m_pipelineTessellationStateCreateInfo;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
