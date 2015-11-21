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

        SVertexData();

        u32                           verticesSize();
        u32                           indicesSize();

        void                          clear();
        void                          malloc(u32 count, u32 index = 0, u32 layer = 1, bool basis = false, bool color = false);
        bool                          empty() const;

        void                          computeTangentBasis();

        std::vector<core::Vector3D>   _vertices;
        std::vector<core::Vector3D>   _colors;
        std::vector<core::Vector3D>   _normals;
        std::vector<core::Vector3D>   _binormals;
        std::vector<core::Vector3D>   _tangents;
        TextureCoords                 _texCoords;

        std::vector<u32>              _indices;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

}
#endif //_V3D_VERTEX_H_
