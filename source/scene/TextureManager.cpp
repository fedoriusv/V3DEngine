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

TextureManager::TextureManager()
{
    TResourceLoader::registerPath("../../../../data/");
    TResourceLoader::registerPath("../../../../../data/");
    TResourceLoader::registerPath("data/");

    std::initializer_list<std::string> ext = { ".png", ".bmp", ".tga", ".jpg" };
    TResourceLoader::registerDecoder(std::make_shared<CTextureResILDecoder>(ext));
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::load(const std::string files[6], const std::string& alias)
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
                        std::string fileExtension = TextureManager::getFileExtension(nameString);
                        const DecoderPtr decoder = TResourceLoader::findDecoder(fileExtension);
                        if (!decoder)
                        {
                            LOG_ERROR("TextureManager: Format not supported file [%s]", nameString.c_str());
                            return nullptr;
                        }

                        stream::IResource* resource = decoder->decode(stream);
                        stream->close();

                        if (!resource)
                        {
                            LOG_ERROR("TextureManager: Streaming error read file [%s]", nameString.c_str());
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

                            LOG_ERROR("TextureManager: Streaming error read file [%s]", nameString.c_str());
                            return nullptr;
                        }

                        cubeImage[i] = image;
                    }
                    else
                    {
                        LOG_ERROR("TextureManager: error read file [%s]", nameString.c_str());
                        return nullptr;
                    }
                }
                else
                {
                    LOG_ERROR("TextureManager: file not found [%s]", nameString.c_str());
                    return nullptr;
                }
            }
        }

        if (wasFoundPath)
        {

            TexturePtr cubeTexture = TextureManager::createCubeTextureFromImages(cubeImage);
            for (u32 i = 0; i < 6; ++i)
            {
                delete cubeImage[i];
                cubeImage[i] = nullptr;
            }

            if (!cubeTexture)
            {

                LOG_ERROR("TextureManager: Create cube texture form images [%s] is failed", aliasNameStr.c_str());
                return nullptr;
            }

            TResourceLoader::insert(cubeTexture, alias.empty() ? aliasNameStr : alias);

            LOG_INFO("TextureManager: File [%s] success loaded", aliasNameStr.c_str());
            return cubeTexture;
        }

        LOG_WARNING("TextureManager: Files [%s] not found", aliasNameStr.c_str());
    }

    return nullptr;
}

TexturePtr TextureManager::load(const std::string& file, const std::string& alias)
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
                    std::string fileExtension = TextureManager::getFileExtension(nameString);
                    const DecoderPtr decoder = TResourceLoader::findDecoder(fileExtension);
                    if (!decoder)
                    {
                        LOG_ERROR("TextureManager: Format not supported file [%s]", nameString.c_str());
                        return nullptr;
                    }

                    stream::IResource* resource = decoder->decode(stream);
                    stream->close();

                    if (!resource)
                    {
                        LOG_ERROR("TextureManager: Streaming error read file [%s]", nameString.c_str());
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

                        LOG_ERROR("TextureManager: Streaming error read file [%s]", nameString.c_str());
                        return nullptr;
                    }

                    TexturePtr texture = TextureManager::createTextureFromImage(image);
                    delete image;
                    image = nullptr;

                    if (!texture)
                    {
                        LOG_ERROR("TextureManager: Error to Create Texture file [%s]", nameString.c_str());
                        return nullptr;
                    }

                    TResourceLoader::insert(texture, alias.empty() ? nameString : aliasString);

                    LOG_INFO("TextureManager: File [%s] success loaded", fullName.c_str());
                    return texture;
                }
            }
        }

        LOG_WARNING("TextureManager: File [%s] not found", nameString.c_str());
    }

    return nullptr;
}

TexturePtr TextureManager::createTextureFromImage(const CImage* image)
{
    if (image && image->isLoaded())
    {
        ETextureTarget target = k_useTextureBuffer ? ETextureTarget::eTextureBuffer : ETextureTarget::eTexture2D;
        if (image->getSize().height == 1)
        {
            target = ETextureTarget::eTexture1D;
        }
        TexturePtr texure = new Texture(target, image->getFormat(), image->getType(), image->getSize(), image->getData(), 3);

        return texure;
    }

    return nullptr;
}

renderer::TexturePtr TextureManager::createCubeTextureFromImages(const resources::CImage* image[6])
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

u32 TextureManager::calculateMipmapDataSize(const core::Dimension3D& size, renderer::EImageFormat format, renderer::EImageType type, u32 mipCount)
{
    u32 componentCount = ImageFormat::componentCount(format);
    u32 typeSize = ImageFormat::typeSize(type);
    core::Dimension3D mipSize = size;

    u32 value = 0;
    for (u32 mip = 0; mip < mipCount; ++mip)
    {
        value += componentCount * typeSize * mipSize.getArea();

        mipSize.width = core::max<u32>(mipSize.width / 2, 1);
        mipSize.height = core::max<u32>(mipSize.height / 2, 1);
        mipSize.depth = core::max<u32>(mipSize.depth / 2, 1);
    }

    return value;
}

void* TextureManager::generateMipMaps(const core::Dimension3D& size, const void* data, renderer::EImageFormat format, renderer::EImageType type, u32 mipCount)
{
    if (size.isNull() || !data)
    {
        return nullptr;
    }

    u32 componentCount = ImageFormat::componentCount(format);
    u32 typeSize = ImageFormat::typeSize(type);
    core::Dimension3D mipSize = size;

    u32 generateSize = TextureManager::calculateMipmapDataSize(size, format, type, mipCount);
    void* generateData = malloc(generateSize);

    u8* nextGenerateData = reinterpret_cast<u8*>(generateData);
    const u8* sourceData = reinterpret_cast<const u8*>(data);
    u32 offset = componentCount * typeSize * mipSize.getArea();
    memcpy(nextGenerateData, data, offset);
    nextGenerateData += offset;

    u32 step = 0;
    for (u32 mip = 1; mip < mipCount; ++mip)
    {
        step = 2 << mip - 1;
        u32 mipAllocSize = TextureManager::culculateMipmapLevelSize(size, format, type, mip);
        u8* allocData = reinterpret_cast<u8*>(malloc(mipAllocSize));

        for (u32 pos = 0; pos < mipSize.getArea(); pos += step)
        {
            memcpy(allocData, data, pos * componentCount * typeSize);
            allocData += componentCount * typeSize;
        }

        mipSize.width = core::max<u32>(mipSize.width / 2, 1);
        mipSize.height = core::max<u32>(mipSize.height / 2, 1);
        mipSize.depth = core::max<u32>(mipSize.depth / 2, 1);

        memcpy(nextGenerateData, allocData, mipAllocSize);
        offset += mipAllocSize;

        free(allocData);
    }

    return generateData;
}

u32 TextureManager::culculateMipmapLevelSize(const core::Dimension3D& size, renderer::EImageFormat format, renderer::EImageType type, u32 level)
{
    u32 componentCount = ImageFormat::componentCount(format);
    u32 typeSize = ImageFormat::typeSize(type);
    core::Dimension3D mipSize = size;

    u32 value = 0;
    for (u32 mip = 0; mip < level; ++mip)
    {
        value = componentCount * typeSize * mipSize.getArea();

        mipSize.width = core::max<u32>(mipSize.width / 2, 1);
        mipSize.height = core::max<u32>(mipSize.height / 2, 1);
        mipSize.depth = core::max<u32>(mipSize.depth / 2, 1);
    }

    return value;
}

std::string TextureManager::getFileExtension(const std::string& fullFileName)
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
