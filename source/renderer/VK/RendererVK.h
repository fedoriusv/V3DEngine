#pragma once

#include "renderer/Renderer.h"

#ifdef _VULKAN_RENDER_
#   include "vulkan/vulkan.h"
#   include "context/DeviceContextVK.h"
#endif //_VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class MemoryManagerVK;

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

    private:

        void                    immediateInit() override;

        void                    immediaterBeginFrame() override;
        void                    immediateEndFrame() override;
        void                    immediatePresentFrame() override;

        void                    immediateDraw() override;

        MemoryManagerVK*        m_memoryMamager;

        VkDevice                m_device;
        u32                     m_queueFamilyIndex;

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
