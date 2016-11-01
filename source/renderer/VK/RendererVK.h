#pragma once

#include "renderer/Renderer.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"
#include "context/DeviceContextVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderStateVK;
    class MemoryManagerVK;
    class CommandBufferVK;
    class CommandPoolVK;
    class FramebufferVK;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::array<VkDynamicState, 3> k_dynamicStateVK =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for general render management. Vulkan render only.
    */
    class RendererVK final : public IRenderer
    {
    public:

        explicit RendererVK(const ContextPtr context);
        ~RendererVK();

        platform::ERenderType   getRenderType() const override;

        const ContextVKPtr      getVulkanContext() const;
        MemoryManagerVK*        getMemoryManager();
        CommandBufferVK*        getCurrentCommandBuffer() const;

        template <class V, typename T>
        static bool checkPresentState(const V& container, T value)
        {
            auto iter = std::find(container.cbegin(), container.cend(), T);
            if (iter != container.cend())
            {
                return true;
            }

            return false;
        }

    private:

        void                    immediateInit() override;

        void                    immediaterBeginFrame() override;
        void                    immediateEndFrame() override;
        void                    immediatePresentFrame() override;

        void                    immediateDraw() override;

        VkPipeline              createGraphicPipeline(const RenderStateVK* renderState, const FramebufferVK* framebuffer);
        VkPipeline              getGraphicPipeline(const RenderStateVK* renderState, const FramebufferVK* framebuffer);
        void                    destroyGraphicPipelines();

        MemoryManagerVK*        m_memoryMamager;

        CommandPoolVK*          m_commandPool;

        VkDevice                m_device;
        u32                     m_queueFamilyIndex;

        std::map<u64, VkPipeline> m_pipelineList;

    public:

        void                preRender(bool clear = false)                                           override;
        void                postRender()                                                            override;



        ShaderPtr           makeSharedShader()                                                      override;
        ShaderProgramPtr    makeSharedProgram()                                                     override;
        GeometryPtr         makeSharedGeometry(const CRenderTechnique* technique)                   override;
        RenderStatePtr      makeSharedRenderState()                                                 override;

        RenderTargetPtr     makeSharedRenderTarget()                                                override;
        GeometryTargetPtr   makeSharedGeometryTarget()                                              override;
        void                resetTextures()                                                         override;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
