#include "TextureManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "Engine.h"
#include "decoders/TextureResILDecoder.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace renderer;
using namespace decoders;

CTextureManager::CTextureManager()
{
    CTextureManager::registerPath("../../../../data/");
    CTextureManager::registerPath("../../../../../data/");
    CTextureManager::registerPath("data/");

    std::initializer_list<std::string> ext = { ".png", ".bmp", ".tga", ".jpg" };
    CTextureManager::registerDecoder(std::make_shared<CTextureResILDecoder>(ext));
}

CTextureManager::~CTextureManager()
{
}

void CTextureManager::add(const CTexture* texture)
{
    std::string name = texture->getResourseName();
    TResourceLoader::insert(texture, name);
}

const CTexture* CTextureManager::load(const std::string* files[6])
{
    //TODO: load cubemap

    return nullptr;
}

const CTexture* CTextureManager::load(const std::string& file, const std::string& alias)
{
    std::string nameStr = file;
    std::transform(file.begin(), file.end(), nameStr.begin(), ::tolower);

    const CTexture* findTexture = TResourceLoader::get(alias.empty() ? nameStr : alias);
    if (findTexture)
    {
        return findTexture;
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
                const stream::FileStreamPtr stream = stream::CStreamManager::createFileStream(fullName, stream::FileStream::e_in);
                if (stream->isOpen())
                {
                    const DecoderPtr decoder = TResourceLoader::findDecoder(fileExtension);
                    if (!decoder)
                    {
                        LOG_ERROR("CTextureManager: Format not supported file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    stream::CResource* resource = decoder->decode(stream);
                    stream->close();

                    if (!resource)
                    {
                        LOG_ERROR("CTextureManager: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    CTexture* texture = static_cast<CTexture*>(resource);

                    texture->m_target = ETextureTarget::eTexture2D;
                    texture->setResourseName(fullName);

                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    texture->setResourseFolder(fullPath);

                    if (!texture->load())
                    {
                        LOG_ERROR("CTextureManager: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    if (!texture->create())
                    {
                        LOG_ERROR("CTextureManager: Error to Create Texture file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    TResourceLoader::insert(texture, alias.empty() ? nameStr : alias);
                    LOG_INFO("CTextureManager: File [%s] success loaded", fullName.c_str());
                    return texture;
                }
            }
            else
            {
                LOG_WARNING("CTextureManager: File [%s] not found", fullName.c_str());
            }
        }
    }

    return nullptr;
}

CTexture* CTextureManager::createTexture2DFromData(const Dimension2D& size, EImageFormat format, EImageType type, void* data)
{
    renderer::CTexture* texture = RENDERER->makeSharedTexture();

    texture->m_target = ETextureTarget::eTexture2D;

    texture->m_data.resize(1);
    texture->m_data[0]._width = size.width;
    texture->m_data[0]._height = size.height;
    texture->m_data[0]._depth = 1;
    texture->m_data[0]._format = format;
    texture->m_data[0]._type = type;
    texture->m_data[0]._data = data;

    texture->create();

    return texture;
}

CTexture* CTextureManager::createTexture2DMSAA(const Dimension2D& size, EImageFormat format, EImageType type)
{
    renderer::CTexture* texture = RENDERER->makeSharedTexture();

    texture->m_target = ETextureTarget::eTexture2DMSAA;

    texture->m_data.resize(1);
    texture->m_data[0]._width = size.width;
    texture->m_data[0]._height = size.height;
    texture->m_data[0]._depth = 1;
    texture->m_data[0]._format = format;
    texture->m_data[0]._type = type;
    texture->m_data[0]._data = nullptr;

    texture->create();

    return texture;
}

void CTextureManager::copyToTexture2D(CTexture* texture, const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data)
{
    if (!texture || texture->getTextureID() <= 0)
    {
        LOG_ERROR("TextureManager: Invalid Texture");
        return;
    }

    if (texture->getTarget() != ETextureTarget::eTexture2D)
    {
        LOG_ERROR("TextureManager: Invalid Target Texture. Only Texture2D");
        return;
    }

    texture->copyToTexture2D(offset, size, format, data);
}
