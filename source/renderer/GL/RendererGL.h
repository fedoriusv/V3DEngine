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
        virtual             ~CRendererGL();
    
        void                init()                                                  override;
    
        void                preRender()                                             override;
        void                postRender()                                            override;
    
        void                reshape(u32 width, u32 height)                          override;

        ShaderPtr           makeSharedShader()                                      override;
        ShaderProgramPtr    makeSharedProgram(const ShaderDataPtr& data)            override;
        GeometryPtr         makeSharedGeometry(const RenderTechniquePtr& technique) override;
        TexturePtr          makeSharedTexture()                                     override;
        RenderStatePtr      makeSharedRenderState()                                 override;
#ifdef _DEBUG
        DebugDrawPtr        makeDebugDraw(const GeometryPtr& geometry)              override;
        DebugDrawPtr        makeDebugLight(Vector3D* position, f32* radius)         override;
#endif
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERER_GL_H_
