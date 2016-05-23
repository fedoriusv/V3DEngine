#include "ModelManager.h"
#include "utils/Logger.h"
#include "decoders/ModelF3DDecoder.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace scene;
using namespace stream;
using namespace decoders;
using namespace renderer;

CModelManager::CModelManager()
: m_modelInc(0)
{
    TResourceLoader::registerPath("../../../../data/");
    TResourceLoader::registerPath("../../../../../data/");
    TResourceLoader::registerPath("data/");

    std::initializer_list<std::string> ext = { ".f3d"};
    TResourceLoader::registerDecoder(std::make_shared<CModelF3DDecoder>(ext));
}

CModelManager::~CModelManager()
{
}

CModel* CModelManager::load(const std::string& name, const std::string& alias)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    CModel* findModel = TResourceLoader::get(alias.empty() ? nameStr : alias);
    if (findModel)
    {
        CModel* cloneModel = findModel->clone();
        if (cloneModel)
        {
            nameStr.append("_" + std::to_string(++m_modelInc));
            TResourceLoader::insert(cloneModel, cloneModel->getName());
        }

        return cloneModel;
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
            const bool isFileExist = FileStream::isFileExist(fullName);
            if (isFileExist)
            {
                const FileStreamPtr stream = CStreamManager::createFileStream(fullName, FileStream::e_in);
                if (stream->isOpen())
                {
                    
                    const DecoderPtr decoder = TResourceLoader::findDecoder(fileExtension);
                    if (!decoder)
                    {
                        LOG_ERROR("CModelManager::load: Format not supported file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    IResource* resource = decoder->decode(stream);
                    stream->close();

                    if (!resource)
                    {
                        LOG_ERROR("CModelManager::load: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    resource->setResourseName(fullName);
                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    resource->setResourseFolder(fullPath);

                    CModel* data = static_cast<CModel*>(resource);
                    if (!data->load())
                    {
                        LOG_ERROR("CModelManager::load: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    TResourceLoader::insert(data, alias.empty() ? nameStr : alias);
                    LOG_INFO("CModelManager: File [%s] success loaded", fullName.c_str());
                    return data;
                }
            }
        }
    }

    LOG_WARNING("CModelManager::load: File [%s] not found", name.c_str());
    return nullptr;
}