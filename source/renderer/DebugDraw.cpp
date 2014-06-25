#include "DebugDraw.h"
#include "RenderJob.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::renderer;

RenderTechniquePtr CDebugDraw::s_tehnique = nullptr;

CDebugDraw::CDebugDraw(const GeometryPtr& geometry)
    : m_geometry(geometry)
    , m_vertex(nullptr)
    , m_fragment(nullptr)
    , m_flag(EDebugFlag::eDebugNone)
{
    s_tehnique = std::make_shared<CRenderTechnique>();

    for (u32 i = 0; i < EDebugDraw::eDrawCount; ++i)
    {
        m_objects[i]._arrayId = 0;
        m_objects[i]._drawMode = EDrawMode::eLines;
        m_objects[i]._vertex.id = 0;
    }
}

CDebugDraw::~CDebugDraw()
{
}

void CDebugDraw::init()
{
    initDrawShader();

    if (m_flag == EDebugFlag::eDebugAll)
    {
        CDebugDraw::initDrawNormalsData();
        initDraw(m_objects[EDebugDraw::eDrawNormals]);

        CDebugDraw::initDrawEdgeData();
        initDraw(m_objects[EDebugDraw::eDrawEdges]);

        return;
    }

    if (m_flag & EDebugFlag::eDebugNormals)
    {
        CDebugDraw::initDrawNormalsData();
        initDraw(m_objects[EDebugDraw::eDrawNormals]);
    }
    if (m_flag & EDebugFlag::eDebugEdges)
    {
        CDebugDraw::initDrawEdgeData();
        initDraw(m_objects[EDebugDraw::eDrawEdges]);
    }
    if (m_flag & EDebugFlag::eDebugLights)
    {
        int a = 0;
        //
    }
}

void CDebugDraw::refresh()
{
    if (m_flag == EDebugFlag::eDebugAll)
    {
        CDebugDraw::initDrawEdgeData();
        refreshDraw(m_objects[EDebugDraw::eDrawNormals]);

        CDebugDraw::initDrawEdgeData();
        refreshDraw(m_objects[EDebugDraw::eDrawEdges]);

        return;
    }

    if (m_flag & EDebugFlag::eDebugNormals)
    {
        CDebugDraw::initDrawNormalsData();
        refreshDraw(m_objects[EDebugDraw::eDrawNormals]);
    }
    if (m_flag & EDebugFlag::eDebugEdges)
    {
        CDebugDraw::initDrawEdgeData();
        refreshDraw(m_objects[EDebugDraw::eDrawEdges]);
    }
    if (m_flag & EDebugFlag::eDebugLights)
    {
        int a = 0;
        //
    }
}

void CDebugDraw::bind(const Vector4D& color)
{
    const RenderPassPtr& pass = s_tehnique->getRenderPass(0);
    const ShaderDataPtr& data = pass->getShaderData();
    data->setUniformVector4("color", color);

    CRenderJob::updateTransform(data);

    pass->bind();
}

void CDebugDraw::setDebugFlag(s32 flag)
{
    m_flag = flag;
}

void CDebugDraw::initDrawShader()
{
    RenderPassPtr pass = std::make_shared<CRenderPass>();

    const ShaderDataPtr& data = pass->getShaderData();
    data->addAttribute("positions", eAttributeVertex);
    data->addDefaultUniform("transform.projectionMatrix", eTypeMatrix4, eTransformProjectionMatrix);
    data->addDefaultUniform("transform.modelMatrix", eTypeMatrix4, eTransformModelMatrix);
    data->addDefaultUniform("transform.viewMatrix", eTypeMatrix4, eTransformViewMatrix);
    data->setUniformVector4("color", Vector4D(0.0f));
    pass->getRenderState()->setCullFace(false);
    pass->getRenderState()->setPolygonMode(ERenderPolygonMode::ePolyModeLine);

    ShaderProgramPtr program = RENDERER->makeSharedProgram(data);
    program->create(*m_vertex, *m_fragment);

    pass->setShaderProgram(program);

    s_tehnique->addRenderPass(pass);
}

void CDebugDraw::initDrawNormalsData()
{
    const SVertexData& data = m_geometry->getData();

    f32 s = 0.3f;
    SVertices<core::Vector3D>& normals = m_objects[eDrawNormals]._vertex;
    normals.vertex.clear();

    for (u32 i = 0; i < data._countVertices; ++i)
    {
        normals.vertex.push_back(data._vertices.vertex[i]);
        normals.vertex.push_back(Vector3D(
            (data._vertices.vertex[i].x + data._normals.vertex[i].x * s),
            (data._vertices.vertex[i].y + data._normals.vertex[i].y * s),
            (data._vertices.vertex[i].z + data._normals.vertex[i].z * s)
            ));
    }
}

void CDebugDraw::initDrawEdgeData()
{
    const SVertexData& data = m_geometry->getData();

    SVertices<core::Vector3D>& edges = m_objects[eDrawEdges]._vertex;
    SVertices<u32>& edgesIdx = m_objects[eDrawEdges]._index;
    m_objects[eDrawEdges]._drawMode = m_geometry->getDrawMode();

    edges.vertex.clear();
    edgesIdx.vertex.clear();

    edges.vertex.resize(data._countVertices);
    std::copy(data._vertices.vertex.begin(), data._vertices.vertex.end(), edges.vertex.begin());

    if (data._countIndices > 0)
    {
        edgesIdx.vertex.resize(data._countIndices);
        std::copy(data._indices.vertex.begin(), data._indices.vertex.end(), edgesIdx.vertex.begin());
    }
}
