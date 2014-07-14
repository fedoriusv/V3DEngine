#include "FontManager.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::scene;

CFontManager::CFontManager()
{
    CFontManager::registerPath("../../../../data/");
    CFontManager::registerPath("data/");
}

CFontManager::~CFontManager()
{
    CFontManager::unloadAll();
}

void CFontManager::add(const renderer::FreeTypeDataPtr& font)
{
    std::string name = font->getResourseName();
    m_fontsData.insert(std::map<std::string, renderer::FreeTypeDataPtr>::value_type(name, font));
}

const renderer::FreeTypeDataPtr& CFontManager::get(const std::string& name)
{
    return m_fontsData[name];
}

const renderer::FreeTypeDataPtr& CFontManager::load(const std::string& name)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    auto it = m_fontsData.find(nameStr);
    if (it != m_fontsData.end())
    {
        return it->second;
    }
    else
    {
        renderer::FreeTypeDataPtr font = std::make_shared<renderer::CFreeTypeData>();
        font->load();

        ///TODO
        m_fontsData.insert(std::map<std::string, renderer::FreeTypeDataPtr>::value_type(nameStr, font));
        return font;
    }

    return nullptr;
}

void CFontManager::unload(const std::string& name)
{
    auto it = m_fontsData.find(name);

    if (it != m_fontsData.end())
    {
        m_fontsData.erase(it);
    }
}

void CFontManager::unload(const renderer::FreeTypeDataPtr& font)
{
    auto predDelete = [font](const std::pair<std::string, renderer::FreeTypeDataPtr>& pair) -> bool
    {
        return pair.second == font;
    };

    auto it = std::find_if(m_fontsData.begin(), m_fontsData.end(), predDelete);

    if (it != m_fontsData.end())
    {
        m_fontsData.erase(it);
    }
}

void CFontManager::unloadAll()
{
    m_fontsData.clear();
}

void CFontManager::registerPath(const std::string& path)
{
    m_pathes.push_back(path);
}

void CFontManager::unregisterPath(const std::string& path)
{
    auto it = std::find(m_pathes.begin(), m_pathes.end(), path);
    if (it != m_pathes.end())
    {
        m_pathes.erase(std::remove(m_pathes.begin(), m_pathes.end(), *it), m_pathes.end());
    }
}