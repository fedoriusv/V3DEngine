#include "renderer/Renderer.h"
#include "context/DriverContext.h"
#include "scene/Light.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderer::CRenderer(const DriverContextPtr& context)
    : m_context(context)
    , m_projectionMatrix(core::Matrix4D())
    , m_orthoMatrix(core::Matrix4D())
    , m_viewMatrix(core::Matrix4D())
    , m_viewPosition(core::Vector3D(0.0f))

    , m_frameIndex(0U)

    , m_maxTextureUnits(0)
    , m_maxAnisotropy(0.0f)

    , m_updateCamera(true)

    , m_defaultRenderTarget(nullptr)
    , m_currentRenderTarget(nullptr)
#ifdef _DEBUG
    , m_debugMode(false)
#endif
{
}

CRenderer::~CRenderer()
{
    m_lightList.clear();
}

void CRenderer::addLight(scene::CLight* lights)
{
    m_lightList.push_back(lights);
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
    f32 aspectRatio = (f32)width / (f32)height;
    m_projectionMatrix = core::buildProjectionMatrixPerspective(45.0f, aspectRatio, 0.5f, 100.0f);

    m_orthoMatrix = core::buildProjectionMatrixOrtho(0.0f, (f32)width, 0.0f, (f32)height, - 1.0f, 1.0f);
    //m_orthoMatrix.makeTransposed();
}

void CRenderer::updateCamera(const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up)
{
    m_viewMatrix = core::buildLookAtMatrix(pos, target, up);
    m_viewMatrix.makeTransposed();
    m_viewPosition = pos;
}

const core::Rect& CRenderer::getViewportSize() const
{
    ASSERT(m_currentRenderTarget && "Current Render Target in null");
    if (m_currentRenderTarget)
    {
        return m_currentRenderTarget->getViewport();
    }
    else
    {
        return m_defaultRenderTarget->getViewport();
    }
}

Vector2D CRenderer::convertPosScreenToCanvas(const Point2D& pos)
{
    const core::Dimension2D& size = m_context->getWindowSize();
    
    Vector2D newpos(0.0f, 0.0f);
    f32 koefW = pos.x / (f32)size.width;
    f32 koefH = (size.height - pos.y) / (f32)size.height;
    newpos.x = -1.0f + 2.0f * koefW;
    newpos.y = -1.0f + 2.0f * koefH;

    return newpos;
}

Point2D CRenderer::convertPosCanvasToScreen(const Vector2D& pos)
{
    const core::Dimension2D& size = m_context->getWindowSize();

    Point2D newpos(0, 0);
    f32 koefW = (pos.x + 1.0f) / 2.0f;
    f32 koefH = (pos.y + 1.0f) / 2.0f;
    newpos.x = (u32)(koefW * size.width);
    newpos.y = size.height - (u32)(koefH * size.height);

    return newpos;
}

u32 CRenderer::getFrameIndex() const
{
    return m_frameIndex;
}

const RenderTargetPtr& CRenderer::getDefaultRenderTarget() const
{
    return m_defaultRenderTarget;
}

const RenderTargetPtr& CRenderer::getCurrentRenderTarget() const
{
    if (m_currentRenderTarget)
    {
        return m_currentRenderTarget;
    }
    else
    {
        return m_defaultRenderTarget;
    }
}

void CRenderer::setCurrentRenderTarget(const RenderTargetPtr& target)
{
    m_currentRenderTarget = target;
}

