#ifndef _V3D_GEOMETRY_H_
#define _V3D_GEOMETRY_H_

#include "Vertex.h"
#include "GeometryTypes.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Buffer;
    class CRenderTechnique;

    class CGeometry
    {
    public:

        struct SInterval
        {
            SInterval();

            u32 _begin;
            u32 _count;
        };

        explicit CGeometry(const CRenderTechnique* technique);
        virtual                 ~CGeometry();

        virtual void            init()      = 0;
        virtual void            free()      = 0;
        virtual void            draw()      = 0;
        virtual void            refresh()   = 0;

        bool                    updated() const;

        SVertexData&            getData();

        EPrimitivesMode         getDrawMode() const;
        void                    setDrawMode(EPrimitivesMode mode);

        void                    setInterval(u32 begin, u32 count);
        const SInterval&        getInterval() const;

        void                    addVertex(const core::Vector3D& vertex);
        void                    addNormal(const core::Vector3D& normal);
        void                    addBinormal(const core::Vector3D& binormal);
        void                    addTangent(const core::Vector3D& tangent);
        void                    addColor(const core::Vector3D& color);
        void                    addTexCoord(u32 layer, const core::Vector2D& texCoord);
        void                    addIndex(u32 index);

    protected:

        void                    setVertexMask(u32 mask);

        EPrimitivesMode         m_drawMode;
        EDataUsageType          m_geometyType;
        SVertexData             m_data;
        const CRenderTechnique* m_technique;

        Buffer*                 m_vertexBuffer;
        Buffer*                 m_indexBuffer;

    private:

        SInterval               m_interval;
        u32                     m_currentVertexMask;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CGeometry> GeometryPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_H_
