#include "Vertex.h"

using namespace v3d;
using namespace core;

SVertexData::SVertexData()
{
}

void SVertexData::clear()
{
    _vertices.clear();
    _colors.clear();
    _normals.clear();
    _binormals.clear();
    _tangents.clear();

    for (auto& layer : _texCoords)
    {
        layer.clear();
    }
    _texCoords.clear();

    _indices.clear();
}

void SVertexData::malloc(u32 count, u32 index, u32 layer, bool basis, bool color)
{
    clear();

    _vertices.resize(count);
    _normals.resize(count);
    if (basis)
    {
        _binormals.resize(count);
        _tangents.resize(count);
    }
    if (color)
    {
        _colors.resize(count);
    }

    for (u32 i = 0; i < layer; ++i)
    {
        std::vector<Vector2D> texCoord;
        texCoord.resize(count);

        _texCoords.push_back(texCoord);
    }

    if (index > 0)
    {
        _indices.resize(index);
    }

}

u32 SVertexData::verticesSize()
{
    return (u32)_vertices.size();
}

u32 SVertexData::indicesSize()
{
    return (u32)_indices.size();
}

bool SVertexData::empty() const
{
    return _vertices.empty()
        && _normals.empty()
        && _binormals.empty()
        && _tangents.empty()
        && _texCoords.at(0).empty()
        && _colors.empty();
}