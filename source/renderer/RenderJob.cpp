#include "RenderJob.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderJob::CRenderJob(const MaterialPtr& material, const GeometryPtr& geomerty)
    : m_material(material)
    , m_geomerty(geomerty)
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

void CRenderJob::job()
{
    for (u32 i = 0; i < m_material->getRenderTechique()->getRenderPassCount(); ++i)
    {
        RenderPassPtr pass = m_material->getRenderTechique()->getRenderPass(i);

        CRenderJob::updateTransform(pass->getShaderData());
        CRenderJob::updateMaterial(pass->getShaderData());

        pass->bind();
    }

    for (u32 layer = 0; layer < m_material->getTextureCount(); ++layer)
    {
        TexturePtr texture = m_material->getTexture(layer);
        texture->bind(layer);
    }

    m_geomerty->draw();
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