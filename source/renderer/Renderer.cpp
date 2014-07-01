#include "renderer/Renderer.h"
#include "context/DriverContext.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderer::CRenderer(const DriverContextPtr& context)
    : m_context(context)
    , m_backColor(core::Vector3D(0))
    , m_projectionMatrix(core::Matrix4D())
    , m_viewMatrix(core::Matrix4D())
    , m_viewPosition(core::Vector3D(0.0f))
    , m_updateCamera(true)
#ifdef _DEBUG
    , m_debugMode(false)
#endif
{
    m_viewportSize = context->getWindow()->getSize();
}

CRenderer::~CRenderer()
{
    m_lightList.clear();
}

void CRenderer::addLight(scene::CLight* lights)
{
    m_lightList.push_back(lights);
}

void CRenderer::setBackColor(const core::Vector3D& color)
{
	m_backColor = color;
}

const core::Vector3D& CRenderer::getBackColor() const
{
	return m_backColor;
}

void CRenderer::checkForErrors(const std::string& location)
{
    m_context->checkForErrors(location);
}

void CRenderer::reshape(u32 width, u32 height)
{
    if (height == 0)
    {
        height = 1;
    }
    m_viewportSize.width = width;
    m_viewportSize.height = height;

    f32 aspectRatio = (f32)m_viewportSize.width / (f32)m_viewportSize.height;
    m_projectionMatrix = core::buildProjectionMatrixPerspective(45.0f, aspectRatio, 0.5f, 10.0f);

}

void CRenderer::updateCamera(const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up)
{
    m_viewMatrix = core::buildLookAtMatrix(pos, target, up);
    m_viewMatrix.makeTransposed();
    m_viewPosition = pos;

}

const core::Dimension2D& CRenderer::getViewportSize() const
{
    return m_viewportSize;
}

void CRenderer::draw(const RenderJobPtr& job)
{
    const MaterialPtr& material = job->getMaterial();
    const GeometryPtr& geomerty = job->getGeometry();
    const u32 passCount = material->getRenderTechique()->getRenderPassCount();
    const core::Matrix4D& transform = job->getTransform();

    for (u32 i = 0; i < passCount; ++i)
    {
        const RenderPassPtr& pass = material->getRenderTechique()->getRenderPass(i);

        if (!checkLOD(transform, pass))
        {
            continue;
        }

        pass->bind();

        updateTransform(transform, pass);
        //updateMaterial(material, data);
        //updateLight(job->getTransform(), data);

        for (u32 layer = 0; layer < material->getTextureCount(); ++layer)
        {
            const TexturePtr& texture = material->getTexture(layer);

            //TODO: check for sampler data
            if (texture->isEnable())
            {
                texture->bind(layer);
            }
            else
            {
                texture->unbind(layer);
            }
        }

        geomerty->draw();
    }
}

#ifdef _DEBUG
void CRenderer::setDebugMode(bool active)
{
    m_debugMode = active;
}

bool CRenderer::isDebugMode() const
{
    return m_debugMode;
}
#endif

void CRenderer::updateTransform(const core::Matrix4D& transform, const RenderPassPtr& pass)
{
    const ShaderDataPtr& data = pass->getShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();
    const u32 shader = program->getShaderID();

    if (data->isExistUniform(CShaderDefaultUniform::eTransformProjectionMatrix))
    {
        program->setUniformMatrix4(shader, "transform.projectionMatrix", m_projectionMatrix);
    }

    if (data->isExistUniform(CShaderDefaultUniform::eTransformModelMatrix))
    {
        core::Matrix4D modelMatrix(transform);
        modelMatrix.makeTransposed();

        program->setUniformMatrix4(shader, "transform.modelMatrix", modelMatrix);
    }

    if (data->isExistUniform(CShaderDefaultUniform::eTransformViewMatrix))
    {
        program->setUniformMatrix4(shader, "transform.viewMatrix", m_viewMatrix);
    }

    if (data->isExistUniform(CShaderDefaultUniform::eTransformViewPosition))
    {
        program->setUniformVector3(shader, "transform.viewPosition", m_viewPosition);
    }

    if (data->isExistUniform(CShaderDefaultUniform::eTransformNormalMatrix))
    {
        core::Matrix4D normalMatrix;
        transform.getInverse(normalMatrix);

        program->setUniformMatrix4(shader, "transform.normalMatrix", normalMatrix);
    }
}

bool CRenderer::checkLOD(const core::Matrix4D& transform, const RenderPassPtr& pass)
{
    const RenderLODPtr& lod = pass->getRenderLOD();

    if (lod->getGeometryDistance() < (transform.getTranslation() - m_viewPosition).length())
    {
        return false;
    }

    return true;
}
