#include "Renderable.h"
#include "Material.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "RenderTechnique.h"
#include "scene/RenderTechniqueManager.h"

namespace v3d
{
namespace renderer
{

Renderable::Renderable()
    : m_material(nullptr)
    , m_renderJob(nullptr)
    , m_renderTechnique(nullptr)
{
}

Renderable::~Renderable()
{
}

void Renderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

void Renderable::setRenderJob(const RenderJobPtr& job)
{
    m_renderJob = job;
}

void Renderable::setGeometry(const GeometryPtr& geometry)
{
    m_geometry = geometry;
}

const MaterialPtr& Renderable::getMaterial() const
{
    return m_material;
}

MaterialPtr& Renderable::getMaterial()
{
    return m_material;
}

const RenderJobPtr& Renderable::getRenderJob() const
{
    return m_renderJob;
}

const GeometryPtr& Renderable::getGeometry() const
{
    return m_geometry;
}

void Renderable::render()
{
    ASSERT(Renderable::getRenderJob(), "Renderable: Render job nullptr");
    ENGINE_RENDERER->draw(Renderable::getRenderJob());
}

const RenderTechnique* Renderable::getRenderTechique() const
{
    return m_renderTechnique;
}

RenderTechnique* Renderable::getRenderTechique()
{
    return const_cast<RenderTechnique*>(m_renderTechnique);
}

bool Renderable::setRenderTechnique(const std::string& file)
{
    const RenderTechnique* technique = scene::RenderTechniqueManager::getInstance()->load(file);
    if (!technique)
    {
        LOG_ERROR("CMaterial: Error read file [%s]", file.c_str());
        return false;
    }

    m_renderTechnique = technique;

    return true;
}

bool Renderable::setRenderTechnique(const stream::IStreamPtr& stream)
{
    RenderTechnique* technique = new RenderTechnique();
    technique->init(stream);
    if (!technique->load())
    {
        LOG_ERROR("CMaterial: Streaming error read");
        return false;
    }

    scene::RenderTechniqueManager::getInstance()->add(technique);
    m_renderTechnique = technique;

    return true;
}

void Renderable::setRenderTechnique(const RenderTechnique* technique)
{
    m_renderTechnique = technique;
}

} //namespace renderer
} //namespace v3d
