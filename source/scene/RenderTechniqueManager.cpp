#include "RenderTechniqueManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace scene;
using namespace stream;
using namespace renderer;

CRenderTechniqueManager::CRenderTechniqueManager()
{
    CRenderTechniqueManager::registerPath("../../../../data/");
    CRenderTechniqueManager::registerPath("../../../../../data/");
    CRenderTechniqueManager::registerPath("data/");
}

CRenderTechniqueManager::~CRenderTechniqueManager()
{
    unloadAll();
}

void CRenderTechniqueManager::add(const renderer::RenderTechniquePtr& technique)
{
    std::string name = technique->getResourseName();
    m_resources.insert(std::map<std::string, renderer::RenderTechniquePtr>::value_type(name, technique));
}

const RenderTechniquePtr CRenderTechniqueManager::load(const std::string& name, const std::string& alias)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    auto it = m_resources.find(nameStr);

    if (it != m_resources.end())
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
                        stream->close();

                        return nullptr;
                    }
                    stream->close();

                    m_resources.insert(std::map<std::string, renderer::RenderTechniquePtr>::value_type(nameStr, technique));

                    return technique;
                }
            }
            else
            {
                LOG_WARNING("CRenderTechniqueManager: File [%s] not found", fullName.c_str());
            }
        }
    }

    return nullptr;
}