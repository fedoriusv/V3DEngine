#include "CubeShape.h"
#include "renderer/Geometry.h"

using namespace f3d;
using namespace f3d::scene;
using namespace f3d::renderer;

CCubeShape::CCubeShape()
{
	m_shapeType = EShapeType::eCubeShape;
}

CCubeShape::~CCubeShape()
{
}

void CCubeShape::render()
{
    if (!m_visible)
    {
        return;
    }

    //Material->bind()

    m_geometry->update();
}

void CCubeShape::update(f64 time)
{
    if (!m_visible)
    {
        return;
    }

    if (m_needUpdate)
    {
        CNode::updateTransform(time);
        m_needUpdate = false;
    }

    //Renderer->updateTransform()
}

void CCubeShape::init()
{
	f3d::f32 s = 0.5f;

	f3d::f32 vertex[][3] =
	{
		{ -s, -s,  s }, {  s, -s,  s }, {  s,  s,  s }, { -s,  s,  s },
		{ -s, -s, -s }, { -s,  s, -s }, {  s,  s, -s }, {  s, -s, -s },
		{ -s,  s, -s }, { -s,  s,  s }, {  s,  s,  s }, {  s,  s, -s },
		{ -s, -s, -s }, {  s, -s, -s }, {  s, -s,  s }, { -s, -s,  s },
		{  s, -s, -s }, {  s,  s, -s }, {  s,  s,  s }, {  s, -s,  s },
		{ -s, -s, -s }, { -s, -s,  s }, { -s,  s,  s }, { -s,  s, -s }
	};

	f3d::f32 normals[][3] =
	{
		{ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f },
		{ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		{ 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
		{-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }
	};

	f3d::f32 texCoord[][2] =
	{
		{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
		{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f },
		{ 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
		{ 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f },
		{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f },
		{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
	};

	f3d::u32 cubeIndices[] =
	{
		 0,  3,  1,  1,  3,  2,	// front
		 4,  7,  5,  5,  7,  6,	// back
		 8, 11,  9,  9, 11, 10, // top
		12, 15, 13, 13, 15, 14, // bottom
		16, 19, 17, 17, 19, 18, // left
		20, 23, 21, 21, 23, 22  // right
	};


	SVertexData& data = CShape::getGeometryData();
	data.malloc(24, 36);
	
	memcpy(data.m_vertices.vertex.data(), vertex, sizeof(vertex));
	memcpy(data.m_normals.vertex.data(), normals, sizeof(normals));
	memcpy(data.m_texCoords.at(0).vertex.data(), texCoord, sizeof(texCoord));
	memcpy(data.m_indices.vertex.data(), cubeIndices, sizeof(cubeIndices));
	CShape::setGeometryDrawMode(EDrawMode::eTriangles);

	m_geometry->init();
}