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
        eDrawNormals = 0,
        eDrawEdges,
        eDrawLights,

        eDrawCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDebugFlag
    {
        eDebugNone    = -1,
        eDebugNormals = 0x00000001,
        eDebugEdges   = 0x00000010,
        eDebugLights  = 0x00000100,

        eDebugAll     = 0x10000000
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SDebugObject
    {
        SVertices<core::Vector3D> _vertex;
        SVertices<v3d::u32>       _index;
        u32                       _arrayId;
        u32                       _drawMode;

        const Vector3D*           _vectors;
        const f32*                _param;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugDraw : public CObject
    {
    public:

        CDebugDraw(const GeometryPtr& geometry);
        CDebugDraw(const Vector3D* position, const f32* radius);
        virtual                     ~CDebugDraw();
                                   
        void                        init();
        void                        refresh();
        void                        bind(const Vector4D& color);

        virtual void                draw()    = 0;
        virtual void                free()    = 0;

        void                        setDebugFlag(s32 flag);

    protected:

        void                        initDrawShader();
        virtual void                initDraw(SDebugObject& object)    = 0;
        virtual void                refreshDraw(SDebugObject& object) = 0;

        void                        initDrawNormalsData();
        void                        initDrawEdgeData();
        void                        initDrawLightData();

        static RenderTechniquePtr   s_tehnique;
        
        GeometryPtr                 m_geometry;

        SDebugObject                m_objects[eDrawCount];

        s32                         m_flag;

        const std::string*          m_vertex;
        const std::string*          m_fragment;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDebugDraw> DebugDrawPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DEBUG_DRAW_H_
