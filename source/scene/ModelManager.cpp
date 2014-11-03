#include "ModelManager.h"
#include "utils/Logger.h"
#include "decoders/ModelF3DDecoder.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::stream;
using namespace v3d::decoders;
using namespace v3d::resources;

CModelManager::CModelManager()
{
    CModelManager::registerPath("../../../../data/");
    CModelManager::registerPath("data/");

    std::initializer_list<std::string> ext = { ".f3d"};
    CModelManager::registerDecoder(std::make_shared<CModelF3DDecoder>(ext));
}

CModelManager::~CModelManager()
{
    m_pathes.clear();
    m_decoders.clear();

    CModelManager::unloadAll();
}

void CModelManager::add(const ModelDataPtr& model)
{
    std::string name = model->getResourseName();
    m_modelList.insert(std::map<std::string, ModelDataPtr>::value_type(name, model));
}

const ModelDataPtr& CModelManager::get(const std::string& name)
{
    return m_modelList[name];
}

ModelDataPtr CModelManager::load(const std::string& name)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    auto it = m_modelList.find(nameStr);

    if (it != m_modelList.end())
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
                const stream::FileStreamPtr& stream = stream::CStreamManager::createFileStream(fullName, stream::FileStream::e_in);
                if (stream->isOpen())
                {
                    auto predCanDecode = [fileExtension](const DecoderPtr& decoder) -> bool
                    {
                        return decoder->isExtensionSupported(fileExtension);
                    };

                    auto iter = std::find_if(m_decoders.begin(), m_decoders.end(), predCanDecode);
                    if (iter == m_decoders.end())
                    {
                        LOG_ERROR("CModelManager: Format not supported file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    const DecoderPtr& decoder = (*iter);
                    stream::ResourcePtr resource = decoder->decode(stream);
                    if (!resource)
                    {
                        LOG_ERROR("CModelManager: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    resource->setResourseName(fullName);
                    const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                    resource->setResourseFolder(fullPath);

                    ModelDataPtr model = std::static_pointer_cast<CModelData>(resource);

                    if (!model->load())
                    {
                        LOG_ERROR("CModelManager: Streaming error read file [%s]", nameStr.c_str());
                        return nullptr;
                    }

                    m_modelList.insert(std::map<std::string, ModelDataPtr>::value_type(nameStr, model));

                    return model;
                }
            }
            else
            {
                LOG_WARNING("CModelManager File [%s] not found", fullName.c_str());
            }
        }
    }

    return nullptr;
}

void CModelManager::unload(const std::string& name)
{
    auto it = m_modelList.find(name);

    if (it != m_modelList.end())
    {
        m_modelList.erase(it);
    }
}

void CModelManager::unload(const ModelDataPtr& model)
{
    auto predDelete = [&model](const std::pair<std::string, ModelDataPtr>& pair) -> bool
    {
        return pair.second == model;
    };

    auto it = std::find_if(m_modelList.begin(), m_modelList.end(), predDelete);

    if (it != m_modelList.end())
    {
        m_modelList.erase(it);
    }
}

void CModelManager::unloadAll()
{
    m_modelList.clear();
}

void CModelManager::registerPath(const std::string& path)
{
    m_pathes.push_back(path);
}

void CModelManager::unregisterPath(const std::string& path)
{
    auto it = std::find(m_pathes.begin(), m_pathes.end(), path);
    if (it != m_pathes.end())
    {
        m_pathes.erase(std::remove(m_pathes.begin(), m_pathes.end(), *it), m_pathes.end());
    }
}

void CModelManager::registerDecoder(DecoderPtr decoder)
{
    m_decoders.push_back(decoder);
}

void CModelManager::unregisterDecoder(DecoderPtr& decoder)
{
    auto it = std::find(m_decoders.begin(), m_decoders.end(), decoder);
    if (it != m_decoders.end())
    {
        m_decoders.erase(std::remove(m_decoders.begin(), m_decoders.end(), *it), m_decoders.end());
    }
}
