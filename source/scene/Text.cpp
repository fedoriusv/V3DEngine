#include "Text.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "scene/FontManager.h"
#include "renderer/Material.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace renderer;
using namespace resources;

CText::CText(const std::string& font)
    : m_text("")
    , m_size(24U)
    , m_align(EAlignMode::eFontAlignLeft)

    , k_textScale(0.02f)
    , k_spacing(0.1f)

    , m_font(font)
    , m_data(nullptr)
{
    m_nodeType = ENodeType::eText;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    Renderable::setMaterial(new CMaterial());
}

CText::~CText()
{
    Renderable::getGeometry()->free();
}

void CText::setText(const std::string& text)
{
    m_text = text;
    CText::refresh();
}

const std::string& CText::getText() const
{
    return m_text;
}

void CText::setSize(const u32 size)
{
    m_size = size;
    CText::refresh();
}

u32 CText::getSize() const
{
    return m_size;
}

void CText::setAlignMode(EAlignMode mode)
{
    m_align = mode;
    CText::refresh();
}

CText::EAlignMode CText::getAlignMode() const
{
    return m_align;
}

void CText::init()
{
    if (m_initialiazed)
    {
        return;
    }

    const RenderTechnique* technique = Renderable::getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CText: RenderTechique doesn't exist");
        ASSERT(false, "CText: RenderTechique doesn't exist");
        return;
    }

//    Renderable::setGeometry(ENGINE_RENDERER->makeSharedGeometry(technique));
    Renderable::setRenderJob(std::make_shared<CRenderJob>(this, CNode::getAbsTransform()));

    if (m_font.empty())
    {
        LOG_WARNING("CText: Font name Empty");
        return;
    }

    m_data = const_cast<CFontData*>(CFontManager::getInstance()->load(m_font));
    if (!m_data)
    {
        LOG_ERROR("CText: Can not load font: %s", m_font.c_str());
        ASSERT(false, "CText: Can not load font");
        return;
    }

    m_data->addCharsToMap(m_text);
    m_data->setFontSize(m_size);

    CText::build();

    Renderable::getGeometry()->setDrawMode(eTriangles);
    Renderable::getGeometry()->init();

    //TODO: Need more texture maps
    Renderable::getMaterial()->setTexture(0, m_data->m_charTexture[0]);
    m_initialiazed = true;
}

void CText::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    Renderable::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CText::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    Renderable::render();
}

void CText::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    m_data->addCharsToMap(m_text);
    m_data->setFontSize(m_size);

    CText::build();

    Renderable::getGeometry()->refresh();
}

f32 CText::getTextWidth()
{
    f32 width = 0.0f;

    for (std::string::const_iterator chr = m_text.begin(); chr < m_text.end(); ++chr)
    {
        const CFontData::SCharDesc* info = m_data->getCharInfo(*chr);
        if (!info)
        {
            continue;
        }

        width += (k_textScale * info->_advX) + adjustForKerningPairs(info, *chr);
    }

    return width;
}

f32 CText::adjustForKerningPairs(const CFontData::SCharDesc* info, s32 charId)
{
    if (charId == 0)
    {
        return 0;
    }

    for (u32 i = 0; i < info->_kerningPairs.size(); i += 2)
    {
        if (info->_kerningPairs[i] == charId)
        {
            return info->_kerningPairs[i + 1] * k_textScale;
        }
    }

    return 0;
}

void CText::build()
{
    f32 x = 0.f;
    f32 y = 0.f;
    f32 z = 0.f;

    f32 textwidth = CText::getTextWidth();
    if (m_align == EAlignMode::eFontAlignCenter)
    {
        x -= textwidth / 2;
    }
    else if (m_align == EAlignMode::eFontAlignLeft)
    {
        x -= textwidth;
    }

    SVertexData& data = Renderable::getGeometry()->getData();
    data.malloc((u32)m_text.size() * 6);

    u32 index = 0;
    for (std::string::const_iterator chr = m_text.begin(); chr < m_text.end(); ++chr)
    {
        const CFontData::SCharDesc* info = m_data->getCharInfo(*chr);

        if (*chr == '\n')
        {
            x = 0;
            y += k_textScale * m_size;
            continue;
        }

        f32 a = k_textScale * static_cast<f32>(info->_advX);
        f32 w = k_textScale * static_cast<f32>(info->_width);
        f32 h = k_textScale * static_cast<f32>(info->_height);
        f32 ox = k_textScale * static_cast<f32>(info->_offX);
        f32 oy = k_textScale * static_cast<f32>(info->_offY);

        data._vertices[index * 6 + 0].set(Vector3D(x + ox, y + oy, z));
        data._vertices[index * 6 + 1].set(Vector3D(x + ox, y + oy + h, z));
        data._vertices[index * 6 + 2].set(Vector3D(x + w + ox, y + oy, z));
        data._vertices[index * 6 + 3].set(Vector3D(x + w + ox, y + oy, z));
        data._vertices[index * 6 + 4].set(Vector3D(x + ox, y + oy + h, z));
        data._vertices[index * 6 + 5].set(Vector3D(x + w + ox, y + oy + h, z));

        f32 u = (static_cast<f32>(info->_srcX)) / m_data->m_mapSize.width;
        f32 v = (static_cast<f32>(info->_srcY)) / m_data->m_mapSize.height;
        f32 u2 = u + (static_cast<f32>(info->_width)) / m_data->m_mapSize.width;
        f32 v2 = v + (static_cast<f32>(info->_height)) / m_data->m_mapSize.height;

        data._texCoords.at(0)[index * 6 + 0].set(Vector2D(u, v));
        data._texCoords.at(0)[index * 6 + 1].set(Vector2D(u, v2));
        data._texCoords.at(0)[index * 6 + 2].set(Vector2D(u2, v));
        data._texCoords.at(0)[index * 6 + 3].set(Vector2D(u2, v));
        data._texCoords.at(0)[index * 6 + 4].set(Vector2D(u, v2));
        data._texCoords.at(0)[index * 6 + 5].set(Vector2D(u2, v2)); 

        data._normals[index * 6 + 0].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 6 + 1].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 6 + 2].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 6 + 3].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 6 + 4].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 6 + 5].set(Vector3D(0.0f, 0.0f, 1.0f));

        ++index;

        x += a;
        if (*chr == ' ')
        {
            x += k_spacing;
        }

        x += adjustForKerningPairs(info, *chr);
    }

    //TODO: maybe need to fix texture coord position
    CNode::setRotation(Vector3D(180.0f, 0.0f, 0.0f));
}
