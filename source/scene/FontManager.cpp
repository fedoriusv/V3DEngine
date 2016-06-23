#include "FontManager.h"
#include "utils/Logger.h"
#include "resources/BitmapFontData.h"
#include "resources/VectorFontData.h"
#include "stream/StreamManager.h"

namespace v3d
{
namespace scene
{

using namespace resources;

CFontManager::CFontManager()
{
    TResourceLoader::registerPath("../../../../data/");
    TResourceLoader::registerPath("data/");
}

CFontManager::~CFontManager()
{
}

void CFontManager::add(const CFontData* font)
{
    const std::string& name = font->getResourseName();
    TResourceLoader::insert(font, name);
}

const CFontData* CFontManager::load(const std::string& name, const std::string& alias)
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
                    //TODO: Will need to create decoders
                    CFontData* font = nullptr;
                    if (fileExtension == ".fnt")
                    {
                        font = new CBitmapFontData(nameStr);
                        font->setFontType(CFontData::EFontType::eBitmapFont);
                    }
#if USED_FREETYPE
                    else if (fileExtension == ".ttf")
                    {
                        font = new CVectorFontData(nameStr);
                        font->setFontType(CFontData::EFontType::eVectorFont);
                    }
#endif //USED_FREETYPE
                    if (!font)
                    {
                        LOG_ERROR("CFontManager: FreeTypeFont Load error [%s]", nameStr.c_str());
                        stream->close();

                        return nullptr;
                    }

                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    font->setResourseFolder(fullPath);

                    font->init(stream);
                    font->setResourseName(fullName);

                    if (!font->load())
                    {
                        LOG_ERROR("CFontManager: FreeTypeFont Load error [%s]", nameStr.c_str());
                        return nullptr;
                    }
                    stream->close();

                    TResourceLoader::insert(font, alias.empty() ? nameStr : alias);
                    LOG_INFO("CFontManager: File [%s] success loaded", fullName.c_str());

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

} //namespace scene
} //namespace v3d
