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
using namespace stream;

CTextureManager::CTextureManager()
{
    TResourceLoader::registerPath("../../../../data/");
    TResourceLoader::registerPath("../../../../../data/");
    TResourceLoader::registerPath("data/");

    std::initializer_list<std::string> ext = { ".png", ".bmp", ".tga", ".jpg" };
    TResourceLoader::registerDecoder(std::make_shared<CTextureResILDecoder>(ext));
}

CTextureManager::~CTextureManager()
{
    TResourceLoader::unloadAll();
}

void CTextureManager::add(CTexture* texture)
{
    std::string name = texture->getResourseName();
    const CTexture* findTexture = TResourceLoader::get(name);
    if (!findTexture)
    {
        TResourceLoader::insert(texture, name);
    }
}

void CTextureManager::add(const CTexture* texture)
{
    std::string name = texture->getResourseName();
    const CTexture* findTexture = TResourceLoader::get(name);
    if (!findTexture)
    {
        TResourceLoader::insert(const_cast<CTexture*>(texture), name);
    }
}

CTexture* CTextureManager::load(const std::string files[6], const std::string& alias)
{
    std::string aliasNameStr = "";
    for (u32 i = 0; i < 6; ++i)
    {
        aliasNameStr.append(files[i]);
    }
    std::transform(aliasNameStr.begin(), aliasNameStr.end(), aliasNameStr.begin(), ::tolower);

    CTexture* findTexture = TResourceLoader::get(alias.empty() ? aliasNameStr : alias);
    if (findTexture)
    {
        return findTexture;
    }
    else
    {
        CTexture* cubeTexture = RENDERER->makeSharedTexture();
        cubeTexture->setStream(stream::CStreamManager::createMemoryStream());

        bool wasFound = false;
        for (std::string& path : m_pathes)
        {
            if (wasFound)
            {
                break;
            }

            for (u32 i = 0; i < 6; ++i)
            {
                std::string nameStr = files[i];
                std::transform(nameStr.begin(), nameStr.end(), nameStr.begin(), ::tolower);

                const std::string fullName = path + nameStr;
                const bool isFileExist = stream::FileStream::isFileExist(fullName);
                if (isFileExist)
                {
                    wasFound = true;
                    const stream::FileStreamPtr stream = stream::CStreamManager::createFileStream(fullName, stream::FileStream::e_in);
                    if (stream->isOpen())
                    {
                        std::string fileExtension = CTextureManager::getFileExtension(nameStr);
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

                        CTextureManager::addStreamToCubeTexture(cubeTexture, resource->getStream());

                        delete resource;
                        resource = nullptr;
                    }
                    else
                    {
                        LOG_ERROR("CTextureManager: error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }
                }
                else
                {
                    continue;
                }
            }
        }

        if (wasFound)
        {
            cubeTexture->m_target = ETextureTarget::eTextureCubeMap;
            cubeTexture->setResourseName(aliasNameStr);
            cubeTexture->setResourseFolder("");

            if (!cubeTexture->load())
            {
                delete cubeTexture;
                cubeTexture = nullptr;

                LOG_ERROR("CTextureManager: Streaming error read file [%s]", aliasNameStr.c_str());
                return nullptr;
            }

            if (!cubeTexture->create())
            {
                delete cubeTexture;
                cubeTexture = nullptr;

                LOG_ERROR("CTextureManager: Error to Create Texture file [%s]", aliasNameStr.c_str());
                return nullptr;
            }

            TResourceLoader::insert(cubeTexture, alias.empty() ? aliasNameStr : alias);

            LOG_INFO("CTextureManager: File [%s] success loaded", aliasNameStr.c_str());
            return cubeTexture;
        }

        LOG_WARNING("CTextureManager: Files [%s] not found", aliasNameStr.c_str());
    }

    return nullptr;
}

CTexture* CTextureManager::load(const std::string& file, const std::string& alias)
{
    std::string nameStr = file;
    std::transform(file.begin(), file.end(), nameStr.begin(), ::tolower);

    CTexture* findTexture = TResourceLoader::get(alias.empty() ? nameStr : alias);
    if (findTexture)
    {
        return findTexture;
    }
    else
    {
        for (std::string& path : m_pathes)
        {
            const std::string fullName = path + nameStr;
            const bool isFileExist = stream::FileStream::isFileExist(fullName);
            if (isFileExist)
            {
                const stream::FileStreamPtr stream = stream::CStreamManager::createFileStream(fullName, stream::FileStream::e_in);
                if (stream->isOpen())
                {
                    std::string fileExtension = CTextureManager::getFileExtension(nameStr);
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
                        delete texture;
                        texture = nullptr;

                        LOG_ERROR("CTextureManager: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    if (!texture->create())
                    {
                        delete texture;
                        texture = nullptr;

                        LOG_ERROR("CTextureManager: Error to Create Texture file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    TResourceLoader::insert(texture, alias.empty() ? fullName : alias);

                    LOG_INFO("CTextureManager: File [%s] success loaded", fullName.c_str());
                    return texture;
                }
            }
        }

        LOG_WARNING("CTextureManager: File [%s] not found", nameStr.c_str());
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

void v3d::scene::CTextureManager::addStreamToCubeTexture(renderer::CTexture* texture, const stream::IStreamPtr& stream)
{
    const IStreamPtr& streamData = texture->getStream();
    stream->seekBeg(0);

    streamData->seekBeg(streamData->size());
    streamData->write(stream->map(stream->size()), stream->size(), sizeof(c8));
    stream->unmap();
}

std::string v3d::scene::CTextureManager::getFileExtension(const std::string& fullFileName)
{
    std::string fileExtension = "";

    const size_t pos = fullFileName.find('.');
    if (pos != std::string::npos)
    {
        fileExtension = std::string(fullFileName.begin() + pos, fullFileName.end());
    }

    return fileExtension;
}
