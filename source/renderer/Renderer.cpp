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

void CRenderer::needUpdateCamera(bool update)
{
    m_updateCamera = update;
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
    m_projectionMatrix = core::buildProjectionMatrixPerspective(45.0f, aspectRatio, 0.5f, 100.0f);

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

void CRenderer::draw(const RenderJobPtr& job, bool updateTransform)
{
    const MaterialPtr& material = job->getMaterial();
    const GeometryPtr& geomerty = job->getGeometry();

    for (u32 i = 0; i < material->getRenderTechique()->getRenderPassCount(); ++i)
    {
        const RenderPassPtr& pass = material->getRenderTechique()->getRenderPass(i);

        const ShaderDataPtr& data = pass->getShaderData();
        
        if (updateTransform)
        {
            CRenderer::updateTransform(job->getTransform(), data);
        }

        CRenderer::updateView(data);
        CRenderer::updateMaterial(material, data);
        CRenderer::updateLight(job->getTransform(), data);

        pass->bind();

        for (u32 layer = 0; layer < material->getTextureCount(); ++layer)
        {
            TexturePtr texture = material->getTexture(layer);

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

void CRenderer::updateTransform(const core::Matrix4D& transform, const ShaderDataPtr& data)
{
    if (data->isExistUniform(eTransformProjectionMatrix))
    {
        data->setUniformMatrix4(eTransformProjectionMatrix, m_projectionMatrix);
    }

    if (data->isExistUniform(eTransformModelMatrix))
    {
        core::Matrix4D modelMatrix = transform;
        modelMatrix.makeTransposed();

        data->setUniformMatrix4(eTransformModelMatrix, modelMatrix);
    }

    if (data->isExistUniform(eTransformNormalMatrix))
    {
        core::Matrix4D normalMatrix;
        transform.getInverse(normalMatrix);

        data->setUniformMatrix4(eTransformNormalMatrix, normalMatrix);
    }
}

void CRenderer::updateView(const ShaderDataPtr& data)
{
    if (!m_updateCamera)
    {
        return;
    }

    if (data->isExistUniform(eTransformViewMatrix))
    {
        data->setUniformMatrix4(eTransformViewMatrix, m_viewMatrix);
    }

    if (data->isExistUniform(eTransformViewPosition))
    {
        data->setUniformVector3(eTransformViewPosition, m_viewPosition);
    }

    m_updateCamera = false;
}

void CRenderer::updateMaterial(const MaterialPtr& material, const ShaderDataPtr& data)
{
    if (!material->m_needUpdate)
    {
        return;
    }

    if (data->isExistUniform(eMaterialAmbient))
    {
        data->setUniformVector4(eMaterialAmbient, material->getAmbientColor());
    }

    if (data->isExistUniform(eMaterialDiffuse))
    {
        data->setUniformVector4(eMaterialDiffuse, material->getDiffuseColor());
    }

    if (data->isExistUniform(eMaterialSpecular))
    {
        data->setUniformVector4(eMaterialSpecular, material->getSpecularColor());
    }

    if (data->isExistUniform(eMaterialEmission))
    {
        data->setUniformVector4(eMaterialEmission, material->getEmissionColor());
    }

    if (data->isExistUniform(eMaterialShininess))
    {
        data->setUniformFloat(eMaterialShininess, material->getShininess());
    }

    material->m_needUpdate = false;
}

void CRenderer::updateLight(const core::Matrix4D& transform, const ShaderDataPtr& data)
{
    if (m_lightList.empty())
    {
        return;
    }

    //std::vector<scene::CLight*> lights = m_lightList;
    //const Vector3D& pos = transform.getTranslation();

    //std::remove_if(lights.begin(), lights.end(), [pos](scene::CLight* light) -> bool
    //{
    //    if (!light->isVisible())
    //    {
    //        return true;
    //    }

    //    f32 distance = (light->getPosition() - pos).length();
    //    if (light->getRadius() < distance)
    //    {
    //        return true;
    //    }

    //    return true;
    //});

    //if (data->isExistUniform(eLightsCount))
    //{
    //    data->setUniformInt(eLightsCount, lights.size());
    //}

    //if (lights.empty())
    //{
    //    return;
    //}

    //s32 index = 0;
    //for (std::vector<scene::CLight*>::iterator light = lights.begin(); light < lights.end(); ++light)
    //{
    //    if (!(*light)->m_needUpdate)
    //    {
    //        //continue;
    //    }

    //    if (data->isExistUniform(eLightPosition))
    //    {
    //        data->setUniformVector4(eLightPosition, Vector4D((*light)->getPosition(), 0.0f), index);
    //    }

    //    if (data->isExistUniform(eLightAmbient))
    //    {
    //        data->setUniformVector4(eLightAmbient, (*light)->getAmbient(), index);
    //    }

    //    if (data->isExistUniform(eLightDiffuse))
    //    {
    //        data->setUniformVector4(eLightDiffuse, (*light)->getDiffuse(), index);
    //    }

    //    if (data->isExistUniform(eLightSpecular))
    //    {
    //        data->setUniformVector4(eLightSpecular, (*light)->getSpecular(), index);
    //    }

    //    if (data->isExistUniform(eLightDirection))
    //    {
    //        data->setUniformVector3(eLightDirection, (*light)->getDirection(), index);
    //    }

    //    if (data->isExistUniform(eLightAttenuation))
    //    {
    //        data->setUniformVector3(eLightAttenuation, (*light)->getAttenuation(), index);
    //    }

    //    if (data->isExistUniform(eLightRadius))
    //    {
    //        data->setUniformFloat(eLightRadius, (*light)->getRadius(), index);
    //    }

    //    (*light)->m_needUpdate = false;
    //}

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
