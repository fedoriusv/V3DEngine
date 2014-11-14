#include "ModelManager.h"
#include "utils/Logger.h"
#include "decoders/ModelF3DDecoder.h"
#include "stream/StreamManager.h"
#include "resources/ModelData.h"
#include "renderer/Mesh.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::stream;
using namespace v3d::decoders;
using namespace v3d::resources;
using namespace v3d::renderer;

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
}

ModelDataPtr CModelManager::load(const std::string& name)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

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
                    LOG_ERROR("CModelManager::load: Format not supported file [%s]", nameStr.c_str());
                    return nullptr;
                }

                const DecoderPtr& decoder = (*iter);
                stream::ResourcePtr resource = decoder->decode(stream);
                if (!resource)
                {
                    LOG_ERROR("CModelManager::load: Streaming error read file [%s]", nameStr.c_str());
                    stream->close();

                    return nullptr;
                }

                resource->setResourseName(fullName);
                const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                resource->setResourseFolder(fullPath);

                ModelDataPtr data = std::static_pointer_cast<CModelData>(resource);
 
                if (!data->load())
                {
                    LOG_ERROR("CModelManager::load: Streaming error read file [%s]", nameStr.c_str());
                    stream->close();

                    return nullptr;
                }
                stream->close();

                return data;
            }
        }
    }

    LOG_WARNING("CModelManager::load: File [%s] not found", name.c_str());

    return nullptr;
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
