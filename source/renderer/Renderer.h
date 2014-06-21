#ifndef _V3D_RENDERER_H_
#define _V3D_RENDERER_H_

#include "Singleton.h"
#include "context/DriverContext.h"
#include "renderer/Shader.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Geometry.h"
#include "renderer/Texture.h"
#include "renderer/DebugDraw.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderJob;

    class CRenderer : public Singleton<CRenderer>
    {
    public:

        CRenderer(const DriverContextPtr& context);
        virtual                  ~CRenderer();

        virtual void             init()                         = 0;
        
        virtual void             preRender()                    = 0;
        virtual void             postRender()                   = 0;

        virtual void             reshape(u32 width, u32 height);

        void                     updateCamera(const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up);
        void                     updateTransform(const core::Matrix4D& transform);

        void                     setBackColor(const core::Vector3D& color);
        const core::Vector3D&    getBackColor() const;

        void                     checkForErrors(const std::string& location = "");

        virtual ShaderPtr        makeSharedShader()                                      = 0;
        virtual ShaderProgramPtr makeSharedProgram(const ShaderDataPtr& data)            = 0;
        virtual GeometryPtr      makeSharedGeometry(const RenderTechniquePtr& technique) = 0;
        virtual TexturePtr       makeSharedTexture()                                     = 0;
        virtual RenderStatePtr   makeSharedRenderState()                                 = 0;
#ifdef _DEBUG
        virtual DebugDrawPtr     makeDebugDraw(const GeometryPtr& geometry)              = 0;
        void                     setDebugMode(bool active);
        bool                     isDebugMode() const;
#endif

        const core::Dimension2D& getViewportSize() const;

    protected:

        friend                   CRenderJob;

        DriverContextPtr         m_context;

        core::Vector3D           m_backColor;
        core::Dimension2D        m_viewportSize;

        core::Matrix4D           m_projectionMatrix;
        core::Matrix4D           m_viewMatrix;
        core::Vector3D           m_viewPosition;
        core::Matrix4D           m_modelMatrix;
        core::Matrix4D           m_normalMatrix;

#ifdef _DEBUG
        bool                     m_debugMode;
#endif
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderer>	RendererPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERER_H_
