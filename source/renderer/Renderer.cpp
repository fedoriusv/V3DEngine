#include "Renderer.h"
#include "Material.h"
#include "context/DriverContext.h"
#include "scene/Light.h"
#include "scene/Camera.h"
#include "utils/Timer.h"

namespace v3d
{
namespace renderer
{

using namespace core;
using namespace scene;

CRenderer::CRenderer(const DriverContextPtr& context)
    : m_context(context)
    , m_frameIndex(0U)

    , m_defaultRenderTarget(nullptr)
    , m_currentRenderTarget(nullptr)
#ifdef _DEBUG
    , m_debugMode(false)
#endif
    , m_camera(nullptr)
{
}

CRenderer::~CRenderer()
{
    m_context->destroyContext();
    m_lightList.clear();
}

const DriverContextPtr& CRenderer::getContext() const
{
    return m_context;
}

void CRenderer::addLight(scene::CLight* lights)
{
    m_lightList.push_back(lights);
}

void CRenderer::checkForErrors(const std::string& location)
{
    m_context->checkForErrors(location);
}

void CRenderer::updateCamera(CCamera* camera)
{
    m_camera = camera;
}

const core::Rect32& CRenderer::getViewportSize() const
{
    ASSERT(m_currentRenderTarget, "CRenderer: Current Render Target in null");
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
    MaterialPtr& material = job->getMaterial();
    const GeometryPtr& geometry = job->getGeometry();
    const core::Matrix4D& transform = job->getTransform();
    CRenderTechnique* techique = material->getRenderTechique();

    u32 targetIndex = job->getRenderTarget();
    u32 passCount = techique->getRenderPassCount();
    for (PassIndexIterConst passIter = job->renderPassIndexBegin(); passIter != job->renderPassIndexEnd(); ++passIter)
    {
        if ((*passIter) < 0 || (*passIter) > passCount)
        {
            continue;
        }

        const RenderPassPtr& pass = techique->getRenderPass((*passIter));
        pass->bind(targetIndex);

        techique->setCurrentPass((*passIter));

        CRenderer::updateTransform(transform, pass);
        CRenderer::updateMaterial(material, pass);
        CRenderer::updateTexture(material, pass);
        CRenderer::updateLight(transform, pass);
        CRenderer::updateAdvanced(pass);

        //Draw Geometry
        if (geometry->updated())
        {
            geometry->refresh();
        }
        geometry->draw();

        pass->unbind(targetIndex);
        resetTextures();
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
#endif //_DEBUG

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

                if (!m_camera)
                {
                    ASSERT(false, "Camera doesn't exist");
                    break;
                }
                program->applyUniformMatrix4(id, m_camera->getProjectionMatrix());

                break;

            case CShaderUniform::eTransformModelMatrix:
            {
                core::Matrix4D modelMatrix(transform);
                modelMatrix.makeTransposed();

                program->applyUniformMatrix4(id, modelMatrix);
            }
                break;

            case CShaderUniform::eTransformViewMatrix:

                if (!m_camera)
                {
                    ASSERT(false, "Camera doesn't exist");
                    break;
                }
                program->applyUniformMatrix4(id, m_camera->getViewMatrix());

                break;

            case CShaderUniform::eTransformViewPosition:

                if (!m_camera)
                {
                    ASSERT(false, "Camera doesn't exist");
                    break;
                }
                program->applyUniformVector3(id, m_camera->getPosition());
                break;

            case CShaderUniform::eTransformViewUpVector:

                if (!m_camera)
                {
                    ASSERT(false, "Camera doesn't exist");
                    break;
                }
                program->applyUniformVector3(id, m_camera->getUpVector());
                break;

            case CShaderUniform::eTransformNormalMatrix:
            {
                core::Matrix4D normalMatrix;
                transform.getInverse(normalMatrix);

                program->applyUniformMatrix4(id, normalMatrix);
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
        s32 id = uniform.second->getID();
        switch (type)
        {
        case CShaderUniform::eMaterialAmbient:
    
            program->applyUniformVector4(id, material->getAmbientColor());
            break;

        case CShaderUniform::eMaterialDiffuse:

            program->applyUniformVector4(id, material->getDiffuseColor());
            break;

        case CShaderUniform::eMaterialSpecular:

            program->applyUniformVector4(id, material->getSpecularColor());
            break;

        case CShaderUniform::eMaterialEmission:

            program->applyUniformVector4(id, material->getEmissionColor());
            break;

        case CShaderUniform::eMaterialShininess:

            program->applyUniformFloat(id, material->getShininess());
            break;

        case CShaderUniform::eMaterialTransparency:

            program->applyUniformFloat(id, material->getTransparency());
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

    if (lights.empty())
    {
        return;
    }

    const ShaderDataPtr& data = pass->getDefaultShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();

    for (std::vector<scene::CLight*>::iterator light = lights.begin(); light < lights.end(); ++light)
    {
        const UniformList& list = data->getUniformList();
        for (auto& uniform : list)
        {
            const CShaderUniform::EUniformData type = uniform.second->getData();
            s32 id = uniform.second->getID();
            switch (type)
            {
            case CShaderUniform::eLightsCount:

                program->applyUniformInt(id, (s32)lights.size());
                break;

            case CShaderUniform::eLightPosition:

                program->applyUniformVector4(id, Vector4D((*light)->getPosition(), 0.0f));
                break;

            case CShaderUniform::eLightAmbient:

                program->applyUniformVector4(id, (*light)->getAmbient());
                break;

            case CShaderUniform::eLightDiffuse:

                program->applyUniformVector4(id, (*light)->getDiffuse());
                break;

            case CShaderUniform::eLightSpecular:

                program->applyUniformVector4(id, (*light)->getSpecular());
                break;

            case CShaderUniform::eLightDirection:

                program->applyUniformVector3(id, (*light)->getDirection());
                break;

            case CShaderUniform::eLightAttenuation:

                program->applyUniformVector3(id, (*light)->getAttenuation());
                break;

            case CShaderUniform::eLightRadius:

                program->applyUniformFloat(id, (*light)->getRadius());
                break;

            default:
                break;
            }
        }
    }
}

void CRenderer::updateTexture(MaterialPtr& material, const RenderPassPtr& pass)
{
    const ShaderDataPtr& defaultData = pass->getDefaultShaderData();
    const SamplerList& samplerList = defaultData->getSamplerList();

    bool isUserSamplers = (material->getTextureCount() > 0) ? true : false;
    if (isUserSamplers)
    {
        u32 layersCount = material->getTextureCount();
        u32 samplerID = 0;
        for (u32 layer = 0; layer < layersCount; ++layer)
        {
            samplerID = layer;
            CTexture* texture = material->getTexture(layer);
            if (texture->isEnable())
            {
                texture->bind(layer, samplerID);
            }
            else
            {
                texture->unbind(layer, samplerID);
            }
        }
    }
    else if (!samplerList.empty())
    {
        u32 layersCount = 0;
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
                {
                    isUserSamplers = true;
                    break;
                }
                break;

                case CShaderSampler::eTextureSampler:
                case CShaderSampler::eRenderTargetSampler:
                {
                    CTexture* texture = sampler->getTexture();
                    if (texture)
                    {
                        if (texture->isEnable())
                        {
                            texture->bind(layersCount, sampler->getID());
                        }
                        else
                        {
                            texture->unbind(layersCount, sampler->getID());
                        }
                        ++layersCount;
                    }
                }
                break;
            }
        }
    }
    else
    {
        resetTextures();
    }
}

void CRenderer::updateAdvanced(const RenderPassPtr & pass)
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
        case CShaderUniform::eCurrentTime:
            {
                u32 time = (u32)utils::CTimer::getCurrentTime();
                program->applyUniformInt(id, time);
            }
            break;

        default:
            break;
        }
    }
}

} //namespace renderer
} //namespace v3d
