#include "Font.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "scene/FontManager.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CFont::CFont(const std::string& font)
    : m_text("")
    , m_size(24U)
    , m_align(EAlignMode::eFontAlignLeft)

    , m_geometry(nullptr)
    , m_material(nullptr)
    , m_renderJob(nullptr)

    , m_font(font)
    , m_data(nullptr)
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
    CFont::refresh();
}

const std::string& CFont::getText() const
{
    return m_text;
}

void CFont::setSize(const u32 size)
{
    m_size = size;
    CFont::refresh();
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

void CFont::setAlignMode(EAlignMode mode)
{
    m_align = mode;
}

CFont::EAlignMode CFont::getAlignMode() const
{
    return m_align;
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

    if (m_font.empty())
    {
        LOG_WARRNING("Font name Empty");
        return;
    }

    m_data = CFontManager::getInstance()->load(m_font);
    if (!m_data)
    {
        LOG_ERROR("Font: Can not load font: %s", m_font.c_str());
        ASSERT(false && "Font: Can not load font");
        return;
    }

    m_text = "abcd";
  /*  m_data->addCharsToMap(m_text);
    m_data->setFontSize(m_size);*/
    CFont::build();

    m_geometry->setDrawMode(CGeometry::eTriangleStrip);
    m_geometry->init();

    //m_material->setTexture(0, m_data->m_charMaterial[0]);

    m_initialiazed = true;
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

void CFont::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

   /* m_data->addCharsToMap(m_text);
    m_data->setFontSize(m_size);*/

    CFont::build();

    m_geometry->refresh();
}

f32 CFont::getTextWidth()
{
    f32 width = 0.0f;
    const f32 scale = 1.0f;

    for (std::string::const_iterator chr = m_text.begin(); chr < m_text.end(); ++chr)
    {
        const CFontData::SCharDesc* info = m_data->getCharInfo(*chr);

        width += (scale * info->_advX) + adjustForKerningPairs(info, *chr);
    }

    return width;
}

f32 CFont::adjustForKerningPairs(const CFontData::SCharDesc* info, const s32 charId)
{
    const f32 scale = 1.0f;

    if (charId == 0)
    {
        return 0;
    }

    for (u32 i = 0; i < info->_kerningPairs.size(); i += 2)
    {
        if (info->_kerningPairs[i] == charId)
        {
            return info->_kerningPairs[i + 1] * scale;
        }
    }

    return 0;
}

void CFont::build()
{
    //m_data->loadCharList();

    const f32 scale = 1.0f;
    f32 scaleW = 1.0f;
    f32 scaleH = 1.0f;
    f32 spacing = 3.f;

    f32 x = 0.f;
    f32 y = 0.f;
    f32 z = -100.f;

    f32 textwidth = CFont::getTextWidth();
    if (m_align == EAlignMode::eFontAlignCenter)
    {
        x -= textwidth / 2;
    }
    else if (m_align == EAlignMode::eFontAlignRight)
    {
        x -= textwidth;
    }

    SVertexData& data = m_geometry->getData();
    data.malloc(m_text.size() * 6);

    u32 index = 0;
    for (std::string::const_iterator chr = m_text.begin(); chr < m_text.end(); ++chr)
    {
        const CFontData::SCharDesc* info = m_data->getCharInfo(*chr);

        if (*chr == 10)
        {
            x = 0;
            y += scale * m_size;
            continue;
        }

        f32 a = scale * static_cast<f32>(info->_advX);
        f32 w = scale * static_cast<f32>(info->_width);
        f32 h = scale * static_cast<f32>(info->_height);
        f32 ox = scale * static_cast<f32>(info->_offX);
        f32 oy = scale * static_cast<f32>(info->_offY);

        data._vertices[index * 4 + 0].set(Vector3D(x + ox, y + oy, z));
        data._vertices[index * 4 + 1].set(Vector3D(x + ox, y + oy + h, z));
        data._vertices[index * 4 + 2].set(Vector3D(x + w + ox, y + oy, z));
        data._vertices[index * 4 + 3].set(Vector3D(x + w + ox, y + oy, z));
        data._vertices[index * 4 + 4].set(Vector3D(x + ox, y + oy + h, z));
        data._vertices[index * 4 + 5].set(Vector3D(x + w + ox, y + oy + h, z));

        f32 u = (static_cast<f32>(info->_srcX)) / scaleW;
        f32 v = (static_cast<f32>(info->_srcY)) / scaleH;
        f32 u2 = u + (static_cast<f32>(info->_width)) / scaleW;
        f32 v2 = v + (static_cast<f32>(info->_height)) / scaleH;

        data._texCoords.at(0)[index * 4 + 0].set(Vector2D(u, -v));
        data._texCoords.at(0)[index * 4 + 1].set(Vector2D(u, -v2));
        data._texCoords.at(0)[index * 4 + 2].set(Vector2D(u2, -v));
        data._texCoords.at(0)[index * 4 + 3].set(Vector2D(u2, -v));
        data._texCoords.at(0)[index * 4 + 4].set(Vector2D(u, -v2));
        data._texCoords.at(0)[index * 4 + 5].set(Vector2D(u2, -v2));

        data._normals[index * 4 + 0].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 4 + 1].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 4 + 2].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 4 + 3].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 4 + 4].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 4 + 5].set(Vector3D(0.0f, 0.0f, 1.0f));

        ++index;

        x += a;
        if (*chr == ' ')
        {
            x += spacing;
        }

        x += adjustForKerningPairs(info, *chr);
    }

    /*data.malloc(4, 6);

    f32 s = 2.0f;
    data._vertices[0].set(Vector3D(-s, -s, 0.0f));
    data._vertices[1].set(Vector3D(-s, s, 0.0f));
    data._vertices[2].set(Vector3D(s, s, 0.0f));
    data._vertices[3].set(Vector3D(s, -s, 0.0f));

    data._texCoords.at(0)[0].set(Vector2D(0, 1));
    data._texCoords.at(0)[1].set(Vector2D(0, 0));
    data._texCoords.at(0)[2].set(Vector2D(1, 0));
    data._texCoords.at(0)[3].set(Vector2D(1, 1));

    data._normals[0].set(Vector3D(0.0f, 0.0f, 1.0f));
    data._normals[1].set(Vector3D(0.0f, 0.0f, 1.0f));
    data._normals[2].set(Vector3D(0.0f, 0.0f, 1.0f));
    data._normals[3].set(Vector3D(0.0f, 0.0f, 1.0f));

    data._indices[0] = 0;
    data._indices[1] = 1;
    data._indices[2] = 2;
    data._indices[3] = 2;
    data._indices[4] = 3;
    data._indices[5] = 0;*/
}