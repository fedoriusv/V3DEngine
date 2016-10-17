#include "TextureManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "Engine.h"
#include "decoders/TextureResILDecoder.h"
#include "resources/Image.h"

namespace v3d
{
namespace scene
{

using namespace core;
using namespace renderer;
using namespace decoders;
using namespace stream;
using namespace resources;

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
}

TexturePtr CTextureManager::load(const std::string files[6], const std::string& alias)
{
    std::string aliasNameStr("");
    for (u32 i = 0; i < 6; ++i)
    {
        aliasNameStr.append(files[i]);
    }
    std::transform(aliasNameStr.begin(), aliasNameStr.end(), aliasNameStr.begin(), ::tolower);

    TexturePtr findTexture = TResourceLoader::get(alias.empty() ? aliasNameStr : alias);
    if (findTexture)
    {
        return findTexture;
    }
    else
    {
        bool wasFoundPath = false;
        const CImage* cubeImage[6];

        for (const std::string& path : m_pathes)
        {
            if (wasFoundPath)
            {
                break;
            }

            for (u32 i = 0; i < 6; ++i)
            {
                std::string nameString(files[i]);
                std::transform(nameString.begin(), nameString.end(), nameString.begin(), ::tolower);

                const std::string fullName(path + nameString);
                const bool isFileExist = stream::FileStream::isFileExist(fullName);
                if (isFileExist)
                {
                    wasFoundPath = true;
                    const stream::FileStreamPtr stream = stream::StreamManager::createFileStream(fullName, stream::FileStream::e_in);
                    if (stream->isOpen())
                    {
                        std::string fileExtension = CTextureManager::getFileExtension(nameString);
                        const DecoderPtr decoder = TResourceLoader::findDecoder(fileExtension);
                        if (!decoder)
                        {
                            LOG_ERROR("CTextureManager: Format not supported file [%s]", nameString.c_str());
                            return nullptr;
                        }

                        stream::IResource* resource = decoder->decode(stream);
                        stream->close();

                        if (!resource)
                        {
                            LOG_ERROR("CTextureManager: Streaming error read file [%s]", nameString.c_str());
                            return nullptr;
                        }

                        CImage* image = static_cast<CImage*>(resource);
                        const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                        image->setResourseFolder(fullPath);
                        image->setResourseName(fullName);

                        if (!image->load())
                        {
                            delete image;
                            image = nullptr;

                            LOG_ERROR("CTextureManager: Streaming error read file [%s]", nameString.c_str());
                            return nullptr;
                        }

                        cubeImage[i] = image;
                    }
                    else
                    {
                        LOG_ERROR("CTextureManager: error read file [%s]", nameString.c_str());
                        return nullptr;
                    }
                }
                else
                {
                    LOG_ERROR("CTextureManager: file not found [%s]", nameString.c_str());
                    return nullptr;
                }
            }
        }

        if (wasFoundPath)
        {

            TexturePtr cubeTexture = CTextureManager::createCubeTextureFromImages(cubeImage);
            for (u32 i = 0; i < 6; ++i)
            {
                delete cubeImage[i];
                cubeImage[i] = nullptr;
            }

            if (!cubeTexture)
            {

                LOG_ERROR("CTextureManager: Create cube texture form images [%s] is failed", aliasNameStr.c_str());
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

TexturePtr CTextureManager::load(const std::string& file, const std::string& alias)
{
    std::string nameString(file);
    std::transform(file.begin(), file.end(), nameString.begin(), ::tolower);

    std::string aliasString(alias);
    std::transform(alias.begin(), alias.end(), aliasString.begin(), ::tolower);

    TexturePtr findTexture = TResourceLoader::get(alias.empty() ? nameString : aliasString);
    if (findTexture)
    {
        return findTexture;
    }
    else
    {
        for (const std::string& path : m_pathes)
        {
            const std::string fullName(path + nameString);
            const bool isFileExist = stream::FileStream::isFileExist(fullName);
            if (isFileExist)
            {
                const stream::FileStreamPtr stream = stream::StreamManager::createFileStream(fullName, stream::FileStream::e_in);
                if (stream->isOpen())
                {
                    std::string fileExtension = CTextureManager::getFileExtension(nameString);
                    const DecoderPtr decoder = TResourceLoader::findDecoder(fileExtension);
                    if (!decoder)
                    {
                        LOG_ERROR("CTextureManager: Format not supported file [%s]", nameString.c_str());
                        return nullptr;
                    }

                    stream::IResource* resource = decoder->decode(stream);
                    stream->close();

                    if (!resource)
                    {
                        LOG_ERROR("CTextureManager: Streaming error read file [%s]", nameString.c_str());
                        return nullptr;
                    }

                    CImage* image = static_cast<CImage*>(resource);
                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    image->setResourseFolder(fullPath);
                    image->setResourseName(fullName);

                    if (!image->load())
                    {
                        delete image;
                        image = nullptr;

                        LOG_ERROR("CTextureManager: Streaming error read file [%s]", nameString.c_str());
                        return nullptr;
                    }

                    TexturePtr texture = CTextureManager::createTextureFromImage(image);
                    delete image;
                    image = nullptr;

                    if (!texture)
                    {
                        LOG_ERROR("CTextureManager: Error to Create Texture file [%s]", nameString.c_str());
                        return nullptr;
                    }

                    TResourceLoader::insert(texture, alias.empty() ? nameString : aliasString);

                    LOG_INFO("CTextureManager: File [%s] success loaded", fullName.c_str());
                    return texture;
                }
            }
        }

        LOG_WARNING("CTextureManager: File [%s] not found", nameString.c_str());
    }

    return nullptr;
}

TexturePtr CTextureManager::createTextureFromImage(const CImage* image)
{
    if (image && image->isLoaded())
    {
        TexturePtr texure = new Texture(k_useTextureBuffer ? ETextureTarget::eTextureBuffer : ETextureTarget::eTexture2D,
            image->getFormat(), image->getType(),image->getSize(), image->getData(), 0);

        return texure;
    }

    return nullptr;
}

renderer::TexturePtr CTextureManager::createCubeTextureFromImages(const resources::CImage* image[6])
{
    const u8* dataList[6];
    for (u32 i = 0; i < 6; ++i)
    {
        if (!image || !image[i]->isLoaded())
        {
            return nullptr;
        }
        ASSERT(image[0]->getFormat() == image[i]->getFormat(), "Different format");
        ASSERT(image[0]->getType() == image[i]->getType(), "Different type");
        ASSERT(image[0]->getSize() == image[i]->getSize(), "Different size");

        dataList[i] = image[i]->getData();
    }

    TexturePtr texure = new Texture(image[0]->getFormat(), image[0]->getType(), image[0]->getSize(), (const void**)dataList);

    return texure;
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

} //namespace scene
} //namespace v3d
