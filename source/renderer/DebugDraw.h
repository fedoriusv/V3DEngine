#ifndef _V3D_DEBUG_DRAW_H_
#define _V3D_DEBUG_DRAW_H_

#include "Object.h"
#include "RenderTechnique.h"
#include "Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDebugDraw
    {
        eDrawNone    = 0, 
        eDrawNormals = 1 << 0,
        eDrawEdges   = 1 << 1,

        eDrawAll
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugDraw : public CObject
    {
    public:

        CDebugDraw(const GeometryPtr& geometry);
        virtual                     ~CDebugDraw();
                                   
        void                        init();
        void                        bind();

        virtual void                draw()    = 0;
        virtual void                refresh() = 0;
        virtual void                free()    = 0;

        void                        setDebugFlag(EDebugDraw flag);

    protected:

        void                        initDrawNormalsShader();
        virtual void                initDrawNormalsData();

        static RenderTechniquePtr   m_tehnique;
        
        GeometryPtr                 m_geometry;
        SVertices<core::Vector3D>   m_normals;
        EDebugDraw                  m_flag;

        const std::string*          m_vertex;
        const std::string*          m_fragment;

        u32                         m_arrayId;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDebugDraw> DebugDrawPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DEBUG_DRAW_H_
