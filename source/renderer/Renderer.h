#ifndef _V3D_RENDERER_H_
#define _V3D_RENDERER_H_

#include "Singleton.h"
#include "context/DriverContext.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Texture.h"
#include "RenderJob.h"
#include "DebugGeometry.h"
#include "DebugLight.h"

namespace v3d
{
namespace scene
{
    class CLight;
    class CSceneManager;
}
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugLight;

    class CRenderer : public Singleton<CRenderer>
    {
    public:

        CRenderer(const DriverContextPtr& context);
        virtual                     ~CRenderer();

        virtual void                init()                         = 0;
        
        virtual void                preRender()                    = 0;
        virtual void                postRender()                   = 0;

        void                        draw(const RenderJobPtr& job);
        virtual void                reshape(u32 width, u32 height);

        void                        updateCamera(const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up);

        void                        setBackColor(const core::Vector3D& color);
        const core::Vector3D&       getBackColor() const;

        void                        checkForErrors(const std::string& location = "");

        virtual ShaderPtr           makeSharedShader()                                           = 0;
        virtual ShaderProgramPtr    makeSharedProgram(const ShaderDataPtr& data)                 = 0;
        virtual GeometryPtr         makeSharedGeometry(const RenderTechniquePtr& technique)      = 0;
        virtual TexturePtr          makeSharedTexture()                                          = 0;
        virtual RenderStatePtr      makeSharedRenderState()                                      = 0;
#ifdef _DEBUG
        virtual DebugGeometryPtr    makeDebugDraw(const GeometryPtr& geometry)                              = 0;
        virtual DebugLightPtr       makeDebugLight(const Vector3D& position, const scene::SLightData& data) = 0;
        void                        setDebugMode(bool active);
        bool                        isDebugMode() const;
#endif

        const core::Dimension2D&    getViewportSize() const;

    protected:

        friend                      scene::CSceneManager;

        friend                      CDebugLight;

        void                        updateLight(const core::Matrix4D& transform, const RenderPassPtr& pass);
        void                        updateMaterial(const MaterialPtr& material, const RenderPassPtr& pass);
        void                        updateTransform(const core::Matrix4D& transform, const RenderPassPtr& pass);

        bool                        checkLOD(const core::Matrix4D& transform, const RenderPassPtr& pass);

        DriverContextPtr            m_context;

        core::Vector3D              m_backColor;
        core::Dimension2D           m_viewportSize;

        core::Matrix4D              m_projectionMatrix;
        core::Matrix4D              m_viewMatrix;
        core::Vector3D              m_viewPosition;

        bool                        m_updateCamera;

#ifdef _DEBUG
        bool                        m_debugMode;
#endif
        void                        addLight(scene::CLight* lights);
        std::vector<scene::CLight*> m_lightList;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderer>	RendererPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERER_H_
