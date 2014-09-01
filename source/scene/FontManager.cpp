#include "FontManager.h"
#include "stream/FileStream.h"
#include "utils/Logger.h"
#include "renderer/BitmapFontData.h"
#include "renderer/VectorFontData.h"

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

void CFontManager::add(const renderer::FontDataPtr& font)
{
    std::string name = font->getResourseName();
    m_fontsData.insert(std::map<std::string, renderer::FontDataPtr>::value_type(name, font));
}

const renderer::FontDataPtr& CFontManager::get(const std::string& name)
{
    return m_fontsData[name];
}

const renderer::FontDataPtr CFontManager::load(const std::string& name)
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
        std::string fileExtension;

        const size_t pos = nameStr.find('.');
        if (pos != std::string::npos)
        {
            fileExtension = std::string(nameStr.begin() + pos, nameStr.end());
        }

        for (std::string& path : m_pathes)
        {
            const std::string fullName = path + nameStr;
            const bool isFileExist = stream::FileStream::isFileExist(fullName);
            if (isFileExist)
            {
                stream::FileStream* stream = new stream::FileStream(fullName, stream::FileStream::e_in);

                if (stream->isOpen())
                {
                    renderer::FontDataPtr font = nullptr;

                    if (fileExtension == ".fnt")
                    {
                        font = std::make_shared<renderer::CBitmapFontData>(nameStr);
                        font->setFontType(renderer::CFontData::EFontType::eBitmapFont);
                    }
                    else if (fileExtension == ".ttf")
                    {
                        font = std::make_shared<renderer::CVectorFontData>(nameStr);
                        font->setFontType(renderer::CFontData::EFontType::eVectorFont);
                    }

                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    font->setResourseFolder(fullPath);

                    if (!font)
                    {
                        LOG_ERROR("FreeTypeFont Load error [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    font->init(stream);
                    font->setResourseName(fullName);

                    if (!font->load())
                    {
                        LOG_ERROR("FreeTypeFont Load error [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    m_fontsData.insert(std::map<std::string, renderer::FontDataPtr>::value_type(nameStr, font));
                    return font;
                }
            }
        }
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

void CFontManager::unload(const renderer::FontDataPtr& font)
{
    auto predDelete = [font](const std::pair<std::string, renderer::FontDataPtr>& pair) -> bool
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