void CRenderer::draw(const RenderJobPtr& job)
{
    const MaterialPtr& material = job->getMaterial();
    const GeometryPtr& geometry = job->getGeometry();
    const core::Matrix4D& transform = job->getTransform();
    const u32 passCount = material->getRenderTechique()->getRenderPassCount();

    for (u32 i = 0; i < passCount; ++i)
    {
        const RenderPassPtr& pass = material->getRenderTechique()->getRenderPass(i);

        pass->bind();

        CRenderer::updateTransform(transform, pass);
        CRenderer::updateMaterial(material, pass);
        CRenderer::updateTexture(material, pass);
        CRenderer::updateLight(transform, pass);

        //Bind Texture
       

        //Draw Geometry
        geometry->draw();

        pass->unbind();
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
    const ShaderDataPtr& data = pass->getDefaultShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();

    const UniformList& list = data->getUniformList();
    for (auto& uniform : list)
    {
        const CShaderUniform::EUniformData type = uniform.second->getData();
        s32 id = uniform.second->getID();
        switch (type)
        {
            case CShaderUniform::eTransformProjectionMatrix:

                program->setUniformMatrix4(id, m_projectionMatrix);
                break;

            case CShaderUniform::eTransformOrthoMatrix:

                program->setUniformMatrix4(id, m_orthoMatrix);
                break;

            case CShaderUniform::eTransformModelMatrix:
            {
                core::Matrix4D modelMatrix(transform);
                modelMatrix.makeTransposed();

                program->setUniformMatrix4(id, modelMatrix);
            }
                break;

            case CShaderUniform::eTransformViewMatrix:

                program->setUniformMatrix4(id, m_viewMatrix);
                break;

            case CShaderUniform::eTransformViewPosition:

                program->setUniformVector3(id, m_viewPosition);
                break;

            case CShaderUniform::eTransformNormalMatrix:
            {
                core::Matrix4D normalMatrix;
                transform.getInverse(normalMatrix);

                program->setUniformMatrix4(id, normalMatrix);
            }
                break;

            default:
                break;
        }
    }
}

void CRenderer::updateMaterial(const MaterialPtr& material, const RenderPassPtr& pass)
{
    const ShaderDataPtr& data = pass->getDefaultShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();

    const UniformList& list = data->getUniformList();
    for (auto& uniform : list)
    {
        const CShaderUniform::EUniformData type = uniform.second->getData();
        const s32 id = uniform.second->getID();
        switch (type)
        {
        case CShaderUniform::eMaterialAmbient:
    
            program->setUniformVector4(id, material->getAmbientColor());
            break;

        case CShaderUniform::eMaterialDiffuse:

            program->setUniformVector4(id, material->getDiffuseColor());
            break;

        case CShaderUniform::eMaterialSpecular:

            program->setUniformVector4(id, material->getSpecularColor());
            break;

        case CShaderUniform::eMaterialEmission:

            program->setUniformVector4(id, material->getEmissionColor());
            break;

        case CShaderUniform::eMaterialShininess:

            program->setUniformFloat(id, material->getShininess());
            break;

        case CShaderUniform::eMaterialTransparency:

            program->setUniformFloat(id, material->getTransparency());
            break;

        default:
            break;
        }
    }
}

void CRenderer::updateLight(const core::Matrix4D& transform, const RenderPassPtr& pass)
{
    if (m_lightList.empty())
    {
        return;
    }

    std::vector<scene::CLight*> lights = m_lightList;
    const Vector3D& pos = transform.getTranslation();

    std::remove_if(lights.begin(), lights.end(), [&pos](scene::CLight* light) -> bool
    {
        if (!light->isVisible())
        {
            return true;
        }

        f32 distance = (light->getPosition() - pos).length();
        if (light->getRadius() < distance)
        {
            return true;
        }

        return true;
    });

    const ShaderDataPtr& data = pass->getDefaultShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();

    if (lights.empty())
    {
        return;
    }

    for (std::vector<scene::CLight*>::iterator light = lights.begin(); light < lights.end(); ++light)
    {
        const UniformList& list = data->getUniformList();
        for (auto& uniform : list)
        {
            const CShaderUniform::EUniformData type = uniform.second->getData();
            const s32 id = uniform.second->getID();
            switch (type)
            {
            case CShaderUniform::eLightsCount:

                program->setUniformInt(id, (s32)lights.size());
                break;

            case CShaderUniform::eLightPosition:

                program->setUniformVector4(id, Vector4D((*light)->getPosition(), 0.0f));
                break;

            case CShaderUniform::eLightAmbient:

                program->setUniformVector4(id, (*light)->getAmbient());
                break;

            case CShaderUniform::eLightDiffuse:

                program->setUniformVector4(id, (*light)->getDiffuse());
                break;

            case CShaderUniform::eLightSpecular:

                program->setUniformVector4(id, (*light)->getSpecular());
                break;

            case CShaderUniform::eLightDirection:

                program->setUniformVector3(id, (*light)->getDirection());
                break;

            case CShaderUniform::eLightAttenuation:

                program->setUniformVector3(id, (*light)->getAttenuation());
                break;

            case CShaderUniform::eLightRadius:

                program->setUniformFloat(id, (*light)->getRadius());
                break;

            default:
                break;
            }
        }
    }
}

void CRenderer::updateTexture(const MaterialPtr& material, const RenderPassPtr& pass)
{
    const ShaderDataPtr& defaultData = pass->getDefaultShaderData();

    const SamplerList& samplerList = defaultData->getSamplerList();
    for (auto& sampler : samplerList)
    {
        if (sampler->getID() < 0)
        {
            continue;
        }

        CShaderSampler::ESamplerType type = sampler->getType();
        switch (type)
        {
        case CShaderSampler::eUserSampler:
            break;

        case CShaderSampler::eTextureSampler:
            break;

        case CShaderSampler::eRenderTargetSampler:
            break;
        }
    }

    u32 layersCount = material->getTextureCount();
    for (u32 layer = 0; layer < layersCount; ++layer)
    {
        const TexturePtr& texture = material->getTexture(layer);
        if (pass->getDefaultShaderData()->getSamplerList().size() >= layer)
        {
            if (texture->isEnable())
            {
                texture->bind(layer);
            }
            else
            {
                texture->unbind(layer);
            }
        }
    }

    if (layersCount == 0)
    {
        resetTexture();
    }
}
