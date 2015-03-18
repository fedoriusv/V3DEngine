#ifndef _V3D_RENDERER_GL_H_
#define _V3D_RENDERER_GL_H_

#include "renderer/Renderer.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    class CRendererGL : public CRenderer
    {
    public:
    
        CRendererGL(const DriverContextPtr& context);
        ~CRendererGL();
    
        void                init()                                                                  override;
    
        void                preRender()                                                             override;
        void                postRender()                                                            override;

        void                reshape(u32 width, u32 height)                                          override;

        ShaderPtr           makeSharedShader()                                                      override;
        ShaderProgramPtr    makeSharedProgram()                                                     override;
        GeometryPtr         makeSharedGeometry(const RenderTechniquePtr& technique)                 override;
        TexturePtr          makeSharedTexture()                                                     override;
        RenderStatePtr      makeSharedRenderState()                                                 override;
        RenderTargetPtr     makeSharedRenderTarget()                                                override;
#ifdef _DEBUG
        DebugGeometryPtr    makeDebugDraw(const GeometryPtr& geometry)                              override;
        DebugLightPtr       makeDebugLight(const Vector3D& position, const scene::SLightData& data) override;
#endif

        bool                isLocked() const;

    private:

        void                resetTexture()                                                          override;

        bool                m_isLocked;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERER_GL_H_
