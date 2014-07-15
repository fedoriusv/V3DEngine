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