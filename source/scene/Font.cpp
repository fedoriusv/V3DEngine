#include "Font.h"
#include "Engine.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CFont::CFont()
    : m_text("")
    , m_size(24U)

    , m_geometry(nullptr)
    , m_material(nullptr)
    , m_renderJob(nullptr)
{
    m_nodeType = ENodeType::eFont;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    m_material = std::make_shared<CMaterial>();
}

CFont::~CFont()
{
}

void CFont::setText(const std::string& text)
{
    m_text = text;
}

const std::string& CFont::getText() const
{
    return m_text;
}

void CFont::setSize(const u32 size)
{
    m_size = size;
}

u32 CFont::getSize() const
{
    return m_size;
}

void CFont::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

const renderer::MaterialPtr& CFont::getMaterial() const
{
    return m_material;
}

void CFont::init()
{
    RenderTechniquePtr technique = m_material->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CShape: Do not exist RenderTechique");
        ASSERT(false && "CShape: Do not exist RenderTechique");
        return;
    }

    m_geometry = RENDERER->makeSharedGeometry(technique);
    m_renderJob = std::make_shared<CRenderJob>(m_material, m_geometry, m_transform);
}

void CFont::update(f64 time)
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    m_renderJob->setTransform(CNode::getAbsTransform());
}

void CFont::render()
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    RENDERER->draw(m_renderJob);
}