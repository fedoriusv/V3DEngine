#ifndef _F3D_GEOMETRY_H_
#define _F3D_GEOMETRY_H_

#include "Vertex.h"
#include "renderer/RenderTechnique.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class EDrawMode
    {
        eTriangleStrip,
        eTriangles,
        eTriangleFan,
    };

    enum class EGeomertyType
    {
        eGeomertyStatic,
        eGeomertyDynamic
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CGeometry
    {
    public:

        CGeometry(const RenderTechiquePtr& techique);
        virtual             ~CGeometry();

        virtual void        init()   = 0;
        virtual void        draw()   = 0;
        virtual void        free()   = 0;

        SVertexData&        getData();

        EDrawMode           getDrawMode() const;
        void                setDrawMode(EDrawMode mode);

        void                copyVertices (const f32 vertices[][3], u32 size);
        void                copyNormals  (const f32 normals[][3], u32 size);
        void                copyTexCoords(const f32 texCoords[][2], u32 layer, u32 size);
        void                copyIndices  (const u32* indices, u32 size);

    protected:

        EDrawMode           m_drawMode;
        EGeomertyType       m_geometyType;
        SVertexData         m_data;
        RenderTechiquePtr   m_techique;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CGeometry> GeometryPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_GEOMETRY_H_