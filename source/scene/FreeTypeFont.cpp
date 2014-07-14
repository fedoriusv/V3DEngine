#include "FreeTypeFont.h"

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

bool CFreeTypeFont::load(const std::string& font)
{
    //TODO:

    return false;
}

void CFreeTypeFont::init()
{
    CFont::init();

    //
}

void CFreeTypeFont::update(f64 time)
{
    CFont::update(time);
}

void CFreeTypeFont::render()
{
    CFont::render();
}