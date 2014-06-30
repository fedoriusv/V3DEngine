#include "DebugGeometry.h"
#include "RenderJob.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::renderer;

RenderPassPtr CDebugGeometry::s_pass = nullptr;

CDebugGeometry::CDebugGeometry(const GeometryPtr& geometry)
    : m_geometry(geometry)
    , m_vertex(nullptr)
    , m_fragment(nullptr)
    , m_flag(EDebugGeometryFlag::eGeometryFlagNone)
{
    if (!s_pass)
    {
        s_pass = std::make_shared<CRenderPass>();
    }

   /* for (u32 i = 0; i < EDebugGeometry::eGeometryCount; ++i)
    {
        m_objects[i]._arrayId = 0;
        m_objects[i]._drawMode = EDrawMode::eLines;
        m_objects[i]._vertex.id = 0;
    }*/
}

CDebugGeometry::~CDebugGeometry()
{
}

void CDebugGeometry::init()
{
    if (m_flag == EDebugGeometryFlag::eGeometryFlagNone)
    {
        return;
    }

    CDebugGeometry::initShader();

    if (m_flag & EDebugGeometryFlag::eGeometryFlagNormals)
    {

        CDebugGeometry::initDrawNormalsData();
        initDraw(m_objects[EDebugGeometry::eGeometryNormals]);
    }
    if (m_flag & EDebugGeometryFlag::eGeometryFlagEdges)
    {
        CDebugGeometry::initDrawEdgeData();
        initDraw(m_objects[EDebugGeometry::eGeometryEdges]);
    }
}

void CDebugGeometry::refresh()
{
    if (m_flag & EDebugGeometryFlag::eGeometryFlagNormals)
    {
        CDebugGeometry::initDrawNormalsData();
        refreshDraw(m_objects[EDebugGeometry::eGeometryNormals]);
    }
    if (m_flag & EDebugGeometryFlag::eGeometryFlagEdges)
    {
        CDebugGeometry::initDrawEdgeData();
        refreshDraw(m_objects[EDebugGeometry::eGeometryEdges]);
    }
}

void CDebugGeometry::bind()
{
    const ShaderDataPtr& data = s_pass->getShaderData();
    //CRenderJob::updateTransform(data);

    s_pass->bind();
}

void CDebugGeometry::setDebugFlag(s32 flag)
{
    m_flag = flag;
}

void CDebugGeometry::initShader()
{
    const ShaderDataPtr& data = s_pass->getShaderData();

    data->addAttribute("positions", CShaderAttribute::eAttributeVertex);
    data->addDefaultUniform("transform.projectionMatrix", CShaderDefaultUniform::eTransformProjectionMatrix);
    data->addDefaultUniform("transform.modelMatrix", CShaderDefaultUniform::eTransformModelMatrix);
    data->addDefaultUniform("transform.viewMatrix", CShaderDefaultUniform::eTransformViewMatrix);
    data->setUniformVector4("color", Vector4D(0.0f));

    s_pass->getRenderState()->setCullFace(false);
    s_pass->getRenderState()->setPolygonMode(CRenderState::ePolyModeLine);

    ShaderProgramPtr program = RENDERER->makeSharedProgram(data);
    program->create(*m_vertex, *m_fragment);

    s_pass->setShaderProgram(program);
}

void CDebugGeometry::initDrawNormalsData()
{
    if (!m_geometry)
    {
        ASSERT(false || "CDebugDraw Empty Geometry");
        return;
    }

    const SVertexData& data = m_geometry->getData();

    f32 s = 0.3f;
   /* SVertices<core::Vector3D>& normals = m_objects[EDebugGeometry::eGeometryNormals]._vertex;
    normals.vertex.clear();

    for (u32 i = 0; i < data._countVertices; ++i)
    {
        normals.vertex.push_back(data._vertices.vertex[i]);
        normals.vertex.push_back(Vector3D(
            (data._vertices.vertex[i].x + data._normals.vertex[i].x * s),
            (data._vertices.vertex[i].y + data._normals.vertex[i].y * s),
            (data._vertices.vertex[i].z + data._normals.vertex[i].z * s)
            ));
    }*/
}

void CDebugGeometry::initDrawEdgeData()
{
    if (!m_geometry)
    {
        ASSERT(false || "CDebugDraw Empty Geometry");
        return;
    }

    const SVertexData& data = m_geometry->getData();
    
    /*SVertices<core::Vector3D>& edges = m_objects[EDebugGeometry::eGeometryEdges]._vertex;
    SVertices<u32>& edgesIdx = m_objects[EDebugGeometry::eGeometryEdges]._index;
    m_objects[EDebugGeometry::eGeometryEdges]._drawMode = m_geometry->getDrawMode();

    edges.vertex.clear();
    edgesIdx.vertex.clear();

    edges.vertex.resize(data._countVertices);
    std::copy(data._vertices.vertex.begin(), data._vertices.vertex.end(), edges.vertex.begin());

    if (data._countIndices > 0)
    {
        edgesIdx.vertex.resize(data._countIndices);
        std::copy(data._indices.vertex.begin(), data._indices.vertex.end(), edgesIdx.vertex.begin());
    }*/
}
