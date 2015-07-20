#include "FontManager.h"
#include "utils/Logger.h"
#include "resources/BitmapFontData.h"
#include "resources/VectorFontData.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace scene;
using namespace resources;

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
    m_resources.insert(std::map<std::string, FontDataPtr>::value_type(name, font));
}

const FontDataPtr CFontManager::load(const std::string& name, const std::string& alias)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    auto it = m_resources.find(nameStr);
    if (it != m_resources.end())
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

                    m_resources.insert(std::map<std::string, FontDataPtr>::value_type(nameStr, font));

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