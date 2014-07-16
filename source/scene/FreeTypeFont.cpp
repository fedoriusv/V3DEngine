#include "FreeTypeFont.h"
#include "FontManager.h"
#include "utils/Logger.h"

#include "GL/glew.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CFreeTypeFont::CFreeTypeFont(const std::string& font)
    : m_font(font)
    , m_data(nullptr)
{
}

CFreeTypeFont::~CFreeTypeFont()
{
}

void CFreeTypeFont::init()
{
    CFont::init();

    if (m_font.empty())
    {
        LOG_WARRNING("Font name Empty");
        return;
    }

    m_data = CFontManager::getInstance()->load(m_font);
    if (!m_data)
    {
        LOG_ERROR("FreeTypeFont: Can not load font: %s", m_font.c_str());
        ASSERT(false && "FreeTypeFont: Can not load font");
        return;
    }

    m_text = "ABC";
    m_data->addCharsToMap(m_text);
    CFreeTypeFont::build();

    m_geometry->setDrawMode(CGeometry::eTriangles);
    m_geometry->init();

    m_material->setTexture(0, m_data->m_charMaterial[0]);

    m_initialiazed = true;
}

void CFreeTypeFont::update(f64 time)
{
    CFont::update(time);
}

void CFreeTypeFont::render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CFont::render();

    glDisable(GL_BLEND);
}

void CFreeTypeFont::setText(const std::string& text)
{
    m_text = text;
    CFreeTypeFont::refresh();
}

void CFreeTypeFont::setSize(const u32 size)
{
    m_size = size;
    CFreeTypeFont::refresh();
}

void CFreeTypeFont::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    //m_data->addCharsToMap(m_text);
    //m_data->setSize()
    CFreeTypeFont::build();

    //m_geometry->refresh();
}

void CFreeTypeFont::build()
{
    m_data->loadCharList();

    SVertexData& data = m_geometry->getData();
    /*data.malloc(m_text.size() * 4);

    for (std::string::const_iterator chr = m_text.begin(); chr < m_text.end(); ++chr)
    {
        const CFreeTypeData::SCharDesc& info = m_data->getCharInfo(*chr);

        info.
    }

    u32 index = 0;
    for (std::string::const_iterator chr = m_text.begin(); chr < m_text.end(); ++chr)
    {
        const CFreeTypeData::SCharDesc& info = m_data->getCharInfo(*chr);

        data._vertices[index].set(Vector3D(0.0f, 0.0f, 0.0f));
        data._vertices[index * 1].set(Vector3D(0.0f, 0.0f, 0.0f));
        data._vertices[index * 2].set(Vector3D(0.0f, 0.0f, 0.0f));
        data._vertices[index * 3].set(Vector3D(0.0f, 0.0f, 0.0f));

        data._texCoords.at(0)[index].set(Vector2D(0, 0));
        data._texCoords.at(0)[index * 1].set(Vector2D(0, 0));
        data._texCoords.at(0)[index * 2].set(Vector2D(0, 0));
        data._texCoords.at(0)[index * 3].set(Vector2D(0, 0));

        data._normals[index].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 1].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 2].set(Vector3D(0.0f, 0.0f, 1.0f));
        data._normals[index * 3].set(Vector3D(0.0f, 0.0f, 1.0f));

        ++index;*/

    data.malloc(4,6);

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
    data._indices[5] = 0;

}