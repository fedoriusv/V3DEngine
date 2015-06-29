#include "DebugLight.h"
#include "RenderJob.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::renderer;

RenderPassPtr CDebugLight::s_pass = nullptr;

CDebugLight::CDebugLight(const core::Vector3D& position, const scene::SLightData& data)
    : m_flag(EDebugLightFlag::eLightFlagNone)
    , m_position(position)
    , m_data(data)
{
    if (!s_pass)
    {
        s_pass = std::make_shared<CRenderPass>();
    }

   /* for (u32 i = 0; i < EDebugLight::eDebugLightCount; ++i)
    {
        m_objects[i]._arrayId = 0;
        m_objects[i]._vertex.id = 0;
    }*/
}

CDebugLight::~CDebugLight()
{
}

void CDebugLight::setDebugFlag(s32 flag)
{
    m_flag = flag;
}

void CDebugLight::init()
{
    if (m_flag == EDebugLightFlag::eLightFlagNone)
    {
        return;
    }

    CDebugLight::initShader();

    if (m_flag & EDebugLightFlag::eLightFlagPosition)
    {
        CDebugLight::initLightPosition();
        initDraw(m_objects[EDebugLight::eDebugLightPosition]);
    }
    if (m_flag & EDebugLightFlag::eLightFlagDirection)
    {
       //TODO:
    }
    if (m_flag & EDebugLightFlag::eLightFlagRadius)
    {
        //TODO:
    }
}

void CDebugLight::bind()
{
   /* const ShaderDataPtr& data = s_pass->getShaderData();

    data->addUniformVector4("color", Vector4D(0.0f, 0.0f, 1.0f, 1.0f));
    data->addUniformMatrix4("transform.projectionMatrix", RENDERER->m_projectionMatrix);
    data->addUniformMatrix4("transform.viewMatrix", RENDERER->m_viewMatrix);
    core::Matrix4D modelMatrix;
    modelMatrix.setTranslation(m_position);
    data->addUniformMatrix4("transform.modelMatrix", modelMatrix);

    s_pass->bind();

    RENDERER->checkForErrors("CDebugLight Bind Error");*/
}

void CDebugLight::initShader()
{
    /*const ShaderDataPtr& data = s_pass->getShaderData();

    data->addAttribute("positions", CShaderAttribute::eAttributeVertex);
    data->addDefaultUniform("transform.projectionMatrix", eTransformProjectionMatrix);
    data->addDefaultUniform("transform.modelMatrix", eTransformModelMatrix);
    data->addDefaultUniform("transform.viewMatrix", eTransformViewMatrix);
    data->addUniformVector4("color", Vector4D(0.0f));

    s_pass->getRenderState()->setCullFace(false);
    s_pass->getRenderState()->setPolygonMode(EPolygonMode::ePolyModeLine);

    ShaderProgramPtr program = RENDERER->makeSharedProgram(data);
    program->create(*m_vertex, *m_fragment);

    s_pass->setShaderProgram(program);*/
}

void CDebugLight::initLightPosition()
{
    const f32 s = 0.1f;
    const f32 vertex[][3] =
    {
        {-s, s, -s},  {s, s, -s },
        { s, s, -s},  {s, -s, -s},
        { s, -s, -s}, {-s,-s, -s},
        {-s, -s, -s}, {-s, s, -s},

        {-s, s, s }, { s, s, s  },
        { s, s, s }, { s, -s, s },
        { s, -s, s}, {-s, -s, s },
        {-s, -s, s}, {-s, s, s  },

        {-s, s, s }, {-s, s, -s },
        { s, s, s }, { s, s, -s },
        { s, -s, s}, { s, -s, -s},
        {-s, -s, s}, {-s, -s, -s}
    };

   /* SVertices<core::Vector3D>& light = m_objects[EDebugLight::eDebugLightPosition]._vertex;

    light.vertex.clear();
    light.vertex.resize(24);

    for (u32 i = 0; i < 24; ++i)
    {
        light.vertex[i] = vertex[i];
    }*/
}
