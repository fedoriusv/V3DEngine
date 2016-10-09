#include "Renderer.h"
#include "RenderThread.h"
#include "Material.h"
#include "RenderTechnique.h"
#include "scene/Light.h"
#include "scene/Camera.h"
#include "utils/Timer.h"

namespace v3d
{
namespace renderer
{

using namespace core;
using namespace scene;

IRenderer::IRenderer(const ContextPtr context, bool isThreaded)
    : m_context(context)
    , m_frameIndex(0U)

    , m_renderThread(nullptr)
    , m_isThreaded(isThreaded)

    , m_defaultRenderTarget(nullptr)
    , m_currentRenderTarget(nullptr)
#ifdef _DEBUG
    , m_debugMode(false)
#endif
    , m_camera(nullptr)
{
}

IRenderer::~IRenderer()
{
    m_lightList.clear();
}

bool IRenderer::create()
{
    if (m_isThreaded)
    {
        m_renderThread = new RenderThread(shared_from_this());
    }

    return true;
}

void IRenderer::init()
{
    if (m_isThreaded)
    {
        m_renderThread->init();
    }
    else
    {
        this->immediateInit();
    }
}

void IRenderer::beginFrame()
{
    if (m_isThreaded)
    {
        m_renderThread->init();
    }
    else
    {
        this->immediaterBeginFrame();
    }
}

void IRenderer::endFrame()
{
    if (m_isThreaded)
    {
        m_renderThread->endFrame();
    }
    else
    {
        this->immediateEndFrame();
    }
}

void IRenderer::presentFrame()
{
    if (m_isThreaded)
    {
        m_renderThread->presentFrame();
    }
    else
    {
        this->immediatePresentFrame();
    }
}

void IRenderer::draw()
{
    if (m_isThreaded)
    {
        m_renderThread->draw();
    }
    else
    {
        this->immediateDraw();
    }
}

void IRenderer::addLight(scene::CLight* lights)
{
    m_lightList.push_back(lights);
}

void IRenderer::checkForErrors(const std::string& location)
{
    m_context.lock()->checkForErrors(location);
}

void IRenderer::updateCamera(CCamera* camera)
{
    m_camera = camera;
}

const core::Rect32& IRenderer::getViewportSize() const
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

Vector2D IRenderer::convertPosScreenToCanvas(const Point2D& pos)
{
    if (m_context.expired())
    {
        return Vector2D();
    }

    const core::Dimension2D& size = m_context.lock()->getWindowSize();
    
    Vector2D newpos(0.0f, 0.0f);
    f32 koefW = pos.x / (f32)size.width;
    f32 koefH = (size.height - pos.y) / (f32)size.height;
    newpos.x = -1.0f + 2.0f * koefW;
    newpos.y = -1.0f + 2.0f * koefH;

    return newpos;
}

Point2D IRenderer::convertPosCanvasToScreen(const Vector2D& pos)
{
    if (m_context.expired())
    {
        return Point2D();
    }

    const core::Dimension2D& size = m_context.lock()->getWindowSize();

    Point2D newpos(0, 0);
    f32 koefW = (pos.x + 1.0f) / 2.0f;
    f32 koefH = (pos.y + 1.0f) / 2.0f;
    newpos.x = (u32)(koefW * size.width);
    newpos.y = size.height - (u32)(koefH * size.height);

    return newpos;
}

u32 IRenderer::getFrameIndex() const
{
    return m_frameIndex;
}

const RenderTargetPtr& IRenderer::getDefaultRenderTarget() const
{
    return m_defaultRenderTarget;
}

const RenderTargetPtr& IRenderer::getCurrentRenderTarget() const
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

void IRenderer::setCurrentRenderTarget(const RenderTargetPtr& target)
{
    m_currentRenderTarget = target;
}

void IRenderer::draw(const RenderJobPtr& job)
{
    MaterialPtr& material = job->getMaterial();
    const GeometryPtr& geometry = job->getGeometry();
    const core::Matrix4D& transform = job->getTransform();
    CRenderTechnique* techique = job->getRenderTechique();

    u32 targetIndex = job->getRenderTarget();
    u32 passCount = techique->getRenderPassCount();
    for (PassIndexIterConst passIter = job->renderPassIndexBegin(); passIter != job->renderPassIndexEnd(); ++passIter)
    {
        if ((*passIter) < 0 || (*passIter) > passCount)
        {
            continue;
        }

        const RenderPassPtr pass = techique->getRenderPass((*passIter));
        pass->bind(targetIndex);

        techique->setCurrentPass((*passIter));

        IRenderer::updateTransform(transform, pass);
        IRenderer::updateMaterial(material, pass);
        IRenderer::updateTexture(material, pass);
        IRenderer::updateLight(transform, pass);
        IRenderer::updateAdvanced(pass);

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
void IRenderer::setDebugMode(bool active)
{
    m_debugMode = active;
}

bool IRenderer::isDebugMode() const
{
    return m_debugMode;
}
#endif //_DEBUG

void IRenderer::updateTransform(const core::Matrix4D& transform, const RenderPassPtr& pass)
{
    const ShaderDataPtr data = pass->getDefaultShaderData();
    const ShaderProgramPtr program = pass->getShaderProgram();

    const UniformList& list = data->getUniformList();
    for (auto& uniform : list)
    {
        const CShaderUniform::EUniformData type = uniform.second->getData();
        s32 id = uniform.second->getID();
        switch (type)
        {
        case CShaderUniform::eTransformProjectionMatrix:
        {
            if (!m_camera)
            {
                ASSERT(false, "Camera doesn't exist");
                break;
            }
            program->applyUniformMatrix4(id, m_camera->getProjectionMatrix());
        }
        break;

        case CShaderUniform::eTransformModelMatrix:
        {
            core::Matrix4D modelMatrix(transform);
            modelMatrix.makeTransposed();

            program->applyUniformMatrix4(id, modelMatrix);
        }
        break;

        case CShaderUniform::eTransformViewMatrix:
        {
            if (!m_camera)
            {
                ASSERT(false, "Camera doesn't exist");
                break;
            }

            program->applyUniformMatrix4(id, m_camera->getViewMatrix());
        }
        break;

        case CShaderUniform::eTransformViewPosition:
        {
            if (!m_camera)
            {
                ASSERT(false, "Camera doesn't exist");
                break;
            }
            program->applyUniformVector3(id, m_camera->getPosition());
        }
        break;

        case CShaderUniform::eTransformViewUpVector:
        {
            if (!m_camera)
            {
                ASSERT(false, "Camera doesn't exist");
                break;
            }
            program->applyUniformVector3(id, m_camera->getUpVector());
        }
        break;

        case CShaderUniform::eTransformNormalMatrix:
        {
            core::Matrix4D normalMatrix;
            transform.getInverse(normalMatrix);

            program->applyUniformMatrix4(id, normalMatrix);
        }
        break;

        case -1:
        default:
            break;
        }
    }
}

void IRenderer::updateMaterial(const MaterialPtr& material, const RenderPassPtr& pass)
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

        case -1:
        default:
            break;
        }
    }
}

void IRenderer::updateLight(const core::Matrix4D& transform, const RenderPassPtr& pass)
{
    if (m_lightList.empty())
    {
        return;
    }

    std::vector<scene::CLight*> lights = m_lightList;
    const Vector3D& pos = transform.getTranslation();

    lights.erase(std::remove_if(lights.begin(), lights.end(), [&pos](scene::CLight* light) -> bool
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
    }), lights.end());


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

