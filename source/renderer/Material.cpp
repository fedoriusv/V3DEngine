#include "Material.h"
#include "stream/StreamManager.h"
#include "utils/Logger.h"
#include "stream/FileStream.h"
#include "scene/RenderTechniqueManager.h"
#include "scene/TextureManager.h"

using namespace v3d;
using namespace v3d::renderer;

CMaterial::CMaterial()
    : m_needUpdate(true)
{
    m_materialData._ambient      = core::Vector4D(0.2f, 0.2f, 0.2f, 1.0f);
    m_materialData._diffuse      = core::Vector4D(0.2f, 0.2f, 0.2f, 1.0f);
    m_materialData._specular     = core::Vector4D(1.0f);
    m_materialData._emission     = core::Vector4D(0.0f);
    m_materialData._shininess    = 128U;
    m_materialData._transparency = 1.0f;
}

CMaterial::~CMaterial()
{
}

void CMaterial::setAmbientColor(const core::Vector4D& color)
{
    m_materialData._ambient = color;
    m_needUpdate = true;
}

void CMaterial::setDiffuseColor(const core::Vector4D& color)
{
	m_materialData._diffuse = color;
    m_materialData._diffuse.w = (m_materialData._diffuse.w > 1.0f) ? 1.0f : m_materialData._diffuse.w;
    CMaterial::setTransparency(m_materialData._diffuse.w);
    m_needUpdate = true;
}

void CMaterial::setSpecularColor(const core::Vector4D& color)
{
    m_materialData._specular = color;
    m_needUpdate = true;
}

void CMaterial::setEmissionColor(const core::Vector4D& color)
{
    m_materialData._emission = color;
    m_needUpdate = true;
}

void CMaterial::setShininess(f32 value)
{
    m_materialData._shininess = value;
    m_needUpdate = true;
}

const core::Vector4D& CMaterial::getAmbientColor() const
{
    return m_materialData._ambient;
}

const core::Vector4D& CMaterial::getDiffuseColor() const
{
    return m_materialData._diffuse;
}

const core::Vector4D& CMaterial::getSpecularColor() const
{
    return m_materialData._specular;
}

const core::Vector4D& CMaterial::getEmissionColor() const
{
    return m_materialData._emission;
}

f32 CMaterial::getShininess() const
{
    return m_materialData._shininess;
}

const CTexture* CMaterial::getTexture(u32 layer) const
{
    if (layer >= ETextureLayer::eLayerMax)
    {
        ASSERT(false && "CMaterial::getTexture: Invalid texture layer") ;
    }

    return m_texture.at(layer);
}

CTexture* CMaterial::getTexture(u32 layer)
{
    if (layer >= ETextureLayer::eLayerMax)
    {
        ASSERT(false && "CMaterial::getTexture: Invalid texture layer");
    }

    return const_cast<CTexture*>(m_texture.at(layer));
}

u32 CMaterial::getTextureCount() const
{
    return (u32)m_texture.size();
}

bool CMaterial::setTexture(u32 layer, const std::string& file)
{
    if (layer >= ETextureLayer::eLayerMax)
    {
        ASSERT(false && "CMaterial::setTexture: Texture Layer range out");
        return false;
    }

    const CTexture* texture = scene::CTextureManager::getInstance()->load(file);
    if (!texture)
    {
        LOG_ERROR("CMaterial::setTexture: Error read file [%s]", file.c_str());
        return false;
    }

    m_texture[layer] = texture;

    return true;
}

bool CMaterial::setTexture(u32 layer, const std::string* files[6])
{
    if (layer >= ETextureLayer::eLayerMax)
    {
        ASSERT(false && "CMaterial::setTexture: Texture Layer range out");
        return false;
    }

    const CTexture* texture = scene::CTextureManager::getInstance()->load(files);
    if (!texture)
    {
        LOG_ERROR("CMaterial: Error read cubemap files");
        return false;
    }

    m_texture[layer] = texture;

    return true;
}

void CMaterial::setTexture(u32 layer, const CTexture* texture)
{
    if (layer >= ETextureLayer::eLayerMax)
    {
        ASSERT(false && "CMaterial::setTexture: Texture Layer range out");
        return;
    }

    m_texture[layer] = texture;

}

void CMaterial::setTransparency(const f32 value)
{
    m_materialData._transparency = (value > 1.0f) ? 1.0f : value;
}

float CMaterial::getTransparency() const
{
    return m_materialData._transparency;
}

const CRenderTechnique* CMaterial::getRenderTechique() const
{
    return m_renderTechnique;
}

CRenderTechnique* CMaterial::getRenderTechique()
{
    return const_cast<CRenderTechnique*>(m_renderTechnique);
}

bool CMaterial::setRenderTechnique(const std::string& file)
{
    const CRenderTechnique* technique = scene::CRenderTechniqueManager::getInstance()->load(file);
    if (!technique)
    {
        LOG_ERROR("CMaterial: Error read file [%s]", file.c_str());
        return false;
    }

    m_renderTechnique = technique;

    return true;
}

bool CMaterial::setRenderTechnique(const stream::IStreamPtr& stream)
{
    CRenderTechnique* technique = new CRenderTechnique();
    technique->init(stream);
    if (technique->load())
    {
        LOG_ERROR("CMaterial: Streaming error read");
        return false;
    }

    scene::CRenderTechniqueManager::getInstance()->add(technique);
    m_renderTechnique = technique;

    return true;
}

void CMaterial::setRenderTechnique(const CRenderTechnique* technique)
{
    m_renderTechnique = technique;
}
