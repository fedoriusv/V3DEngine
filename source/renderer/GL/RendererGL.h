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
    
        void                init()                         override;
    
        void                preRender()                    override;
        void                postRender()                   override;
    
        void                reshape(u32 width, u32 height) override;

        ShaderPtr           makeSharedShader()                                     override;
        ShaderProgramPtr    makeSharedProgram(const ShaderDataPtr& data)           override;
        GeometryPtr         makeSharedGeometry(const RenderTechniquePtr& technique)override;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERER_GL_H_
