#pragma once

#include "renderer/Renderer.h"

#ifdef _OPENGL_RENDER_

namespace v3d
{
namespace renderer
{
namespace gl
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    /**
    * Inherited class for general render management. GL render only.
    */
    class RendererGL final : public IRenderer
    {
    public:
    
        explicit RendererGL(const ContextPtr context);
        ~RendererGL();

        platform::ERenderType   getRenderType() const override;

    private:

        void           immediateInit() override;

        void           immediaterBeginFrame() override;
        void           immediateEndFrame() override;
        void           immediatePresentFrame() override;

        void           immediateDraw() override;


    public:
    
        void                preRender(bool clear = false)                                           override;
        void                postRender()                                                            override;



        ShaderPtr           makeSharedShader()                                                      override;
        ShaderProgramPtr    makeSharedProgram()                                                     override;
//        GeometryPtr         makeSharedGeometry(const RenderTechnique* technique)                   override;
        RenderStatePtr      makeSharedRenderState()                                                 override;

        RenderTargetPtr     makeSharedRenderTarget()                                                override;
        GeometryTargetPtr   makeSharedGeometryTarget()                                              override;

        bool                isLocked() const;

    private:

        void                resetTextures()                                                          override;

        bool                m_isLocked;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_
