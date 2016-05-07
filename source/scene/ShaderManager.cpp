#include "ShaderManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

namespace v3d
{
namespace scene
{

using namespace stream;
using namespace resources;
using namespace renderer;

CShaderManager::CShaderManager()
{
    TResourceLoader::registerPath("../../../../data/shaders/");
    TResourceLoader::registerPath("../../../../../data/shaders/");
    TResourceLoader::registerPath("data/shaders/");
}

CShaderManager::~CShaderManager()
{
    TResourceLoader::unloadAll();
}

void CShaderManager::add(const CShaderSourceData* shader)
{
    std::string name = shader->getName();
    TResourceLoader::insert(shader, name);
}

const CShaderSourceData* CShaderManager::load(const std::string& name, const std::string& alias)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    const CShaderSourceData* findShader = TResourceLoader::get(alias.empty() ? nameStr : alias);
    if (findShader)
    {
        return findShader;
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
                    CShaderSourceData* shader = new CShaderSourceData();

                    shader->init(stream);
                    shader->setResourseName(fullName);
                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    shader->setResourseFolder(fullPath);

                    if (!shader->load())
                    {
                        LOG_ERROR("CShaderManager: Streaming error read file [%s]", nameStr.c_str());
                        stream->close();

                        return nullptr;
                    }
                    stream->close();

                    TResourceLoader::insert(shader, alias.empty() ? nameStr : alias);
                    LOG_INFO("CShaderManager: File [%s] success loaded", fullName.c_str());
                    return shader;
                }
            }
        }
    }

    LOG_WARNING("CShaderManager::load: File [%s] not found", name.c_str());
    return nullptr;
}

void CShaderManager::add(const ShaderPtr shader)
{
    std::size_t hash = shader->m_data->getHash();
    if (m_shaderList.find(hash) == m_shaderList.cend())
    {
        m_shaderList.insert(std::map<std::size_t, ShaderPtr>::value_type(hash, shader));
    }
}

const ShaderWPtr CShaderManager::get(const ShaderPtr shader) const
{
    std::size_t hash = shader->m_data->getHash();
    ShaderHashMap::const_iterator iter = m_shaderList.find(hash);
    if (iter != m_shaderList.cend())
    {
        return (*iter).second;
    }

    return ShaderWPtr();
}

renderer::ShaderWPtr CShaderManager::get(std::size_t hash) const
{
    ShaderHashMap::const_iterator iter = m_shaderList.find(hash);
    if (iter != m_shaderList.cend())
    {
        return (*iter).second;
    }

    return ShaderWPtr();
}

} //namespace scene
} //namespace v3d
