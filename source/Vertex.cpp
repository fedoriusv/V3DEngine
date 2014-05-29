#include "Vertex.h"

using namespace v3d;

void SVertexData::clear()
{
	if (!m_vertices.vertex.empty())
	{
		m_vertices.vertex.clear();
		m_vertices.id = 0;
	}

	if (!m_normals.vertex.empty())
	{
		m_normals.vertex.clear();
		m_normals.id = 0;
	}

	if (!m_texCoords.empty())
	{
		for (auto layer : m_texCoords)
		{
			if (!layer.vertex.empty())
			{
				layer.vertex.clear();
				layer.id = 0;
			}
		}
		m_texCoords.clear();
	}

	if (!m_indices.vertex.empty())
	{
		m_indices.vertex.clear();
		m_indices.id = 0;
	}

	m_countVertices = 0;
	m_countIndices = 0;

}

void SVertexData::malloc(v3d::u32 count, v3d::u32 index, v3d::u32 layer)
{
	clear();

	m_countVertices = count;
	m_vertices.vertex.reserve(count);
	m_normals.vertex.reserve(count);

	for (v3d::u32 i = 0; i < layer; ++i)
	{
		SVertices<core::Vector2D> texCoord;
		texCoord.vertex.reserve(count);

		m_texCoords.push_back(texCoord);

	}

	m_countIndices  = index;
	m_indices.vertex.reserve(index);

}

bool SVertexData::empty() const
{
	return m_vertices.vertex.empty() 
		|| m_normals.vertex.empty() 
		|| m_texCoords.at(0).vertex.empty() 
		|| m_indices.vertex.empty();
}