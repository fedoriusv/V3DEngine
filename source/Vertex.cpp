#include "Vertex.h"

using namespace v3d;

SVertexData::SVertexData()
    : _verticesId(0)
    , _indicesId(0)
    , _countVertices(0)
    , _countIndices(0)
{
}

void SVertexData::clear()
{
	if (!_vertices.empty())
	{
		_vertices.clear();
	}

	if (!_normals.empty())
	{
		_normals.clear();
	}

    if (!_colors.empty())
    {
        _colors.clear();
    }

    if (!_tangents.empty())
    {
        _tangents.clear();
    }

	if (!_texCoords.empty())
	{
		for (auto layer : _texCoords)
		{
			if (!layer.empty())
			{
				layer.clear();
			}
		}
		_texCoords.clear();
	}

	if (!_indices.empty())
	{
		_indices.clear();
	}

	_countVertices = 0;
	_countIndices = 0;
}

void SVertexData::malloc(v3d::u32 count, v3d::u32 index, v3d::u32 layer, bool color, bool tangent)
{
    clear();

    _countVertices = count;
    _vertices.resize(count);
    _normals.resize(count);
    if (color)
    {
        _colors.resize(count);
    }
    if (tangent)
    {
        _tangents.resize(tangent);
    }

    for (v3d::u32 i = 0; i < layer; ++i)
    {
        std::vector<core::Vector2D> texCoord;
        texCoord.resize(count);

        _texCoords.push_back(texCoord);
    }

    _countIndices  = index;
    if (index > 0)
    {
        _indices.resize(index);
    }

}

bool SVertexData::empty() const
{
    return _vertices.empty()
        && _normals.empty()
        && _texCoords.at(0).empty()
        && _colors.empty()
        && _tangents.empty();

}