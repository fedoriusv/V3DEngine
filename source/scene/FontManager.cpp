#include "FontManager.h"
#include "utils/Logger.h"
#include "resources/BitmapFontData.h"
#include "resources/VectorFontData.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::resources;

CFontManager::CFontManager()
{
    CFontManager::registerPath("../../../../data/");
    CFontManager::registerPath("data/");
}

CFontManager::~CFontManager()
{
    CFontManager::unloadAll();
}

void CFontManager::add(const FontDataPtr& font)
{
    std::string name = font->getResourseName();
    m_fontsData.insert(std::map<std::string, FontDataPtr>::value_type(name, font));
}

const FontDataPtr& CFontManager::get(const std::string& name)
{
    return m_fontsData[name];
}

const FontDataPtr CFontManager::load(const std::string& name)
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
                stream::FileStreamPtr stream = stream::CStreamManager::createFileStream(fullName, stream::FileStream::e_in);

                if (stream->isOpen())
                {
                    FontDataPtr font = nullptr;

                    if (fileExtension == ".fnt")
                    {
                        font = std::make_shared<CBitmapFontData>(nameStr);
                        font->setFontType(CFontData::EFontType::eBitmapFont);
                    }
                    else if (fileExtension == ".ttf")
                    {
                        font = std::make_shared<CVectorFontData>(nameStr);
                        font->setFontType(CFontData::EFontType::eVectorFont);
                    }

                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    font->setResourseFolder(fullPath);

                    if (!font)
                    {
                        LOG_ERROR("CFontManager: FreeTypeFont Load error [%s]", nameStr.c_str());
                        stream->close();

                        return nullptr;
                    }

                    font->init(stream);
                    font->setResourseName(fullName);

                    if (!font->load())
                    {
                        LOG_ERROR("CFontManager: FreeTypeFont Load error [%s]", nameStr.c_str());
                        stream->close();

                        return nullptr;
                    }
                    stream->close();

                    m_fontsData.insert(std::map<std::string, FontDataPtr>::value_type(nameStr, font));

                    return font;
                }
            }
            else
            {
                LOG_WARNING("CFontManager: File [%s] not found", fullName.c_str());
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

void CFontManager::unload(const FontDataPtr& font)
{
    auto predDelete = [&font](const std::pair<std::string, FontDataPtr>& pair) -> bool
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
