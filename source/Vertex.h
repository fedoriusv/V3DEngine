#ifndef _V3D_VERTEX_H_
#define _V3D_VERTEX_H_

#include "common.h"

namespace v3d
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef  std::vector<std::vector<core::Vector2D>> TextureCoords;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SVertexData
    {
        std::vector<core::Vector3D>   _vertices;
        std::vector<core::Vector3D>   _normals;
        std::vector<core::Vector3D>   _colors;
        std::vector<core::Vector3D>   _tangents;
        TextureCoords                 _texCoords;

        std::vector<v3d::u32>         _indices;

        v3d::u32                      _verticesId;
        v3d::u32                      _indicesId;

        v3d::u32                      _countVertices;
        v3d::u32                      _countIndices;

        void                          clear();
        void                          malloc(v3d::u32 count, v3d::u32 index = 0, v3d::u32 layer = 1, bool color = false, bool tangent = false);
        bool                          empty() const;

        SVertexData();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

}
#endif //_V3D_VERTEX_H_
