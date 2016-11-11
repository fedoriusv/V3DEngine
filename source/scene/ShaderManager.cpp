#include "ShaderManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "decoders/ShaderSpirVDecoder.h"

namespace v3d
{
namespace scene
{

using namespace stream;
using namespace resources;
using namespace renderer;

ShaderManager::ShaderManager()
{
    TResourceLoader::registerPath("../../../../data/shaders/");
    TResourceLoader::registerPath("../../../../../data/shaders/");
    TResourceLoader::registerPath("data/shaders/");

    std::initializer_list<std::string> extSrc = { ".vert", ".frag", ".tesc", ".tese", ".geom", ".comp" };
    std::initializer_list<std::string> extBin = { ".spirv" };
    TResourceLoader::registerDecoder(std::make_shared<decoders::ShaderSpirVDecoder>(extSrc, extBin));
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::add(const ShaderData* shader)
{
    std::string name = shader->getName();
    TResourceLoader::insert(shader, name);
}

const ShaderData* ShaderManager::load(const std::string& name, const std::string& alias)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    const ShaderData* findShader = TResourceLoader::get(alias.empty() ? nameStr : alias);
    if (findShader)
    {
        return findShader;
    }
    else
    {
        for (std::string& path : m_pathes)
        {
            const std::string fullName = path + nameStr;
            const bool isFileExist = stream::FileStream::isFileExist(fullName);
            if (isFileExist)
            {
                const stream::FileStreamPtr stream = stream::StreamManager::createFileStream(fullName, stream::FileStream::e_in);
                if (stream->isOpen())
                {
                    std::string fileExtension = ShaderManager::getFileExtension(nameStr);
                    const decoders::DecoderPtr decoder = TResourceLoader::findDecoder(fileExtension);
                    if (!decoder)
                    {
                        LOG_ERROR("ShaderManager: Extension not supported. File [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    stream::IResource* resource = decoder->decode(stream);
                    stream->close();

                    if (!resource)
                    {
                        LOG_ERROR("ShaderManager: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    ShaderData* shader = static_cast<ShaderData*>(resource);
                    shader->init(stream);
                    shader->setResourseName(fullName);
                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    shader->setResourseFolder(fullPath);

                    if (!shader->load())
                    {
                        LOG_ERROR("ShaderManager: Streaming error read file [%s]", nameStr.c_str());
                        stream->close();

                        return nullptr;
                    }
                    stream->close();

                    TResourceLoader::insert(shader, alias.empty() ? nameStr : alias);
                    LOG_INFO("ShaderManager: File [%s] success loaded", fullName.c_str());
                    return shader;
                }
            }
        }
    }

    LOG_WARNING("ShaderManager::load: File [%s] not found", name.c_str());
    return nullptr;
}

void ShaderManager::add(const ShaderPtr shader)
{
    std::size_t hash = shader->m_data.getHash();
    if (m_shaderList.find(hash) == m_shaderList.cend())
    {
        m_shaderList.insert(std::map<std::size_t, ShaderPtr>::value_type(hash, shader));
    }
}

const ShaderWPtr ShaderManager::get(const ShaderPtr shader) const
{
    std::size_t hash = shader->m_data.getHash();
    ShaderHashMap::const_iterator iter = m_shaderList.find(hash);
    if (iter != m_shaderList.cend())
    {
        return (*iter).second;
    }

    return ShaderWPtr();
}

renderer::ShaderWPtr ShaderManager::get(std::size_t hash) const
{
    ShaderHashMap::const_iterator iter = m_shaderList.find(hash);
    if (iter != m_shaderList.cend())
    {
        return (*iter).second;
    }

    return ShaderWPtr();
}

std::string ShaderManager::getFileExtension(const std::string& fullFileName)
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