            case -1:
            default:
                break;
            }
        }
    }
}

void IRenderer::updateTexture(MaterialPtr& material, const RenderPassPtr& pass)
{
    const ShaderDataPtr& defaultData = pass->getDefaultShaderData();
    const SamplerList& samplerList = defaultData->getSamplerList();

    bool isUserSamplers = (material->getTextureCount() > 0) ? true : false;
    if (isUserSamplers)
    {
        u32 textureCount = material->getTextureCount();
        u32 samplerID = 0;
        for (u32 unit = 0; unit < textureCount; ++unit)
        {
            TexturePtr texture = material->getTexture(unit);
            if (texture->isEnable())
            {
                texture->bind(unit);
            }
            else
            {
                texture->unbind();
            }
        }
    }
    else if (!samplerList.empty())
    {
        for (auto& sampler : samplerList)
        {
            if (sampler->getID() < 0)
            {
                continue;
            }

            CShaderSampler::ESamplerType type = sampler->getType();
            switch (type)
            {

            case CShaderSampler::ESamplerType::eUserSampler:
            {
                isUserSamplers = true;
                break;
            }
            break;

            case CShaderSampler::ESamplerType::eTextureSampler:
            case CShaderSampler::ESamplerType::eRenderTargetSampler:
            {
                TexturePtr texture = sampler->getTexture();
                if (texture)
                {
                    if (texture->isEnable())
                    {
                        texture->bind(sampler->getID());
                    }
                    else
                    {
                        texture->unbind();
                    }
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

void IRenderer::updateAdvanced(const RenderPassPtr & pass)
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

        case CShaderUniform::eViewportSize:
        {
            const core::Rect32& rect = IRenderer::getCurrentRenderTarget()->getViewport();
            program->applyUniformVector2(id, core::Vector2D(static_cast<f32>(rect.getWidth()), static_cast<f32>(rect.getHeight())));
        }
        break;

        default:
            break;
        }
    }
}

} //namespace renderer
} //namespace v3d
