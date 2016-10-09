#pragma once

#include "renderer/Renderer.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

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

    private:

        void                    immediateInit() override;

        void                    immediaterBeginFrame() override;
        void                    immediateEndFrame() override;
        void                    immediatePresentFrame() override;

        void                    immediateDraw() override;

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
