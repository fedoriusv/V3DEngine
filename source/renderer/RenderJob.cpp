#include "RenderJob.h"
#include "scene/Light.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderJob::CRenderJob(const MaterialPtr& material, const GeometryPtr& geomerty, const Matrix4D& transform)
    : m_material(material)
    , m_geomerty(geomerty)
    , m_transform(transform)
{
}

CRenderJob::~CRenderJob()
{
}

void CRenderJob::setMaterial(const MaterialPtr& material)
{
   m_material = material;
}

const MaterialPtr& CRenderJob::getMaterial() const
{
    return m_material;
}

void CRenderJob::setGeomery(const GeometryPtr& geomerty)
{
   m_geomerty = geomerty;
}

const GeometryPtr& CRenderJob::getGeometry() const
{
    return m_geomerty;
}

void CRenderJob::setTransform(const Matrix4D& transform)
{
    m_transform = transform;
}

const Matrix4D& CRenderJob::getTransform() const
{
    return m_transform;
}

void CRenderJob::job(const bool updated)
{
    for (u32 i = 0; i < m_material->getRenderTechique()->getRenderPassCount(); ++i)
    {
        const RenderPassPtr& pass = m_material->getRenderTechique()->getRenderPass(i);

        const ShaderDataPtr& data = pass->getShaderData();
        if (updated)
        {
            CRenderJob::updateTransform(data);
        }

        CRenderJob::updateMaterial(data);
        CRenderJob::updateLight(data);

        pass->bind();

       for (u32 layer = 0; layer < m_material->getTextureCount(); ++layer)
       {
            TexturePtr texture = m_material->getTexture(layer);

            if (texture->isEnable())
            {
                texture->bind(layer);
            }
            else
            {
                texture->unbind(layer);
            }
        }

        m_geomerty->draw();
    }

}

void CRenderJob::updateMaterial(const ShaderDataPtr& data)
{
    if (!m_material->m_needUpdate)
    {
        return;
    }

    if (data->isExistUniform(eMaterialAmbient))
    {
        data->setUniformVector4(eMaterialAmbient, m_material->getAmbientColor());
    }

    if (data->isExistUniform(eMaterialDiffuse))
    {
        data->setUniformVector4(eMaterialDiffuse, m_material->getDiffuseColor());
    }

    if (data->isExistUniform(eMaterialSpecular))
    {
        data->setUniformVector4(eMaterialSpecular, m_material->getSpecularColor());
    }

    if (data->isExistUniform(eMaterialEmission))
    {
        data->setUniformVector4(eMaterialEmission, m_material->getEmissionColor());
    }

    if (data->isExistUniform(eMaterialShininess))
    {
        data->setUniformFloat(eMaterialShininess, m_material->getShininess());
    }

    m_material->m_needUpdate = false;
}

void CRenderJob::updateTransform(const ShaderDataPtr& data)
{
    if (data->isExistUniform(eTransformProjectionMatrix))
    {
        data->setUniformMatrix4(eTransformProjectionMatrix, RENDERER->m_projectionMatrix);
    }

    if (data->isExistUniform(eTransformModelMatrix))
    {
        data->setUniformMatrix4(eTransformModelMatrix, RENDERER->m_modelMatrix);
    }

    if (data->isExistUniform(eTransformViewMatrix))
    {
        data->setUniformMatrix4(eTransformViewMatrix, RENDERER->m_viewMatrix);
    }

    if (data->isExistUniform(eTransformNormalMatrix))
    {
        data->setUniformMatrix4(eTransformNormalMatrix, RENDERER->m_normalMatrix);
    }

    if (data->isExistUniform(eTransformViewPosition))
    {
        data->setUniformVector3(eTransformViewPosition, RENDERER->m_viewPosition);
    }
}

void CRenderJob::updateLight(const ShaderDataPtr& data)
{
    if (RENDERER->m_lightList.empty())
    {
        return;
    }

    std::vector<scene::CLight*> lights = RENDERER->m_lightList;
    const Vector3D& pos = m_transform.getTranslation();

    std::remove_if(lights.begin(), lights.end(), [pos](scene::CLight* light) -> bool
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

    if (data->isExistUniform(eLightsCount))
    {
        data->setUniformInt(eLightsCount, lights.size());
    }

    if (lights.empty())
    {
        return;
    }

    s32 index = 0;
    for (std::vector<scene::CLight*>::iterator light = lights.begin(); light < lights.end(); ++light)
    {
        if (!(*light)->m_needUpdate)
        {
            //continue;
        }

        if (data->isExistUniform(eLightPosition))
        {
            data->setUniformVector4(eLightPosition, Vector4D((*light)->getPosition(), 0.0f), index);
        }

        if (data->isExistUniform(eLightAmbient))
        {
            data->setUniformVector4(eLightAmbient, (*light)->getAmbient(), index);
        }

        if (data->isExistUniform(eLightDiffuse))
        {
            data->setUniformVector4(eLightDiffuse, (*light)->getDiffuse(), index);
        }

        if (data->isExistUniform(eLightSpecular))
        {
            data->setUniformVector4(eLightSpecular, (*light)->getSpecular(), index);
        }

        if (data->isExistUniform(eLightDirection))
        {
            data->setUniformVector3(eLightDirection, (*light)->getDirection(), index);
        }

        if (data->isExistUniform(eLightAttenuation))
        {
            data->setUniformVector3(eLightAttenuation, (*light)->getAttenuation(), index);
        }

        if (data->isExistUniform(eLightRadius))
        {
            data->setUniformFloat(eLightRadius, (*light)->getRadius(), index);
        }

        (*light)->m_needUpdate = false;
    }

}
