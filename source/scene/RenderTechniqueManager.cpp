#include "RenderTechniqueManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace scene;
using namespace stream;
using namespace renderer;

CRenderTechniqueManager::CRenderTechniqueManager()
{
    TResourceLoader::registerPath("../../../../data/");
    TResourceLoader::registerPath("../../../../../data/");
    TResourceLoader::registerPath("data/");
}

CRenderTechniqueManager::~CRenderTechniqueManager()
{
}

void CRenderTechniqueManager::add(const renderer::CRenderTechnique* technique)
{
    std::string name = technique->getResourseName();
    TResourceLoader::insert(technique, name);
}

const CRenderTechnique* CRenderTechniqueManager::load(const std::string& name, const std::string& alias)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    const CRenderTechnique* findTechnique = TResourceLoader::get(alias.empty() ? nameStr : alias);
    if (findTechnique)
    {
        return findTechnique;
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
                    renderer::CRenderTechnique* technique = new renderer::CRenderTechnique();

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

                    TResourceLoader::insert(technique, alias.empty() ? nameStr : alias);
                    LOG_INFO("CRenderTechniqueManager: File [%s] success loaded", fullName.c_str());
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