#include "RenderTechniqueManager.h"
#include "stream/FileStream.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::scene;

CRenderTechniqueManager::CRenderTechniqueManager()
{
    CRenderTechniqueManager::registerPath("../../../../data/");
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

renderer::RenderTechniquePtr CRenderTechniqueManager::get(const std::string& name)
{
    return m_renderTechniques[name];
}

renderer::RenderTechniquePtr CRenderTechniqueManager::load(const std::string& name)
{
    auto it = m_renderTechniques.find(name);

    if (it != m_renderTechniques.end())
    {
        return it->second;
    }
    else
    {
        std::string fileExtension;

        const size_t pos = name.find('.');
        if (pos != std::string::npos)
        {
            fileExtension = std::string(name.begin() + pos, name.end());
        }

        for (std::string& path : m_pathes)
        {
            const std::string fullName = path + name;
            const bool isFileExist = stream::FileStream::isFileExist(fullName);
            if (isFileExist)
            {
                stream::FileStream* stream = new stream::FileStream(fullName, stream::FileStream::e_in);

                if (stream->isOpen())
                {
                    renderer::RenderTechniquePtr technique = std::make_shared<renderer::CRenderTechnique>();

                    technique->init(stream);
                    if (!technique->load())
                    {
                        LOG_ERROR("Streaming error read file [%s]", name.c_str());
                        return nullptr;
                    }

                    m_renderTechniques.insert(std::map<std::string, renderer::RenderTechniquePtr>::value_type(name, technique));

                    return technique;
                    
                }
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