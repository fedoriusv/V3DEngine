#ifndef _V3D_GEOMETRY_H_
#define _V3D_GEOMETRY_H_

#include "Vertex.h"
#include "renderer/RenderTechnique.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDrawMode
    {
        eTriangleStrip,
        eTriangles,
        eTriangleFan,
        eLines,

        eCount
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

        CGeometry(const RenderTechniquePtr& technique);
        virtual             ~CGeometry();

        virtual void        init()   = 0;
        virtual void        draw()   = 0;
        virtual void        free()   = 0;
        virtual void        refresh()= 0;

        SVertexData&        getData();

        EDrawMode           getDrawMode() const;
        void                setDrawMode(EDrawMode mode);

        void                copyToVertices (const f32 vertices[][3], u32 size);
        void                copyToNormals  (const f32 normals[][3], u32 size);
        void                copyToTexCoords(const f32 texCoords[][2], u32 layer, u32 size);
        void                copyToIndices  (const u32* indices, u32 size);

    protected:

        EDrawMode           m_drawMode;
        EGeomertyType       m_geometyType;
        SVertexData         m_data;
        RenderTechniquePtr  m_technique;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CGeometry> GeometryPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_GEOMETRY_H_
