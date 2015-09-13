#include "Material.h"
#include "stream/StreamManager.h"
#include "utils/Logger.h"
#include "stream/FileStream.h"
#include "scene/RenderTechniqueManager.h"
#include "scene/TextureManager.h"

using namespace v3d;
using namespace renderer;
using namespace scene;

CMaterial::SMaterialData& CMaterial::SMaterialData::operator = (const SMaterialData& material)
{
    if (this == &material)
    {
        return *this;
    }

    _ambient = material._ambient;
    _diffuse = material._diffuse;
    _specular = material._specular;
    _emission = material._emission;
    _shininess = material._shininess;
    _glossiness = material._glossiness;
    _transparency = material._transparency;

    return *this;
}

CMaterial::CMaterial()
    : m_needUpdate(true)
    , m_name("")
{
    m_materialData._ambient      = core::Vector4D(0.2f, 0.2f, 0.2f, 1.0f);
    m_materialData._diffuse      = core::Vector4D(0.2f, 0.2f, 0.2f, 1.0f);
    m_materialData._specular     = core::Vector4D(1.0f);
    m_materialData._emission     = core::Vector4D(0.0f);
    m_materialData._shininess    = 1.0f;
    m_materialData._glossiness   = 0.0f;
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

void CMaterial::setGlossiness(f32 value)
{
    m_materialData._glossiness = value;
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

f32 CMaterial::getGlossiness() const
{
    return m_materialData._glossiness;
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

    scene::CTextureManager::getInstance()->add(texture);
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

void CMaterial::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

bool CMaterial::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CMaterial: Empty Stream with name [%s]", CResource::getResourseName().c_str());
        return false;
    }

    if (stream->size() > 0)
    {
        stream->seekBeg(0);

        stream->read(m_name);

        u32 countTextures;
        stream->read(countTextures);
        for (u32 i = 0; i < countTextures; ++i)
        {
            std::string textureName;
            stream->read(textureName);
            CMaterial::setTexture(i, "textures/" + textureName);
        }

        stream->read(&m_materialData._diffuse.x, sizeof(core::Vector4D), 1);
        stream->read(&m_materialData._ambient.x, sizeof(core::Vector4D), 1);
        stream->read(&m_materialData._specular.x, sizeof(core::Vector4D), 1);
        stream->read(&m_materialData._emission.x, sizeof(core::Vector4D), 1);

        stream->read(m_materialData._transparency);
        stream->read(m_materialData._shininess);
        stream->read(m_materialData._glossiness);
    }

    return true;
}

const std::string& CMaterial::getName() const
{
    return m_name;
}

MaterialPtr CMaterial::clone()
{
    MaterialPtr clone = std::make_shared<CMaterial>();
    
    clone->m_materialData = m_materialData;
    clone->m_texture = m_texture;
    clone->m_renderTechnique = m_renderTechnique;
    clone->m_name = m_name;
    clone->m_needUpdate = true;

    clone->init(CMaterial::getStream());

    return clone;
}
