#ifndef _V3D_DEBUG_GEOMETRY_H_
#define _V3D_DEBUG_GEOMETRY_H_

#include "Object.h"
#include "RenderPass.h"
#include "Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDebugGeometryFlag
    {
        eGeometryFlagNone    = -1,
        eGeometryFlagNormals = 0x00000001,
        eGeometryFlagEdges   = 0x00000010,

        eGeometryFlagAll     = 0x10000000
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDebugGeometry
    {
        eGeometryNormals = 0,
        eGeometryEdges,

        eGeometryCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SDebugObject
    {
       /* SVertices<core::Vector3D> _vertex;
        SVertices<v3d::u32>       _index;*/
        u32                       _arrayId;
        u32                       _drawMode;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugGeometry : public CObject
    {
    public:

        CDebugGeometry(const GeometryPtr& geometry);
        virtual                     ~CDebugGeometry();
                                   
        void                        init();
        void                        refresh();
        void                        bind();

        virtual void                draw()    = 0;
        virtual void                free()    = 0;

        void                        setDebugFlag(s32 flag);

    protected:

        void                        initShader();
        virtual void                initDraw(SDebugObject& object)    = 0;
        virtual void                refreshDraw(SDebugObject& object) = 0;

        void                        initDrawNormalsData();
        void                        initDrawEdgeData();

        static RenderPassPtr        s_pass;
        
        GeometryPtr                 m_geometry;

        SDebugObject                m_objects[EDebugGeometry::eGeometryCount];

        s32                         m_flag;

        const std::string*          m_vertex;
        const std::string*          m_fragment;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDebugGeometry> DebugGeometryPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DEBUG_GEOMETRY_H_
