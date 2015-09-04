#ifndef _V3D_RENDERER_D3D_H_
#define _V3D_RENDERER_D3D_H_

#include "renderer/Renderer.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    class CRendererD3D : public CRenderer
    {
    public:
    
        CRendererD3D(const DriverContextPtr& context);
        ~CRendererD3D();
    
        void                init()                                                                  override;
    
        void                preRender()                                                             override;
        void                postRender()                                                            override;

        ShaderPtr           makeSharedShader()                                                      override;
        ShaderProgramPtr    makeSharedProgram()                                                     override;
        GeometryPtr         makeSharedGeometry(const CRenderTechnique* technique)                   override;
        CTexture*           makeSharedTexture()                                                     override;
        RenderStatePtr      makeSharedRenderState()                                                 override;
        RenderTargetPtr     makeSharedRenderTarget()                                                override;

        bool                isLocked() const;

    private:

        void                resetTexture()                                                          override;

        bool                m_isLocked;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERER_D3D_H_
