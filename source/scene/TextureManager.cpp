#include "TextureManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "Engine.h"
#include "decoders/TextureResILDecoder.h"

using namespace v3d;
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
    unloadAll();
}

const TexturePtr CTextureManager::get(const std::string& name)
{
    TextureMap::const_iterator it = m_textures.find(name);
    if (it != m_textures.end())
    {
        return it->second;
    }

    return nullptr;
}

const TexturePtr CTextureManager::load(const std::string* files[6])
{
    //TODO: load cubemap

    return nullptr;
}

const TexturePtr CTextureManager::load(const std::string& file, const std::string& alias)
{
    std::string nameStr = file;
    std::transform(file.begin(), file.end(), nameStr.begin(), ::tolower);

    const TexturePtr findTexture = CTextureManager::get(alias.empty() ? nameStr : alias);
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
                    auto predCanDecode = [fileExtension](const DecoderPtr& decoder) -> bool
                    {
                        return decoder->isExtensionSupported(fileExtension);
                    };
                    
                    auto iter = std::find_if(m_decoders.begin(), m_decoders.end(), predCanDecode);
                    if (iter == m_decoders.end())
                    {
                        LOG_ERROR("CTextureManager: Format not supported file [%s]", nameStr.c_str());
                        stream->close();

                        return nullptr;
                    }

                    const DecoderPtr& decoder = (*iter);
                    stream::ResourcePtr resource = decoder->decode(stream);
                    stream->close();

                    if (!resource)
                    {
                        LOG_ERROR("CTextureManager: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    TexturePtr texture = std::static_pointer_cast<CTexture>(resource);

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

                    m_textures.insert(std::map<std::string, TexturePtr>::value_type(alias.empty() ? nameStr : alias, texture));

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

void CTextureManager::unload(const std::string& name)
{
    auto it = m_textures.find(name);

    if (it != m_textures.end())
    {
        m_textures.erase(it);
    }
}

void CTextureManager::unload(const TexturePtr& texture)
{
    auto predDelete = [texture](const std::pair<std::string, TexturePtr>& pair) -> bool
    {
        return pair.second == texture;
    };

    auto it = std::find_if(m_textures.begin(), m_textures.end(), predDelete);

    if (it != m_textures.end())
    {
        m_textures.erase(it);
    }
}

void CTextureManager::unloadAll()
{
    m_textures.clear();
}

void CTextureManager::registerPath(const std::string& path)
{
    m_pathes.push_back(path);
}

void CTextureManager::unregisterPath(const std::string& path)
{
    auto it = std::find(m_pathes.begin(), m_pathes.end(), path);
    if (it != m_pathes.end())
    {
        m_pathes.erase(std::remove(m_pathes.begin(), m_pathes.end(), *it), m_pathes.end());
    }
}

void CTextureManager::registerDecoder(DecoderPtr decoder)
{
    m_decoders.push_back(decoder);
}

void CTextureManager::unregisterDecoder(DecoderPtr& decoder)
{
    auto it = std::find(m_decoders.begin(), m_decoders.end(), decoder);
    if (it != m_decoders.end())
    {
        m_decoders.erase(std::remove(m_decoders.begin(), m_decoders.end(), *it), m_decoders.end());
    }
}

TexturePtr CTextureManager::createTexture2DFromData(const Dimension2D& size, EImageFormat format, EImageType type, void* data)
{
    renderer::TexturePtr texture = RENDERER->makeSharedTexture();

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

TexturePtr CTextureManager::createTexture2DMSAA(const Dimension2D& size, EImageFormat format, EImageType type)
{
    renderer::TexturePtr texture = RENDERER->makeSharedTexture();

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

void CTextureManager::copyToTexture2D(const TexturePtr& texture, const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data)
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
