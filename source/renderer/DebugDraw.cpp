#include "DebugDraw.h"
#include "RenderJob.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::renderer;

RenderTechniquePtr CDebugDraw::m_tehnique = nullptr;

CDebugDraw::CDebugDraw(const GeometryPtr& geometry)
    : m_geometry(geometry)
    , m_vertex(nullptr)
    , m_fragment(nullptr)
    , m_flag(EDebugDraw::eDrawNone)
{
    m_tehnique = std::make_shared<CRenderTechnique>();
}

CDebugDraw::~CDebugDraw()
{
}

void CDebugDraw::init()
{
    switch (m_flag)
    {
    case EDebugDraw::eDrawNormals:

        initDrawNormalsShader();
        initDrawNormalsData();
        break;

    case EDebugDraw::eDrawAll:

        initDrawNormalsShader();
        initDrawNormalsData();
        break;

    case EDebugDraw::eDrawNone:
    default:
        break;
    }
}

void CDebugDraw::bind()
{
    switch (m_flag)
    {
        case EDebugDraw::eDrawNormals:
        {
            const RenderPassPtr& pass = m_tehnique->getRenderPass(0);
            CRenderJob::updateTransform(pass->getShaderData());

            pass->bind();
        }
        break;

        case EDebugDraw::eDrawAll:
        {
            for (u32 i = 0; i < m_tehnique->getRenderPassCount(); ++i)
            {
                const RenderPassPtr& pass = m_tehnique->getRenderPass(i);
                CRenderJob::updateTransform(pass->getShaderData());

                pass->bind();

                //TODO: need draw here
            }
        }
        break;

    case EDebugDraw::eDrawNone:
    default:
        break;
    }
}

void CDebugDraw::setDebugFlag(EDebugDraw flag)
{
    m_flag = flag;
}

void CDebugDraw::initDrawNormalsShader()
{
    RenderPassPtr pass = std::make_shared<CRenderPass>();

    ShaderDataPtr data = pass->getShaderData();
    data->addAttribute("positions", eAttributeVertex);
    data->addDefaultUniform("transform.projectionMatrix", eTypeMatrix4, eTransformProjectionMatrix);
    data->addDefaultUniform("transform.modelMatrix", eTypeMatrix4, eTransformModelMatrix);
    data->addDefaultUniform("transform.viewMatrix", eTypeMatrix4, eTransformViewMatrix);

    ShaderProgramPtr program = RENDERER->makeSharedProgram(data);
    program->create(*m_vertex, *m_fragment);

    pass->setShaderProgram(program);

    m_tehnique->addRenderPass(pass);
}

void CDebugDraw::initDrawNormalsData()
{
    const SVertexData& data = m_geometry->getData();

    f32 s = 0.3f;
    m_normals.vertex.clear();
    for (u32 i = 0; i < data._countVertices; ++i)
    {
        m_normals.vertex.push_back(data._vertices.vertex[i]);
        m_normals.vertex.push_back(Vector3D(
            (data._vertices.vertex[i].x + data._normals.vertex[i].x * s),
            (data._vertices.vertex[i].y + data._normals.vertex[i].y * s),
            (data._vertices.vertex[i].z + data._normals.vertex[i].z * s)
            ));
    }
}