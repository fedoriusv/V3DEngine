#ifndef _V3D_RENDERER_D3D_H_
#define _V3D_RENDERER_D3D_H_

#include "renderer/Renderer.h"

#ifdef _V3D_TEXTURE_D3D_H_

namespace v3d
{
namespace renderer
{
namespace d3d
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    class RendererD3D : public IRenderer
    {
    public:
    
        RendererD3D(const ContextPtr context);
        ~RendererD3D();
    
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

        platform::ERenderType getRenderType() const                                                 override;

    private:

        void                resetTextures()                                                         override;

        bool                m_isLocked;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace d3d
} //namespace renderer
} //namespace v3d

#endif //_V3D_TEXTURE_D3D_H_

#endif //_V3D_RENDERER_D3D_H_


