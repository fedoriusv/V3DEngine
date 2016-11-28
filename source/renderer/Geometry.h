#pragma once

#include "Vertex.h"
#include "GeometryTypes.h"
#include "RenderState.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Buffer;
    class RenderTechnique;

    class Geometry
    {
    public:

        explicit Geometry(const RenderTechnique* technique);
        virtual                 ~Geometry();

        virtual void            init() = 0;
        virtual void            free() = 0;
        virtual void            draw() = 0;
        virtual void            refresh() = 0;

        bool                    updated() const;

        SVertexData&            getData();

        EPrimitivesTopology     getDrawMode() const;
        void                    setDrawMode(EPrimitivesTopology mode);

        void                    addVertex(const core::Vector3D& vertex);
        void                    addNormal(const core::Vector3D& normal);
        void                    addBinormal(const core::Vector3D& binormal);
        void                    addTangent(const core::Vector3D& tangent);
        void                    addColor(const core::Vector3D& color);
        void                    addTexCoord(u32 layer, const core::Vector2D& texCoord);
        void                    addIndex(u32 index);

    protected:

        void                    setVertexMask(u32 mask);
//        s32                     computeBufferSize(const ShaderDataList& shaderDataList);

        EPrimitivesTopology     m_drawMode;
        EDataUsageType          m_geometyType;
        SVertexData             m_data;
        const RenderTechnique*  m_technique;

        Buffer*                 m_vertexBuffer;
        Buffer*                 m_indexBuffer;

    private:

        u32                     m_currentVertexMask;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<Geometry> GeometryPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
