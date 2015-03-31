#include "ShaderManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace scene;
using namespace stream;
using namespace resources;

CShaderManager::CShaderManager()
{
    CShaderManager::registerPath("../../../../data/");
    CShaderManager::registerPath("../../../../../data/");
    CShaderManager::registerPath("data/");
}

CShaderManager::~CShaderManager()
{
    unloadAll();
}

void CShaderManager::add(const ShaderSourceDataPtr& shader)
{
    std::string name = shader->getName();
    m_shaders.insert(std::map<std::string, ShaderSourceDataPtr>::value_type(name, shader));
}

const ShaderSourceDataPtr CShaderManager::load(const std::string& name)
{
    //TODO:
    return nullptr;
}

void CShaderManager::unload(const std::string& name)
{
    auto it = m_shaders.find(name);

    if (it != m_shaders.end())
    {
        m_shaders.erase(it);
    }
}

void CShaderManager::unload(const ShaderSourceDataPtr& shader)
{
    auto predDelete = [shader](const std::pair<std::string, ShaderSourceDataPtr>& pair) -> bool
    {
        return pair.second == shader;
    };

    auto it = std::find_if(m_shaders.begin(), m_shaders.end(), predDelete);

    if (it != m_shaders.end())
    {
        m_shaders.erase(it);
    }
}

void CShaderManager::unloadAll()
{
    m_shaders.clear();
}

void CShaderManager::registerPath(const std::string& path)
{
    m_pathes.push_back(path);
}

void CShaderManager::unregisterPath(const std::string& path)
{
    auto it = std::find(m_pathes.begin(), m_pathes.end(), path);
    if (it != m_pathes.end())
    {
        m_pathes.erase(std::remove(m_pathes.begin(), m_pathes.end(), *it), m_pathes.end());
    }
}
