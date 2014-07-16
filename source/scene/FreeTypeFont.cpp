#include "FreeTypeFont.h"
#include "FontManager.h"
#include "utils/Logger.h"

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

    m_data->addCharsToMap(m_text);
    CFreeTypeFont::build();

    //m_geometry->init();

    m_initialiazed = true;
}

void CFreeTypeFont::update(f64 time)
{
    CFont::update(time);
}

void CFreeTypeFont::render()
{
    CFont::render();
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

    m_data->addCharsToMap(m_text);
    //m_data->setSize()
    CFreeTypeFont::build();

    //m_geometry->refresh();
}

void CFreeTypeFont::build()
{
}