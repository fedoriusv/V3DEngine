#include "Vertex.h"

using namespace v3d;

void SVertexData::clear()
{
	if (!_vertices.vertex.empty())
	{
		_vertices.vertex.clear();
		_vertices.id = 0;
	}

	if (!_normals.vertex.empty())
	{
		_normals.vertex.clear();
		_normals.id = 0;
	}

	if (!_texCoords.empty())
	{
		for (auto layer : _texCoords)
		{
			if (!layer.vertex.empty())
			{
				layer.vertex.clear();
				layer.id = 0;
			}
		}
		_texCoords.clear();
	}

	if (!_indices.vertex.empty())
	{
		_indices.vertex.clear();
		_indices.id = 0;
	}

	_countVertices = 0;
	_countIndices = 0;

}

void SVertexData::malloc(v3d::u32 count, v3d::u32 index, v3d::u32 layer)
{
	clear();

	_countVertices = count;
    _vertices.vertex.resize(count);
	_normals.vertex.resize(count);

	for (v3d::u32 i = 0; i < layer; ++i)
	{
		SVertices<core::Vector2D> texCoord;
		texCoord.vertex.resize(count);

		_texCoords.push_back(texCoord);

	}

	_countIndices  = index;
    _indices.vertex.resize(index);

}

bool SVertexData::empty() const
{
    return _vertices.vertex.empty()
        || _normals.vertex.empty()
        || _texCoords.at(0).vertex.empty();
}