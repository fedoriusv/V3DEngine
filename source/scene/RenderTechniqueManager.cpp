#include "RenderTechniqueManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::stream;
using namespace v3d::renderer;

CRenderTechniqueManager::CRenderTechniqueManager()
{
    CRenderTechniqueManager::registerPath("../../../../data/");
    CRenderTechniqueManager::registerPath("data/");
}

CRenderTechniqueManager::~CRenderTechniqueManager()
{
    unloadAll();
}

void CRenderTechniqueManager::add(const renderer::RenderTechniquePtr& technique)
{
    std::string name = technique->getResourseName();
    m_renderTechniques.insert(std::map<std::string, renderer::RenderTechniquePtr>::value_type(name, technique));
}

const RenderTechniquePtr& CRenderTechniqueManager::get(const std::string& name)
{
    return m_renderTechniques[name];
}

const RenderTechniquePtr CRenderTechniqueManager::load(const std::string& name)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    auto it = m_renderTechniques.find(nameStr);

    if (it != m_renderTechniques.end())
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
                FileStreamPtr stream = stream::CStreamManager::createFileStream(fullName, stream::FileStream::e_in);

                if (stream->isOpen())
                {
                    renderer::RenderTechniquePtr technique = std::make_shared<renderer::CRenderTechnique>();

                    technique->init(stream);
                    technique->setResourseName(fullName);
                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    technique->setResourseFolder(fullPath);

                    if (!technique->load())
                    {
                        LOG_ERROR("CRenderTechniqueManager: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    m_renderTechniques.insert(std::map<std::string, renderer::RenderTechniquePtr>::value_type(nameStr, technique));

                    return technique;
                    
                }
            }
            else
            {
                LOG_WARRNING("CRenderTechniqueManager: File [%s] not found", fullName.c_str());
            }
        }
    }

    return nullptr;
}

void CRenderTechniqueManager::unload(const std::string& name)
{
    auto it = m_renderTechniques.find(name);

    if (it != m_renderTechniques.end())
    {
        m_renderTechniques.erase(it);
    }
}

void CRenderTechniqueManager::unload(const renderer::RenderTechniquePtr& technique)
{
    auto predDelete = [technique](const std::pair<std::string, renderer::RenderTechniquePtr>& pair) -> bool
    {
        return pair.second == technique;
    };

    auto it = std::find_if(m_renderTechniques.begin(), m_renderTechniques.end(), predDelete);

    if (it != m_renderTechniques.end())
    {
        m_renderTechniques.erase(it);
    }
}

void CRenderTechniqueManager::unloadAll()
{
    m_renderTechniques.clear();
}

void CRenderTechniqueManager::registerPath(const std::string& path)
{
    m_pathes.push_back(path);
}

void CRenderTechniqueManager::unregisterPath(const std::string& path)
{
    auto it = std::find(m_pathes.begin(), m_pathes.end(), path);
    if (it != m_pathes.end())
    {
        m_pathes.erase(std::remove(m_pathes.begin(), m_pathes.end(), *it), m_pathes.end());
    }
}
